#ifndef _UEQUERY_DATAENTRYCTRL_H
#include "dataentryctrl.h"
#endif
#ifndef _UEBASE_DBGTRACER_H
#include "uebase/dbglogger.h"
#endif
#ifndef _UEBASE_PATHBASIC_H
#include "uebase/pathbasic.h"
#endif
#ifndef _UEBASE_CONFIGTOBIN_H
#include "uebase/pathconfig.h"
#endif
//////////////////////////////////////////////////////////////////////////
namespace UeBase
{
	class CDbgLogger;
}
//////////////////////////////////////////////////////////////////////////
namespace UeQuery
{
	void CDataEntryCtrl::Log(const TCHAR *format, ...)
	{
		static UeBase::CDbgLogger logCtrl(_T("uequery"));
		//
		va_list v;
		va_start(v,format);
		//
		TCHAR printBuf[1024];
		vswprintf(printBuf,format,v);
		va_end(v);
		//
		logCtrl.Log(printBuf);
	}
	void CDataEntryCtrl::GetDataPath(tstring &tstrPath)
	{
		tstrPath = CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath);
	}
	void CDataEntryCtrl::GetDataScrPath(tstring &tstrPath)
	{
		tstrPath = CPathConfig::GetRootPath();
    const CPathBasic &pathBasic(CPathBasic::Get());
    pathBasic.GetPathSeperator(tstrPath);
		tstrPath += _T("attrsold");
		pathBasic.GetPathSeperator(tstrPath);
	}
	void CDataEntryCtrl::GetNameDataPath(tstring &tstrPath)
	{
    tstrPath = CPathConfig::GetCommonPath(CPathConfig::CPK_NamesPath);
	}
}