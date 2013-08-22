#include "myaddressbookhook.h"
#include "maphook.h"
#include "messagedialoghook.h"
using namespace UeGui;

CMyAddressBookHook::CMyAddressBookHook():m_userWrapper(CUserDataWrapper::Get())
{
  m_strTitle = "地址簿";
  m_vecHookFile.push_back(_T("myaddressbookhook.bin"));
}
char CMyAddressBookHook::m_sendName[128]={};
char CMyAddressBookHook::m_sendAddr[128]={};
char CMyAddressBookHook::m_sendTelphone[128]={};

CMyAddressBookHook::~CMyAddressBookHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMyAddressBookHook::GetAddressBookInfor(char* name, char* address,char* telephone)
{
  strcpy(name, m_sendName);
  strcpy(address, m_sendAddr);
  strcpy(telephone, m_sendTelphone);
}

void CMyAddressBookHook::Load()
{
  //清空行数据
  CleanAllRow();
  //清空页数信息
  m_currentPageInfoCtrl.ClearCaption();
  m_totalPageInfoCtrl.ClearCaption();
  m_cleanButtonCtrl.SetEnable(false);
  m_importButtonCtrl.SetEnable(false);
  m_exportButtonCtrl.SetEnable(false);
  m_nextPageCtrl.SetEnable(false);
  m_previousPageCtrl.SetEnable(false);
  //
  SetAddressBookPageInfo();
  ListShowPage(ABHRT_ONE);
}

void CMyAddressBookHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_RowOne,	"RowOne"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_RowTwo,	"RowTwo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_RowThree,	"RowThree"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_RowFour,	"RowFour"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_RowFive,	"RowFive"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_RowSix,	"RowSix"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditOne,	"EditOne"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditTwo,	"EditTwo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditThree,	"EditThree"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditFour,	"EditFour"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditFive,	"EditFive"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditSix,	"EditSix"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteOne,	"DeleteOne"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteTwo,	"DeleteTwo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteThree,	"DeleteThree"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteFour,	"DeleteFour"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteFive,	"DeleteFive"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteSix,	"DeleteSix"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditOneIcon,	"EditOneIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditTwoIcon,	"EditTwoIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditThreeIcon,	"EditThreeIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditFourIcon,	"EditFourIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditFiveIcon,	"EditFiveIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_EditeSixIcon,	"EditeSixIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteOneIcon,	"DeleteOneIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteTwoIcon,	"DeleteTwoIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteThreeIcon,	"DeleteThreeIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteFourIcon,	"DeleteFourIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteFiveIcon,	"DeleteFiveIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_DeleteSixIcon,	"DeleteSixIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_PreviousPage,	"PreviousPage"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NextPage,	"NextPage"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_CurrentPageInfo,	"CurrentPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_TotalPageInfo,	"TotalPageInfo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_PageSeparator,	"PageSeparator"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_PreviousPageIcon,	"PreviousPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NextPageIcon,	"NextPageIcon"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_CleanButton,	"CleanButton"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_ImportButton,	"ImportButton"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_ExportButton,	"ExportButton"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NameOne,	"NameOne"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NameTwo,	"NameTwo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NameThree,	"NameThree"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NameFour,	"NameFour"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NameFive,	"NameFive"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_NameSix,	"NameSix"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SearchOne,	"SearchOne"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SearchTwo,	"SearchTwo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SearchThree,	"SearchThree"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SearchFour,	"SearchFour"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SearchFive,	"SearchFive"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SearchSix,	"SearchSix"));  
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SeparotorOne,	"SeparatorOne"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SeparotorTwo,	"SeparatorTwo"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SeparotorThree,	"SeparatorThree"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SeparotorFour,	"SeparatorFour"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SeparotorFive,	"SeparatorFive"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_SeparotorSix,	"SeparatorSix"));
  m_ctrlNames.insert(GuiName::value_type(myaddressbookhook_BottomBack,	"BottomBack"));
}

void CMyAddressBookHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_cleanButtonCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_CleanButton));
  m_currentPageInfoCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_CurrentPageInfo));
  m_deleteFiveCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_DeleteFive));
  m_deleteFiveCtrl.SetIconElement(GetGuiElement(myaddressbookhook_DeleteFiveIcon));
  m_deleteFourCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_DeleteFour));
  m_deleteFourCtrl.SetIconElement(GetGuiElement(myaddressbookhook_DeleteFourIcon));
  m_deleteOneCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_DeleteOne));
  m_deleteOneCtrl.SetIconElement(GetGuiElement(myaddressbookhook_DeleteOneIcon));
  m_deleteSixCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_DeleteSix));
  m_deleteSixCtrl.SetIconElement(GetGuiElement(myaddressbookhook_DeleteSixIcon));
  m_deleteThreeCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_DeleteThree));
  m_deleteThreeCtrl.SetIconElement(GetGuiElement(myaddressbookhook_DeleteThreeIcon));
  m_deleteTwoCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_DeleteTwo));
  m_deleteTwoCtrl.SetIconElement(GetGuiElement(myaddressbookhook_DeleteTwoIcon));
  m_editFiveCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_EditFive));
  m_editFiveCtrl.SetIconElement(GetGuiElement(myaddressbookhook_EditFiveIcon));
  m_editFourCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_EditFour));
  m_editFourCtrl.SetIconElement(GetGuiElement(myaddressbookhook_EditFourIcon));
  m_editOneCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_EditOne));
  m_editOneCtrl.SetIconElement(GetGuiElement(myaddressbookhook_EditOneIcon));
  m_editSixCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_EditSix));
  m_editThreeCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_EditThree));
  m_editThreeCtrl.SetIconElement(GetGuiElement(myaddressbookhook_EditThreeIcon));
  m_editTwoCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_EditTwo));
  m_editTwoCtrl.SetIconElement(GetGuiElement(myaddressbookhook_EditTwoIcon));
  m_editSixCtrl.SetIconElement(GetGuiElement(myaddressbookhook_EditeSixIcon));
  m_exportButtonCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_ExportButton));
  m_importButtonCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_ImportButton));
  m_nameFiveCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_NameFive));
  m_nameFourCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_NameFour));
  m_nameOneCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_NameOne));
  m_nameSixCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_NameSix));
  m_nameThreeCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_NameThree));
  m_nameTwoCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_NameTwo));
  m_nextPageCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_NextPage));
  m_nextPageCtrl.SetIconElement(GetGuiElement(myaddressbookhook_NextPageIcon));
  m_previousPageCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_PreviousPage));
  m_previousPageCtrl.SetIconElement(GetGuiElement(myaddressbookhook_PreviousPageIcon));
  m_rowFiveCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_RowFive));
  m_rowFourCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_RowFour));
  m_rowOneCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_RowOne));
  m_rowSixCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_RowSix));
  m_rowThreeCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_RowThree));
  m_rowTwoCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_RowTwo));
  m_totalPageInfoCtrl.SetCenterElement(GetGuiElement(myaddressbookhook_TotalPageInfo));
  m_searchOneCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_SearchOne));
  m_searchTwoCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_SearchTwo));
  m_searchThreeCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_SearchThree));
  m_searchFourCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_SearchFour));
  m_searchFiveCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_SearchFive));
  m_searchSixCtrl.SetLabelElement(GetGuiElement(myaddressbookhook_SearchSix));
}

short CMyAddressBookHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case myaddressbookhook_CleanButton:
    {
      m_cleanButtonCtrl.MouseDown();
      AddRenderUiControls(&m_cleanButtonCtrl);
    }
    break;
  case myaddressbookhook_DeleteFive:
  case myaddressbookhook_DeleteFiveIcon:
    {
      m_deleteFiveCtrl.MouseDown();
      if (!m_deleteFiveCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deleteFiveCtrl);
    }
    break;
  case myaddressbookhook_DeleteFour:
  case myaddressbookhook_DeleteFourIcon:
    {
      m_deleteFourCtrl.MouseDown();
      if (!m_deleteFourCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deleteFourCtrl);
    }
    break;
  case myaddressbookhook_DeleteOne:
  case myaddressbookhook_DeleteOneIcon:
    {
      m_deleteOneCtrl.MouseDown();
      if (!m_deleteOneCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deleteOneCtrl);
    }
    break;
  case myaddressbookhook_DeleteSix:
  case myaddressbookhook_DeleteSixIcon:
    {
      m_deleteSixCtrl.MouseDown();
      if (!m_deleteSixCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deleteSixCtrl);
    }
    break;
  case myaddressbookhook_DeleteThree:
  case myaddressbookhook_DeleteThreeIcon:
    {
      m_deleteThreeCtrl.MouseDown();
      if (!m_deleteThreeCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deleteThreeCtrl);
    }
    break;
  case myaddressbookhook_DeleteTwo:
  case myaddressbookhook_DeleteTwoIcon:
    {
      m_deleteTwoCtrl.MouseDown();
      if (!m_deleteTwoCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_deleteTwoCtrl);
    }
    break;
  case myaddressbookhook_EditFive:
  case myaddressbookhook_EditFiveIcon:
    {
      m_editFiveCtrl.MouseDown();
      if (!m_editFiveCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_editFiveCtrl);
    }
    break;
  case myaddressbookhook_EditFour:
  case myaddressbookhook_EditFourIcon:
    {
      m_editFourCtrl.MouseDown();
      if (!m_editFourCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_editFourCtrl);
    }
    break;
  case myaddressbookhook_EditOne:
  case myaddressbookhook_EditOneIcon:
    {
      m_editOneCtrl.MouseDown();
      if (!m_editOneCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_editOneCtrl);
    }
    break;
  case myaddressbookhook_EditSix:
  case myaddressbookhook_EditeSixIcon:
    {
      m_editSixCtrl.MouseDown();
      if (!m_editSixCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_editSixCtrl);
    }
    break;
  case myaddressbookhook_EditThree:
  case myaddressbookhook_EditThreeIcon:
    {
      m_editThreeCtrl.MouseDown();
      if (!m_editThreeCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_editThreeCtrl);
    }
    break;

  case myaddressbookhook_EditTwo:
  case myaddressbookhook_EditTwoIcon:
    {
      m_editTwoCtrl.MouseDown();
      if (!m_editTwoCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_editTwoCtrl);
    }
    break;
  case myaddressbookhook_ExportButton:
    {
      m_exportButtonCtrl.MouseDown();
      AddRenderUiControls(&m_exportButtonCtrl);
    }
    break;
  case myaddressbookhook_ImportButton:
    {
      m_importButtonCtrl.MouseDown();
      AddRenderUiControls(&m_importButtonCtrl);
    }
    break;
  case myaddressbookhook_NextPage:
  case myaddressbookhook_NextPageIcon:
    {
      m_nextPageCtrl.MouseDown();
      if (!m_nextPageCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_nextPageCtrl);
    }
    break;
  case myaddressbookhook_PreviousPage:
  case myaddressbookhook_PreviousPageIcon:
    {
      m_previousPageCtrl.MouseDown();
      if (!m_previousPageCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_previousPageCtrl);
    }
    break;
  case myaddressbookhook_RowFive:
  case myaddressbookhook_NameFive:
    {
      m_rowFiveCtrl.MouseDown();
      m_nameFiveCtrl.MouseDown();
      AddRenderUiControls(&m_rowFiveCtrl);
      AddRenderUiControls(&m_nameFiveCtrl);
      AddRenderUiControls(&m_deleteFiveCtrl);
      AddRenderUiControls(&m_editFiveCtrl);
      AddRenderUiControls(&m_searchFiveCtrl);
      AddRenderElements(GetGuiElement(myaddressbookhook_SeparotorFive));
    }
    break;
  case myaddressbookhook_RowFour:
  case myaddressbookhook_NameFour:
    {
      m_rowFourCtrl.MouseDown();
      m_nameFourCtrl.MouseDown();
      AddRenderUiControls(&m_rowFourCtrl);
      AddRenderUiControls(&m_nameFourCtrl);
      AddRenderUiControls(&m_deleteFourCtrl);
      AddRenderUiControls(&m_editFourCtrl);
      AddRenderUiControls(&m_searchFourCtrl);
      AddRenderElements(GetGuiElement(myaddressbookhook_SeparotorFour));
    }
    break;
  case myaddressbookhook_RowOne:
  case myaddressbookhook_NameOne:
    {
      m_rowOneCtrl.MouseDown();
      m_nameOneCtrl.MouseDown();
      AddRenderUiControls(&m_rowOneCtrl);
      AddRenderUiControls(&m_nameOneCtrl);
      AddRenderUiControls(&m_deleteOneCtrl);
      AddRenderUiControls(&m_editOneCtrl);
      AddRenderUiControls(&m_searchOneCtrl);
      AddRenderElements(GetGuiElement(myaddressbookhook_SeparotorOne));
    }
    break;
  case myaddressbookhook_RowSix:
  case myaddressbookhook_NameSix:
    {
      m_rowSixCtrl.MouseDown();
      m_nameSixCtrl.MouseDown();
      AddRenderUiControls(&m_rowSixCtrl);
      AddRenderUiControls(&m_nameSixCtrl);
      AddRenderUiControls(&m_deleteSixCtrl);
      AddRenderUiControls(&m_editSixCtrl);
      AddRenderUiControls(&m_searchSixCtrl);
      AddRenderElements(GetGuiElement(myaddressbookhook_SeparotorSix));
    }
    break;
  case myaddressbookhook_RowThree:
  case myaddressbookhook_NameThree:
    {
      m_rowThreeCtrl.MouseDown();
      m_nameThreeCtrl.MouseDown();
      AddRenderUiControls(&m_rowThreeCtrl);
      AddRenderUiControls(&m_nameThreeCtrl);
      AddRenderUiControls(&m_deleteThreeCtrl);
      AddRenderUiControls(&m_editThreeCtrl);
      AddRenderUiControls(&m_searchThreeCtrl);
      AddRenderElements(GetGuiElement(myaddressbookhook_SeparotorThree));
    }
    break;
  case myaddressbookhook_RowTwo:
  case myaddressbookhook_NameTwo:
    {
      m_rowTwoCtrl.MouseDown();
      m_nameTwoCtrl.MouseDown();
      AddRenderUiControls(&m_rowTwoCtrl);
      AddRenderUiControls(&m_nameTwoCtrl);
      AddRenderUiControls(&m_deleteTwoCtrl);
      AddRenderUiControls(&m_editTwoCtrl);
      AddRenderUiControls(&m_searchTwoCtrl);
      AddRenderElements(GetGuiElement(myaddressbookhook_SeparotorTwo));
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

short CMyAddressBookHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMyAddressBookHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case myaddressbookhook_CleanButton:
    {
      m_cleanButtonCtrl.MouseUp();
      if (m_cleanButtonCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        CleanAllRecord();
      }
    }
    break;
  case myaddressbookhook_DeleteFive:
  case myaddressbookhook_DeleteFiveIcon:
    {
      m_deleteFiveCtrl.MouseUp();
      if (m_deleteFiveCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        DeleteRecord(ABHRT_FIVE);
      }
    }
    break;
  case myaddressbookhook_DeleteFour:
  case myaddressbookhook_DeleteFourIcon:
    {
      m_deleteFourCtrl.MouseUp();
      if (m_deleteFourCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        DeleteRecord(ABHRT_FOUR);
      }
    }
    break;
  case myaddressbookhook_DeleteOne:
  case myaddressbookhook_DeleteOneIcon:
    {
      m_deleteOneCtrl.MouseUp();
      if (m_deleteOneCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        DeleteRecord(ABHRT_ONE);
      }
    }
    break;
  case myaddressbookhook_DeleteSix:
  case myaddressbookhook_DeleteSixIcon:
    {
      m_deleteSixCtrl.MouseUp();
      if (m_deleteSixCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        DeleteRecord(ABHRT_SIX);
      }
    }
    break;
  case myaddressbookhook_DeleteThree:
  case myaddressbookhook_DeleteThreeIcon:
    {
      m_deleteThreeCtrl.MouseUp();
      if (m_deleteThreeCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        DeleteRecord(ABHRT_THREE);
      }
    }
    break;
  case myaddressbookhook_DeleteTwo:
  case myaddressbookhook_DeleteTwoIcon:
    {
      m_deleteTwoCtrl.MouseUp();
      if (m_deleteTwoCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        DeleteRecord(ABHRT_THREE);
      }
    }
    break;
  case myaddressbookhook_EditFive:
  case myaddressbookhook_EditFiveIcon:
    {
      m_editFiveCtrl.MouseUp();
      if (m_editFiveCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_FIVE;
        int currentPage = m_pageTurning.GetCurrentPage();
        CDetailEditHook::RecordPosition((currentPage-1)*6 + m_recordType - 1);
        OnEidt(this,ABHRT_FIVE,currentPage);
        TurnTo(DHT_DetailEditHook);
      }
    }
    break;
  case myaddressbookhook_EditFour:
  case myaddressbookhook_EditFourIcon:
    {
      m_editFourCtrl.MouseUp();
      if (m_editFourCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_FOUR;
        int currentPage = m_pageTurning.GetCurrentPage();
        CDetailEditHook::RecordPosition((currentPage-1)*6 + m_recordType - 1);
        OnEidt(this,ABHRT_FOUR,currentPage);
        TurnTo(DHT_DetailEditHook);
      }
    }
    break;
  case myaddressbookhook_EditOne:
  case myaddressbookhook_EditOneIcon:
    {
      m_editOneCtrl.MouseUp();
      if (m_editOneCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_ONE;
        int currentPage = m_pageTurning.GetCurrentPage();
        CDetailEditHook::RecordPosition((currentPage-1)*6 + m_recordType - 1);
        OnEidt(this,ABHRT_ONE,currentPage);
        TurnTo(DHT_DetailEditHook);
      }
    }
    break;
  case myaddressbookhook_EditSix:
  case myaddressbookhook_EditeSixIcon:
    {
      m_editSixCtrl.MouseUp();
      if (m_editSixCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_SIX;
        int currentPage = m_pageTurning.GetCurrentPage();
        CDetailEditHook::RecordPosition((currentPage-1)*6 + m_recordType - 1);
        OnEidt(this,ABHRT_SIX,currentPage);
        TurnTo(DHT_DetailEditHook);
      }
    }
    break;
  case myaddressbookhook_EditThree:
  case myaddressbookhook_EditThreeIcon:
    {
      m_editThreeCtrl.MouseUp();
      if (m_editThreeCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_THREE;
        int currentPage = m_pageTurning.GetCurrentPage();
        CDetailEditHook::RecordPosition((currentPage-1)*6 + m_recordType - 1);
        OnEidt(this,ABHRT_THREE,currentPage);
        TurnTo(DHT_DetailEditHook);
      }
    }
    break;
  case myaddressbookhook_EditTwo:
  case myaddressbookhook_EditTwoIcon:
    {
      m_editTwoCtrl.MouseUp();
      if (m_editTwoCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_TWO;
        int currentPage = m_pageTurning.GetCurrentPage();
        CDetailEditHook::RecordPosition((currentPage-1)*6 + m_recordType - 1);
        OnEidt(this,ABHRT_TWO,currentPage);
        TurnTo(DHT_DetailEditHook);
      }
    }
    break;
  case myaddressbookhook_ExportButton:
    {
      m_exportButtonCtrl.MouseUp();
      if (m_exportButtonCtrl.IsEnable())
      {
        ExportAddrbook();
      }
    }
    break;
  case myaddressbookhook_ImportButton:
    {
      m_importButtonCtrl.MouseUp();
      if (m_importButtonCtrl.IsEnable())
      {
        ImportAddrbook();
      }
    }
    break;
  case myaddressbookhook_NextPage:
  case myaddressbookhook_NextPageIcon:
    {
      m_nextPageCtrl.MouseUp();
      if (m_nextPageCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_pageTurning.NextPage();
        CleanAllRow();        
        ListShowPage(m_pageTurning.GetCurrentPage());
        setpagetext();
        SwitchPageBtnVisble();
      }
    }
    break;
  case myaddressbookhook_PreviousPage:
  case myaddressbookhook_PreviousPageIcon:
    {
      m_previousPageCtrl.MouseUp();
      if (m_previousPageCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_pageTurning.PreviousPage();
        CleanAllRow();        
        ListShowPage(m_pageTurning.GetCurrentPage());
        setpagetext();
        SwitchPageBtnVisble();
      }
    }
    break;
  case myaddressbookhook_RowFive:
  case myaddressbookhook_NameFive:
    {
      m_rowFiveCtrl.MouseUp();
      m_nameFiveCtrl.MouseUp();
      if (m_rowFiveCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_FIVE;
        int currentPage = m_pageTurning.GetCurrentPage();
        GoToMapPosition((currentPage-1)*6+m_recordType);
      }
    }
    break;
  case myaddressbookhook_RowFour:
  case myaddressbookhook_NameFour:
    {
      m_rowFourCtrl.MouseUp();
      m_nameFourCtrl.MouseUp();
      if (m_rowFourCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_FOUR;
        int currentPage = m_pageTurning.GetCurrentPage();
        GoToMapPosition((currentPage-1)*6+m_recordType);
      }
    }
    break;
  case myaddressbookhook_RowOne:
  case myaddressbookhook_NameOne:
    {
      m_rowOneCtrl.MouseUp();
      m_nameOneCtrl.MouseUp();
      if (m_rowOneCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_ONE;
        int currentPage = m_pageTurning.GetCurrentPage();
        GoToMapPosition((currentPage-1)*6+m_recordType);
      }
    }
    break;
  case myaddressbookhook_RowSix:
  case myaddressbookhook_NameSix:
    {
      m_rowSixCtrl.MouseUp();
      m_nameSixCtrl.MouseUp();
      if (m_rowSixCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_SIX;
        int currentPage = m_pageTurning.GetCurrentPage();
        GoToMapPosition((currentPage-1)*6+m_recordType);
      }
    }
    break;
  case myaddressbookhook_RowThree:
  case myaddressbookhook_NameThree:
    {
      m_rowThreeCtrl.MouseUp();
      m_nameThreeCtrl.MouseUp();
      if (m_rowThreeCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_THREE;
        int currentPage = m_pageTurning.GetCurrentPage();
        GoToMapPosition((currentPage-1)*6+m_recordType);
      }
    }
    break;
  case myaddressbookhook_RowTwo:
  case myaddressbookhook_NameTwo:
    {
      m_rowTwoCtrl.MouseUp();
      m_nameTwoCtrl.MouseUp();
      if (m_rowTwoCtrl.IsEnable() && ctrlType == m_downElementType)
      {
        m_recordType=ABHRT_TWO;
        int currentPage = m_pageTurning.GetCurrentPage();
        GoToMapPosition((currentPage-1)*6+m_recordType);
      }
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
void CMyAddressBookHook::MyAddressBookCallBack()
{
  IView *view = IView::GetView();
  CMyAddressBookHook* myhook=(CMyAddressBookHook *)(view->GetHook(CViewHook::DHT_MyAddressBookHook));
  myhook->ListShowPage(1);
}
void CMyAddressBookHook::ListShowPage(int n)
{
  if(m_totalData>0)
  {
    int k=(n-1)*EACHPAGECOUNT;
    std::vector<FavoriteEntry> m_fvec;
    m_userWrapper.ConnectToFavorite();
    for(;k<n*EACHPAGECOUNT && k<m_totalData ;k++)
    { 
      const FavoriteEntry* curEntry=m_userWrapper.GetFavorite(k);
      m_fvec.push_back(*curEntry);
    }
    m_userWrapper.DisconnectFavorite(); 
    FavoriteEntry ftmp;
    if(m_fvec.size()<EACHPAGECOUNT)
    {
      int i=m_fvec.size();
      for(;i<EACHPAGECOUNT;i++)
      {
        m_fvec.push_back(ftmp);
      }
    }
    if(m_fvec.size()>EACHPAGECOUNT)
    {
      assert(false);
    }
    DoListShowPage(&m_fvec[0]);     //如果界面要显示的元素的条目增加，则修改DoListShowPage增加PositionTextSetting函数的调用
  }
  if(m_totalData==0)
  {
    FavoriteEntry *ftmp=NULL;
    DoListShowPage(ftmp);
    m_cleanButtonCtrl.SetEnable(false);
  }
  
}
void CMyAddressBookHook::DoListShowPage(const FavoriteEntry* pfe)
{
  if(pfe!=NULL)
  {
    PositionTextSetting((char *)pfe->m_name,&m_rowOneCtrl,&m_nameOneCtrl,&m_deleteOneCtrl,&m_editOneCtrl,&m_searchOneCtrl);
    PositionTextSetting((char *)(++pfe)->m_name,&m_rowTwoCtrl,&m_nameTwoCtrl,&m_deleteTwoCtrl,&m_editTwoCtrl,&m_searchTwoCtrl);
    PositionTextSetting((char *)(++pfe)->m_name,&m_rowThreeCtrl,&m_nameThreeCtrl,&m_deleteThreeCtrl,&m_editThreeCtrl,&m_searchThreeCtrl);
    PositionTextSetting((char *)(++pfe)->m_name,&m_rowFourCtrl,&m_nameFourCtrl,&m_deleteFourCtrl,&m_editFourCtrl,&m_searchFourCtrl);
    PositionTextSetting((char *)(++pfe)->m_name,&m_rowFiveCtrl,&m_nameFiveCtrl,&m_deleteFiveCtrl,&m_editFiveCtrl,&m_searchFiveCtrl);
    PositionTextSetting((char *)(++pfe)->m_name,&m_rowSixCtrl,&m_nameSixCtrl,&m_deleteSixCtrl,&m_editSixCtrl,&m_searchSixCtrl);
  }
  else
  {
    PositionTextSetting("",&m_rowOneCtrl,&m_nameOneCtrl,&m_deleteOneCtrl,&m_editOneCtrl,&m_searchOneCtrl);
    PositionTextSetting("",&m_rowTwoCtrl,&m_nameTwoCtrl,&m_deleteTwoCtrl,&m_editTwoCtrl,&m_searchTwoCtrl);
    PositionTextSetting("",&m_rowThreeCtrl,&m_nameThreeCtrl,&m_deleteThreeCtrl,&m_editThreeCtrl,&m_searchThreeCtrl);
    PositionTextSetting("",&m_rowFourCtrl,&m_nameFourCtrl,&m_deleteFourCtrl,&m_editFourCtrl,&m_searchFourCtrl);
    PositionTextSetting("",&m_rowFiveCtrl,&m_nameFiveCtrl,&m_deleteFiveCtrl,&m_editFiveCtrl,&m_searchFiveCtrl);
    PositionTextSetting("",&m_rowSixCtrl,&m_nameSixCtrl,&m_deleteSixCtrl,&m_editSixCtrl,&m_searchSixCtrl);
  }
}
void CMyAddressBookHook::PositionTextSetting(const char* str,CUiButton* const pbutton,CUiLabel* const plabel,
                                             CUiBitButton* const pdelete, CUiBitButton* const pedit, CUiLabel* const psearch) 
{
  if(str!=NULL && pbutton!=NULL && plabel!=NULL && pdelete!=NULL && pedit!=NULL)
  {
    if(*str=='\0')
    {
      plabel->SetCaption("");
      pbutton->SetEnable(false);
      pdelete->SetEnable(false);
      pedit->SetEnable(false);
      psearch->SetVisible(false);
    }
    else
    {
      plabel->SetCaption(str);
      pbutton->SetEnable(true);
      pdelete->SetEnable(true);
      pedit->SetEnable(true);
      psearch->SetVisible(true);
    }
  }
  else
  {
    assert(false);
  }
}
void CMyAddressBookHook::setpagetext()
{
  char currentPage[10] = {};
  char totalPage[10] = {};
  ::sprintf(currentPage, "%d", m_pageTurning.GetCurrentPage());
  ::sprintf(totalPage, "%d", m_pageTurning.GetTotalPage());
  m_currentPageInfoCtrl.SetCaption(currentPage);
  m_totalPageInfoCtrl.SetCaption(totalPage);
}
void CMyAddressBookHook::SwitchPageBtnVisble()
{
  if (m_pageTurning.IsFirstPage())
  {
    m_previousPageCtrl.SetEnable(false);
  }
  else
  {
    m_previousPageCtrl.SetEnable(true);
  }
  if (m_pageTurning.IsLastPage())
  {
    m_nextPageCtrl.SetEnable(false);
  }
  else
  {
    m_nextPageCtrl.SetEnable(true);
  }
}
int CMyAddressBookHook::GetAddressBookCount()
{
  /*UeQuery::SQLSetting setting;
  m_query->ConnectTo(UeQuery::DT_Favorite, setting);
  int m_curTotalCount=m_query->GetFavoriteCount();
  m_query->Disconnect(UeQuery::DT_Favorite);*/
  int m_curTotalCount = m_userWrapper.GetFavoriteCount();
  return m_curTotalCount;
}
void CMyAddressBookHook::CleanAllRow()
{
  PositionTextSetting("",&m_rowOneCtrl,&m_nameOneCtrl,&m_deleteOneCtrl,&m_editOneCtrl,&m_searchOneCtrl);
  PositionTextSetting("",&m_rowTwoCtrl,&m_nameTwoCtrl,&m_deleteTwoCtrl,&m_editTwoCtrl,&m_searchTwoCtrl);
  PositionTextSetting("",&m_rowThreeCtrl,&m_nameThreeCtrl,&m_deleteThreeCtrl,&m_editThreeCtrl,&m_searchThreeCtrl);
  PositionTextSetting("",&m_rowFourCtrl,&m_nameFourCtrl,&m_deleteFourCtrl,&m_editFourCtrl,&m_searchFourCtrl);
  PositionTextSetting("",&m_rowFiveCtrl,&m_nameFiveCtrl,&m_deleteFiveCtrl,&m_editFiveCtrl,&m_searchFiveCtrl);
  PositionTextSetting("",&m_rowSixCtrl,&m_nameSixCtrl,&m_deleteSixCtrl,&m_editSixCtrl,&m_searchSixCtrl);
}
void CMyAddressBookHook::SetAddressBookPageInfo()
{
  int total = m_userWrapper.GetFavoriteCount();
  m_totalData = total;
  if (total <= 0)
  {
    m_cleanButtonCtrl.SetEnable(false);
    m_exportButtonCtrl.SetEnable(false);
  }
  else
  {
    m_cleanButtonCtrl.SetEnable(true);
    m_exportButtonCtrl.SetEnable(true);
  }
  if (m_userWrapper.IsIOAddrbookExist())
  {
    m_importButtonCtrl.SetEnable(true);
  }
  else
  {
    m_importButtonCtrl.SetEnable(false);
  }
  m_pageTurning.Clear();
  m_pageTurning.SetTotal(total);
  m_pageTurning.SetQuantityOfOnePage(ABHRT_END - 1);
  setpagetext();
  SwitchPageBtnVisble();
}
void UeGui::CMyAddressBookHook::CleanAllRecord()
{
  ////弹出消息对话框确认是否删除该路线
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_MyAddressBookHook, &UeGui::CMyAddressBookHook::OnCleanAllRecord);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所有记录将被删除?", dialogEvent);
}
//清空所有数据事件
void UeGui::CMyAddressBookHook::OnCleanAllRecord( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CMyAddressBookHook* myAddressBookHook = dynamic_cast<CMyAddressBookHook*>(sender);
    myAddressBookHook->RemoveAllFavor();
  }
}
void CMyAddressBookHook::RemoveAllFavor()
{
  //m_userWrapper.ConnectToFavorite();
  /*int fcount=m_userWrapper.GetFavoriteCount();
  int i=0;
  for(;i<fcount;i++)
  {
    m_userWrapper.RemoveFavorite(0);
  }*/
  m_userWrapper.RemoveAllFavorite();
  //m_userWrapper.DisconnectFavorite();
  CleanAllRow();
  SetAddressBookPageInfo();
}
void UeGui::CMyAddressBookHook::OnEidt(CAggHook* sender,AddreessBookHookRecordType type,int page)
{
  CDetailEditEvent editEvent(sender,DHT_MyAddressBookHook);
  EditData edata;
  m_userWrapper.ConnectToFavorite();
  const FavoriteEntry* curEntry = m_userWrapper.GetFavorite((page - 1) * EACHPAGECOUNT + type - 1);
  m_userWrapper.FavoriteEntry2EditData(curEntry, &edata);
  m_userWrapper.DisconnectFavorite();
  m_editHook.ShowDetailEditHook(&edata, editEvent);
  m_recordType=type;
}

