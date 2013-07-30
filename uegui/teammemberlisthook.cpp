#include "teammemberlisthook.h"
#include "CarInfoHook.h"
#include "fleetmanagementhook.h"

using namespace UeGui;

Cteammemberlisthook::Cteammemberlisthook()
{
}

Cteammemberlisthook::~Cteammemberlisthook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void Cteammemberlisthook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  m_pageDownBtnCtrl.SetEnable(false);
  m_pageDownIconCtrl.SetEnable(false);
  m_pageUpBtnCtrl.SetEnable(false);
  m_pageUpIconCtrl.SetEnable(false);
}

tstring Cteammemberlisthook::GetBinaryFileName()
{
  return _T("teammemberlisthook.bin");
}

void Cteammemberlisthook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_BGMenuText,	"BGMenuText"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_LogicBackBtn,	"LogicBackBtn"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_PageInfo,	"PageInfo"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListLeftBtn1,	"ListLeftBtn1"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListCenterBtn1,	"ListCenterBtn1"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListRightBtn1,	"ListRightBtn1"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListLeftBtn2,	"ListLeftBtn2"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListCenterBtn2,	"ListCenterBtn2"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListRightBt2,	"ListRightBt2"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListLeftBtn3,	"ListLeftBtn3"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListCenterBtn3,	"ListCenterBtn3"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListRightBtn3,	"ListRightBtn3"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListLeftBtn4,	"ListLeftBtn4"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListCenterBtn4,	"ListCenterBtn4"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListRightBtn4,	"ListRightBtn4"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListLeftBtn5,	"ListLeftBtn5"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListCenterBtn5,	"ListCenterBtn5"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_ListRightBtn5,	"ListRightBtn5"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_CancelLeft,	"CancelLeft"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_CancelCenter,	"CancelCenter"));
  m_ctrlNames.insert(GuiName::value_type(teammemberlisthook_CancelRight,	"CancelRight"));
}

void Cteammemberlisthook::MakeControls()
{
  m_bGMenuTextCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_BGMenuText));
  m_backGroundCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_BackGround));
  m_cancelCenterCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_CancelCenter));
  m_cancelCenterCtrl.SetLeftElement(GetGuiElement(teammemberlisthook_CancelLeft));
  m_cancelCenterCtrl.SetRightElement(GetGuiElement(teammemberlisthook_CancelRight));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_GotoMapBtn));
  m_listCenterBtn1Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListCenterBtn1));
  m_listCenterBtn2Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListCenterBtn2));
  m_listCenterBtn3Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListCenterBtn3));
  m_listCenterBtn4Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListCenterBtn4));
  m_listCenterBtn5Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListCenterBtn5));
  m_listLeftBtn1Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListLeftBtn1));
  m_listLeftBtn2Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListLeftBtn2));
  m_listLeftBtn3Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListLeftBtn3));
  m_listLeftBtn4Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListLeftBtn4));
  m_listLeftBtn5Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListLeftBtn5));
  m_listRightBt2Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListRightBt2));
  m_listRightBtn1Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListRightBtn1));
  m_listRightBtn3Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListRightBtn3));
  m_listRightBtn4Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListRightBtn4));
  m_listRightBtn5Ctrl.SetCenterElement(GetGuiElement(teammemberlisthook_ListRightBtn5));
  m_logicBackBtnCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_LogicBackBtn));
  m_pageDownBtnCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_PageDownBtn));
  m_pageDownIconCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_PageDownIcon));
  m_pageInfoCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_PageInfo));
  m_pageUpBtnCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_PageUpBtn));
  m_pageUpIconCtrl.SetCenterElement(GetGuiElement(teammemberlisthook_PageUpIcon));
}

