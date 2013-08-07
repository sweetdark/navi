#include "myinformationhook.h"
#include "myaddressbookhook.h"
#include "myjourneyhook.h"
#include "settingwrapper.h"
#include "guioperationright.h"
#include "userdatawrapper.h"

using namespace UeGui;

CMyInformationHook::CMyInformationHook() : m_userWrapper(CUserDataWrapper::Get())
{
}

CMyInformationHook::~CMyInformationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMyInformationHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

void CMyInformationHook::Init()
{
  //初始化权限
  m_oneBtnCtrl.SetEnable(false);
  m_twoBtnCtrl.SetEnable(false);
  m_threeBtnCtrl.SetEnable(false);
  m_fourBtnCtrl.SetEnable(false);
  m_fiveBtnCtrl.SetEnable(false);

  if (CGuiOperationRight::HaveRight(OR_HISINFORMATION))
  {
    m_oneBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_MYADDRESSBOOK))
  {
    m_twoBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_MYJOURNEY))
  {
    m_threeBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_MYFAVORITE))
  {
    m_fourBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_CAPACITY))
  {
    m_fiveBtnCtrl.SetEnable(true);
  }
}

void UeGui::CMyInformationHook::Load()
{
  //初始化历史信息按钮状态
  if (CGuiOperationRight::HaveRight(OR_HISINFORMATION))
  {
    int RecordCount = 0;
    int HistoryRecordCount = 0;
    RecordCount = m_userWrapper.GetRecentCount();
    HistoryRecordCount = m_userWrapper.GetHistoryRecordCount();

    if ((HistoryRecordCount > 0) || (RecordCount > 0))
    {
      m_oneBtnCtrl.SetEnable(true);
    }
    else
    {
      m_oneBtnCtrl.SetEnable(false);
    }
  }
  //初始化地址簿按钮状态
  if (CGuiOperationRight::HaveRight(OR_MYADDRESSBOOK))
  {
    if (m_userWrapper.GetFavoriteCount() > 0)
    {
      m_twoBtnCtrl.SetEnable(true);
    }
    else
    {
      m_twoBtnCtrl.SetEnable(false);
    }
  }
  //初始化我的行程按钮状态
  if (CGuiOperationRight::HaveRight(OR_MYJOURNEY))
  {
    if (CUserDataWrapper::Get().GetJourneyCount() > 0)
    {
      m_threeBtnCtrl.SetEnable(true);
    }
    else
    {
      m_threeBtnCtrl.SetEnable(false);
    }
  }
}

tstring CMyInformationHook::GetBinaryFileName()
{
  return _T("myinformationhook.bin");
}

void CMyInformationHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_OneBtn,	"OneBtn"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_OneText,	"OneText"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_TwoBtn,	"TwoBtn"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_TwoText,	"TwoText"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_ThreeBtn,	"ThreeBtn"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_ThreeText,	"ThreeText"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_FourBtn,	"FourBtn"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_FourText,	"FourText"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_FiveBtn,	"FiveBtn"));
  m_ctrlNames.insert(GuiName::value_type(MyInformationHook_FiveText,	"FiveText"));
}

void CMyInformationHook::MakeControls()
{
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(MyInformationHook_GotoMapBtn));
  m_backButtonCtrl.SetIconElement(GetGuiElement(MyInformationHook_BackButton));
  m_oneBtnCtrl.SetIconElement(GetGuiElement(MyInformationHook_OneBtn));
  m_oneBtnCtrl.SetCenterElement(GetGuiElement(MyInformationHook_OneText));  
  m_twoBtnCtrl.SetIconElement(GetGuiElement(MyInformationHook_TwoBtn));
  m_twoBtnCtrl.SetCenterElement(GetGuiElement(MyInformationHook_TwoText));
  m_threeBtnCtrl.SetIconElement(GetGuiElement(MyInformationHook_ThreeBtn));
  m_threeBtnCtrl.SetCenterElement(GetGuiElement(MyInformationHook_ThreeText));
  m_fourBtnCtrl.SetIconElement(GetGuiElement(MyInformationHook_FourBtn));
  m_fourBtnCtrl.SetCenterElement(GetGuiElement(MyInformationHook_FourText));
  m_fiveBtnCtrl.SetIconElement(GetGuiElement(MyInformationHook_FiveBtn));
  m_fiveBtnCtrl.SetCenterElement(GetGuiElement(MyInformationHook_FiveText));
}

short CMyInformationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MyInformationHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
      AddRenderUiControls(&m_backButtonCtrl);
    }
    break;
  case MyInformationHook_BackGround:
  case MyInformationHook_BackGroundText:
    break;
  case MyInformationHook_FiveBtn:
  case MyInformationHook_FiveText:
    {
      m_fiveBtnCtrl.MouseDown();
      if (m_fiveBtnCtrl.IsEnable())
        AddRenderUiControls(&m_fiveBtnCtrl);
    }
    break;
  case MyInformationHook_FourBtn:
  case MyInformationHook_FourText:
    {
      m_fourBtnCtrl.MouseDown();
      if (m_fourBtnCtrl.IsEnable())
        AddRenderUiControls(&m_fourBtnCtrl);
    }
    break;
  case MyInformationHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
      AddRenderUiControls(&m_gotoMapBtnCtrl);
    }
    break;
  case MyInformationHook_OneBtn:
  case MyInformationHook_OneText:
    {
      m_oneBtnCtrl.MouseDown();
      if (m_oneBtnCtrl.IsEnable())
        AddRenderUiControls(&m_oneBtnCtrl);
    }
    break;
  case MyInformationHook_ThreeBtn:
  case MyInformationHook_ThreeText:
    {
      m_threeBtnCtrl.MouseDown();
      if (m_threeBtnCtrl.IsEnable())
        AddRenderUiControls(&m_threeBtnCtrl);
    }
    break;
  case MyInformationHook_TwoBtn:
  case MyInformationHook_TwoText:
    {
      m_twoBtnCtrl.MouseDown();
      if (m_twoBtnCtrl.IsEnable())
        AddRenderUiControls(&m_twoBtnCtrl);
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

short CMyInformationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMyInformationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MyInformationHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }   
      AddRenderUiControls(&m_gotoMapBtnCtrl);
    }
    break;
  case MyInformationHook_BackButton:
    {
      m_backButtonCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_MyInformationHook;
        CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;*/
        CAggHook::Return();
        ((CAggHook*)m_view->GetHook(CViewHook::DHT_MyInformationHook))->Load();
      }     
      AddRenderUiControls(&m_backButtonCtrl);
    }
    break;
  case MyInformationHook_OneBtn:
  case MyInformationHook_OneText:
    {
      m_oneBtnCtrl.MouseUp();
      //打开历史信息
      if ((ctrlType == m_downElementType) && (m_oneBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_HistoryInformationHook;*/
        CAggHook::TurnTo(CViewHook::DHT_HistoryInformationHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_HistoryInformationHook);
        aggHook->Load();
        AddRenderUiControls(&m_oneBtnCtrl);
      }
    }
    break;
  case MyInformationHook_TwoBtn:
  case MyInformationHook_TwoText:
    {
      //打开地址簿
      m_twoBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_twoBtnCtrl.IsEnable()))
      {
        CMyAddressBookHook *addrBookHook = (CMyAddressBookHook *)m_view->GetHook(CViewHook::DHT_MyAddressBookHook);
        addrBookHook->Init();
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MyAddressBookHook;*/
        CAggHook::TurnTo(DHT_MyAddressBookHook);
        AddRenderUiControls(&m_twoBtnCtrl);
      }
    }
    break;
  case MyInformationHook_ThreeBtn:
  case MyInformationHook_ThreeText:
    {
      //打开我的行程
      m_threeBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_threeBtnCtrl.IsEnable()))
      {
        //从配置中读取我的行程容量上限
        CSettingWrapper &settingWrapper = CSettingWrapper::Get();
        size_t maxCapacity = settingWrapper.GetMyJourneyCapacity();

        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MyJourneyHook;*/
        CAggHook::TurnTo(CViewHook::DHT_MyJourneyHook);
        CMyJourneyHook* myJourneyHook = (CMyJourneyHook*)m_view->GetHook(CViewHook::DHT_MyJourneyHook);
        myJourneyHook->DynamicShow();
        AddRenderUiControls(&m_threeBtnCtrl);
      }
    }
    break;
  case MyInformationHook_FourBtn:
  case MyInformationHook_FourText:
    {
      //打开常用点
      m_fourBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_fourBtnCtrl.IsEnable()))
      {
        AddRenderUiControls(&m_fourBtnCtrl);
      }      
    }
    break;
  case MyInformationHook_FiveBtn:
  case MyInformationHook_FiveText:
    {
      //打开容量信息
      m_fiveBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_fiveBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_CapacityInformationHook;*/
        CAggHook::TurnTo(DHT_CapacityInformationHook);
        CAggHook* aggHook = (CAggHook*)m_view->GetHook(CViewHook::DHT_CapacityInformationHook);
        aggHook->Load();
        AddRenderUiControls(&m_fiveBtnCtrl);
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

bool CMyInformationHook::operator ()()
{
  return false;
}