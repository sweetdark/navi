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
#ifndef _UEQUERY_QUERYDEF_H
#define _UEQUERY_QUERYDEF_H

// Refer to EXPORT & IMPORT macro defintions
#ifndef _UEQUERY_BASE_H
#include "ueinfo.h"
#endif

// Refer to portable types
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to geometry basic definition
#ifndef _UEBASE_GEOMBASIC_H
#include "uebase\geombasic.h"
#endif
#define USE_NEW_DATA
//#define CREATE_DATA_MODE
//#define CREATE_PTIRE_DATA
//#define MAKE_NEXTWORD_DATA

const static char DATA_HEAD[] = "SmartPoint/Herman";

using namespace UeBase;
// Declare namespace
namespace UeQuery
{
	typedef void QUERY_CALLBACK(void);
	//
	typedef unsigned short defPoiKindType;
	typedef unsigned short defDistCodeType;
	//分词位置类型
	typedef char defTermPosType;
	//分词编码类型
	typedef unsigned short defTermType;
	static const long g_maxWordPosCnt = 32;
	/**
	* \brief 查询数据
	* Data base types
	**/
	enum DBType
	{
		//包括DT_General,DT_Address,DT_Favorite
		DT_Misc = 0,
		DT_General,
		//pois.db数据
		DT_PoiDB,
		DT_RoadDB,
		DT_CityDB,
		//crosses.db数据
		DT_Address,
		DT_Table,
		//favors.db数据
		DT_Favorite,
		//recents.db数据
		DT_Recents,
		//hisrecords.db数据
		DT_HistoryRecords
	};

	/**
	* \brief
	* Index type 查询方式
	**/
	enum IndexType
	{
		IT_Invalid = 0,
		//周边查询
		IT_Grid,
		IT_NearByPos,
		IT_NearByRoad,
		IT_NearByRoute,
		//类型查询
		IT_Kind,
		IT_Legend,
		IT_Addr,
		IT_Acronym,
		//交叉路口查询
		IT_Cross,
		IT_QuickWord,
		IT_CrossEx,
		IT_Address,
		//
		IT_PoiName,
		IT_PoiAcro,
		IT_RoadName,
		IT_RoadAcro,
		IT_CityName,
		IT_CityAcro,
	};

	/**
	* \brief 查询结果展现给用的一个排序方式
	* Results fetched are ordered as ...
	**/
	enum OrderType
	{
		OT_None = 0,
		//距离屏幕中心点由近到远排序
		OT_Dist,
		//拼音简写从小到大排序
		OT_Ascii,
		//汉字从小到大排序
		OT_Unicode,
		OT_Idx,
		//暂时没用
		OT_Candidate
	};
  enum RecordDir
  {
    RT_North,
    RT_South,
    RT_West,
    RT_East,
    RT_NorthEast,
    RT_SouthEast,
    RT_SouthWest,
    RT_NorthWest,
  };
	enum TermDataMode
	{
		TM_UNKNOWN,
		TM_POINAME,
		TM_POIACRO,
		TM_ROADNAME,
		TM_ROADACRO,
		TM_CITYNAME,
		TM_CITYACRO,
	};
	/**
	* \brief 信息检索查询条件
	* Primitive SQL sentence for querying
	**/
	struct SQLSentence
	{
		SQLSentence(void):m_indexType(IT_Invalid),m_addrOne(-1),
			m_radius(0.),m_kind(0),m_bIsCheckHave(false),m_numOfWantTo(0),m_pvecNextWord(0),
			m_pFilterNextWord(0),m_isProxy(false),m_misses(6),m_isFromCandidates(false) {}

		// For length specification of one name of current SQL sentence
		const static int m_maxStrLen = 256;
		// Index method
		IndexType m_indexType;
		//行政区域编码(1byte省编码|1byte城市编码|1byte区县编码)
		unsigned m_addrOne;
		unsigned m_addrTwo;
		//周边中心点
		CGeoPoint<long> m_pos;
		//周边半径
		double m_radius;
		//周边矩形区域，用于结果一级过滤
		CGeoRect<long> m_srchExtent;
		//检索poi类型(4bits大类编码|4bits中类编码|8bits小类编码)
		long m_kind;
		//检索关键字
		char m_acronyms[m_maxStrLen];
		tstring m_uniName;

