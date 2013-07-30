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
#include "geogramindex.h"
using namespace UeIndex;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor
//
/**
* Read index information then construct "layer-tile" structure in advance
*/
CGeoGramIndex::CGeoGramIndex(const tstring &indexFile, GeoLayer &curLayer, bool isTiled) : m_parcels(0), CGeoIndexImpl(indexFile, curLayer, isTiled)
{
	CreateIndex(indexFile);
}

/**
*
*/
CGeoGramIndex::~CGeoGramIndex()
{
	// Release indices for this Layer-Tile structure
	int i = 0;
	int tiles = m_layer.m_xCount * m_layer.m_yCount;
	for(; i < tiles; i++)
	{
		::free(m_parcels[i]);
	}

	//
	::free(m_parcels);
	m_parcels = 0;
}

/**
* Binary predicate seperately using Y and X values
*/
unsigned long CGeoGramIndex::GetParcelID(unsigned long gateIdx, const CGeoPoint<double> &location)
{
	unsigned short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		unsigned short tileIdx = GetTileID(layerIdx, location);
		assert(IsValidTile(tileIdx));

		if(tileIdx != -1)
		{
			GeoTile &curTile = m_layer.m_tiles[tileIdx];
			int parcels = curTile.m_xCount;

			// Note:
			// Here m_xCount means the total number of parcels in this tile and m_yCount means the minimum
			// height of parcels in this tile
			int start = 0;
			int end = parcels - 1;
			int mid = (start + end) / 2;
			mid = BinaryPredicateY(layerIdx, tileIdx, start, end, mid, location);

			// Binary predicate as X value
			// Get the total number of elements belonging to the same Y value
			int count = mid;
			CGeoRect<double> cur = m_parcels[tileIdx][count]->m_mbr;
			CGeoRect<double> next = cur; 
			while((count < parcels - 1) && (cur.m_minY == next.m_minY))
			{
				count++;
				next = m_parcels[tileIdx][count]->m_mbr;
			}

			// Predication in above element extent
			start = mid;
			end = count;
			mid = (start + end)/2;
			mid = BinaryPredicateX(layerIdx, tileIdx, start, end, mid, location);

			// 
			return tileIdx << 24 | m_parcels[tileIdx][mid]->m_idx;
		}
	}

	return -1;
}

