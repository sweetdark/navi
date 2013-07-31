#include "detailmessagehook.h"
#include "uequery\querybasic.h"

using namespace UeGui;

CDetailMessageHook::CDetailMessageHook() : m_userDataWrapper(CUserDataWrapper::Get())
{
  m_strTitle = "œÍœ∏–≈œ¢";
  m_vecHookFile.push_back(_T("detailmessagehook.bin"));
  ::memset(&m_detailInfo, 0, sizeof(DetailInfo));
}

CDetailMessageHook::~CDetailMessageHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CDetailMessageHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_TitleLabel,	"TitleLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AddressLabel,	"AddressLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AddressInfoLabel,	"AddressInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_PhoneLabel,	"PhoneLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_PhoneInfoLabel,	"PhoneInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CodeLabel,	"CodeLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CodeInfoLabel,	"CodeInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SaveLabel,	"SaveLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_Home,	"Home"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_Company,	"Company"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_FavoratePosition1,	"FavoratePosition1"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_FavoratePosition2,	"FavoratePosition2"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_FavoratePosition3,	"FavoratePosition3"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AvoidArea,	"AvoidArea"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_BootPosition,	"BootPosition"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AddressBook,	"AddressBook"));
}

void CDetailMessageHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_titleLabel.SetLabelElement(GetGuiElement(DetailMessageHook_TitleLabel));
  m_addressInfoLabel.SetLabelElement(GetGuiElement(DetailMessageHook_AddressInfoLabel));;
  m_phoneInfoLabel.SetLabelElement(GetGuiElement(DetailMessageHook_PhoneInfoLabel));;
  m_codeInfoLabel.SetLabelElement(GetGuiElement(DetailMessageHook_CodeInfoLabel));;
  m_addressBookBtn.SetCenterElement(GetGuiElement(DetailMessageHook_AddressBook));
  m_homeBtn.SetCenterElement(GetGuiElement(DetailMessageHook_Home));
  m_companyBtn.SetCenterElement(GetGuiElement(DetailMessageHook_Company));
  m_favoratePosition1Btn.SetCenterElement(GetGuiElement(DetailMessageHook_FavoratePosition1));
  m_favoratePosition2Btn.SetCenterElement(GetGuiElement(DetailMessageHook_FavoratePosition2));
  m_favoratePosition3Btn.SetCenterElement(GetGuiElement(DetailMessageHook_FavoratePosition3));
  m_avoidAreaBtn.SetCenterElement(GetGuiElement(DetailMessageHook_AvoidArea));
  m_bootPositionBtn.SetCenterElement(GetGuiElement(DetailMessageHook_BootPosition));
}

void UeGui::CDetailMessageHook::Load()
{
  m_titleLabel.SetCaption(m_detailInfo.m_name);
  m_addressInfoLabel.SetCaption(m_detailInfo.m_address);
  m_phoneInfoLabel.SetCaption(m_detailInfo.m_telephone);
}

void UeGui::CDetailMessageHook::UnLoad()
{

}

void UeGui::CDetailMessageHook::SetDetailInfoData( const DetailInfo& data )
{
  ::memcpy(&m_detailInfo, &data, sizeof(DetailInfo));
}

short CDetailMessageHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DetailMessageHook_AddressBook:
    {
      m_addressBookBtn.MouseDown();
    }
    break;
  case DetailMessageHook_Home:
    {
      m_homeBtn.MouseDown();
    }
    break;
  case DetailMessageHook_Company:
    {
      m_companyBtn.MouseDown();
    }
    break;
  case DetailMessageHook_FavoratePosition1:
    {
      m_favoratePosition1Btn.MouseDown();
    }
    break;
  case DetailMessageHook_FavoratePosition2:
    {
      m_favoratePosition2Btn.MouseDown();
    }
    break;
  case DetailMessageHook_FavoratePosition3:
    {
      m_favoratePosition3Btn.MouseDown();
    }
    break;
  case DetailMessageHook_AvoidArea:
    {
      m_avoidAreaBtn.MouseDown();
    }
    break;
  case DetailMessageHook_BootPosition:
    {
      m_bootPositionBtn.MouseDown();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
    }    
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDetailMessageHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDetailMessageHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case DetailMessageHook_AddressBook:
    {
      m_addressBookBtn.MouseUp();
      SaveToAddressBook();
    }
    break;
  case DetailMessageHook_Home:
    {
      m_homeBtn.MouseUp();
      SaveToCommonPoint(CT_HOME);
    }
    break;
  case DetailMessageHook_Company:
    {
      m_companyBtn.MouseUp();
      SaveToCommonPoint(CT_COMPANY);
    }
    break;
  case DetailMessageHook_FavoratePosition1:
    {
      m_favoratePosition1Btn.MouseUp();
      SaveToCommonPoint(CT_POINTONE);
    }
    break;
  case DetailMessageHook_FavoratePosition2:
    {
      m_favoratePosition2Btn.MouseUp();
      SaveToCommonPoint(CT_POINTTWO);
    }
    break;
  case DetailMessageHook_FavoratePosition3:
    {
      m_favoratePosition3Btn.MouseUp();
      SaveToCommonPoint(CT_POINTTHREE);
    }
    break;
  case DetailMessageHook_AvoidArea:
    {
      m_avoidAreaBtn.MouseUp();
    }
    break;
  case DetailMessageHook_BootPosition:
    {
      m_bootPositionBtn.MouseUp();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
    }
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void UeGui::CDetailMessageHook::SaveToAddressBook()
{
  UeQuery::FavoriteEntry favorData;
  favorData.m_x = m_detailInfo.m_position.m_x;
  favorData.m_y = m_detailInfo.m_position.m_y;
  m_userDataWrapper.AddFavorite(favorData);
}

void UeGui::CDetailMessageHook::SaveToCommonPoint( short pointType )
{

}

void UeGui::CDetailMessageHook::SaveToSysStartPosition()
{

}