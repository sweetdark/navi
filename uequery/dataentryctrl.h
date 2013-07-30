#ifndef _UEQUERY_DATAENTRYCTRL_H
#define _UEQUERY_DATAENTRYCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_QUERYDEF_H
#include "querybasic.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	class UEQUERY_CLASS CDataEntryCtrl
	{
	public:
		CDataEntryCtrl(void) {};
		virtual ~CDataEntryCtrl(void) {};
		virtual unsigned Open(void) = 0;
		virtual unsigned Close(void) = 0;
		//
		static void Log(const TCHAR *format, ...);
		static void GetDataPath(tstring &tstrPath);
		static void GetDataScrPath(tstring &tstrPath);
		static void GetNameDataPath(tstring &tstrPath);
		//获取区域数目
		virtual unsigned GetDistCount(void) const = 0;
		//获取数据数目
		virtual unsigned GetDataCount(void) const = 0;
		//设置所需要的结果索引/编号，获取类型
		virtual unsigned GetKind(long docIdx) = 0;
		//设置所需要的结果索引/编号，获取区域编码
		virtual unsigned GetCode(long distIdx) const = 0;
		virtual bool GetSubDistIndex(unsigned fDistCode,
			defDistCodeType &sStartIdx,defDistCodeType &sEndIdx) const = 0;
		//设置所需要的结果索引/编号，获取名称长度
		virtual unsigned GetNameLen(long docIdx)  = 0;
		//设置所需要的结果索引/编号，填充查询结果
		virtual SQLRecord *FixRecord(long docIdx) = 0;
		/* 通过文档索引/编号获取poi名称
		 * docIdx：文档索引
		 * nameLen：该poi的长度
		 * pchNameBuffer：保存名称的缓存
		*/
		virtual bool GetEntryName(long docIdx,long nameLen,char *pchNameBuffer) = 0;
	};
}
#endif