/*
 * The UrbanEye PND solution is a collection of packages based on native C++ classes
 * that implement the fundamental algorithms & operations required to satisfy one kind
 * of mobile or portable devices with competitive qualities being a leading one.
 *
 * Copyright (C) 2008 Xiujie Sheng
 *
 * This library isn't a free software, so you cann't take the sourcecode without author's permit.
 * Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
 * if you illegally got it. 
 *
 * NOTE: 
 * AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
 * LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
 *
 * For more information, contact:
 *
 * China University of Geoseciences(Wuhan)
 * Graduate School
 *
 * Xiujie Sheng
 * smartpoint@sohu.com
 */
// Refer to UeModel package
#include "geoseqindex.h"
using namespace UeIndex;

// Refer to UeBase package
#include "uebase\pathbasic.h"
#include "uebase\filebasic.h"
using namespace UeBase;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor
//
/**
* Read index information then construct "layer-tile" structure in advance
*/
CGeoSeqIndex::CGeoSeqIndex(const tstring &indexFile, GeoLayer &m_layer, bool isTiled) : CGeoIndexImpl(indexFile, m_layer, isTiled)
{
	CreateIndex(indexFile);
}

/**
*
*/
CGeoSeqIndex::~CGeoSeqIndex()
{
}

/**
* Binary predicate seperately using Y and X values
*/
unsigned long CGeoSeqIndex::GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		unsigned short tileIdx = GetTileID(layerIdx, location);
		assert(IsValidTile(tileIdx));

		if(tileIdx != -1)
		{
			GeoTile &oneTile = m_layer.m_tiles[tileIdx];
			int parcels = oneTile.m_xCount;

			int i = 0;
			for(; i < parcels; i++)
			{
				if(oneTile.m_parcels[i].m_mbr.IsContain(location))
				{
					return oneTile.m_idx << 24 | oneTile.m_parcels[i].m_idx;
				}
			}
		}
	}

	return -1;
}

/*
*
*/
void CGeoSeqIndex::GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		CMemVector tileIdxes(sizeof(unsigned short));
		GetTileID(layerIdx, mbr, tolerance, tileIdxes);
		assert(tileIdxes.GetCount());
		
		CGeoRect<double> cur;
		cur.m_minX = mbr.m_minX - tolerance;
		cur.m_maxX = mbr.m_maxX + tolerance;
		cur.m_minY = mbr.m_minY - tolerance;
		cur.m_maxY = mbr.m_maxY + tolerance;

		int i = 0;
		int count = tileIdxes.GetCount();
		for(; i < count; i++)
		{
			unsigned short tileIdx = *(reinterpret_cast<unsigned short *>(tileIdxes[i]));
			int parcels = m_layer.m_tiles[tileIdx].m_xCount;

			int j = 0;
			for(; j < parcels; j++)
			{
				if(m_layer.m_tiles[tileIdx].m_parcels[j].m_mbr.IsIntersect(cur))
				{
					unsigned long parcelIdx = tileIdx << 24 | m_layer.m_tiles[tileIdx].m_parcels[j].m_idx;
					indice.Add(&parcelIdx);
				}
			}
		}
	}
}

/**
* Here, member variable m_xCount of GeoTile is directly stored as the total number of parcels in this tile, and m_yCount
* is to store the minimum height value of those parcels in this tile. m_yCount will be used to get the maximum bars when
* applying Y-Histogram algorithm.
*/
inline long CGeoSeqIndex::GetParcelCount(unsigned long gateIdx)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		CMemVector tileIdxes(sizeof(unsigned short));
		GetTileID(layerIdx, m_layer.m_mbr, 0., tileIdxes);
		assert(tileIdxes.GetCount());
		
		int parcels = 0;
		int i = 0;
		int count = tileIdxes.GetCount();
		for(; i < count; i++)
		{
			unsigned short tileIdx = *(reinterpret_cast<unsigned short *>(tileIdxes[i]));
			parcels += m_layer.m_tiles[tileIdx].m_xCount;
		}

		return parcels;
	}

	return 0;
}

/**
* Note:
* Here the maximum memory usage is
* (sizeof(unsigned int *) * parcels) * tiles * layers
*/
void CGeoSeqIndex::CreateIndex(const tstring &indexFile)
{
	#pragma message (__FILE__" >> Not really load index data for this index mechansim!")
	return;

	//TCHAR path[256] = {0, };
	//const CPathBasic &pathBasic = CPathBasic::Get();
	//pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);

	void *mbrHandle = 0;
	void *mbrMapping = 0;
	CGeoRect<long> *mbrs = 0;
	const CFileBasic &fileBasic = CFileBasic::Get();
	bool rt = fileBasic.CreateMemoryMapping(indexFile, &mbrHandle, &mbrMapping, (void **)&mbrs, CFileBasic::UL_Native);
	assert(rt && fileBasic.IsValidHandle(mbrHandle, CFileBasic::UL_Native));
	if(!rt || !fileBasic.IsValidHandle(mbrHandle, CFileBasic::UL_Native))
	{
		assert(false);
		return;
	}

	//
	int parcelCount = fileBasic.GetFileSize(mbrHandle, CFileBasic::UL_Native) / sizeof(CGeoRect<long>);
	m_layer.m_idx = 0;
	m_layer.m_xCount = 1;
	m_layer.m_yCount = 1;
	m_layer.m_tiles = new GeoTile[m_layer.m_xCount * m_layer.m_yCount];

	// Only one tile
	int tileIdx = 0;
	GeoTile &oneTile = m_layer.m_tiles[tileIdx];
	oneTile.m_idx = tileIdx;
	oneTile.m_xCount = parcelCount;
	oneTile.m_yCount = 0; 
	oneTile.m_parcels = new GeoParcel[oneTile.m_xCount];

	CGeoRect<double> layerExtent;
	layerExtent.m_minX = mbrs[0].m_minX;
	layerExtent.m_minY = mbrs[0].m_minY;
	layerExtent.m_maxX = mbrs[0].m_maxX;
	layerExtent.m_maxY = mbrs[0].m_maxY;

	int parcelIdx = 0;
	for(; parcelIdx < parcelCount; parcelIdx++)
	{
		GeoParcel &oneParcel = oneTile.m_parcels[parcelIdx];
		oneParcel.m_idx = parcelIdx;
		oneParcel.m_mbr.m_minX = mbrs[parcelIdx].m_minX;
		oneParcel.m_mbr.m_minY = mbrs[parcelIdx].m_minY;
		oneParcel.m_mbr.m_maxX = mbrs[parcelIdx].m_maxX;
		oneParcel.m_mbr.m_maxY = mbrs[parcelIdx].m_maxY;

		layerExtent.Union(oneParcel.m_mbr);
	}

	oneTile.m_mbr.m_minX = m_layer.m_mbr.m_minX = layerExtent.m_minX;
	oneTile.m_mbr.m_minY = m_layer.m_mbr.m_minY = layerExtent.m_minY;
	oneTile.m_mbr.m_maxY = m_layer.m_mbr.m_maxY = layerExtent.m_maxY;
	oneTile.m_mbr.m_maxX = m_layer.m_mbr.m_maxX = layerExtent.m_maxX;

	fileBasic.ReleaseMemoryMapping(&mbrHandle, &mbrMapping, (void **)&mbrs, CFileBasic::UL_Native);
}

