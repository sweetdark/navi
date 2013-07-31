#include "restoredefaultshook.h"
#include "navisettinghook.h"
#include "guisetting.h"
#include "usuallyfile.h"
#include "myjourneyhook.h"
#include "userdatawrapper.h"
using namespace UeGui;

CRestoreDefaultshook::CRestoreDefaultshook()
{
}

CRestoreDefaultshook::~CRestoreDefaultshook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void UeGui::CRestoreDefaultshook::Init()
{
  ;
}
void CRestoreDefaultshook::prepare()
{
  m_personalInfo.SetCheck(false);
  m_historicalInfo.SetCheck(false);
}
void CRestoreDefaultshook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CRestoreDefaultshook::GetBinaryFileName()
{
  return _T("restoredefaultshook.bin");
}

void CRestoreDefaultshook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_MesageText1,	"MesageText1"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_MessageText2,	"MessageText2"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_HistoricalInfoLabel,	"HistoricalInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_PersonalInfoLabel,	"PersonalInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_HistoricalInfoCenter,	"HistoricalInfoCenter"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_HistoricalInfoIcon,	"HistoricalInfoIcon"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_HistoricalInfoLeft,	"HistoricalInfoLeft"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_HistoricalInfoRight,	"HistoricalInfoRight"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_PersonalInfoCenter,	"PersonalInfoCenter"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_PersonalInfoIcon,	"PersonalInfoIcon"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_PersonalInfoLeft,	"PersonalInfoLeft"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_PersonalInfoRight,	"PersonalInfoRight"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_OnOk,	"OnOk"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_OnOKLeft,	"OnOKLeft"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_OnOKRight,	"OnOKRight"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_OnCance,	"OnCance"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_OnCanceLeft,	"OnCanceLeft"));
  m_ctrlNames.insert(GuiName::value_type(RestoreDefaultshook_OnCanceRight,	"OnCanceRight"));
}

void CRestoreDefaultshook::MakeControls()
{
  m_backGroundCtrl.SetCenterElement(GetGuiElement(RestoreDefaultshook_BackGround));
  m_historicalInfo.SetCenterElement(GetGuiElement(RestoreDefaultshook_HistoricalInfoCenter));
  m_historicalInfo.SetIconElement(GetGuiElement(RestoreDefaultshook_HistoricalInfoIcon));
  m_historicalInfo.SetLabelElement(GetGuiElement(RestoreDefaultshook_HistoricalInfoLabel));
  m_historicalInfo.SetLeftElement(GetGuiElement(RestoreDefaultshook_HistoricalInfoLeft));
  m_historicalInfo.SetRightElement(GetGuiElement(RestoreDefaultshook_HistoricalInfoRight));
  m_mesageText1Ctrl.SetCenterElement(GetGuiElement(RestoreDefaultshook_MesageText1));
  m_messageText2Ctrl.SetCenterElement(GetGuiElement(RestoreDefaultshook_MessageText2));
  m_onCanceCtrl.SetCenterElement(GetGuiElement(RestoreDefaultshook_OnCance));
  m_onCanceCtrl.SetLeftElement(GetGuiElement(RestoreDefaultshook_OnCanceLeft));
  m_onCanceCtrl.SetRightElement(GetGuiElement(RestoreDefaultshook_OnCanceRight));
  m_onOkCtrl.SetLeftElement(GetGuiElement(RestoreDefaultshook_OnOKLeft));
  m_onOkCtrl.SetRightElement(GetGuiElement(RestoreDefaultshook_OnOKRight));
  m_onOkCtrl.SetCenterElement(GetGuiElement(RestoreDefaultshook_OnOk));
  m_personalInfo.SetCenterElement(GetGuiElement(RestoreDefaultshook_PersonalInfoCenter));
  m_personalInfo.SetIconElement(GetGuiElement(RestoreDefaultshook_PersonalInfoIcon));
  m_personalInfo.SetLabelElement(GetGuiElement(RestoreDefaultshook_PersonalInfoLabel));
  m_personalInfo.SetLeftElement(GetGuiElement(RestoreDefaultshook_PersonalInfoLeft));
  m_personalInfo.SetRightElement(GetGuiElement(RestoreDefaultshook_PersonalInfoRight));
}