//删除某行数据
void UeGui::CMyAddressBookHook::DeleteRecord( AddreessBookHookRecordType row )
{
  m_isNeedRefesh = false;
  //记录要删除的行
  m_recordType = row;
  ////弹出消息对话框确认是否删除该路线
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_MyAddressBookHook, &UeGui::CMyAddressBookHook::OnDeleteRecord);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所选记录将被删除?", dialogEvent);
}
//处理删除轨迹事件
void UeGui::CMyAddressBookHook::OnDeleteRecord( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CMyAddressBookHook* myAddressBookHook = dynamic_cast<CMyAddressBookHook*>(sender);
    myAddressBookHook->RemoveSingleFavor();
  }
}
void CMyAddressBookHook::RemoveSingleFavor()
{
  int n = GetDataIndex(m_recordType);
  if(n>=0 && n<=LIST_LEN && n<=m_totalData)
  {
    m_userWrapper.ConnectToFavorite();
    m_userWrapper.RemoveFavorite(n);
    m_userWrapper.DisconnectFavorite();
    SetAddressBookPageInfo();
    int currentPage = m_pageTurning.GetCurrentPage();
    ListShowPage(currentPage);
  }
  //标记归位
  m_recordType = ABHRT_NONE;
}
void CMyAddressBookHook::GoToMapPosition(int n)
{
    PointList resultList;
    PointInfo pointInfo; 
    CMapHook* maphook = (CMapHook *)m_view->GetHook(CViewHook::DHT_MapHook);
    m_userWrapper.ConnectToFavorite();
    if(n >= 1 && n <= LIST_LEN && n <= m_totalData)
    {
      for (int i = 0; i < m_pageTurning.GetPageEndPosition(); ++i)
      {
      const FavoriteEntry *tempEntry = m_userWrapper.GetFavorite(i); 
      FavoriteEntry curEntry;
      ::memcpy(&curEntry, tempEntry, sizeof(FavoriteEntry));
      const CGeoPoint<long> point(curEntry.m_x,curEntry.m_y);
      char* name = reinterpret_cast<char*>(curEntry.m_name);  
      pointInfo.m_point = point; 
      ::strcpy(pointInfo.m_name, name);
      resultList.push_back(pointInfo);
      if (i == n-1)
      {
        ::strcpy(m_sendName, (char*)curEntry.m_name);
        ::strcpy(m_sendAddr, (char*)curEntry.m_addr);
        ::strcpy(m_sendTelphone, (char*)curEntry.m_telphone);
      }
      }
    } 
    else
    {
      assert(false);
    }
    m_userWrapper.DisconnectFavorite();
    TurnTo(DHT_MapHook);
    maphook->SetPickPos(resultList, n-1);  
}
int UeGui::CMyAddressBookHook::GetDataIndex( AddreessBookHookRecordType& row )
{
  int currentPage = m_pageTurning.GetCurrentPage();
  if (1 == currentPage)
  {
    return row - 1;
  }
  else if (currentPage > 1)
  {
    return m_pageTurning.GetPageStartPosition() + row - 2;
  }
  return -1;
}

