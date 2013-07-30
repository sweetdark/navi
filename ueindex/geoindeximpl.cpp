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
// Refer to UeIndex package
#include "geoindeximpl.h"
using namespace UeIndex;

// Refer to CLog2 function
#include "uebase\mathbasic.h"
using namespace UeBase;

//////////////////////////////////////////////////////////////////////////////////
// Constructors & deconstructor
//
/**
* 
*/
CGeoIndexImpl::CGeoIndexImpl(const tstring &indexFile, GeoLayer &curLayer, bool isTiled) : m_isTiled(isTiled), m_layer(curLayer)
{
	// Since the right bits are used to express the identifers of data layers, it need firstly calculate
	// how many bits can express those layers from zero to its maximum
	m_shift = CMathBasic::Get().CLog2(MAXLAYERCOUNT + 1);

	// Get mask for above shiftting bits
	m_mask = CMathBasic::Get().GetMask(m_shift, 1);
}

/**
*
*/
CGeoIndexImpl::~CGeoIndexImpl()
{
}

////////////////////////////////////////////////////////////////////////////
// Methods for getting identifers of this "TILE-LAYER" structure
// 
/**
*
*/
inline unsigned long CGeoIndexImpl::GetGateID(unsigned short layerIdx, unsigned short tileIdx) const
{
	// If those layer are tiled, means each tile corresponds to one plain file
	if(m_isTiled)
	{
		assert(IsValidLayer(layerIdx) && IsValidTile(tileIdx));
		return (IsValidLayer(layerIdx) && IsValidTile(tileIdx)) ?
			tileIdx << m_shift | layerIdx : -1;
	}
	else
	{
		assert(IsValidLayer(layerIdx));
		return (IsValidLayer(layerIdx)) ? layerIdx : -1;
	}
}

/**
*
*/
inline unsigned long CGeoIndexImpl::GetGateID(unsigned short layerIdx, const CGeoPoint<double> &location) const
{
	if(m_isTiled)
	{
		assert(IsValidLayer(layerIdx));
		if(IsValidLayer(layerIdx))
		{
			unsigned short tileIdx = GetTileID(layerIdx, location);
			assert(IsValidTile(tileIdx));

			return (IsValidTile(tileIdx)) ? ((tileIdx << m_shift) | layerIdx) : -1;
		}
	}
	else
	{
		assert(IsValidLayer(layerIdx));
		return (IsValidLayer(layerIdx)) ? layerIdx : -1;
	}

	return -1;
}

/**
* Principle is to let layer++
*/
inline unsigned long CGeoIndexImpl::GetUpGateID(unsigned long gateIdx) const
{
    short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx) && IsValidLayer(layerIdx+1));

	if(m_isTiled)
	{
		short tileIdx = GetTileID(gateIdx);
		assert(IsValidTile(tileIdx));

		return (IsValidLayer(layerIdx+1) && IsValidTile(tileIdx)) ? GetGateID(++layerIdx, tileIdx) : -1;
	}
	else
	{
		assert(IsValidLayer(layerIdx+1));
		return IsValidLayer(layerIdx+1) ? ++layerIdx : -1; 
	}
}

/**
* Principle is to let layer--
*/
inline unsigned long CGeoIndexImpl::GetDownGateID(unsigned long gateIdx) const
{
    short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx) && IsValidLayer(layerIdx - 1));

	if(m_isTiled)
	{
		short tileIdx = GetTileID(gateIdx);
		assert(IsValidTile(tileIdx));	

		return (IsValidLayer(layerIdx - 1) && IsValidTile(tileIdx)) ? GetGateID(--layerIdx, tileIdx) : -1;
	}
	else
	{
		assert(IsValidLayer(layerIdx-1));
		return (IsValidLayer(layerIdx-1)) ? --layerIdx : -1;
	}
}

/**
* Note:
* Recursive calling causes stack overflow issue: GetLayerID call to IsValidXX and IsValidXX call to GetLayerID etc.
*/
inline unsigned short CGeoIndexImpl::GetLayerID(unsigned long gateIdx) const
{
	return gateIdx & m_mask;
}

