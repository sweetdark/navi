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
#include "georectindex.h"
using namespace UeIndex;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor
//
/**
* Read index information then construct "layer-tile" structure in advance
*/
CGeoRectIndex::CGeoRectIndex(const tstring &indexFile, GeoLayer &curLayer, bool isTiled) : CGeoIndexImpl(indexFile, curLayer, isTiled)
{
	CreateIndex(indexFile);
}

/**
*
*/
CGeoRectIndex::~CGeoRectIndex()
{
}

/**
*
*/
unsigned long CGeoRectIndex::GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location)
{
    short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		/// Since this gateIdx maybe not contain the tile idx information, it had better directly get it
		unsigned short tileIdx = GetTileID(layerIdx, location);
		assert(IsValidTile(tileIdx));

		if(tileIdx != -1)
		{
			if(IsValidTile(tileIdx) && (m_layer.m_tiles + tileIdx))
			{
				// Get parcel definition specified to one tile in one layer ...
				return (tileIdx << 24) | GetParcelID(m_layer.m_tiles[tileIdx], location);
			}
		}
	}

    return -1;
}

/*
*
*/
void CGeoRectIndex::GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice)
{
    // Loop all parcels belonging to one tile ...
    short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		CMemVector tileIdxes(sizeof(unsigned short));
		GetTileID(layerIdx, mbr, tolerance, tileIdxes);

		int i = 0;
		int count = tileIdxes.GetCount();
		for(; i < count; i++)
		{
			unsigned short tileIdx = *(reinterpret_cast<unsigned short *>(tileIdxes[i]));
			assert(IsValidTile(tileIdx));

			int j = 0;
			GeoTile &oneTile = m_layer.m_tiles[tileIdx];
			GetParcelID(oneTile, mbr, tolerance, indice);
		}
	}
}

/**
*
*/
inline long CGeoRectIndex::GetParcelCount(unsigned long gateIdx)
{
    short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		CMemVector tileIdxes(sizeof(unsigned short));
		GetTileID(layerIdx, m_layer.m_mbr, 0., tileIdxes);

		int i = 0;
		int count = tileIdxes.GetCount();
		int parcels = 0;
		for(; i < count; i++)
		{
			unsigned short tileIdx = *(reinterpret_cast<unsigned short *>(tileIdxes[i]));
			parcels += m_layer.m_tiles[tileIdx].m_yCount * m_layer.m_tiles[tileIdx].m_xCount;
		}

		return parcels;
	}

    return 0;
}

/**
*
*/
inline unsigned long CGeoRectIndex::GetParcelID(const GeoTile &oneTile, const CGeoPoint<double> &location) const
{
	int i = 0;
	for(; i < oneTile.m_yCount; i++)
	{
		// Exception
		assert(oneTile.m_parcels);

		// Directly skip some rows
		unsigned long parcelIdx = i * oneTile.m_xCount;
		GeoParcel &oneParcel = oneTile.m_parcels[parcelIdx];
		if(location.m_y < oneParcel.m_mbr.m_minY ||  location.m_y > oneParcel.m_mbr.m_maxY)
		{
			continue;
		}

		int j = 0;
		for(; j < oneTile.m_xCount; j++)
		{
			unsigned long parcelIdx = i * oneTile.m_xCount + j;
			if(oneTile.m_parcels[parcelIdx].m_mbr.m_minX <= location.m_x && 
				location.m_x <= oneTile.m_parcels[parcelIdx].m_mbr.m_maxX)
			{
				return (oneTile.m_idx << 24) | parcelIdx;
			}
		}
	}

	return -1;
}

/**
*
*/
inline void CGeoRectIndex::GetParcelID(const GeoTile &oneTile, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice) const
{
	// Inflate or deflate specified rectangle
	// Inflate orn deflate this rectangle
	CGeoRect<double> cur;
	cur.m_minX = mbr.m_minX - tolerance;
	cur.m_maxX = mbr.m_maxX + tolerance;
	cur.m_minY = mbr.m_minY - tolerance;
	cur.m_maxY = mbr.m_maxY + tolerance;

	int i = 0;
	for(; i < oneTile.m_yCount; i++)
	{
		// Directly skip some rows
		unsigned long parcelIdx = i * oneTile.m_xCount;
		GeoParcel &oneParcel = oneTile.m_parcels[parcelIdx];
		if(oneParcel.m_mbr.m_minY > cur.m_maxY || oneParcel.m_mbr.m_maxY < cur.m_minY)
		{
			continue;
		}

		int j = 0;
		for(; j < oneTile.m_xCount; j++)
		{
			unsigned long parcelIdx = i * oneTile.m_xCount + j;
			if(oneTile.m_parcels[parcelIdx].m_mbr.IsIntersect(cur))
			{
				parcelIdx = (oneTile.m_idx << 24) | parcelIdx;
				indice.Add(&parcelIdx);
			}
		}
	}
}

/**
*
**/
bool CGeoRectIndex::GetParcelExtent(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<double> &extent)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		unsigned short tileIdx = GetTileID(gateIdx);
		if(tileIdx != -1)
		{
			GeoTile &oneTile = m_layer.m_tiles[tileIdx];
			int parcels = oneTile.m_xCount * oneTile.m_yCount;
			parcelIdx &= 0x00FFFFFF;
			assert(parcelIdx < parcels);

			extent = oneTile.m_parcels[parcelIdx].m_mbr;
			return true;
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* TODO:
* Need to define INDEX file specification in order to directly get index from that file
*/
void CGeoRectIndex::CreateIndex(const tstring &indexFile)
{
}
