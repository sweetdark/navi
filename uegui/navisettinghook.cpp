#include "navisettinghook.h"
#include "messagedialoghook.h"
#include "guisetting.h"
#include "guioperationright.h"
#include "uequery\query.h"
#include "myjourneyhook.h"
#include "usuallyfile.h"
#include "RestoreDefaultshook.h"
#include "userdatawrapper.h"

using namespace UeGui;

CNaviSettingHook::CNaviSettingHook()
{
}

CNaviSettingHook::~CNaviSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CNaviSettingHook::MakeGUI()
{
#if __FOR_DEVICE__
  FetchWithBinary();
  MakeNames();
  MakeControls();
#endif
}

tstring CNaviSettingHook::GetBinaryFileName()
{
  return _T("navisettinghook.bin");
}

void CNaviSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_Back,	"Back"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_Previous,	"Previous"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_NaviText,	"NaviText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_MapSetting,	"MapSetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_MapSettingText,	"MapSettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_CrossPicSetting,	"CrossPicSetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_CrossPicSettingText,	"CrossPicSettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_EEyeSetting,	"EEyeSetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_EEyeSettingText,	"EEyeSettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_PromptSetting,	"PromptSetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_PromptSettingText,	"PromptSettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_RouteSetting,	"RouteSetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_RouteSettingText,	"RouteSettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_TrackSetting,	"TrackSetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_TrackSettingText,	"TrackSettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_SafetySetting,	"SafetySetting"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_SafetySettingText,	"SafetySettingText"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_Restore,	"Restore"));
  m_ctrlNames.insert(GuiName::value_type(NaviSettingHook_RestoreText,	"RestoreText"));
}

void CNaviSettingHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_Previous));
  
  m_mapSettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_MapSetting));
  m_mapSettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_MapSettingText));
  m_crossPicSettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_CrossPicSetting));
  m_crossPicSettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_CrossPicSettingText));
  m_eEyeSettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_EEyeSetting));
  m_eEyeSettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_EEyeSettingText));
  m_promptSettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_PromptSetting));
  m_promptSettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_PromptSettingText));

  m_routeSettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_RouteSetting));
  m_routeSettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_RouteSettingText));
  m_trackSettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_TrackSetting));
  m_trackSettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_TrackSettingText));
  m_safetySettingCtrl.SetIconElement(GetGuiElement(NaviSettingHook_SafetySetting));
  m_safetySettingCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_SafetySettingText));
  m_restoreCtrl.SetIconElement(GetGuiElement(NaviSettingHook_Restore));
  m_restoreCtrl.SetCenterElement(GetGuiElement(NaviSettingHook_RestoreText));
}

void UeGui::CNaviSettingHook::Init()
{
  m_mapSettingCtrl.SetEnable(false);
  m_crossPicSettingCtrl.SetEnable(false);
  m_eEyeSettingCtrl.SetEnable(false);
  m_promptSettingCtrl.SetEnable(false);
  m_routeSettingCtrl.SetEnable(false);
  m_trackSettingCtrl.SetEnable(false);
  m_safetySettingCtrl.SetEnable(false);
  m_restoreCtrl.SetEnable(false);

  if (CGuiOperationRight::HaveRight(OR_MAPSETTING))
  {
    m_mapSettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_CROSSPIC))
  {
    m_crossPicSettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_EEYESETTING))
  {
    m_eEyeSettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_PROMPTSETTING))
  {
    m_promptSettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_ROUTESETTING))
  {
    m_routeSettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_TRACKSETTING))
  {
    m_trackSettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_SAFETYSETTING))
  {
    m_safetySettingCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_RESTORE))
  {
    m_restoreCtrl.SetEnable(true);
  }
}

void UeGui::CNaviSettingHook::Load()
{

}

short CNaviSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  switch(ctrlType)
  {
  case NaviSettingHook_Map:
    {
      m_mapCtrl.MouseDown();
      AddRenderUiControls(&m_mapCtrl);
    }
    break;
  case NaviSettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
      AddRenderUiControls(&m_previousCtrl);
    }
    break;
  case NaviSettingHook_MapSetting:
    {
      m_mapSettingCtrl.MouseDown();
      if (m_mapSettingCtrl.IsEnable())
        AddRenderUiControls(&m_mapSettingCtrl);
    }
    break;
  case NaviSettingHook_CrossPicSetting:
    {
      m_crossPicSettingCtrl.MouseDown();
      if (m_crossPicSettingCtrl.IsEnable())
        AddRenderUiControls(&m_crossPicSettingCtrl);
    }
    break;
  case NaviSettingHook_EEyeSetting:
    {
      m_eEyeSettingCtrl.MouseDown();
      if (m_eEyeSettingCtrl.IsEnable())
        AddRenderUiControls(&m_eEyeSettingCtrl);
    }
    break;
  case NaviSettingHook_PromptSetting:
    {
      m_promptSettingCtrl.MouseDown();
      if (m_promptSettingCtrl.IsEnable())
        AddRenderUiControls(&m_promptSettingCtrl);
    }
    break;
  case NaviSettingHook_RouteSetting:
    {
      m_routeSettingCtrl.MouseDown();
      if (m_routeSettingCtrl.IsEnable())
        AddRenderUiControls(&m_routeSettingCtrl);
    }
    break;
  case NaviSettingHook_TrackSetting:
    {
      m_trackSettingCtrl.MouseDown();
      if (m_trackSettingCtrl.IsEnable())
        AddRenderUiControls(&m_trackSettingCtrl);
    }
    break;
  case NaviSettingHook_SafetySetting:
    {
      m_safetySettingCtrl.MouseDown();
      if (m_safetySettingCtrl.IsEnable())
        AddRenderUiControls(&m_safetySettingCtrl);
    }
    break;
  case NaviSettingHook_Restore:
    {
      m_restoreCtrl.MouseDown();
      if (m_restoreCtrl.IsEnable())
        AddRenderUiControls(&m_restoreCtrl);
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

short CNaviSettingHook::MouseMove(UeBase::CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CNaviSettingHook::MouseUp(UeBase::CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  switch(m_downElementType)
  {
  case NaviSettingHook_Map:
    {
      m_mapCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_mapCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
    }
    m_isNeedRefesh = false;
    m_view->Refresh();
    break;
  case NaviSettingHook_Previous:
    {
      m_previousCtrl.MouseUp();
      if ((ctrlType == m_downElementType)  && (m_previousCtrl.IsEnable()))
      {
        // TODO: Extract Methods.
        /*short prevHookType = CViewHook::m_prevHookType;*/
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;*/
        CAggHook::Return();
      }
    }
    m_isNeedRefesh = false;
    m_view->Refresh();
    break;
  case NaviSettingHook_MapSetting:
    {
      m_mapSettingCtrl.MouseUp();
      if ((ctrlType == m_downElementType)  && (m_mapSettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapSettingHook;*/
        CAggHook::TurnTo(DHT_MapSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_MapSettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_mapSettingCtrl);
      }
    }
    break;
  case NaviSettingHook_CrossPicSetting:
    {
      m_crossPicSettingCtrl.MouseUp();

      if ((ctrlType == m_downElementType)  && (m_crossPicSettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_CrossPicSettingHook;*/
        CAggHook::TurnTo(DHT_CrossPicSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_CrossPicSettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_crossPicSettingCtrl);
      }
    }
    break;
  case NaviSettingHook_EEyeSetting:
    {
      m_eEyeSettingCtrl.MouseUp();
      if ((ctrlType == m_downElementType)  && (m_eEyeSettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_EEyeSettingHook;*/
        CAggHook::TurnTo(DHT_EEyeSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_EEyeSettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_eEyeSettingCtrl);
      }
    }
    break;
  case NaviSettingHook_PromptSetting:
    {
      m_promptSettingCtrl.MouseUp();

      if ((ctrlType == m_downElementType)  && (m_promptSettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_PromptSettingHook;*/
        CAggHook::TurnTo(DHT_PromptSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_PromptSettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_promptSettingCtrl);
      }
    }
    break;
  case NaviSettingHook_RouteSetting:
    {
      m_routeSettingCtrl.MouseUp();

      if ((ctrlType == m_downElementType)  && (m_routeSettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_RouteSettingHook;*/
        CAggHook::TurnTo(DHT_RouteSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_RouteSettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_routeSettingCtrl);
      }
    }
    break;
  case NaviSettingHook_TrackSetting:
    {
      m_trackSettingCtrl.MouseUp();

      if ((ctrlType == m_downElementType)  && (m_trackSettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_TrackSettingHook;*/
        CAggHook::TurnTo(DHT_TrackSettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_TrackSettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_trackSettingCtrl);
      }
    }
    break;
  case NaviSettingHook_SafetySetting:
    {
      m_safetySettingCtrl.MouseUp();

      if ((ctrlType == m_downElementType)  && (m_safetySettingCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_SafetySettingHook;*/
        CAggHook::TurnTo(DHT_SafetySettingHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_SafetySettingHook);
        aggHook->Load();
        AddRenderUiControls(&m_safetySettingCtrl);
      }
    }
    break;
  case NaviSettingHook_Restore:
    {
      m_restoreCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_restoreCtrl.IsEnable()))
      {
        //CMessageDialogHook::ShowMessageDialog(UeGui::MB_INFORMATION,"将清空数据恢复到出厂状态",
        //  CMessageDialogEvent(this, CViewHook::DHT_NaviSettingHook, OnRestore), 0);

        //初始化历史信息和个人数据
        CRestoreDefaultshook *RestoreDef = (CRestoreDefaultshook*)(m_view->GetHook(CViewHook::DHT_RestoreDefaultshook));
        RestoreDef->prepare();
        CAggHook::TurnTo(DHT_RestoreDefaultshook);
        AddRenderUiControls(&m_restoreCtrl);
      }      
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


bool CNaviSettingHook::operator ()()
{
  return false;
}

void CNaviSettingHook::OnRestore(CAggHook* sender, ModalResultType modalResult)
{
  if (modalResult == UeGui::MR_OK)
  {
    CNaviSettingHook* naviSettingHook = dynamic_cast<CNaviSettingHook*>(sender);
    naviSettingHook->DoRestore();
  } 
  else
  {
    return;
  }
}

void UeGui::CNaviSettingHook::DoRestore()
{
  CGuiSettings* guisett = CGuiSettings::GetGuiSettings();
  if (guisett)
  {
    guisett->Restore();

    MapsHeader header;
    CUeMapsIO mapsIO;
    mapsIO.GetHeader(header);
    GpsCar gpsInfo;
    gpsInfo.m_curPos.m_x = header.m_baseX;
    gpsInfo.m_curPos.m_y = header.m_baseY;
    m_view->SetGpsCar(gpsInfo);
    m_view->SetGpsPosInfo(gpsInfo);
    m_view->SetCarIcon(guisett->GetCarIcon());
  }  
  ClearAllData();
}

void CNaviSettingHook::ClearAllData() 
{
  UeQuery::SQLSetting setting;

  const CUserDataWrapper& userWrapper = CUserDataWrapper::Get();
  
  userWrapper.RemoveAllRecent();

  userWrapper.RemoveAllHistoryRecord();

  CUsuallyFile favorite;
  favorite.RemoveRecord(RT_HOME);
  favorite.RemoveRecord(RT_COMPANY);
  favorite.RemoveRecord(RT_ONE);
  favorite.RemoveRecord(RT_TWO);
  favorite.RemoveRecord(RT_THREE);

  userWrapper.RemoveAllJourney();
}

