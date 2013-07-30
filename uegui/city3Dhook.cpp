#include "city3dhook.h"

using namespace UeGui;

CCity3DHook::CCity3DHook()
{
}

CCity3DHook::~CCity3DHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCity3DHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  ConfigInit();
}

tstring CCity3DHook::GetBinaryFileName()
{
  return _T("city3dhook.bin");
}

void CCity3DHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(City3DHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_OneBtn,	"OneBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_OneText,	"OneText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_TwoBtn,	"TwoBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_TwoText,	"TwoText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_ThreeBtn,	"ThreeBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_ThreeText,	"ThreeText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_FourBtn,	"FourBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_FourText,	"FourText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_FiveBtn,	"FiveBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_FiveText,	"FiveText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_SixBtn,	"SixBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_SixText,	"SixText"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_UpBtn,	"UpBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_UpBtnIcon,	"UpBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_DownBtn,	"DownBtn"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_DownBtnIcon,	"DownBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(City3DHook_PageText,	"PageText"));
}

void CCity3DHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(City3DHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_GotoMapBtn));
  m_downBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_DownBtn));
  m_downBtnCtrl.SetIconElement(GetGuiElement(City3DHook_DownBtnIcon));
  m_upBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_UpBtn));
  m_upBtnCtrl.SetIconElement(GetGuiElement(City3DHook_UpBtnIcon));

  m_oneBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_OneBtn));
  m_oneBtnCtrl.SetLabelElement(GetGuiElement(City3DHook_OneText));
  m_twoBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_TwoBtn));
  m_twoBtnCtrl.SetLabelElement(GetGuiElement(City3DHook_TwoText));
  m_threeBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_ThreeBtn));
  m_threeBtnCtrl.SetLabelElement(GetGuiElement(City3DHook_ThreeText));
  m_fourBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_FourBtn));
  m_fourBtnCtrl.SetLabelElement(GetGuiElement(City3DHook_FourText));
  m_fiveBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_FiveBtn));
  m_fiveBtnCtrl.SetLabelElement(GetGuiElement(City3DHook_FiveText));
  m_sixBtnCtrl.SetCenterElement(GetGuiElement(City3DHook_SixBtn));
  m_sixBtnCtrl.SetLabelElement(GetGuiElement(City3DHook_SixText));
  //m_pageTextCtrl.SetCenterElement(GetGuiElement(City3DHook_PageText));
}

short CCity3DHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case City3DHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case City3DHook_DownBtn:
  case City3DHook_DownBtnIcon:
    {
      m_downBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_FiveBtn:
  case City3DHook_FiveText:
    {
      m_fiveBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_FourBtn:
  case City3DHook_FourText:
    {
      m_fourBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_OneBtn:
  case City3DHook_OneText:
    {
      m_oneBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_SixBtn:
  case City3DHook_SixText:
    {
      m_sixBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_ThreeBtn:
  case City3DHook_ThreeText:
    {
      m_threeBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_TwoBtn:
  case City3DHook_TwoText:
    {
      m_twoBtnCtrl.MouseDown();
    }
    break;
  case City3DHook_UpBtn:
  case City3DHook_UpBtnIcon:
    {
      m_upBtnCtrl.MouseDown();
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

short CCity3DHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCity3DHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case City3DHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_curHookType = CViewHook::m_prevHookType; */       
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case City3DHook_DownBtn:
  case City3DHook_DownBtnIcon:
    {
      m_downBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_FiveBtn:
  case City3DHook_FiveText:
    {
      m_fiveBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_FourBtn:
  case City3DHook_FourText:
    {
      m_fourBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_OneBtn:
  case City3DHook_OneText:
    {
      m_oneBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_PageText:
    break;
  case City3DHook_SixBtn:
  case City3DHook_SixText:
    {
      m_sixBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_ThreeBtn:
  case City3DHook_ThreeText:
    {
      m_threeBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_TwoBtn:
  case City3DHook_TwoText:
    {
      m_twoBtnCtrl.MouseUp();
    }
    break;
  case City3DHook_UpBtn:
  case City3DHook_UpBtnIcon:
    {
      m_upBtnCtrl.MouseUp();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CCity3DHook::operator ()()
{
  return false;
}

void CCity3DHook::ConfigInit()
{
  m_threeBtnCtrl.SetEnable(false);
  m_fourBtnCtrl.SetEnable(false);
  m_fiveBtnCtrl.SetEnable(false);
  m_sixBtnCtrl.SetEnable(false);
  m_upBtnCtrl.SetEnable(false);
  m_downBtnCtrl.SetEnable(false);
}