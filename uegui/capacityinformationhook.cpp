#include "capacityinformationhook.h"
#include "guisetting.h"
#include "userdatawrapper.h"

using namespace UeGui;

CCapacityInformationHook::CCapacityInformationHook()
{
}

CCapacityInformationHook::~CCapacityInformationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCapacityInformationHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

void CCapacityInformationHook::Init()
{

}

tstring CCapacityInformationHook::GetBinaryFileName()
{
  return _T("capacityinformationhook.bin");
}

void CCapacityInformationHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_OneLabelLeftText,	"OneLabelLeftText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_TwoLabelLeftText,	"TwoLabelLeftText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_ThreeLabelLeftText,	"ThreeLabelLeftText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FourLabelLeftText,	"FourLabelLeftText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FiveLabelLeftText,	"FiveLabelLeftText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_OneProgressBackGround,	"OneProgressBackGround"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_TwoProgressBackGround,	"TwoProgressBackGround"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_ThreeProgressBackGround,	"ThreeProgressBackGround"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FourProgressBackGround,	"FourProgressBackGround"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FiveProgressBackGround,	"FiveProgressBackGround"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_OneProgressleft,	"OneProgressleft"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_OneProgressCenter,	"OneProgressCenter"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_OneProgressRight,	"OneProgressRight"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_TwoProgressleft,	"TwoProgressleft"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_TwoProgressCenter,	"TwoProgressCenter"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_TwoProgressRight,	"TwoProgressRight"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_ThreeProgressleft,	"ThreeProgressleft"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_ThreeProgressCenter,	"ThreeProgressCenter"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_ThreeProgressRight,	"ThreeProgressRight"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FourProgressleft,	"FourProgressleft"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FourProgressCenter,	"FourProgressCenter"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FourProgressRight,	"FourProgressRight"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FiveProgressleft,	"FiveProgressleft"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FiveProgressCenter,	"FiveProgressCenter"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FiveProgressRight,	"FiveProgressRight"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_OneLabelRightText,	"OneLabelRightText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_TwoLabelRightText,	"TwoLabelRightText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_ThreeLabelRightText,	"ThreeLabelRightText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FourLabelRightText,	"FourLabelRightText"));
  m_ctrlNames.insert(GuiName::value_type(CapacityInformationHook_FiveLabelRightText,	"FiveLabelRightText"));
}

void CCapacityInformationHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_BackButton));
  m_backGroundCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_BackGround));
  m_backGroundTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_BackGroundText));
  m_fiveLabelLeftTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_FiveLabelLeftText));
  m_fiveLabelRightTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_FiveLabelRightText));
  m_fiveProgressBackGroundCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_FiveProgressBackGround));
  m_fiveProgressCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_FiveProgressCenter));
  m_fiveProgressCtrl.SetRightElement(GetGuiElement(CapacityInformationHook_FiveProgressRight));
  m_fiveProgressCtrl.SetLeftElement(GetGuiElement(CapacityInformationHook_FiveProgressleft));
  m_fourLabelLeftTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_FourLabelLeftText));
  m_fourLabelRightTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_FourLabelRightText));
  m_fourProgressBackGroundCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_FourProgressBackGround));
  m_fourProgressCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_FourProgressCenter));
  m_fourProgressCtrl.SetRightElement(GetGuiElement(CapacityInformationHook_FourProgressRight));
  m_fourProgressCtrl.SetLeftElement(GetGuiElement(CapacityInformationHook_FourProgressleft));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_GotoMapBtn));
  m_oneLabelLeftTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_OneLabelLeftText));
  m_oneLabelRightTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_OneLabelRightText));
  m_oneProgressBackGroundCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_OneProgressBackGround));
  m_oneProgressCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_OneProgressCenter));
  m_oneProgressCtrl.SetRightElement(GetGuiElement(CapacityInformationHook_OneProgressRight));
  m_oneProgressCtrl.SetLeftElement(GetGuiElement(CapacityInformationHook_OneProgressleft));
  m_threeLabelLeftTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_ThreeLabelLeftText));
  m_threeLabelRightTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_ThreeLabelRightText));
  m_threeProgressBackGroundCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_ThreeProgressBackGround));
  m_threeProgressCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_ThreeProgressCenter));
  m_threeProgressCtrl.SetRightElement(GetGuiElement(CapacityInformationHook_ThreeProgressRight));
  m_threeProgressCtrl.SetLeftElement(GetGuiElement(CapacityInformationHook_ThreeProgressleft));
  m_twoLabelLeftTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_TwoLabelLeftText));
  m_twoLabelRightTextCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_TwoLabelRightText));
  m_twoProgressBackGroundCtrl.SetLabelElement(GetGuiElement(CapacityInformationHook_TwoProgressBackGround));
  m_twoProgressCtrl.SetCenterElement(GetGuiElement(CapacityInformationHook_TwoProgressCenter));
  m_twoProgressCtrl.SetRightElement(GetGuiElement(CapacityInformationHook_TwoProgressRight));
  m_twoProgressCtrl.SetLeftElement(GetGuiElement(CapacityInformationHook_TwoProgressleft));
}