/**
*
*/
inline unsigned short CGeoIndexImpl::GetTileID(unsigned long gateIdx) const
{
	return (gateIdx & (~m_mask)) >> m_shift;
}

/**
*
*/
inline unsigned short CGeoIndexImpl::GetTileID(unsigned short layerIdx, const CGeoPoint<double> &location) const
{
	int i = 0;
	for(; i < m_layer.m_yCount; i++)
	{
		// Directly skip some rows
		int tileIdx = i * m_layer.m_xCount;
		GeoTile &oneTile = m_layer.m_tiles[tileIdx];
		if(location.m_y < oneTile.m_mbr.m_minY || location.m_y > oneTile.m_mbr.m_maxY)
		{
			continue;
		}

		//
		int j = 0;
		for(; j < m_layer.m_xCount; j++)
		{
			unsigned short tileIdx = i * m_layer.m_xCount + j;

			if(m_layer.m_tiles[tileIdx].m_mbr.m_minX <= location.m_x && 
				location.m_x <= m_layer.m_tiles[tileIdx].m_mbr.m_maxX)
			{
				return tileIdx;
			}
		}
	}

	return -1;
}

/**
*
*/
inline void CGeoIndexImpl::GetTileID(unsigned short layerIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice) const
{
	// Inflate orn deflate this rectangle
	CGeoRect<double> cur;
	cur.m_minX = mbr.m_minX - tolerance;
	cur.m_maxX = mbr.m_maxX + tolerance;
	cur.m_minY = mbr.m_minY - tolerance;
	cur.m_maxY = mbr.m_maxY + tolerance;

	int i = 0;
	for(; i < m_layer.m_yCount; i++)
	{
		// Directly skip some rows
		unsigned short tileIdx = i * m_layer.m_xCount;
		GeoTile &oneTile = m_layer.m_tiles[tileIdx];
		if(oneTile.m_mbr.m_minY > cur.m_maxY || oneTile.m_mbr.m_maxY < cur.m_minY)
		{
			continue;
		}

		int j = 0;
		for(; j < m_layer.m_xCount; j++)
		{
			unsigned short tileIdx = i * m_layer.m_xCount + j;
			if(m_layer.m_tiles[tileIdx].m_mbr.IsIntersect(cur))
			{
				indice.Add(&tileIdx);
			}
		}
	}

	return;
}

/**
*
**/
bool CGeoIndexImpl::GetParcelExtent(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<double> &extent)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		unsigned short tileIdx = GetTileID(gateIdx);
		assert(IsValidTile(tileIdx));

		if(tileIdx != -1)
		{
			GeoTile &oneTile = m_layer.m_tiles[tileIdx];
			int parcels = oneTile.m_xCount * oneTile.m_yCount;
			assert(oneTile.m_parcels);

			int i = 0;
			for(; i < parcels; i++)
			{
				if(oneTile.m_parcels[i].m_idx == parcelIdx)
				{
					extent = oneTile.m_parcels[i].m_mbr;
					return true;
				}
			}
		}
	}

	return false;
}

/**
*
**/
inline bool CGeoIndexImpl::GetLayerExtent(unsigned long gateIdx, CGeoRect<double> &extent)
{
	int layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		extent = m_layer.m_mbr;
		return true;
	}

	return false;
}


/**
*
**/
inline void CGeoIndexImpl::GetParcelID(unsigned long gateIdx, const CGeoRect<int> &directions, CMemVector &indice)
{
	// Currently this function mainly is used by regular grid index mechansim
	assert(false);
}

/**
*
**/
inline void CGeoIndexImpl::GetParcelDirections(unsigned long gateIdx, unsigned long parcelIdx, CGeoRect<int> &directions)
{
	// Currently this function is mainly designed for regular index mechansim
	assert(false);
}

/**
*
**/
inline void CGeoIndexImpl::GetTileDirections(unsigned long gateIdx, CGeoRect<int> &directions)
{
	// Currently this function is mainly designed for regular index mechansim
	assert(false);
}