void UeGui::CMyAddressBookHook::ExportAddrbook()
{
  m_userWrapper.ConnectToAddrbookRecord();
  if (m_userWrapper.ExportAddrbook() == 0/*SQL_SUCCESS*/)
  {
    m_importButtonCtrl.SetEnable(true);
    CMessageDialogEvent dialogEvent(this, DHT_MyAddressBookHook, NULL);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "已成功导出地址簿至 userdata\\addrbook", dialogEvent);
    Sleep(1000);
    CMessageDialogHook::CloseMessageDialog();
  }
  m_userWrapper.DisconnectAddrbookRecord();
}

void UeGui::CMyAddressBookHook::ImportAddrbook()
{
  if (m_userWrapper.IsIOAddrbookExist())
  {
    int index = 0;
    const FavoriteEntry *fEntry;
    m_userWrapper.ConnectToAddrbookRecord();
    m_userWrapper.ConnectToFavorite();
    while ((fEntry = m_userWrapper.GetAddrbookData(index++)) != NULL)
    {
      FavoriteEntry entry;
      ::memcpy(&entry, fEntry, sizeof(FavoriteEntry));    
      m_userWrapper.AddFavorite(entry); 
    }
    m_userWrapper.DisconnectFavorite();
    m_userWrapper.DisconnectAddrbookRecord();
    Load();
    CMessageDialogEvent dialogEvent(this, DHT_MyAddressBookHook, NULL);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "已成功导入地址簿", dialogEvent);
    Sleep(1000);
    CMessageDialogHook::CloseMessageDialog();
  }
}