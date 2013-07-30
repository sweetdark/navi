#include "fleetmanagementhook.h"
#include "teammemberlisthook.h"
#include "messagedialoghook.h"

using namespace UeGui;

CFleetManagementhook::CFleetManagementhook() : m_isHasTeam(false)
{
}

CFleetManagementhook::~CFleetManagementhook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CFleetManagementhook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  m_pageDownBtnCtrl.SetEnable(false);
  m_pageUpBtnCtrl.SetEnable(false);
  m_pageUpIconCtrl.SetEnable(false);
  m_pageDownIconCtrl.SetEnable(false);
}

tstring CFleetManagementhook::GetBinaryFileName()
{
  return _T("fleetmanagementhook.bin");
}

void CFleetManagementhook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_BGMenuText,	"BGMenuText"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_LogicBackBtn,	"LogicBackBtn"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_PageInfo,	"PageInfo"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListLeftBtn1,	"ListLeftBtn1"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListCenterBtn1,	"ListCenterBtn1"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListRightBtn1,	"ListRightBtn1"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListLeftBtn2,	"ListLeftBtn2"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListCenterBtn2,	"ListCenterBtn2"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListRightBt2,	"ListRightBt2"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListLeftBtn3,	"ListLeftBtn3"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListCenterBtn3,	"ListCenterBtn3"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListRightBtn3,	"ListRightBtn3"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListLeftBtn4,	"ListLeftBtn4"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListCenterBtn4,	"ListCenterBtn4"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListRightBtn4,	"ListRightBtn4"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListLeftBtn5,	"ListLeftBtn5"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListCenterBtn5,	"ListCenterBtn5"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_ListRightBtn5,	"ListRightBtn5"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_CreateTeamLeft,	"CreateTeamLeft"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_CreateTeamCenter,	"CreateTeamCenter"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_CreateTeamRight,	"CreateTeamRight"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_CancelLeft,	"CancelLeft"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_CancelCenter,	"CancelCenter"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_CancelRight,	"CancelRight"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_LookTeamLeft,	"LookTeamLeft"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_LookTeamCenter,	"LookTeamCenter"));
  m_ctrlNames.insert(GuiName::value_type(fleetmanagehook_LookTeamRight,	"LookTeamRight"));
}

void CFleetManagementhook::MakeControls()
{
  m_bGMenuTextCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_BGMenuText));
  m_backGroundCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_BackGround));
  m_cancelCenterCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_CancelCenter));
  m_cancelCenterCtrl.SetLeftElement(GetGuiElement(fleetmanagehook_CancelLeft));
  m_cancelCenterCtrl.SetRightElement(GetGuiElement(fleetmanagehook_CancelRight));
  m_createTeamCenterCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_CreateTeamCenter));
  m_createTeamCenterCtrl.SetLeftElement(GetGuiElement(fleetmanagehook_CreateTeamLeft));
  m_createTeamCenterCtrl.SetRightElement(GetGuiElement(fleetmanagehook_CreateTeamRight));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_GotoMapBtn));
  m_listCenterBtn1Ctrl.SetCenterElement(GetGuiElement(fleetmanagehook_ListCenterBtn1));
  m_listCenterBtn2Ctrl.SetCenterElement(GetGuiElement(fleetmanagehook_ListCenterBtn2));
  m_listCenterBtn3Ctrl.SetCenterElement(GetGuiElement(fleetmanagehook_ListCenterBtn3));
  m_listCenterBtn4Ctrl.SetCenterElement(GetGuiElement(fleetmanagehook_ListCenterBtn4));
  m_listCenterBtn5Ctrl.SetCenterElement(GetGuiElement(fleetmanagehook_ListCenterBtn5));
  m_listCenterBtn1Ctrl.SetLeftElement(GetGuiElement(fleetmanagehook_ListLeftBtn1));
  m_listCenterBtn2Ctrl.SetLeftElement(GetGuiElement(fleetmanagehook_ListLeftBtn2));
  m_listCenterBtn3Ctrl.SetLeftElement(GetGuiElement(fleetmanagehook_ListLeftBtn3));
  m_listCenterBtn4Ctrl.SetLeftElement(GetGuiElement(fleetmanagehook_ListLeftBtn4));
  m_listCenterBtn5Ctrl.SetLeftElement(GetGuiElement(fleetmanagehook_ListLeftBtn5));
  m_listCenterBtn2Ctrl.SetRightElement(GetGuiElement(fleetmanagehook_ListRightBt2));
  m_listCenterBtn1Ctrl.SetRightElement(GetGuiElement(fleetmanagehook_ListRightBtn1));
  m_listCenterBtn3Ctrl.SetRightElement(GetGuiElement(fleetmanagehook_ListRightBtn3));
  m_listCenterBtn4Ctrl.SetRightElement(GetGuiElement(fleetmanagehook_ListRightBtn4));
  m_listCenterBtn5Ctrl.SetRightElement(GetGuiElement(fleetmanagehook_ListRightBtn5));
  m_logicBackBtnCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_LogicBackBtn));
  m_pageDownBtnCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_PageDownBtn));
  m_pageDownIconCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_PageDownIcon));
  m_pageInfoCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_PageInfo));
  m_pageUpBtnCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_PageUpBtn));
  m_pageUpIconCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_PageUpIcon));
  m_lookTeamCenterCtrl.SetCenterElement(GetGuiElement(fleetmanagehook_LookTeamCenter));
  m_lookTeamCenterCtrl.SetLeftElement(GetGuiElement(fleetmanagehook_LookTeamLeft));
  m_lookTeamCenterCtrl.SetRightElement(GetGuiElement(fleetmanagehook_LookTeamRight));
}