short CRestoreDefaultshook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RestoreDefaultshook_HistoricalInfoCenter:
  case RestoreDefaultshook_HistoricalInfoIcon:
  case RestoreDefaultshook_HistoricalInfoLabel:
  case RestoreDefaultshook_HistoricalInfoLeft:
  case RestoreDefaultshook_HistoricalInfoRight:
    {
      m_historicalInfo.MouseDown();
      AddRenderUiControls(&m_historicalInfo);
      AddRenderElements(GetGuiElement(RestoreDefaultshook_HistoricalInfoLabel)); 
    }
    break;
  case RestoreDefaultshook_OnCance:
  case RestoreDefaultshook_OnCanceLeft:
  case RestoreDefaultshook_OnCanceRight:
    {
      m_onCanceCtrl.MouseDown();
    }
    break;
  case RestoreDefaultshook_OnOKLeft:
  case RestoreDefaultshook_OnOKRight:
  case RestoreDefaultshook_OnOk:
    {
      m_onOkCtrl.MouseDown();
    }
    break;
  case RestoreDefaultshook_PersonalInfoCenter:
  case RestoreDefaultshook_PersonalInfoIcon:
  case RestoreDefaultshook_PersonalInfoLabel:
  case RestoreDefaultshook_PersonalInfoLeft:
  case RestoreDefaultshook_PersonalInfoRight:
    {
      m_personalInfo.MouseDown();
      AddRenderUiControls(&m_personalInfo);
      AddRenderElements(GetGuiElement(RestoreDefaultshook_PersonalInfoLabel)); 
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

short CRestoreDefaultshook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRestoreDefaultshook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RestoreDefaultshook_HistoricalInfoCenter:
  case RestoreDefaultshook_HistoricalInfoIcon:
  case RestoreDefaultshook_HistoricalInfoLabel:
  case RestoreDefaultshook_HistoricalInfoLeft:
  case RestoreDefaultshook_HistoricalInfoRight:
    {
      m_historicalInfo.MouseUp();
      AddRenderUiControls(&m_historicalInfo);
      AddRenderElements(GetGuiElement(RestoreDefaultshook_HistoricalInfoLabel)); 
    }
    break;
  case RestoreDefaultshook_OnCance:
  case RestoreDefaultshook_OnCanceLeft:
  case RestoreDefaultshook_OnCanceRight:
    {
      m_onCanceCtrl.MouseUp();
      CAggHook::Return();
    }
    break;
  case RestoreDefaultshook_OnOKLeft:
  case RestoreDefaultshook_OnOKRight:
  case RestoreDefaultshook_OnOk:
    {
      m_onOkCtrl.MouseUp();
      UpdateSettings();
      CAggHook::Return();
    }
    break;
  case RestoreDefaultshook_PersonalInfoCenter:
  case RestoreDefaultshook_PersonalInfoIcon:
  case RestoreDefaultshook_PersonalInfoLabel:
  case RestoreDefaultshook_PersonalInfoLeft:
  case RestoreDefaultshook_PersonalInfoRight:
    {
      m_personalInfo.MouseUp();
      AddRenderUiControls(&m_personalInfo);
      AddRenderElements(GetGuiElement(RestoreDefaultshook_PersonalInfoLabel)); 
    }
    break;
  default:
    assert(false);
    m_isNeedRefesh = false;
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CRestoreDefaultshook::operator ()()
{
  return false;
}
void CRestoreDefaultshook::ClearHistoryData()
{
  //清除历史记录
  const CUserDataWrapper& userWrapper = CUserDataWrapper::Get();
  userWrapper.RemoveAllHistoryRecord();

  //清除历史路线
  userWrapper.RemoveAllRecent();
  
}
void CRestoreDefaultshook::ClearPersonalData()
{
  const CUserDataWrapper& userWrapper = CUserDataWrapper::Get();
  //清除地址薄数据 
  userWrapper.RemoveAllFavorite();

  //清除常用点信息
  CUsuallyFile favorite;
  favorite.RemoveRecord(RT_HOME);
  favorite.RemoveRecord(RT_COMPANY);
  favorite.RemoveRecord(RT_ONE);
  favorite.RemoveRecord(RT_TWO);
  favorite.RemoveRecord(RT_THREE);

  //清除行程信息
  userWrapper.RemoveAllJourney();
}
void CRestoreDefaultshook::Restore()
{
  CGuiSettings* guisett = CGuiSettings::GetGuiSettings();
  if (guisett)
  {
    //初始化所有设置
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
}
void CRestoreDefaultshook::UpdateSettings()
{
    if(m_historicalInfo.Checked())
    {
      //清除历史信息
      ClearHistoryData();
    }
    if(m_personalInfo.Checked())
    {
      //清除个人数据
      ClearPersonalData();
    }
    //初始化设置
    Restore();
}