/*
*
*/
void CGeoGramIndex::GetParcelID(unsigned long gateIdx, const CGeoRect<double> &mbr, double tolerance, CMemVector &indice)
{
	unsigned short layerIdx = GetLayerID(gateIdx);
	assert(IsValidLayer(layerIdx));

	if(IsValidLayer(layerIdx))
	{
		CMemVector tileIdxes(sizeof(unsigned short));
		GetTileID(layerIdx, mbr, tolerance, tileIdxes);
		assert(tileIdxes.GetCount());

		// Infalted or deflated rectangle
		CGeoRect<double> cur;
		cur.m_minX = mbr.m_minX - tolerance;
		cur.m_maxX = mbr.m_maxX + tolerance;
		cur.m_minY = mbr.m_minY - tolerance;
		cur.m_maxY = mbr.m_maxY + tolerance;

		//
		int i = 0;
		int count = tileIdxes.GetCount();
		for(; i < count; i++)
		{
			unsigned short tileIdx = *(reinterpret_cast<unsigned short *>(tileIdxes[i]));
			GeoTile &curTile = m_layer.m_tiles[tileIdx];
			int parcels = curTile.m_xCount;

			// Binary predicate to locate the rough extent 
			CGeoPoint<double> location(cur.m_minX, cur.m_minY);
			int start = 0;
			int end = parcels - 1;
			int yStart = (start+end)/2;
			yStart = BinaryPredicateY(layerIdx, tileIdx, start, end, yStart, location);

			location.m_x = cur.m_maxX;
			location.m_y = cur.m_maxY;
			int yEnd = (start+end)/2;
			yEnd = BinaryPredicateY(layerIdx, tileIdx, start, end, yEnd, location);

			// Sequence predicate to locate concrete extent
			int j = yStart;
			for(; j < yEnd; j++)
			{
				CGeoRect<double> &curMbr = m_parcels[tileIdx][j]->m_mbr;
				if(curMbr.IsIntersect(mbr))
				{
					unsigned long idx = tileIdx << 24 | m_parcels[tileIdx][j]->m_idx;
					indice.Add(&idx);
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
inline long CGeoGramIndex::GetParcelCount(unsigned long gateIdx)
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
void CGeoGramIndex::CreateIndex(const tstring &indexFile)
{
	//int i = 0;
	//for(; i < PT_Max; i++)
	//{
	//	NetLayer &oneLayer = m_layers[i];
	//	oneLayer.m_idx = i;
	//	oneLayer.m_xCount = 2;
	//	oneLayer.m_yCount = 2;
	//	oneLayer.m_tiles = new NetTile[oneLayer.m_xCount * oneLayer.m_yCount];

	//	oneLayer.m_mbr.m_minX = 0;
	//	oneLayer.m_mbr.m_minY = 0;
	//	oneLayer.m_mbr.m_maxY = 10000;
	//	oneLayer.m_mbr.m_maxX = 10000;

	//	int j = 0;
	//	for(; j < oneLayer.m_yCount; j++)
	//	{
	//		int k = 0;
	//		for(; k < oneLayer.m_xCount; k++)
	//		{
	//			int tileIdx = j * oneLayer.m_xCount + k;
	//			NetTile &oneTile = oneLayer.m_tiles[tileIdx];
	//			
	//			oneTile.m_idx = tileIdx;
	//			oneTile.m_mbr.m_minX = oneLayer.m_mbr.m_minX + k * oneLayer.m_mbr.Width() / oneLayer.m_xCount;
	//			oneTile.m_mbr.m_minY = oneLayer.m_mbr.m_minY + j * oneLayer.m_mbr.Height() / oneLayer.m_yCount;
	//			oneTile.m_mbr.m_maxX = oneTile.m_mbr.m_minX + oneLayer.m_mbr.Width() / oneLayer.m_xCount;
	//			oneTile.m_mbr.m_maxY = oneTile.m_mbr.m_minY + oneLayer.m_mbr.Height() / oneLayer.m_yCount;

	//			oneTile.m_xCount = 100 * 100;
	//			oneTile.m_yCount = oneTile.m_mbr.Height()/100;
	//			oneTile.m_parcels = new NetParcel[oneTile.m_xCount];

	//			int m = 0;
	//			for(; m < 100; m++)
	//			{
	//				int n = 0;
	//				for(; n < 100; n++)
	//				{
	//					int parcelIdx = m * 100 + n;

	//					NetParcel &oneParcel = oneTile.m_parcels[parcelIdx];
	//					oneParcel.m_idx = parcelIdx;
	//					oneParcel.m_mbr.m_minX = oneTile.m_mbr.m_minX + n * oneTile.m_mbr.Width() / 100;
	//					oneParcel.m_mbr.m_minY = oneTile.m_mbr.m_minY + m * oneTile.m_mbr.Height() / 100;
	//					oneParcel.m_mbr.m_maxX = oneParcel.m_mbr.m_minX + oneTile.m_mbr.Width() / 100;
	//					oneParcel.m_mbr.m_maxY = oneParcel.m_mbr.m_minY + oneTile.m_mbr.Height() / 100;
	//				}
	//			}
	//		}
	//	}
	//}

	///// Allocate memory for indexing those parcels in one tile
	//assert(!m_parcels);
	//m_parcels = reinterpret_cast<NetParcel* ***>(::malloc(sizeof(NetParcel*) * m_layerNum));

	///// Secondly to set up histogram table
	//int histogramNum = 0;
	//SortedY *histogram = 0;

	//i = 0;
	//for(; i < m_layerNum; i++)
	//{
	//	NetLayer &oneLayer = m_layers[i];
	//	int tiles = oneLayer.m_xCount * oneLayer.m_yCount;
	//	m_parcels[i] = reinterpret_cast<NetParcel* **>(::malloc(sizeof(NetParcel*) * tiles));

	//	int j = 0;
	//	for(; j < tiles; j++)
	//	{
	//		NetTile &oneTile = oneLayer.m_tiles[j];
	//		int parcels = oneTile.m_xCount;
	//		m_parcels[i][j] = reinterpret_cast<NetParcel* *>(::malloc(sizeof(NetParcel*) * parcels));

	//		/// Note: 
	//		/// here m_yCount make special sense and this value can be got by getting the minimum height
	//		/// of those parcels in this tile
	//		CGeoRect<double> &tileExtent = oneTile.m_mbr;
	//		if(histogramNum < tileExtent.Height() / oneTile.m_yCount)
	//		{
	//			if(histogram)
	//			{
	//				::free(histogram);
	//			}

	//			histogramNum = tileExtent.Height() / oneTile.m_yCount + .5;
	//			histogram = reinterpret_cast<SortedY *>(::malloc(sizeof(SortedY) * histogramNum));
	//		}
	//		::memset(histogram, 0x00, sizeof(SortedY) * histogramNum);
	//		
	//		/// Get Y-histogram
	//		int k = 0;
	//		for(; k < parcels; k++)
	//		{
	//			NetParcel &oneParcel = oneLayer.m_tiles[j].m_parcels[k];

	//			/// Note:
	//			/// It had better specifiy one value range for one subscript else there maybe many and 
	//			/// many subscripts caused by calculation round off issues
	//			int subscript = (oneParcel.m_mbr.m_minY - tileExtent.m_minY) / oneTile.m_yCount + .5;
	//			histogram[subscript].m_start++;
	//		}

	//		/// Convert Y-histogram into the array index of zero value
	//		k = 0;
	//		int start = 0;
	//		for(; k < histogramNum; k++)
	//		{
	//			int tmp = histogram[k].m_start;
	//			histogram[k].m_start = start;
	//			start += tmp;
	//		}

	//		/// Fill pointers just allocated above ordered by Y value
	//		k = 0;
	//		for(; k < parcels; k++)
	//		{
	//			NetParcel &oneParcel = oneLayer.m_tiles[j].m_parcels[k];
	//			int subscript = (oneParcel.m_mbr.m_minY - tileExtent.m_minY) / oneTile.m_yCount + .5;

	//			SortedY &sort = histogram[subscript];
	//			m_parcels[i][j][sort.m_start + sort.m_num] = &oneParcel;
	//			sort.m_num++;
	//		}

	//		/// Next to order parcel in X value
	//		k = 0;
	//		for(; k < histogramNum; k++)
	//		{
	//			SortedY &sort = histogram[k];
	//			if(sort.m_num)
	//			{
	//				QSort(&m_parcels[i][j][sort.m_start], sort.m_num);
	//			}
	//		}
	//	}
	//}

	///// Never forget to release memory
	//::free(histogram);
}

/**
*
*/
void CGeoGramIndex::QSort(GeoParcel **parcels, int num)
{
	//
    GeoParcel **limit = parcels + num;
    GeoParcel **base = parcels;

    GeoParcel **stack[80];
    GeoParcel ***top = stack;

    for(;;)
    {
        // Use base + len/2 as the pivot
        int len = static_cast<int>(limit - base);
		GeoParcel **pivot = 0;
		GeoParcel **i = 0;
		GeoParcel **j = 0;

		if(len > 9)
		{
			pivot = base + len / 2;
			Swap(base, pivot);

			i = base + 1;
			j = limit - 1;

			// Ensure that i <= base <= j 
			if((*j)->m_mbr.m_minX < (*i)->m_mbr.m_minX)
			{
				Swap(i, j);
			}

			if((*base)->m_mbr.m_minX < (*i)->m_mbr.m_minX)
			{
				Swap(base, i);
			}

			if((*j)->m_mbr.m_minX < (*base)->m_mbr.m_minX)
			{
				Swap(base, j);
			}

			for(;;)
			{
				int x = (*base)->m_mbr.m_minX;
				do i++; while((*i)->m_mbr.m_minX < x);
				do j--; while(x < (*j)->m_mbr.m_minX);

				if(i > j)
				{
					break;
				}

				Swap(i, j);
			}
			Swap(base, j);

			// Push the largest sub-array
			if(j - base > limit - i)
			{
				top[0] = base;
				top[1] = j;
				base   = i;
			}
			else
			{
				top[0] = i;
				top[1] = limit;
				limit  = j;
			}
			top += 2;
		}
		else
		{
            // The sub-array is small, perform insertion sort
            j = base;
            i = j + 1;

            for(; i < limit; j = i, i++)
            {
				for(; j[1]->m_mbr.m_minX < (*j)->m_mbr.m_minX; j--)
                {
                    Swap(j + 1, j);
                    if (j == base)
                    {
                        break;
                    }
                }
            }

            if(top > stack)
            {
                top  -= 2;
                base  = top[0];
                limit = top[1];
            }
            else
            {
                break;
            }
		}
	}
}

/**
*
*/
inline void CGeoGramIndex::Swap(GeoParcel **a, GeoParcel **b)
{
	GeoParcel *tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
*
*/
inline int CGeoGramIndex::BinaryPredicateY(unsigned short layerIdx, unsigned short tileIdx, int start, int end, int mid, const CGeoPoint<double> &location)
{
	// Binary predicate as Y value
	CGeoRect<double> mbr = m_parcels[tileIdx][mid]->m_mbr;
	while(true)
	{
		if(location.m_y < mbr.m_minY)
		{
			end = mid - 1;
		}
		else if(location.m_y > mbr.m_maxY)
		{
			start = mid + 1;
		}
		else
		{
			// Until find the border
			while(mid-1 >= 0 && location.m_y >= m_parcels[tileIdx][mid-1]->m_mbr.m_minY && 
				location.m_y <= m_parcels[tileIdx][mid-1]->m_mbr.m_maxY)
			{
				mid--;
			}
			
			break;
		}

		// Dead loop
		mid = (start + end) / 2;
		if(::abs(end - start) == 1)
		{
			// Until find the border
			while(mid-1 >= 0 && location.m_y >= m_parcels[tileIdx][mid-1]->m_mbr.m_minY && 
				location.m_y <= m_parcels[tileIdx][mid-1]->m_mbr.m_maxY)
			{
				mid--;
			}

			break;
		}
		mbr = m_parcels[tileIdx][mid]->m_mbr;
	}

	return mid;
}

/**
*
*/
inline int CGeoGramIndex::BinaryPredicateX(unsigned short layerIdx, unsigned short tileIdx, int start, int end, int mid, const CGeoPoint<double> &location)
{
	// Binary predicate as Y value
	CGeoRect<double> mbr = m_parcels[tileIdx][mid]->m_mbr;
	while(true)
	{
		if(location.m_x < mbr.m_minX)
		{
			end = mid - 1;
		}
		else if(location.m_x > mbr.m_maxX)
		{
			start = mid + 1;
		}
		else
		{
			// Until find the border
			while(mid-1 >= 0 && location.m_x >= m_parcels[tileIdx][mid-1]->m_mbr.m_minX && 
				location.m_x <= m_parcels[tileIdx][mid-1]->m_mbr.m_maxX)
			{
				mid--;
			}
			break;
		}

		// Dead loop
		mid = (start + end) / 2;
		if(::abs(end - start) == 1)
		{
			// Until find the border
			while(mid-1 >= 0 && location.m_x >= m_parcels[tileIdx][mid-1]->m_mbr.m_minX && 
				location.m_x <= m_parcels[tileIdx][mid-1]->m_mbr.m_maxX)
			{
				mid--;
			}

			break;
		}
		mbr = m_parcels[tileIdx][mid]->m_mbr;
	}

	return mid;
}