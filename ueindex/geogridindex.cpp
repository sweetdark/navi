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
#include "geogridindex.h"
using namespace UeIndex;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor
//
/**
* Directly construct this index object using specified extent and corresponding scale level
*/
CGeoGridIndex::CGeoGridIndex(const CGeoRect<double> &extent, unsigned long xCount, unsigned long yCount, GeoLayer &curLayer, short scaleLevel, bool isTiled) 
	: CGeoIndexImpl(_T(""), curLayer, isTiled)
{
	CreateIndex(extent, xCount, yCount, scaleLevel);
}

/**
*
*/
CGeoGridIndex::~CGeoGridIndex()
{
}

/**
*
*/
unsigned long CGeoGridIndex::GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location)
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
				return /*(tileIdx << 24) |*/ GetParcelID(m_layer.m_tiles[tileIdx], location);
			}
		}
	}

    return -1;
}

/*
*
*/
void CGeoGridIndex::GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice)
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
inline long CGeoGridIndex::GetParcelCount(unsigned long gateIdx)
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
inline unsigned long CGeoGridIndex::GetParcelID(const GeoTile &oneTile, const CGeoPoint<double> &location) const
{
	int i = 0;
	for(; i < oneTile.m_yCount; i++)
	{
		double minY = oneTile.m_mbr.m_minY + i * oneTile.m_yHeight;
		double maxY = minY + oneTile.m_yHeight;
		if(location.m_y < minY ||  location.m_y > maxY)
		{
			continue;
		}

		int j = 0;
		for(; j < oneTile.m_xCount; j++)
		{
			double minX = oneTile.m_mbr.m_minX + j * oneTile.m_xWidth;
			double maxX = minX + oneTile.m_xWidth;

			if(minX <= location.m_x && location.m_x <= maxX)
			{
				return /*(oneTile.m_idx << 24) |*/ (i * oneTile.m_xCount + j);
			}
		}
	}

	return -1;
}

/**
*
*/
inline void CGeoGridIndex::GetParcelID(const GeoTile &oneTile, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice) const
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
		double minY = oneTile.m_mbr.m_minY + i * oneTile.m_yHeight;
		double maxY = minY + oneTile.m_yHeight;
		if(cur.m_maxY < minY ||  cur.m_minY > maxY)
		{
			continue;
		}

		int j = 0;
		for(; j < oneTile.m_xCount; j++)
		{
			double minX = oneTile.m_mbr.m_minX + j * oneTile.m_xWidth;
			double maxX = minX + oneTile.m_xWidth;

			CGeoRect<double> parcelMbr;
			parcelMbr.m_minX = minX;
			parcelMbr.m_minY = minY;
			parcelMbr.m_maxX = maxX;
			parcelMbr.m_maxY = maxY;
			if(parcelMbr.IsIntersect(cur))
			{
				unsigned int parcelIdx = /*(oneTile.m_idx << 24) |*/ (i * oneTile.m_xCount + j);
				indice.Add(&parcelIdx);
			}
		}
	}
}

/**
*
**/
bool CGeoGridIndex::GetParcelExtent(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<double> &extent)
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

			int yDirection = parcelIdx / oneTile.m_xCount;
			int xDirection = parcelIdx - yDirection * oneTile.m_xCount;

			extent.m_minX = oneTile.m_mbr.m_minX + xDirection * oneTile.m_xWidth;
			extent.m_minY = oneTile.m_mbr.m_minY + yDirection * oneTile.m_yHeight;
			extent.m_maxX = extent.m_minX + oneTile.m_xWidth;
			extent.m_maxY = extent.m_minY + oneTile.m_yHeight;

			return true;
		}
	}

	return false;
}

/**
*
**/
inline void CGeoGridIndex::GetParcelID(unsigned long gateIdx, const CGeoRect<int> &directions, CMemVector &indice)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		unsigned short tileIdx = GetTileID(gateIdx);
		if(tileIdx != -1)
		{
			GeoTile &oneTile = m_layer.m_tiles[tileIdx];

			// Exceptions
			if(directions.m_minX >= oneTile.m_xCount || 
				directions.m_minY >= oneTile.m_yCount)
			{
				return;
			}

			int minY = (directions.m_minY < 0) ? 0 : directions.m_minY;
			int maxY = (directions.m_maxY > oneTile.m_yCount) ? oneTile.m_yCount : directions.m_maxY;
			int minX = (directions.m_minX < 0) ? 0 : directions.m_minX;
			int maxX = (directions.m_maxX > oneTile.m_xCount) ? oneTile.m_xCount : directions.m_maxX;
			
			int i = minY;
			for(; i < maxY; i++)
			{
				int j = minX;
				for(; j < maxX; j++)
				{
					unsigned long parcelIdx = /*(oneTile.m_idx << 24) |*/ (i * oneTile.m_xCount + j);
					indice.Add(&parcelIdx);
				}
			}
		}
	}
}

/**
*
**/
inline void CGeoGridIndex::GetParcelDirections(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<int> &directions)
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

			directions.m_minY = parcelIdx / oneTile.m_xCount;
			directions.m_minX = parcelIdx - directions.m_minY * oneTile.m_xCount;
		}
	}
}

/**
*
**/
inline void CGeoGridIndex::GetTileDirections(unsigned long gateIdx, CGeoRect<int> &directions)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		unsigned short tileIdx = GetTileID(gateIdx);
		if(tileIdx != -1)
		{
			GeoTile &oneTile = m_layer.m_tiles[tileIdx];
			directions.m_minX = oneTile.m_xCount;
			directions.m_minY = oneTile.m_yCount;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* TODO:
* Need to define INDEX file specification in order to directly get index from that file
*/
void CGeoGridIndex::CreateIndex(const CGeoRect<double> &extent, unsigned long xCount, unsigned long yCount, short scaleLevel)
{
	if(m_layer.m_tiles)
	{
		delete []m_layer.m_tiles;
	}
	
	// Basic setting
	int parcelCount = xCount * yCount;
	m_layer.m_idx = 0;
	m_layer.m_xCount = 1;
	m_layer.m_yCount = 1;
	m_layer.m_tiles = new GeoTile[m_layer.m_xCount * m_layer.m_yCount];

	// Only one tile
	int tileIdx = 0;
	GeoTile &oneTile = m_layer.m_tiles[tileIdx];

	// Tile is equal layer
	oneTile.m_mbr.m_minX = m_layer.m_mbr.m_minX = extent.m_minX;
	oneTile.m_mbr.m_minY = m_layer.m_mbr.m_minY = extent.m_minY;
	oneTile.m_mbr.m_maxY = m_layer.m_mbr.m_maxY = extent.m_maxY;
	oneTile.m_mbr.m_maxX = m_layer.m_mbr.m_maxX = extent.m_maxX;
	oneTile.m_idx = tileIdx;
	oneTile.m_xCount = xCount;
	oneTile.m_yCount = yCount; 
	oneTile.m_xWidth = oneTile.m_mbr.Width() / xCount;
	oneTile.m_yHeight = oneTile.m_mbr.Height() / yCount;
}