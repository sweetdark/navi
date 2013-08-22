#include "eeyelisthook.h"

#include "settingwrapper.h"

#include "eeyesettinghook.h"

#include "messagedialoghook.h"

#include "maphook.h"

using namespace UeGui;

CEEyeListHook::CEEyeListHook()
{
  m_strTitle = "电子眼设置";
  m_vecHookFile.push_back(_T("eeyelisthook.bin"));
}

CEEyeListHook::~CEEyeListHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEEyeListHook::Load()
{
  int dataCount = CUserDataWrapper::Get().GetUserEEyeEntryDataCount();
  m_pageController.SetTotal(dataCount);
  m_pageController.SetQuantityOfOnePage(LISTNUM);
  ShowSettings();
  ShowList();
}

void CEEyeListHook::UnLoad()
{
  unsigned char eyeType = 0;
  if (m_redLightBtn.Checked())
  {
    eyeType |= VoiceSettings::EYE_TrafficLight;
  }
  if (m_speedBtn.Checked())
  {
    eyeType |= VoiceSettings::EYE_Speedding;
  }
  if (m_ruleBtn.Checked())
  {
    eyeType |= VoiceSettings::EYE_IllegalMonitoring;
  }
  if (m_channelBtn.Checked())
  {
    eyeType |= VoiceSettings::EYE_RailwayCrossing;
  }
  CSettingWrapper::Get().SetEEyeType(eyeType);
  CSettingWrapper::Get().SaveNaviationSettings();
}

void CEEyeListHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_RowOne,	"RowOne"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_RowTwo,	"RowTwo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_RowThree,	"RowThree"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_RowFour,	"RowFour"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_RowFive,	"RowFive"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_RowSix,	"RowSix"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditOne,	"EditOne"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditTwo,	"EditTwo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditThree,	"EditThree"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditFour,	"EditFour"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditFive,	"EditFive"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditSix,	"EditSix"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteOne,	"DeleteOne"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteTwo,	"DeleteTwo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteThree,	"DeleteThree"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteFour,	"DeleteFour"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteFive,	"DeleteFive"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteSix,	"DeleteSix"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditOneIcon,	"EditOneIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditTwoIcon,	"EditTwoIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditThreeIcon,	"EditThreeIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditFourIcon,	"EditFourIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditFiveIcon,	"EditFiveIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_EditeSixIcon,	"EditeSixIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteOneIcon,	"DeleteOneIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteTwoIcon,	"DeleteTwoIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteThreeIcon,	"DeleteThreeIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteFourIcon,	"DeleteFourIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteFiveIcon,	"DeleteFiveIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_DeleteSixIcon,	"DeleteSixIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_CurrentPageInfo,	"CurrentPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_TotalPageInfo,	"TotalPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_PageSeparator,	"PageSeparator"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_BottomBack,	"BottomBack"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_CleanButton,	"CleanButton"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_Line4,	"Line4"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_Line5,	"Line5"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_TipLabel,	"TipLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_TipBtn,	"TipBtn"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NameOne,	"NameOne"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NameTwo,	"NameTwo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NameThree,	"NameThree"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NameFour,	"NameFour"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NameFive,	"NameFive"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_NameSix,	"NameSix"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnOne,	"eeyeBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnTwo,	"eeyeBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnThree,	"eeyeBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnFour,	"eeyeBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnOneIcon,	"eeyeBtnOneIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnTwoIcon,	"eeyeBtnTwoIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnThreeIcon,	"eeyeBtnThreeIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnFourIcon,	"eeyeBtnFourIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnOneLabel,	"eeyeBtnOneLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnTwoLabel,	"eeyeBtnTwoLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnThreeLabel,	"eeyeBtnThreeLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeListHook_eeyeBtnFourLabel,	"eeyeBtnFourLabel"));
}

void CEEyeListHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  //列表
  for (int i=0, j=EEyeListHook_RowOne; i<6; i++)
  {
    m_listBtn[i].SetCenterElement(GetGuiElement(j++));
    m_listBtn[i].SetLabelElement(GetGuiElement(j++));
  }
  //编辑按钮
  for (int i=0, j=EEyeListHook_EditOne; i<6; i++)
  {
    m_editBtn[i].SetCenterElement(GetGuiElement(j++));
    m_editBtn[i].SetIconElement(GetGuiElement(j++));
  }
  //删除按钮
  for (int i=0, j=EEyeListHook_DeleteOne; i<6; i++)
  {
    m_deleteBtn[i].SetCenterElement(GetGuiElement(j++));
    m_deleteBtn[i].SetIconElement(GetGuiElement(j++));
  }
  //清空按钮
  m_clearBtn.SetCenterElement(GetGuiElement(EEyeListHook_CleanButton));
  //翻页控制
  m_pageUpBtn.SetCenterElement(GetGuiElement(EEyeListHook_PreviousPage));
  m_pageUpBtn.SetIconElement(GetGuiElement(EEyeListHook_PreviousPageIcon));
  m_pageDownBtn.SetCenterElement(GetGuiElement(EEyeListHook_NextPage));
  m_pageDownBtn.SetIconElement(GetGuiElement(EEyeListHook_NextPageIcon));
  m_curPageLabel.SetLabelElement(GetGuiElement(EEyeListHook_CurrentPageInfo));
  m_totalPageLabel.SetLabelElement(GetGuiElement(EEyeListHook_TotalPageInfo));
  //电子眼类型选择
  m_eeyeBtn.SetIconElement(GetGuiElement(EEyeListHook_TipBtn));

  m_redLightBtn.SetCenterElement(GetGuiElement(EEyeListHook_eeyeBtnOne));
  m_redLightBtn.SetIconElement(GetGuiElement(EEyeListHook_eeyeBtnOneIcon));
  m_redLightBtn.SetLabelElement(GetGuiElement(EEyeListHook_eeyeBtnOneLabel));

  m_speedBtn.SetCenterElement(GetGuiElement(EEyeListHook_eeyeBtnTwo));
  m_speedBtn.SetIconElement(GetGuiElement(EEyeListHook_eeyeBtnTwoIcon));
  m_speedBtn.SetLabelElement(GetGuiElement(EEyeListHook_eeyeBtnTwoLabel));

  m_ruleBtn.SetCenterElement(GetGuiElement(EEyeListHook_eeyeBtnThree));
  m_ruleBtn.SetIconElement(GetGuiElement(EEyeListHook_eeyeBtnThreeIcon));
  m_ruleBtn.SetLabelElement(GetGuiElement(EEyeListHook_eeyeBtnThreeLabel));

  m_channelBtn.SetCenterElement(GetGuiElement(EEyeListHook_eeyeBtnFour));
  m_channelBtn.SetIconElement(GetGuiElement(EEyeListHook_eeyeBtnFourIcon));
  m_channelBtn.SetLabelElement(GetGuiElement(EEyeListHook_eeyeBtnFourLabel));
}

short CEEyeListHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EEyeListHook_PreviousPage:
  case EEyeListHook_PreviousPageIcon:
    {
      m_pageUpBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageUpBtn);
    }
    break;
  case EEyeListHook_NextPage:
  case EEyeListHook_NextPageIcon:
    {
      m_pageDownBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_pageDownBtn);
    }
    break;
  case EEyeListHook_CleanButton:
    {
      m_clearBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_clearBtn);
    }
    break;
  case EEyeListHook_TipBtn:
    {
      m_eeyeBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_eeyeBtn);
    }
    break;
  case EEyeListHook_eeyeBtnOne:
  case EEyeListHook_eeyeBtnOneLabel:
  case EEyeListHook_eeyeBtnOneIcon:
    {
      m_redLightBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_redLightBtn);
    }
    break;
  case EEyeListHook_eeyeBtnTwo:
  case EEyeListHook_eeyeBtnTwoLabel:
  case EEyeListHook_eeyeBtnTwoIcon:
    {
      m_speedBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_speedBtn);
    }
    break;
  case EEyeListHook_eeyeBtnThree:
  case EEyeListHook_eeyeBtnThreeLabel:
  case EEyeListHook_eeyeBtnThreeIcon:
    {
      m_ruleBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_ruleBtn);
    }
    break;
  case EEyeListHook_eeyeBtnFour:
  case EEyeListHook_eeyeBtnFourLabel:
  case EEyeListHook_eeyeBtnFourIcon:
    {
      m_channelBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_channelBtn);
    }
    break;
  default:
    if (ctrlType >= EEyeListHook_RowOne && ctrlType <= EEyeListHook_NameSix)
    {
      int index = (ctrlType-EEyeListHook_RowOne)/2;
      m_listBtn[index].MouseDown();
      MOUSEDOWN_3RENDERCTRL(m_listBtn[index], m_editBtn[index], m_deleteBtn[index]);
    } 
    else if (ctrlType >= EEyeListHook_EditOne && ctrlType <= EEyeListHook_EditeSixIcon)
    {
      int index = (ctrlType-EEyeListHook_EditOne)/2;
      m_editBtn[index].MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_editBtn[index]);
    } 
    else if (ctrlType >= EEyeListHook_DeleteOne && ctrlType <= EEyeListHook_DeleteSixIcon)
    {
      int index = (ctrlType-EEyeListHook_DeleteOne)/2;
      m_deleteBtn[index].MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_deleteBtn[index]);
    } 
    else
    {
      return CMenuBackgroundHook::MouseDown(scrPoint);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CEEyeListHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEEyeListHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EEyeListHook_PreviousPage:
  case EEyeListHook_PreviousPageIcon:
    {
      m_pageUpBtn.MouseUp();
      if (m_pageUpBtn.IsEnable())
      {
        m_pageController.PreviousPage();
        ShowList();
      }
    }
    break;
  case EEyeListHook_NextPage:
  case EEyeListHook_NextPageIcon:
    {
      m_pageDownBtn.MouseUp();
      if (m_pageDownBtn.IsEnable())
      {
        m_pageController.NextPage();
        ShowList();
      }
    }
    break;
  case EEyeListHook_CleanButton:
    {
      m_clearBtn.MouseUp();
      if (m_clearBtn.IsEnable())
      {
        DeleteAllData();
      }
    }
    break;
  case EEyeListHook_TipBtn:
    {
      m_eeyeBtn.MouseUp();
      if (m_eeyeBtn.Checked())
      {
        CSettingWrapper::Get().SetIsEEyeOpen(OS_ON);
        m_redLightBtn.SetEnable(true);
        m_speedBtn.SetEnable(true);
        m_ruleBtn.SetEnable(true);
        m_channelBtn.SetEnable(true);
      }
      else
      {
        CSettingWrapper::Get().SetIsEEyeOpen(OS_OFF);
        m_redLightBtn.SetEnable(false);
        m_speedBtn.SetEnable(false);
        m_ruleBtn.SetEnable(false);
        m_channelBtn.SetEnable(false);
      }
    }
    break;
  case EEyeListHook_eeyeBtnOne:
  case EEyeListHook_eeyeBtnOneLabel:
  case EEyeListHook_eeyeBtnOneIcon:
    {
      m_redLightBtn.MouseUp();
    }
    break;
  case EEyeListHook_eeyeBtnTwo:
  case EEyeListHook_eeyeBtnTwoLabel:
  case EEyeListHook_eeyeBtnTwoIcon:
    {
      m_speedBtn.MouseUp();
    }
    break;
  case EEyeListHook_eeyeBtnThree:
  case EEyeListHook_eeyeBtnThreeLabel:
  case EEyeListHook_eeyeBtnThreeIcon:
    {
      m_ruleBtn.MouseUp();
    }
    break;
  case EEyeListHook_eeyeBtnFour:
  case EEyeListHook_eeyeBtnFourLabel:
  case EEyeListHook_eeyeBtnFourIcon:
    {
      m_channelBtn.MouseUp();
    }
    break;
  default:
    if (ctrlType >= EEyeListHook_RowOne && ctrlType <= EEyeListHook_NameSix)
    {
      m_index = (ctrlType-EEyeListHook_RowOne)/2;
      m_listBtn[m_index].MouseUp();
      if (m_listBtn[m_index].IsEnable())
      {
        TurnTo(DHT_MapHook);
        CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
        pMapHook->SetPickPos(m_pointList, m_index);
      }
    } 
    else if (ctrlType >= EEyeListHook_EditOne && ctrlType <= EEyeListHook_EditeSixIcon)
    {
      m_index = (ctrlType-EEyeListHook_EditOne)/2;
      m_editBtn[m_index].MouseUp();
      if (m_editBtn[m_index].IsEnable())
      {
        CEEyeSettingHook *pSettingHook((CEEyeSettingHook *)(m_view->GetHook(CViewHook::DHT_EEyeSettingHook)));
        pSettingHook->SetEditEEyeData(m_index + m_pageController.GetPageStartPosition() - 1, &m_eEyeEntryDataList[m_index]);
        TurnTo(DHT_EEyeSettingHook);
      }
    } 
    else if (ctrlType >= EEyeListHook_DeleteOne && ctrlType <= EEyeListHook_DeleteSixIcon)
    {
      m_index = (ctrlType-EEyeListHook_DeleteOne)/2;
      m_deleteBtn[m_index].MouseUp();
      if (m_deleteBtn[m_index].IsEnable())
      {
        DeleteOneData();
      }
    } 
    else
    {
      return CMenuBackgroundHook::MouseUp(scrPoint);
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CEEyeListHook::DeleteOneData()
{
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, DHT_EEyeListHook, &CEEyeListHook::DoDeleteOneData);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所选记录将被删除！", dialogEvent);
}

void CEEyeListHook::DoDeleteOneData(CAggHook* sender, ModalResultType modalResult)
{
  if (modalResult == MR_OK)
  {
    CEEyeListHook* eEyelistHook = dynamic_cast<CEEyeListHook*>(sender);
    eEyelistHook->OnDeleteOneData();
  }
}

void CEEyeListHook::OnDeleteOneData()
{
  CUserDataWrapper::Get().DeleteUserEEyeData(m_index + m_pageController.GetPageStartPosition() - 1);
  Load();
}

void CEEyeListHook::DeleteAllData()
{
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, DHT_EEyeListHook, &CEEyeListHook::DoDeleteAllData);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所有记录将被删除！", dialogEvent);
}

