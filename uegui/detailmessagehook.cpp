#include "detailmessagehook.h"
#include "uequery\querybasic.h"
#include "usuallyfile.h"
#include "messagedialoghook.h"

using namespace UeGui;

CDetailMessageHook::CDetailMessageHook() : m_userDataWrapper(CUserDataWrapper::Get())
{
  m_strTitle = "详细信息";
  m_vecHookFile.push_back(_T("detailmessagehook.bin"));
  ::memset(&m_detailInfo, 0, sizeof(DetailInfo));
  ::memset(m_QCode, 0, sizeof(m_QCode));
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
  char buf[12] = {};
  ::sprintf(buf, "%c%c%c-%c%c%c-%c%c%c",m_QCode[0], m_QCode[1], m_QCode[2], m_QCode[3], m_QCode[4], m_QCode[5], m_QCode[6], m_QCode[7], m_QCode[8]);
  m_codeInfoLabel.SetCaption(buf);
}

void UeGui::CDetailMessageHook::UnLoad()
{

}

void UeGui::CDetailMessageHook::SetDetailInfoData( const DetailInfo& data )
{
  //  拷贝数据
  ::memcpy(&m_detailInfo, &data, sizeof(DetailInfo));
  //读取Q码
  float median = 100000.0;
  double dX = m_detailInfo.m_position.m_x / median;
  double dY = m_detailInfo.m_position.m_y / median;
  QCode_MapToCode(dX, dY, m_QCode, QCODE_LENGTH);  
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
      SaveToUsuallyRecord(RT_HOME);
    }
    break;
  case DetailMessageHook_Company:
    {
      m_companyBtn.MouseUp();
      SaveToUsuallyRecord(RT_COMPANY);
    }
    break;
  case DetailMessageHook_FavoratePosition1:
    {
      m_favoratePosition1Btn.MouseUp();
      SaveToUsuallyRecord(RT_ONE);
    }
    break;
  case DetailMessageHook_FavoratePosition2:
    {
      m_favoratePosition2Btn.MouseUp();
      SaveToUsuallyRecord(RT_TWO);
    }
    break;
  case DetailMessageHook_FavoratePosition3:
    {
      m_favoratePosition3Btn.MouseUp();
      SaveToUsuallyRecord(RT_THREE);
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
  ::strcpy((char*)favorData.m_name, m_detailInfo.m_name);
  ::strcpy((char*)favorData.m_addr, m_detailInfo.m_address);
  ::strcpy((char*)favorData.m_telphone, m_detailInfo.m_telephone);
  unsigned int rt = m_userDataWrapper.AddFavorite(favorData);
  if (UeQuery::SQL_Success == rt)
  {
    CMessageDialogEvent dialogEvent(this, DHT_DetailMessageHook, NULL);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "已存入地址簿", dialogEvent);
    Sleep(1000);
    CMessageDialogHook::CloseMessageDialog();
  }
}

void UeGui::CDetailMessageHook::SaveToUsuallyRecord( UsuallyRecordType type )
{
  if (m_userDataWrapper.IsUsuallyExist(type))
  {
    m_isNeedRefesh = false;
    CMessageDialogEvent dialogEvent(this, DHT_DetailMessageHook, &CDetailMessageHook::OnSaveUsuallyRecord);
    const char* msgInfo = GetUsuallyRecordMsgInfo(type);
    m_saveUsuallyRecordType = type;
    CMessageDialogHook::ShowMessageDialog(MB_WARNING, msgInfo, dialogEvent);
  }
  else
  {
    int rt = SaveUsuallyRecord(type);
    if (0 == rt)
    {
      CMessageDialogEvent dialogEvent(this, DHT_DetailMessageHook, NULL);
      CMessageDialogHook::ShowMessageDialog(MB_NONE, "已保存为常用点", dialogEvent);
      Sleep(1000);
      CMessageDialogHook::CloseMessageDialog();
    }
  }
}

void UeGui::CDetailMessageHook::OnSaveUsuallyRecord( CAggHook* sender, ModalResultType modalResult )
{
  if ((sender) && (MR_OK == modalResult))
  {
    CDetailMessageHook* detailHook = dynamic_cast<CDetailMessageHook*>(sender);
    if (detailHook)
    {
      detailHook->SaveUsuallyRecord(detailHook->GetUsuallyRecordType());
    }
  }
}

int UeGui::CDetailMessageHook::SaveUsuallyRecord( UsuallyRecordType type )
{
  UsuallyRecord data;
  data.m_x = m_detailInfo.m_position.m_x;
  data.m_y = m_detailInfo.m_position.m_y;
  ::strcpy((char*)data.m_name, m_detailInfo.m_name);
  return m_userDataWrapper.UpdateUsuallyRecord(type, &data);
}

const char* UeGui::CDetailMessageHook::GetUsuallyRecordMsgInfo( UsuallyRecordType type )
{
  switch (type)
  {
  case RT_HOME:
    {
      return "是否替换已保存的家？";
    }
  case RT_COMPANY:
    {
      return "是否替换已保存的单位？";
    }
  case RT_ONE:
    {
      return "是否替换已保存的常用点1？";
    }
  case RT_TWO:
    {
      return "是否替换已保存的常用点2？";
    }
  case RT_THREE:
    {
      return "是否替换已保存的常用点3？";
    }
  default:
    {
      return "是否替换已保存的数据？";
    }
  }
}

UeGui::UsuallyRecordType UeGui::CDetailMessageHook::GetUsuallyRecordType()
{
  return m_saveUsuallyRecordType;
}

void UeGui::CDetailMessageHook::SaveToSysStartPosition()
{

}