void UeGui::CCapacityInformationHook::Load()
{
  CGuiSettings* guiSettings = CGuiSettings::GetGuiSettings();
  if (guiSettings)
  {
    int curCount = 0;
    int maxCount = 0;

    const CUserDataWrapper& userData = CUserDataWrapper::Get();
    //初始化历史记录容量信息
    curCount = userData.GetHistoryRecordCount();
    maxCount = guiSettings->GetHistoryRecordCapacity();
    
    Progress(curCount, maxCount, &m_oneProgressCtrl, &m_oneLabelRightTextCtrl, &m_oneProgressBackGroundCtrl);

    //初始化历史路线容量信息
    curCount = userData.GetRecentCount();
    maxCount = guiSettings->GetHistoryRouteCapacity();
    
    Progress(curCount, maxCount, &m_twoProgressCtrl, &m_twoLabelRightTextCtrl, &m_twoProgressBackGroundCtrl);

    //初始化历史轨迹容量信息
    curCount = 0;
    maxCount = guiSettings->GetHistoryTrajectoryCapacity();
    Progress(curCount, maxCount, &m_threeProgressCtrl, &m_threeLabelRightTextCtrl, &m_threeProgressBackGroundCtrl);

    //初始化地址簿容量信息
    curCount = userData.GetFavoriteCount();
    maxCount = guiSettings->GetAddressBookCapacity();
    Progress(curCount, maxCount, &m_fourProgressCtrl, &m_fourLabelRightTextCtrl, &m_fourProgressBackGroundCtrl);

    //初始化我的行程容量信息
    maxCount = guiSettings->GetMyJourneyCapacity();
    curCount = userData.GetJourneyCount();
    Progress(curCount, maxCount, &m_fiveProgressCtrl, &m_fiveLabelRightTextCtrl, &m_fiveProgressBackGroundCtrl);
  }
}

short CCapacityInformationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case CapacityInformationHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case CapacityInformationHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
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

short CCapacityInformationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCapacityInformationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case CapacityInformationHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_CapacityInformationHook;
        CViewHook::m_curHookType = CViewHook::DHT_MyInformationHook;*/
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case CapacityInformationHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
      m_gotoMapBtnCtrl.MouseUp();
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

bool CCapacityInformationHook::operator ()()
{
  return false;
}

void CCapacityInformationHook::Progress(int n,int total,CUiButton* const pbutton,CUiLabel* const ptext,CUiLabel* const pback)
{
  if (n < 0 || n > total)
  {
    return;
  }
  else if(n == 0)
  {
    pbutton->SetVisible(false);
  }
  else if(n >= 1 && n <= total)
  {
    int lentmp = pback->GetLabelElement()->m_width - pbutton->GetLeftElement()->m_width - pbutton->GetRightElement()->m_width;
    float ftmp = (float)lentmp / (float)total * (float)n;
    pbutton->GetCenterElement()->m_width = float2int(ftmp);
    pbutton->GetRightElement()->m_startX = pbutton->GetCenterElement()->m_startX + pbutton->GetCenterElement()->m_width;
    pbutton->SetVisible(true);
  }
  char *tmp1 = new char[10];
  char *tmp2 = new char[10];
  ptext->SetCaption(::strcat(::strcat(::_itoa(n, tmp1, 10),"/"), ::_itoa(total,tmp2,10)));
  delete [] tmp1;
  delete [] tmp2;
}

int CCapacityInformationHook::float2int(float f)
{
  int n;
  if(f>=0.5f)
  {
    f+=0.5f;
    n=(int)f;
  }
  else
  {
    n=(int)f;
  }
  return n;
}