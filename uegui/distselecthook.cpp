#include "distselecthook.h"
using namespace UeGui;

CDistSelectHook::CDistSelectHook()
{
  m_pCurItemCtrl = &CCodeIndexCtrl::GetDistCodeCtrl();
  m_moreIndex = 0;
}

CDistSelectHook::~CDistSelectHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CDistSelectHook::GetBinaryFileName()
{
  return _T("inputselecthook.bin");
}

void CDistSelectHook::Load()
{
  if (m_pCurItemCtrl)
  {
    m_pCurItemCtrl->GetComItem(m_vecListItem);
    m_moreIndex = m_vecListItem.size() - 1;
    if (m_moreIndex > 4 )
    {
      m_moreIndex = 4;
    }
    ShowList();
  }
}

void CDistSelectHook::SetCallBackFun(void *pDoCallBackObj, DistrictCallBack pCallBackEvent)
{
  m_callBackObject = pDoCallBackObj;
  m_queryCallBack = pCallBackEvent;
}

void CDistSelectHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List5Btn,	"List5Btn"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List1BtnLabel,	"List1BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List2BtnLabel,	"List2BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List3BtnLabel,	"List3BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List4BtnLabel,	"List4BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_List5BtnLabel,	"List5BtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_Line4,	"Line4"));
  m_ctrlNames.insert(GuiName::value_type(DistSelectHook_GrayBack,	"GrayBack"));
}

void CDistSelectHook::MakeControls()
{
  m_grayBackCtrl.SetCenterElement(GetGuiElement(DistSelectHook_GrayBack));

  for (int i=0, j=DistSelectHook_List1Btn; i<5; i++)
  {
    m_listBtn[i].SetCenterElement(GetGuiElement(j++));
    m_listBtn[i].SetLabelElement(GetGuiElement(j++));
  }
}

short CDistSelectHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DistSelectHook_GrayBack:
    {
      m_grayBackCtrl.MouseDown();
      AddRenderUiControls(&m_grayBackCtrl);
    }
    break;
  default:
    if (ctrlType >= DistSelectHook_List1Btn && ctrlType <= DistSelectHook_List5BtnLabel)
    {
      int index = (ctrlType - DistSelectHook_List1Btn)/2;
      if (m_listBtn[index].IsEnable())
      {
        m_listBtn[index].MouseDown();
        AddRenderUiControls(&m_listBtn[index]);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    } 
    else
    {
      m_isNeedRefesh = false;
      assert(false);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDistSelectHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDistSelectHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case DistSelectHook_GrayBack:
    {
      m_grayBackCtrl.MouseUp();
      Return(false);
    }
    break;
  default:
    if (ctrlType >= DistSelectHook_List1Btn && ctrlType <= DistSelectHook_List5BtnLabel)
    {
      int index = (ctrlType - DistSelectHook_List1Btn)/2;
      if (m_listBtn[index].IsEnable())
      {
        m_listBtn[index].MouseUp();
        DoClickListBtn(index);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    } 
    else
    {
      m_isNeedRefesh = false;
      assert(false);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CDistSelectHook::operator ()()
{
  return false;
}

void CDistSelectHook::ShowList()
{
  int i;
  for (i = 0; i < m_moreIndex; ++i)
  {
    m_listBtn[i].SetCaption(m_vecListItem[i+1].m_chName);
    m_listBtn[i].SetEnable(true);
  }

  m_listBtn[m_moreIndex].SetCaption("¸ü¶à¡­");
  m_listBtn[m_moreIndex].SetEnable(true);

  ++i;
  for (; i < 5; ++i)
  {
    m_listBtn[i].SetEnable(false);
    m_listBtn[i].ClearCaption();
  }
}

void CDistSelectHook::DoClickListBtn(int index)
{
  if (index < m_moreIndex)
  {
    TCodeEntry item = m_vecListItem[index+1];
    if (m_queryCallBack)
    {
      SQLRecord record;
      record.m_addrCode = item.m_uCode;
      record.m_asciiName = item.m_chName;
      m_queryCallBack(m_callBackObject, &record);
      Return();
    }
  }
  else
  {
    CDistrictSelectionHook* hook = (CDistrictSelectionHook*)m_view->GetHook(DHT_DistrictSelectionHook);
    if (hook)
    {
      hook->SetCallBackFun(this, DistSwitchCallBack);
    }
    TurnTo(DHT_DistrictSelectionHook);
  }
}

void CDistSelectHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CDistSelectHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CDistSelectHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  if (m_queryCallBack)
  {
    m_queryCallBack(m_callBackObject, pResult);
  }
  Return();
}