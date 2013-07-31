#include "mainmenuhook.h"
#include "messagedialoghook.h"
using namespace UeGui;

CMainMenuHook::CMainMenuHook()
{
  m_strTitle = "功能";
  m_vecHookFile.push_back(_T("mainmenuhook.bin"));
}

CMainMenuHook::~CMainMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CMainMenuHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_RoutePlanBackground,	"RoutePlanBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_AddressBookBackground,	"AddressBookBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_HistoryRecordBackground,	"HistoryRecordBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ServeBackground,	"ServeBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_HSplit,	"HSplit"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_Display3DBackground,	"Display3DBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_FitstSplit,	"FitstSplit"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_SecondSplit,	"SecondSplit"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ThirdSplit,	"ThirdSplit"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_MyJourneyBackground,	"MyJourneyBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_SettingBackground,	"SettingBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ExitBackground,	"ExitBackground"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_RoutePlanBtn,	"RoutePlanBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_RoutePlanLable,	"RoutePlanLable"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_AddressBookBtn,	"AddressBookBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_AddressBookLable,	"AddressBookLable"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_HistoryRecordBtn,	"HistoryRecordBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_HistoryRecordLable,	"HistoryRecordLable"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ServeBtn,	"ServeBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ServeLable,	"ServeLable"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_Display3DBtn,	"Display3DBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_Display3DLable,	"Display3DLable"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_MyJourneyBtn,	"MyJourneyBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_MyJourneyLable,	"MyJourneyLable"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_SettingBtn,	"SettingBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_SettingLabel,	"SettingLabel"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ExitBtn,	"ExitBtn"));
  m_ctrlNames.insert(GuiName::value_type(mainmenuhook_ExitLabel,	"ExitLabel"));
}

void CMainMenuHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_addressBookBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_AddressBookBackground));
  m_addressBookBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_AddressBookBtn));
  m_addressBookLableCtrl.SetCenterElement(GetGuiElement(mainmenuhook_AddressBookLable));
  m_display3DBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_Display3DBackground));
  m_display3DBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_Display3DBtn));
  m_display3DLableCtrl.SetCenterElement(GetGuiElement(mainmenuhook_Display3DLable));
  m_exitBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ExitBackground));
  m_exitBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ExitBtn));
  m_exitLabelCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ExitLabel));
  m_fitstSplitCtrl.SetCenterElement(GetGuiElement(mainmenuhook_FitstSplit));
  m_hSplitCtrl.SetCenterElement(GetGuiElement(mainmenuhook_HSplit));
  m_historyRecordBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_HistoryRecordBackground));
  m_historyRecordBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_HistoryRecordBtn));
  m_historyRecordLableCtrl.SetCenterElement(GetGuiElement(mainmenuhook_HistoryRecordLable));
  m_myJourneyBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_MyJourneyBackground));
  m_myJourneyBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_MyJourneyBtn));
  m_myJourneyLableCtrl.SetCenterElement(GetGuiElement(mainmenuhook_MyJourneyLable));
  m_routePlanBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_RoutePlanBackground));
  m_routePlanBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_RoutePlanBtn));
  m_routePlanLableCtrl.SetCenterElement(GetGuiElement(mainmenuhook_RoutePlanLable));
  m_secondSplitCtrl.SetCenterElement(GetGuiElement(mainmenuhook_SecondSplit));
  m_serveBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ServeBackground));
  m_serveBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ServeBtn));
  m_serveLableCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ServeLable));
  m_settingBackgroundCtrl.SetCenterElement(GetGuiElement(mainmenuhook_SettingBackground));
  m_settingBtnCtrl.SetCenterElement(GetGuiElement(mainmenuhook_SettingBtn));
  m_settingLabelCtrl.SetCenterElement(GetGuiElement(mainmenuhook_SettingLabel));
  m_thirdSplitCtrl.SetCenterElement(GetGuiElement(mainmenuhook_ThirdSplit));
}

short CMainMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mainmenuhook_AddressBookBackground:
  case mainmenuhook_AddressBookBtn:
  case mainmenuhook_AddressBookLable:
    {
      m_addressBookBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_addressBookBackgroundCtrl, m_addressBookBtnCtrl, m_addressBookLableCtrl);
    }
    break;
  case mainmenuhook_Display3DBackground:
  case mainmenuhook_Display3DBtn:
  case mainmenuhook_Display3DLable:
    {
      m_display3DBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_display3DBackgroundCtrl, m_display3DBtnCtrl, m_display3DLableCtrl);
    }
    break;
  case mainmenuhook_ExitBackground:
  case mainmenuhook_ExitBtn:
  case mainmenuhook_ExitLabel:
    {
      m_exitBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_exitBackgroundCtrl, m_exitBtnCtrl, m_exitLabelCtrl);
      
    }
    break;
  case mainmenuhook_HistoryRecordBackground:
  case mainmenuhook_HistoryRecordBtn:
  case mainmenuhook_HistoryRecordLable:
    {
      m_historyRecordBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_historyRecordBackgroundCtrl, m_historyRecordBtnCtrl, m_historyRecordLableCtrl);
    }
    break;
  case mainmenuhook_MyJourneyBackground:
  case mainmenuhook_MyJourneyBtn:
  case mainmenuhook_MyJourneyLable:
    {
      m_myJourneyBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_myJourneyBackgroundCtrl, m_myJourneyBtnCtrl, m_myJourneyLableCtrl);
    }
    break;
  case mainmenuhook_RoutePlanBackground:
  case mainmenuhook_RoutePlanBtn:
  case mainmenuhook_RoutePlanLable:
    {
      m_routePlanBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_routePlanBackgroundCtrl, m_routePlanBtnCtrl, m_routePlanLableCtrl);
    }
    break;
  case mainmenuhook_ServeBackground:
  case mainmenuhook_ServeBtn:
  case mainmenuhook_ServeLable:
    {
      m_serveBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_serveBackgroundCtrl, m_serveBtnCtrl, m_serveLableCtrl);
    }
    break;
  case mainmenuhook_SettingBackground:
  case mainmenuhook_SettingBtn:
  case mainmenuhook_SettingLabel:
    {
      m_settingBackgroundCtrl.MouseDown();
      MOUSEDONW_3RENDERCTRL(m_settingBackgroundCtrl, m_settingBtnCtrl, m_settingLabelCtrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CMainMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMainMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case mainmenuhook_AddressBookBackground:
  case mainmenuhook_AddressBookBtn:
  case mainmenuhook_AddressBookLable:
    {
      m_addressBookBackgroundCtrl.MouseUp();
      TurnTo(DHT_MyAddressBookHook);
    }
    break;
  case mainmenuhook_Display3DBackground:
  case mainmenuhook_Display3DBtn:
  case mainmenuhook_Display3DLable:
    {
      m_display3DBackgroundCtrl.MouseUp();
    }
    break;
  case mainmenuhook_ExitBackground:
  case mainmenuhook_ExitBtn:
  case mainmenuhook_ExitLabel:
    {
      m_exitBackgroundCtrl.MouseUp();
      CMessageDialogEvent dialogEvent(this, CViewHook::DHT_MainMenuHook, &UeGui::CMainMenuHook::OnClose);
      CMessageDialogHook::ShowMessageDialog(MB_INFORMATION, "确定要关闭系统？", dialogEvent);
      m_isNeedRefesh = false;
    }
    break;
  case mainmenuhook_HistoryRecordBackground:
  case mainmenuhook_HistoryRecordBtn:
  case mainmenuhook_HistoryRecordLable:
    {
      m_historyRecordBackgroundCtrl.MouseUp();
      TurnTo(DHT_HistoryInformationHook);
    }
    break;
  case mainmenuhook_MyJourneyBackground:
  case mainmenuhook_MyJourneyBtn:
  case mainmenuhook_MyJourneyLable:
    {
      m_myJourneyBackgroundCtrl.MouseUp();
      TurnTo(DHT_MyJourneyHook);
    }
    break;
  case mainmenuhook_RoutePlanBackground:
  case mainmenuhook_RoutePlanBtn:
  case mainmenuhook_RoutePlanLable:
    {
      m_routePlanBackgroundCtrl.MouseUp();
    }
    break;
  case mainmenuhook_ServeBackground:
  case mainmenuhook_ServeBtn:
  case mainmenuhook_ServeLable:
    {
      m_serveBackgroundCtrl.MouseUp();
    }
    break;
  case mainmenuhook_SettingBackground:
  case mainmenuhook_SettingBtn:
  case mainmenuhook_SettingLabel:
    {
      m_settingBackgroundCtrl.MouseUp();
      TurnTo(DHT_SystemSettingHook);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}


void CMainMenuHook::OnClose( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    ::SendMessage((HWND)(IView::GetView()->GetWndHandle()), WM_DESTROY, NULL, NULL);
  }
}
