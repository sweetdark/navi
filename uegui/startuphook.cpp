#include "startuphook.h"
#include "maphook.h"
#include "messagedialoghook.h"
#include "settingwrapper.h"
#include "userdatawrapper.h"
#include "routewrapper.h"

using namespace UeGui;

CStartUpHook::CStartUpHook() : m_viewWrapper(CViewWrapper::Get()), m_routeWrapper(CRouteWrapper::Get())
{
}

CStartUpHook::~CStartUpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CStartUpHook::GetBinaryFileName()
{
  return _T("startuphook.bin");
}

bool CStartUpHook::operator ()()
{
  return false;
}

void CStartUpHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackLeftBtn,	"BackLeftBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackCenterBtn,	"BackCenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackRightBtn,	"BackRightBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackLeftBtnIcon,	"BackLeftBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackCenterBtnIcon,	"BackCenterBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackRightBtnIcon,	"BackRightBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackLeftBtnLabel,	"BackLeftBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackCenterBtnLabel,	"BackCenterBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackRightBtnLabel,	"BackRightBtnLabel"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackBottom,	"BackBottom"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_NotAgainBtn,	"NotAgainBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_LastNaviBtn,	"LastNaviBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackHomeBtn,	"BackHomeBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_BackCompanyBtn,	"BackCompanyBtn"));
  m_ctrlNames.insert(GuiName::value_type(StartUpHook_CloseBtn,	"CloseBtn"));
}

void CStartUpHook::MakeControls()
{
  //背景  
  m_background.SetCenterElement(GetGuiElement(StartUpHook_Background));
  m_background.SetParent(this);
  //搜索
  m_searchBtn.SetCenterElement(GetGuiElement(StartUpHook_BackLeftBtn));
  m_searchBtn.SetIconElement(GetGuiElement(StartUpHook_BackLeftBtnIcon));
  m_searchBtn.SetLabelElement(GetGuiElement(StartUpHook_BackLeftBtnLabel));
  m_searchBtn.SetParent(this);
  //历史记录  
  m_historyRecord.SetCenterElement(GetGuiElement(StartUpHook_BackCenterBtn));
  m_historyRecord.SetIconElement(GetGuiElement(StartUpHook_BackCenterBtnIcon));
  m_historyRecord.SetLabelElement(GetGuiElement(StartUpHook_BackCenterBtnLabel));
  m_historyRecord.SetParent(this);
  //周边  
  m_around.SetCenterElement(GetGuiElement(StartUpHook_BackRightBtn));
  m_around.SetIconElement(GetGuiElement(StartUpHook_BackRightBtnIcon));
  m_around.SetLabelElement(GetGuiElement(StartUpHook_BackRightBtnLabel));
  m_around.SetParent(this);
  //不再提示
  m_notAskAgain.SetCenterElement(GetGuiElement(StartUpHook_NotAgainBtn));
  m_notAskAgain.SetParent(this);
  //上次导航  
  m_lastNaigation.SetCenterElement(GetGuiElement(StartUpHook_LastNaviBtn));
  m_lastNaigation.SetParent(this);
  //回家  
  m_backHome.SetCenterElement(GetGuiElement(StartUpHook_BackHomeBtn));
  m_backHome.SetParent(this);
  //回单位  
  m_backCompany.SetCenterElement(GetGuiElement(StartUpHook_BackCompanyBtn));
  m_backCompany.SetParent(this);
  //关闭  
  m_close.SetCenterElement(GetGuiElement(StartUpHook_CloseBtn));
  m_close.SetParent(this);
}

short CStartUpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case StartUpHook_CloseBtn:
    {
      m_close.MouseDown();
      m_close.Refresh();
    }
    break;
  case StartUpHook_BackLeftBtn:
  case StartUpHook_BackLeftBtnIcon:
  case StartUpHook_BackLeftBtnLabel:
    {
      m_searchBtn.MouseDown();
      m_searchBtn.Refresh();
    }
    break;
  case StartUpHook_BackCenterBtn:
  case StartUpHook_BackCenterBtnIcon:
  case StartUpHook_BackCenterBtnLabel:
    {
      m_historyRecord.MouseDown();
      m_historyRecord.Refresh();
    }
    break;
  case StartUpHook_BackRightBtn:
  case StartUpHook_BackRightBtnIcon:
  case StartUpHook_BackRightBtnLabel:
    {
      m_around.MouseDown();
      m_around.Refresh();
      m_close.Refresh();
    }
    break;
  case StartUpHook_NotAgainBtn:
    {
      m_notAskAgain.MouseDown();
      m_notAskAgain.Refresh();
    }
    break;
  case StartUpHook_LastNaviBtn:
    {
      m_lastNaigation.MouseDown();
      if (m_lastNaigation.IsEnable())
      {
        m_lastNaigation.Refresh();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case StartUpHook_BackHomeBtn:
    {
      m_backHome.MouseDown();
      if (m_backHome.IsEnable())
      {
        m_backHome.Refresh();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case StartUpHook_BackCompanyBtn:
    {
      m_backCompany.MouseDown();
      if (m_backCompany.IsEnable())
      {
        m_backCompany.Refresh();
      }
      else
      {
        m_isNeedRefesh = false;
      }      
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CStartUpHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CStartUpHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
    case StartUpHook_Background:
      {
        //点击背景
        Close();
      }
      break;
    case StartUpHook_CloseBtn:
      {
        m_close.MouseUp();
        Close();
      }
      break;
    case StartUpHook_BackLeftBtn:
    case StartUpHook_BackLeftBtnIcon:
    case StartUpHook_BackLeftBtnLabel:
      {
        //搜索
        m_searchBtn.MouseUp();
        OpenSearchMenu();
      }
      break;
    case StartUpHook_BackCenterBtn:
    case StartUpHook_BackCenterBtnIcon:
    case StartUpHook_BackCenterBtnLabel:
      {
        //历史记录
        m_historyRecord.MouseUp();
        OpenHistoryRecordMenu();
      }
      break;
    case StartUpHook_BackRightBtn:
    case StartUpHook_BackRightBtnIcon:
    case StartUpHook_BackRightBtnLabel:
      {
        //周边信息
        m_around.MouseUp();
        OpenAroundMenu();
      }
      break;
    case StartUpHook_NotAgainBtn:
      {
        m_notAskAgain.MouseUp();
        CSettingWrapper &settingWrapper = CSettingWrapper::Get();
        settingWrapper.SetIsShowShortcutPanel(false);
        settingWrapper.SaveSystemSettings();
        Close();
      }
      break;
    case StartUpHook_LastNaviBtn:
      {
        m_lastNaigation.MouseUp();
        if (m_lastNaigation.IsEnable())
        {
           RestoreRote();
        }
        else
        {
          m_isNeedRefesh = false;
        }
      }
      break;
    case StartUpHook_BackHomeBtn:
      {
        m_backHome.MouseUp();
        m_isNeedRefesh = false;
        if (m_backHome.IsEnable())
        {
          BackHome();
          m_viewWrapper.Refresh();
        }
      }
      break;
    case StartUpHook_BackCompanyBtn:
      {
        m_backCompany.MouseUp();
        m_isNeedRefesh = false;
        if (m_backCompany.IsEnable())
        {
          BackCompany();
          m_viewWrapper.Refresh();
        }    
      }
      break;
    default:
      m_isNeedRefesh = false;
      assert(false);
      break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void UeGui::CStartUpHook::Load()
{
  const CUserDataWrapper &userDataWrapper = CUserDataWrapper::Get();

  ::memset(&m_homePoi, 0, sizeof(UsuallyRecord));
  int rt = userDataWrapper.GetUsuallyRecord(RT_HOME, &m_homePoi);
  if ((0 == rt) && (0 != m_homePoi.m_x) && 0 != m_homePoi.m_y)
  {
    m_backHome.SetEnable(true);
  }
  else
  {
    m_backHome.SetEnable(false);
  }

  ::memset(&m_companyPoi, 0, sizeof(UsuallyRecord));
  rt = userDataWrapper.GetUsuallyRecord(RT_COMPANY, &m_companyPoi);
  if ((0 == rt) && (0 != m_companyPoi.m_x) && 0 != m_companyPoi.m_y)
  {
    m_backCompany.SetEnable(true);
  }
  else
  {
    m_backCompany.SetEnable(false);
  }
}

void UeGui::CStartUpHook::UnLoad()
{

}

void UeGui::CStartUpHook::Close()
{
  Return(false);
}

void UeGui::CStartUpHook::SetRestoreRouteState( bool value )
{
  if (value)
  {
    m_lastNaigation.SetEnable(true);
  }
  else
  {
    m_lastNaigation.SetEnable(false);
  }
}

void UeGui::CStartUpHook::RestoreRote()
{
  Close();
  Refresh();
  CMapHook *mapHook = (CMapHook*)m_viewWrapper.GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    mapHook->RestoreRote();
  }
}

void UeGui::CStartUpHook::OpenSearchMenu()
{
  Close();
  CMapHook *mapHook = (CMapHook*)m_viewWrapper.GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    mapHook->OpenSearchMenu();
  }
}

void UeGui::CStartUpHook::OpenHistoryRecordMenu()
{
  Close();
  TurnTo(DHT_QueryHistoryHook);
}

void UeGui::CStartUpHook::OpenAroundMenu()
{
  Close();
  CMapHook *mapHook = (CMapHook*)m_viewWrapper.GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    mapHook->OpenAroundSearchMenu();
  }
}

void UeGui::CStartUpHook::DoRoutePlan( CGeoPoint<long>& endPos )
{  
  CMapHook *mapHook = (CMapHook*)m_viewWrapper.GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    //重新设目的地，去除原先规避的影响
    m_routeWrapper.SetBlock(true);
    const GpsCar &carInfo = m_viewWrapper.GetGpsCar();
    PlanPosition position;
    //设起点
    position.m_type = UeRoute::PT_Start;    
    position.m_pos.m_x = carInfo.m_curPos.m_x;
    position.m_pos.m_y = carInfo.m_curPos.m_y;
    m_routeWrapper.SetPosition(position);
    //设目的地
    ::memset(&position, 0, sizeof(PlanPosition));
    position.m_type = UeRoute::PT_End;    
    position.m_pos = endPos;
    unsigned int rt = m_routeWrapper.SetPosition(position);
    if (UeRoute::PEC_TooShortest == rt)
    {
      CMessageDialogEvent dialogEvent(this, DHT_MapHook, NULL);
      CMessageDialogHook::ShowMessageDialog(MB_NONE, "起点和目的地距离太近", dialogEvent);
      Sleep(1000);
      CMessageDialogHook::CloseMessageDialog();
      return;
    }
    mapHook->RoutePlan(CMapHook::Plan_Multiple);
  }
}

void UeGui::CStartUpHook::BackHome()
{
  Close();
  CGeoPoint<long> endPos;
  endPos.m_x = m_homePoi.m_x;
  endPos.m_y = m_homePoi.m_y;
  DoRoutePlan(endPos);
}

void UeGui::CStartUpHook::BackCompany()
{
  Close();
  CGeoPoint<long> endPos;
  endPos.m_x = m_companyPoi.m_x;
  endPos.m_y = m_companyPoi.m_y;
  DoRoutePlan(endPos);
}