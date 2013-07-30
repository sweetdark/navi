#ifndef _UEQUERY_POIENTRYCTRL_H
#define _UEQUERY_POIENTRYCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
#ifndef _UEMODEL_UENETBASIC_H
#include "uemodel/uenetbasic.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	#pragma pack(1)
	struct TPoiEntry
	{
		TPoiEntry(void)
		{
			::memset(this,0,sizeof(*this));
		}
		bool operator < (const TPoiEntry &other) const;
		bool operator == (const TPoiEntry &other) const;
		//
		static bool PoiEntryPtrCmp(const TPoiEntry *first,const TPoiEntry *second)
		{
			return (*first)<(*second);
		}
		//
		long m_x;
		long m_y;
		long m_distCode;
		long m_telOffset;
		long m_nameOffset;
		long m_addrOffset;
		unsigned short m_kind;
		TPoiEntry *m_pNextPoi;
	};
	struct TPoiEntry2th
	{
		TPoiEntry2th(void):m_x(0),m_y(0),
			m_distCode(0),m_telOffset(0),
			m_nameOffset(0),m_addrOffset(0) {};
		//
		UeModel::UeLongInteger m_x;
		UeModel::UeLongInteger m_y;
		unsigned short m_distCode;
		unsigned long m_telOffset;
		unsigned long m_nameOffset;
		unsigned long m_addrOffset;
	};
	#pragma pack()
	class UEQUERY_CLASS CPoiEntryCtrl
	{
	public:
		CPoiEntryCtrl(bool bReadMode):m_bReadMode(bReadMode),
			m_wPoiCount(0),m_pRPoiTelHandle(0),m_pRPoiNameHandle(0),
			m_pRPoiFileHandle(0),m_pWPoiFileHandle(0),m_bufDocBeg(0),
			m_pEndNameRecord(m_pNameRecord+g_NameBufDocCnt-1) {};
		virtual ~CPoiEntryCtrl(void) {Close();};
		//
		virtual unsigned Open(void)
		{
			unsigned rt(InitReadMode());
			if (rt==SQL_Success && !m_bReadMode)
			{
				rt = InitWriteMode();
			}
			return rt;
		}
		virtual unsigned Close(void);
		//转换数据
		void SetWPoiCount(long poiCount)
		{
			m_wPoiCount = poiCount;
			m_nameLenOffset = sizeof(m_indexHead);
			m_dataEntryOffset = m_nameLenOffset+poiCount;
		}
		bool AddPoiEntry(const TPoiEntry &poiEntry);
		bool GetPoiEntry(long idx,UeQuery::TableEntry &poiEntry);
		bool GetPoiTelephone(unsigned telOffset,char *pchTelBuffer);
		bool GetPoiEntryName(long nameOffset,char *pchNameBuffer,unsigned char &uStrLen);
		/*通过文档索引获取poi名称
		 * docIdx：文档索引
		 * nameLen：该poi的长度
		 * pchNameBuffer：保存名称的缓存
		*/
		bool GetPoiEntryNameByIdx(long docIdx,long nameLen,char *pchNameBuffer);
		//获取转换后的数据
		bool GetPoiEntry2th(long idx,TPoiEntry2th &poiEntry);
		bool GetPoiEntry2th(long startIdx,TPoiEntry2th *pPoiEntryBuf,int returnCnt);
		//
		virtual unsigned GetNameLen(long idx);
		virtual SQLRecord *FixRecord(long docIdx);
		unsigned GetPoiCount(void) const {return m_indexHead.m_rPoiCount;};
	private:
		#pragma pack(1)
		struct TIndexHead
		{
			TIndexHead(void):m_poiDBVer(0),
				m_rPoiCount(0)
			{
				::memcpy(head,DATA_HEAD,sizeof(head));
			}
			char head[16];
			unsigned short m_poiDBVer;
			unsigned m_rPoiCount;
		};
		struct TNameRecord
		{
			//按从小到大进行排序
			static bool Compare(const TNameRecord &first,const TNameRecord &second)
			{
				return first.m_docIdx<second.m_docIdx;
			}
			//
			long m_docIdx;
			unsigned char m_len;
			char m_nameStr[127];
		};
		#pragma pack()
		unsigned InitReadMode(void);
		unsigned InitWriteMode(void);
		//
		const TNameRecord *GetNameRecord(long docIdx) const;
		//
		const static long g_bufDocCnt = 100000;
		const static long g_NameBufDocCnt = 1000;
		//
		bool m_bReadMode;
		TIndexHead m_indexHead;
		//写模式下保存原来的poi数目，读模式下保存
		//新版数据的数目
		unsigned m_wPoiCount;
		unsigned m_nameLenOffset;
		unsigned m_dataEntryOffset;
		//
		void *m_pRPoiTelHandle;
		void *m_pRPoiNameHandle;
		void *m_pRPoiFileHandle;
		//
		void *m_pWPoiFileHandle;
		//
		long m_bufDocBeg;
		TNameRecord *m_pCurNameRecord;
		TNameRecord *m_pEndNameRecord;
		unsigned char m_pNameLen[g_bufDocCnt];
		TNameRecord m_pNameRecord[g_NameBufDocCnt];
	};
}
#endif