void CEEyeListHook::DoDeleteAllData(CAggHook* sender, ModalResultType modalResult)
{
  if (modalResult == MR_OK)
  {
    CUserDataWrapper::Get().DeleteAllUserEEyeData();
    CEEyeListHook* eEyelistHook = dynamic_cast<CEEyeListHook*>(sender);
    eEyelistHook->Load();
  }
}

void CEEyeListHook::ClearList()
{
  for (int i=0; i<LISTNUM; i++)
  {
    m_listBtn[i].SetCaption("");
    m_listBtn[i].SetEnable(false);
    m_editBtn[i].SetEnable(false);
    m_deleteBtn[i].SetEnable(false);
  }
}

void CEEyeListHook::ShowSettings()
{
  bool isOpen = CSettingWrapper::Get().GetIsEEyeOpen();
  m_eeyeBtn.SetCheck(isOpen);
  m_redLightBtn.SetEnable(isOpen);
  m_speedBtn.SetEnable(isOpen);
  m_ruleBtn.SetEnable(isOpen);
  m_channelBtn.SetEnable(isOpen);

  unsigned char eyeType = CSettingWrapper::Get().GetEEyeType();
  if (eyeType & VoiceSettings::EYE_TrafficLight)
  {
    m_redLightBtn.SetCheck(true);
  }
  else
  {
    m_redLightBtn.SetCheck(false);
  }
  if (eyeType & VoiceSettings::EYE_Speedding)
  {
    m_speedBtn.SetCheck(true);
  }
  else
  {
    m_speedBtn.SetCheck(false);
  }
  if (eyeType & VoiceSettings::EYE_IllegalMonitoring)
  {
    m_ruleBtn.SetCheck(true);
  }
  else
  {
    m_ruleBtn.SetCheck(false);
  }
  if (eyeType & VoiceSettings::EYE_RailwayCrossing)
  {
    m_channelBtn.SetCheck(true);
  }
  else
  {
    m_channelBtn.SetCheck(false);
  }
}

void CEEyeListHook::ShowList()
{
  ClearList();
  SetPageBtn();

  int startPos = m_pageController.GetPageStartPosition() - 1;
  int endPos = m_pageController.GetPageEndPosition() - 1;
  if (startPos < 0)
  {
    return;
  }
  //获取数据
  int showNum;
  CUserDataWrapper userDataWrapper(CUserDataWrapper::Get());
  int dataCount = userDataWrapper.GetUserEEyeEntryDataCount();
  dataCount>LISTNUM ? showNum=LISTNUM : showNum=dataCount;
  userDataWrapper.GetUserEEyeEntryData(startPos, showNum, m_eEyeEntryDataList);
  //放到列表
  m_pointList.clear();
  for (int i=0; i<m_eEyeEntryDataList.size(); i++)
  {
    PointInfo pointInfo;
    ::memcpy(pointInfo.m_name, m_eEyeEntryDataList[i].m_name, sizeof(pointInfo.m_name));
    pointInfo.m_point.m_x = m_eEyeEntryDataList[i].m_x;
    pointInfo.m_point.m_y = m_eEyeEntryDataList[i].m_y;
    m_pointList.push_back(pointInfo);

    m_listBtn[i].SetEnable(true);
    m_listBtn[i].SetCaption(pointInfo.m_name);
    m_editBtn[i].SetEnable(true);
    m_deleteBtn[i].SetEnable(true);
  }
}

void CEEyeListHook::SetPageBtn()
{
  char* curPage = m_curPageLabel.GetCaption();
  char* totalPage = m_totalPageLabel.GetCaption();
  ::sprintf(curPage,"%d",m_pageController.GetCurrentPage());
  ::sprintf(totalPage,"%d",m_pageController.GetTotalPage());
  m_curPageLabel.SetCaption(curPage);
  m_totalPageLabel.SetCaption(totalPage);

  if (m_pageController.IsFirstPage() && m_pageController.IsLastPage())
  {
    m_pageUpBtn.SetEnable(false);
    m_pageDownBtn.SetEnable(false);
  }
  else if (m_pageController.IsFirstPage())
  {
    m_pageUpBtn.SetEnable(false);
    m_pageDownBtn.SetEnable(true);
  }
  else if (m_pageController.IsLastPage())
  {
    m_pageUpBtn.SetEnable(true);
    m_pageDownBtn.SetEnable(false);
  }
  else
  {
    m_pageUpBtn.SetEnable(true);
    m_pageDownBtn.SetEnable(true);
  }
}