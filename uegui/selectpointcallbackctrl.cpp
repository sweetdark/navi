#include "selectpointcallbackctrl.h"
namespace UeGui
{
  CSelectPointCallBackCtrl &CSelectPointCallBackCtrl::Get()
  {
    static CSelectPointCallBackCtrl selectPointCallBackCtrl;
    return selectPointCallBackCtrl;
  }

  CSelectPointCallBackCtrl::CSelectPointCallBackCtrl()
  {
    m_queryCallBack = NULL;
  }

  void CSelectPointCallBackCtrl::SetCallBackFun(void *pDoCallBackObj, SelectPointEvent pCallBackEvent)
  {
    m_callBackObject = pDoCallBackObj;
    m_queryCallBack = pCallBackEvent;
  }

  void CSelectPointCallBackCtrl::SetSelectedPoint(const UeQuery::SQLRecord *record)
  {
    if (m_queryCallBack != NULL)
    {
      m_queryCallBack(m_callBackObject, record);
      m_queryCallBack = NULL;
    }
  }

  void CSelectPointCallBackCtrl::SetIsCallBackFunExist(bool isExist)
  {
    m_isCallBackFunExist = isExist;
  }

  bool CSelectPointCallBackCtrl::IsCallBackFunExist()
  {
    return m_isCallBackFunExist;
  }

  void *CSelectPointCallBackCtrl::GetCallBackObj()
  {
    return m_callBackObject;
  }

  SelectPointEvent CSelectPointCallBackCtrl::GetEvent()
  {
    return m_queryCallBack;
  }
}