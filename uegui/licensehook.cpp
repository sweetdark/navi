#include "licensehook.h"
#include "settingwrapper.h"
using namespace UeGui;

tstring CLicenseHook::GetBinaryFileName()
{
  return _T("licensehook.bin");
}

void CLicenseHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  SetButtonInfo();
}

void UeGui::CLicenseHook::Init()
{
  //读取是否开启启动声明
//   CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
//   if (ueSettings)
//   {
//     if (UeBase::OS_ON == settingWrapper.GetIsOpenStartStatement())
//     {
//       m_alwaysAcceptBtn.SetCheck(false);
//     }
//     else
//     {
//       m_alwaysAcceptBtn.SetCheck(true);
//     }
//   }
  m_alwaysAcceptBtn.SetCaption("测试换行\n我的换行\n换行读取\n文A字");
}

void UeGui::CLicenseHook::SaveCheckSetting(bool isAlways)
{
  CSettingWrapper &settingWrapper = CSettingWrapper::Get();
    if (isAlways)
    {
      settingWrapper.SetIsOpenStartStatement(UeBase::OS_OFF);
    }
    else
    {
      settingWrapper.SetIsOpenStartStatement(UeBase::OS_ON);
    }
    settingWrapper.SaveSystemSettings();
}

short CLicenseHook::MouseDown( CGeoPoint<short> &scrPoint )
{
  /*
  * 根据鼠标点击的位置来判断当前应该那个功能区做响应
  */
  short downElementType = CAggHook::MouseDown(scrPoint);
  switch (downElementType)
  {
    case LicenseHook_AcceptBtn:
      {
        //处理对应小按钮的效果
        m_acceptBtn.MouseDown();
        AddRenderUiControls(&m_acceptBtn);
        break;
      }
    case LicenseHook_RejectBtn:
      {
        //处理对应大按钮的效果   
        m_rejectBtn.MouseDown();
        AddRenderUiControls(&m_rejectBtn);
        break;
      }
    case LicenseHook_AlwaysAcceptBtn:
      {
        m_alwaysAcceptBtn.MouseDown();
        AddRenderUiControls(&m_alwaysAcceptBtn);
        break;
      }
    default:
      {
        assert(false);
        m_startpoint = scrPoint;
        m_isNeedRefesh = false;
        break;
      }   
  }
  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return downElementType;
}

short CLicenseHook::MouseUp( CGeoPoint<short> &scrPoint )
{
  /*
  * 根据鼠标点击的位置来判断当前应该那个功能区做响应
  */
  short upElementType = CAggHook::MouseUp(scrPoint);
  
  switch (m_downElementType)
  {
    case LicenseHook_AcceptBtn:
      {
        //处理对应小按钮的效果
        m_acceptBtn.MouseUp();
        //如果弹起和点击为同一按钮则处理事件
        if (upElementType == m_downElementType)
        {
          if (m_acceptBtn.IsEnable())
          {
            SaveCheckSetting(false);
            OpenMapHook(); 
          }
        }
        break;
      }
    case LicenseHook_RejectBtn:
      {
        //处理对应大按钮的效果   
        m_rejectBtn.MouseUp();        
        //如果弹起和点击为同一按钮则处理事件
        if (upElementType == m_downElementType)
        {
          CloseSys();
        }        
        break;
      }
    case LicenseHook_AlwaysAcceptBtn:
      {
        m_alwaysAcceptBtn.MouseUp();
        if (m_alwaysAcceptBtn.IsEnable())
        {
          SaveCheckSetting(true);
          OpenMapHook(); 
        }
        break;
      }
    default:
      {
        assert(false);
        m_startpoint = scrPoint;
        m_isNeedRefesh = false;
        break;
      }   
  }
  if (m_isNeedRefesh)
  {
    m_view->Refresh();
  }
  m_isNeedRefesh = true;
  return upElementType;
}

short CLicenseHook::MouseMove( CGeoPoint<short> &scrPoint )
{
  return -1;
}

void CLicenseHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(LicenseHook_AcceptBtn,	"AcceptBtn"));
  m_ctrlNames.insert(GuiName::value_type(LicenseHook_AlwaysAcceptBtn,	"AlwaysAcceptBtn"));
  m_ctrlNames.insert(GuiName::value_type(LicenseHook_RejectBtn,	"RejectBtn"));
  m_ctrlNames.insert(GuiName::value_type(LicenseHook_LicenseCtrl,	"LicenseCtrl"));
}

bool CLicenseHook::operator()()
{
  return false;
}

void CLicenseHook::SetButtonInfo()
{
  ///
  m_acceptBtn.SetCenterElement(GetGuiElement(LicenseHook_AcceptBtn));
  ///
  m_rejectBtn.SetCenterElement(GetGuiElement(LicenseHook_RejectBtn));    
  ///
  m_alwaysAcceptBtn.SetCenterElement(GetGuiElement(LicenseHook_AlwaysAcceptBtn));
}

void CLicenseHook::OpenMapHook()
{
  CAggHook::GoToMapHook();
}

void CLicenseHook::CloseSys()
{
  ::SendMessage((HWND)m_view->GetWndHandle(), WM_DESTROY, NULL, NULL);
}
