#ifndef _UEGUI_SELECTPOINTCALLBACKCTRL_H
#define _UEGUI_SELECTPOINTCALLBACKCTRL_H
//////////////////////////////////////////////////////////////////////////
#ifndef _UEQUERY_CODEINDEXCTRL_H
#include "uequery/uerecord.h"
#endif
#ifndef _UEQUERY_QUERYDEF_H
#include "uequery/querybasic.h"
#endif
#ifndef _UEGUI_GUIBASIC_H
#include "guibasic.h"
#endif
namespace UeGui
{
  class CSelectPointCallBackCtrl
  {
  public:
    static CSelectPointCallBackCtrl &Get();
    //用于图上选点的回凋函数
    void SetCallBackFun(void *pDoCallBackObj, SelectPointEvent pCallBackEvent);
    //设置选点并回调
    void SetSelectedPoint(const UeQuery::SQLRecord *record);
    //设置是否有回调函数
    void SetIsCallBackFunExist(bool isExist);
    //判断是否有回调函数
    bool IsCallBackFunExist();
    void *GetCallBackObj();
    SelectPointEvent GetEvent();
  private:
    CSelectPointCallBackCtrl();
    SelectPointEvent m_queryCallBack;
    void *m_callBackObject;
    bool m_isCallBackFunExist;
  };
}
#endif