short CFleetManagementhook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case fleetmanagehook_CancelCenter:
  case fleetmanagehook_CancelLeft:
  case fleetmanagehook_CancelRight:
    {
      m_cancelCenterCtrl.MouseDown();
    }
    break;
  case fleetmanagehook_CreateTeamCenter:
  case fleetmanagehook_CreateTeamLeft:
  case fleetmanagehook_CreateTeamRight:
    {
      m_createTeamCenterCtrl.MouseDown();
    }
    break;
  case fleetmanagehook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  //case fleetmanagehook_ListCenterBtn1:
  //case fleetmanagehook_ListLeftBtn1:
  //case fleetmanagehook_ListRightBtn1:
  //  {
  //    m_listCenterBtn1Ctrl.MouseDown();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn2:
  //case fleetmanagehook_ListLeftBtn2:
  //case fleetmanagehook_ListRightBt2:
  //  {
  //    m_listCenterBtn2Ctrl.MouseDown();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn3:
  //case fleetmanagehook_ListLeftBtn3:
  //case fleetmanagehook_ListRightBtn3:
  //  {
  //    m_listCenterBtn3Ctrl.MouseDown();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn4:
  //case fleetmanagehook_ListLeftBtn4:
  //case fleetmanagehook_ListRightBtn4:
  //  {
  //    m_listCenterBtn4Ctrl.MouseDown();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn5:
  //case fleetmanagehook_ListRightBtn5:
  //case fleetmanagehook_ListLeftBtn5:
  //  {
  //    m_listCenterBtn5Ctrl.MouseDown();
  //  }
  //  break;
  case fleetmanagehook_LookTeamCenter:
    {
      m_lookTeamCenterCtrl.MouseDown();
    }
    break;
  case fleetmanagehook_LogicBackBtn:
    {
      m_logicBackBtnCtrl.MouseDown();
    }
    break;
  case fleetmanagehook_PageDownBtn:
    {
      m_pageDownBtnCtrl.MouseDown();
    }
    break;
  case fleetmanagehook_PageUpBtn:
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

short CFleetManagementhook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CFleetManagementhook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case fleetmanagehook_CancelCenter:
  case fleetmanagehook_CancelLeft:
  case fleetmanagehook_CancelRight:
    {
      m_cancelCenterCtrl.MouseUp();
      m_curHookType = CViewHook::DHT_TruckMainMenuHook;
    }
    break;
  case fleetmanagehook_CreateTeamCenter:
  case fleetmanagehook_CreateTeamLeft:
  case fleetmanagehook_CreateTeamRight:
    {
      m_createTeamCenterCtrl.MouseUp();
     /* m_prevHookType = m_curHookType;
      m_curHookType = CViewHook::DHT_TeamMemberListHook;*/
      CViewHook::SkipToHook(DHT_TeamMemberListHook);
      Cteammemberlisthook* hook = (Cteammemberlisthook*)m_view->GetHook(CViewHook::DHT_TeamMemberListHook);
      hook->Prepare();
      m_isHasTeam = true;
    }
    break;
  case fleetmanagehook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseUp();
      m_curHookType = CViewHook::DHT_MapHook;
      ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
    }
    break;
  //case fleetmanagehook_ListCenterBtn1:
  //case fleetmanagehook_ListLeftBtn1:
  //case fleetmanagehook_ListRightBtn1:
  //  {
  //    m_listCenterBtn1Ctrl.MouseUp();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn2:
  //case fleetmanagehook_ListLeftBtn2:
  //case fleetmanagehook_ListRightBt2:
  //  {
  //    m_listCenterBtn2Ctrl.MouseUp();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn3:
  //case fleetmanagehook_ListLeftBtn3:
  //case fleetmanagehook_ListRightBtn3:
  //  {
  //    m_listCenterBtn3Ctrl.MouseUp();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn4:
  //case fleetmanagehook_ListLeftBtn4:
  //case fleetmanagehook_ListRightBtn4:
  //  {
  //    m_listCenterBtn4Ctrl.MouseUp();
  //  }
  //  break;
  //case fleetmanagehook_ListCenterBtn5:
  //case fleetmanagehook_ListRightBtn5:
  //case fleetmanagehook_ListLeftBtn5:
  //  {
  //    m_listCenterBtn5Ctrl.MouseUp();
  //  }
  //  break;
  case fleetmanagehook_LookTeamCenter:
    {
      m_lookTeamCenterCtrl.MouseUp();
      if(!m_isHasTeam)
      {
        CMessageDialogEvent dialogEvent(this,CViewHook::m_curHookType);
        CMessageDialogHook::ShowMessageDialog(MB_NONE, "您不在任何队伍当中！",dialogEvent,2);
      }
      else
      {
       /* m_prevHookType = m_curHookType;
        m_curHookType = CViewHook::DHT_TeamMemberListHook;*/
        CViewHook::SkipToHook(DHT_TeamMemberListHook);
      }
      
    }
    break;
  case fleetmanagehook_LogicBackBtn:
    {
      m_logicBackBtnCtrl.MouseUp();
      //m_curHookType = CViewHook::DHT_TruckMainMenuHook;
      CViewHook::SkipToHook(DHT_TruckMainMenuHook);
    }
    break;
  case fleetmanagehook_PageDownBtn:
    {
      m_pageDownBtnCtrl.MouseUp();
    }
    break;
  case fleetmanagehook_PageDownIcon:
    {
      m_pageDownIconCtrl.MouseUp();
    }
    break;
  case fleetmanagehook_PageUpBtn:
    {
      m_pageUpBtnCtrl.MouseUp();
    }
    break;
  case fleetmanagehook_PageUpIcon:
    {
      m_pageUpIconCtrl.MouseUp();
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

bool CFleetManagementhook::operator ()()
{
  return false;
}