		//是否只是判断是否有该结果
		bool m_bIsCheckHave;
		//需要检索多少数据
		unsigned m_numOfWantTo;
		//保存当前关键字检索中下个汉字或字符
		std::vector<string> *m_pvecNextWord;
		unsigned short *m_pFilterNextWord;
		//
		OrderType m_orderType;
		bool m_isProxy; // it should be proxy record or not
		unsigned char m_misses; // For Acronym index usgage
		// Filter code for getting what we want
		bool m_isFromCandidates; // Whether this sentence would be imposed on current selected results not
									// do real query action again and again
	};
	//
	struct TTermInfo
	{
		TTermInfo(void):m_frq(0),m_idx(0)
		{
			::memset(this,0,sizeof(*this));
		};
		static const long g_maxTermNum = g_maxWordPosCnt;
		//
		bool NextPosition(void)
		{
			if (++m_idx>=m_frq)
				return false;
			return true;
		}
		defTermPosType Position(void) const
		{
			return m_pos[m_idx];
		}
		//
		defTermType m_code;
		//我们的数据字数及长度要求低于unsigned char表示的范围
		defTermPosType m_frq;
		//
		defTermPosType m_idx;
		defTermPosType m_pos[g_maxTermNum];
#ifdef MAKE_NEXTWORD_DATA
		defTermType m_nextWord[g_maxTermNum];
#endif
	};
	#pragma pack(1)
	/**
	* \brief pois.db中的poi信息
	* Stored unit as one entry of main body file
	**/
	struct TableEntry
	{
#ifndef USE_NEW_DATA
		TableEntry(void):m_x(0),m_y(0),m_gridX(0),
			m_gridY(0), m_kind(0), m_addrCode(-1),
			m_teleNumber(0),m_nameOffset(-1) //, m_streetOffset(-1) , m_houseOffset(-1), m_zipOffset(-1)
		{
		}
		// Position
		//poi相对于其所在网格的经度
		unsigned short m_x;
		//poi相对于其所在网格的纬度
		unsigned short m_y;
		//poi所在网格纵向索引
		unsigned short m_gridX;
		//poi所在网格横向索引
		unsigned short m_gridY;

		// Code indicating POI categrory
		// Code for the following address search
		//poi所属类型
		unsigned short m_kind;
		//poi区域编号
		unsigned short m_addrCode;
#else
		TableEntry(void):m_x(0),m_y(0),m_addrCode(-1),
			m_teleNumber(0),m_nameOffset(-1) //, m_streetOffset(-1) , m_houseOffset(-1), m_zipOffset(-1)
		{
		}
		long m_x;
		long m_y;
		unsigned m_addrCode;
#endif
		//电话号码
		unsigned int m_teleNumber;	// Note: No area code for this telenumber

		// Useful information in another file
		//poi名称在名称文件中的偏移量
		long m_nameOffset;
		long m_addrOffset;

		// TODO:
		// As far, for mainland there is no need to define offsets for one house or zip code
		//long m_streetOffset;
		//long m_houseOffset;
		//long m_zipOffset;
	};
	/**
	* Stored unit as one entry of main body file
	**/
	struct FavoriteEntry
	{
		//
		const static int MAXFAVORITES = 256;

		// Position
		unsigned int m_x;
		unsigned int m_y;

		// Code indicating POI categrory
		// Code for the following address search
		unsigned short m_kind;
		unsigned int m_addrCode;
		unsigned int m_teleNumber;

		// TODO:
		// Remove magic number
		unsigned char m_name[128];
		unsigned char m_addr[128];
		unsigned char m_telphone[128];

		/**
		*
		**/
		FavoriteEntry() :m_x(0), m_y(0), m_kind(0),m_addrCode(-1),m_teleNumber(-1)
		{
			::memset(m_name, 0x00, 128);
		}

		/**
		*
		**/
		bool IsSameWith(const FavoriteEntry &other)
		{
			return !(m_x != other.m_x || m_y != other.m_y ||
				::strcmp((char *)m_name, (char *)other.m_name));
		}
	};

	/**
	* Stored unit as one entry of main body file  
	**/
	struct HistoryRecordEntry	//历史记录
	{
		const static int MAXHISRECORDS = 256; //最大历史记录条数
		const static int MAXADDRNAMELENGTH = 128;	//地址名最大长度

		//经纬度 高度
		unsigned int m_X;
		unsigned int m_Y;

		//地址名
		unsigned char m_addrName[MAXADDRNAMELENGTH];

		HistoryRecordEntry():m_X(0),m_Y(0)
		{
			::memset(m_addrName,0x00,MAXADDRNAMELENGTH);
		}
		bool IsSameWith(const HistoryRecordEntry &other) const
		{
      if (this==&other)
        return true;
      return (m_X==other.m_X && m_Y==other.m_Y
        && !::strcmp((char*)m_addrName,(char*)other.m_addrName));
		}

	};

	/**
	* Stored unit as one entry of main body file
	**/
	struct RecentEntry
	{
		const static int MAXRECENTS = 128;
    //线路上的最多的点数(起点、终点、经由点的数目)
    const static long MAXROUTEPOSCNT=6;
		RecentEntry(void):m_routePosCnt(0),
			m_timeStamp(0.),m_routeMode(-1)
		{
			::memset(m_recentName,0x00,MAXRECENTS);
		}
		bool IsSameWith(const RecentEntry &other) const
		{
      if (this==&other)
        return true;
      if (m_routePosCnt!=other.m_routePosCnt)
        return false;
      //
      bool bIsSame(true);
      for (long i(0); i<m_routePosCnt; ++i)
      {
        if (!m_routePos[i].IsSameWith(other.m_routePos[i]))
        {
          bIsSame = false;
          break;
        }
      }
      return bIsSame;
		}
    //线路上的点数
    long m_routePosCnt;
    HistoryRecordEntry m_routePos[MAXROUTEPOSCNT];
		//
		double m_timeStamp;
		char m_recentName[MAXRECENTS];
		//规划方式
		unsigned m_routeMode;
	};
	#pragma pack()