short Cteammemberlisthook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case teammemberlisthook_CancelCenter:
  case teammemberlisthook_CancelLeft:
  case teammemberlisthook_CancelRight:
    {
      m_cancelCenterCtrl.MouseDown();
    }
    break;
  case teammemberlisthook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  //case teammemberlisthook_ListCenterBtn1:
  //  {
  //    m_listCenterBtn1Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListCenterBtn2:
  //  {
  //    m_listCenterBtn2Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListCenterBtn3:
  //  {
  //    m_listCenterBtn3Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListCenterBtn4:
  //  {
  //    m_listCenterBtn4Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListCenterBtn5:
  //  {
  //    m_listCenterBtn5Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListLeftBtn1:
  //  {
  //    m_listLeftBtn1Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListLeftBtn2:
  //  {
  //    m_listLeftBtn2Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListLeftBtn3:
  //  {
  //    m_listLeftBtn3Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListLeftBtn4:
  //  {
  //    m_listLeftBtn4Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListLeftBtn5:
  //  {
  //    m_listLeftBtn5Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListRightBt2:
  //  {
  //    m_listRightBt2Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListRightBtn1:
  //  {
  //    m_listRightBtn1Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListRightBtn3:
  //  {
  //    m_listRightBtn3Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListRightBtn4:
  //  {
  //    m_listRightBtn4Ctrl.MouseDown();
  //  }
  //  break;
  //case teammemberlisthook_ListRightBtn5:
  //  {
  //    m_listRightBtn5Ctrl.MouseDown();
  //  }
  //  break;
  case teammemberlisthook_LogicBackBtn:
    {
      m_logicBackBtnCtrl.MouseDown();
    }
    break;
  case teammemberlisthook_PageDownBtn:
    {
      m_pageDownBtnCtrl.MouseDown();
    }
    break;
  case teammemberlisthook_PageInfo:
    {
      m_pageInfoCtrl.MouseDown();
    }
    break;
  case teammemberlisthook_PageUpBtn:
    {
      m_pageUpBtnCtrl.MouseDown();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short Cteammemberlisthook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short Cteammemberlisthook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case teammemberlisthook_LogicBackBtn:
    {
      m_logicBackBtnCtrl.MouseUp();
      m_curHookType = CViewHook::DHT_FleetManagementHook;
    }
    break;
  case teammemberlisthook_CancelCenter:
  case teammemberlisthook_CancelLeft:
  case teammemberlisthook_CancelRight:
    {
      m_cancelCenterCtrl.MouseUp();
      m_listCenterBtn1Ctrl.ClearCaption();
      m_curHookType = CViewHook::DHT_FleetManagementHook;
      CFleetManagementhook* hook = (CFleetManagementhook*)m_view->GetHook(CViewHook::DHT_FleetManagementHook);
      hook->LeaveTeam();
    }
    break;
  
  case teammemberlisthook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseUp();
      m_curHookType = CViewHook::DHT_MapHook;
      ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
    }
    break;
 /* case teammemberlisthook_ListCenterBtn1:
    {
      m_listCenterBtn1Ctrl.MouseUp();
     
    }
    break;
  case teammemberlisthook_ListCenterBtn2:
    {
      m_listCenterBtn2Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListCenterBtn3:
    {
      m_listCenterBtn3Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListCenterBtn4:
    {
      m_listCenterBtn4Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListCenterBtn5:
    {
      m_listCenterBtn5Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListLeftBtn1:
    {
      m_listLeftBtn1Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListLeftBtn2:
    {
      m_listLeftBtn2Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListLeftBtn3:
    {
      m_listLeftBtn3Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListLeftBtn4:
    {
      m_listLeftBtn4Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListLeftBtn5:
    {
      m_listLeftBtn5Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListRightBt2:
    {
      m_listRightBt2Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListRightBtn1:
    {
      m_listRightBtn1Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListRightBtn3:
    {
      m_listRightBtn3Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListRightBtn4:
    {
      m_listRightBtn4Ctrl.MouseUp();
    }
    break;
  case teammemberlisthook_ListRightBtn5:
    {
      m_listRightBtn5Ctrl.MouseUp();
    }
    break;*/

  //case teammemberlisthook_PageDownBtn:
  //  {
  //    m_pageDownBtnCtrl.MouseUp();
  //  }
  //  break;
  //case teammemberlisthook_PageDownIcon:
  //  {
  //    m_pageDownIconCtrl.MouseUp();
  //  }
  //  break;
  //case teammemberlisthook_PageInfo:
  //  {
  //    m_pageInfoCtrl.MouseUp();
  //  }
  //  break;
  //case teammemberlisthook_PageUpBtn:
  //  {
  //    m_pageUpBtnCtrl.MouseUp();
  //  }
  //  break;
  //case teammemberlisthook_PageUpIcon:
  //  {
  //    m_pageUpIconCtrl.MouseUp();
  //  }
  //  break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool Cteammemberlisthook::operator ()()
{
  return false;
}

void Cteammemberlisthook::Prepare()
{

  m_listCenterBtn1Ctrl.ClearCaption();
  CCarInfoHook* carHook = (CCarInfoHook*)m_view->GetHook(CViewHook::DHT_VehicleInformationHook);
  CarInfoData& data = carHook->GetCarInfoData();
  char prefix[256] = "³µÅÆºÅ:";
  const char* license = reinterpret_cast<const char*>(data.carLicence);
  if(::strlen(license) > 0) 
  {
    ::strcat(prefix,license);
    m_listCenterBtn1Ctrl.SetCaption(prefix);
  }

}