	#pragma pack(2)
	//
	struct TDocIdxInfo
	{
		TDocIdxInfo(void):m_idx(-1),
			m_distIdx(0) {};
		//
		long m_idx;
		union
		{
			double m_dist2th;
			unsigned short m_distIdx;
		};
	};
	/**
	* \brief 查询结果
	**/
	struct SQLRecord
	{
		// Global variables for outside directly use
		static tstring m_candidateInfo;

		// Idx
		//在pois.db数据中的poi数据的索引
		int m_idx;

		// Position
		//POI/Cross 用来保存其经度，Road用来保存其网格索引
		union
		{
			long m_x;
			long m_parcelIdx;
			long m_crossOffset;
		};
		//POI/Cross 用来保存其纬度，Road用来保存其在网格中的link索引
		union
		{
			long m_y;
			long m_linkIdx;
			long m_linkOffset;
		};
		//保存行政区域编码
		unsigned int m_addrCode;
		//POI 保存电话号码信息，Road用来保存名称偏移
		union
		{
			unsigned m_telephone;
			unsigned m_nameOffset;
		};

		// Spatial information
		unsigned char m_direction;
		//POI周边保存距中心点的距离，Road用来保存开始的经度
		union
		{
			unsigned long m_dist;// Distance to specified position
			long m_startX;
		};
		double m_dist2th;
		//POI周边保存距中心点的方位，Road用来保存开始的纬度
		union
		{
			int m_azimuth;
			long m_startY;
		};
		// Note:
		//poi名称长度,不包括‘\0’
		unsigned char m_uniLen;
		unsigned char m_asciiLen;
		//poi名称
		char *m_uniName;
		//poi名称对应的拼音简写
		char *m_asciiName;
		//
		char *m_pchTelStr;
		char *m_pchAddrStr;
		/**
		*
		**/
		SQLRecord() : m_idx(0), m_x(0), m_y(0), m_dist(0), m_uniLen(0), m_direction(0), m_uniName(0), m_asciiLen(0), m_asciiName(0), m_addrCode(0), m_telephone(0)
		{
		}

		/**
		*
		**/
		static unsigned short GetFirstCategory(unsigned short kind)
		{
			return (kind >> 8 & 0x00FF);
		}

		/**
		*
		**/
		static unsigned short GetSecondCategory(unsigned short kind)
		{
			return (kind & 0x00FF);
		}

		/**
		*
		**/
		static unsigned short MakeCategory(unsigned short first, unsigned short second)
		{
			return ((first << 8) | (second & 0x00FF));
		}
	};
	#pragma pack()

	//
	const static int SQLRECORDSIZE = sizeof(SQLRecord);

	/**
	* Error code
	**/
	enum SQLErrorCode
	{
		SQL_Success					= 0,

		// DB initialization
		SQL_NotExistDB				= 0x01000001,
		SQL_ConnectFailure			= 0x01000002,

		// Index initialization
		SQL_NotSpecifyIndex			= 0x02000001,
		SQL_InvalidIndex			= 0x02000002,
		SQL_InitIndexFailure		= 0x02000003,
		SQL_InvalidSentence			= 0x02000004,
		SQL_NotExistIndex			= 0x02000005,

		// Result information
		SQL_ZeroRecord				= 0x04000001,
		SQL_MeetTail				= 0x04000002,
		SQL_Filtered				= 0x04000003,
		SQL_InvalidRecord			= 0x04000004,
		SQL_ExistedRecord			= 0x04000005,
	};

	/**
	*
	**/
	struct SQLSetting
	{
		// Specify fetching behavior
		int m_pageSize;
		int m_cursor;
		int m_curPage;
		int m_pages;

		// Inner index
		int m_curIdx;
		int m_startIdx;
		int m_endIdx;

		/**
		*
		**/
		SQLSetting() : m_pageSize(100), m_cursor(0), m_curPage(0), m_pages(0), m_curIdx(0), m_startIdx(0), m_endIdx(0)
		{
		}

		/**
		*
		**/
		const SQLSetting &operator = (const SQLSetting &other)
		{
			//
			if(this == &other)
			{
				return *this;
			}

			//
			m_pageSize = other.m_pageSize;
			m_cursor = other.m_cursor;
			m_curPage = other.m_curPage;
			m_pages = other.m_pages;

			//
			return *this;
		}
	};

	/**
	*
	**/
	struct BodyDesc
	{
		//
		long m_startX;
		long m_startY;
		long m_gapX;
		long m_gapY;
		short m_xCount;
		short m_yCount;

		/**
		*
		**/
		BodyDesc() : m_startX(0), m_startY(0), m_gapX(0), m_gapY(0), m_xCount(0), m_yCount(0)
		{
		}
	};

	/**
	*
	**/
	enum AddrLevel
	{
		AL_Invalid = 0,
		AL_State,
		AL_City,
		AL_Town,
		AL_Street,
		AL_Cross,
		AL_Max
	};
}
#endif