#include "createteamhook.h"
#include "InputHook.h"

using namespace UeGui;

CCreateTeamHook::CCreateTeamHook()
{
}

CCreateTeamHook::~CCreateTeamHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCreateTeamHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CCreateTeamHook::GetBinaryFileName()
{
  return _T("createteamhook.bin");
}

void CCreateTeamHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(createteamhook_BlackGround,	"BlackGround"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Previous,	"Previous"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_HeadLabel,	"HeadLabel"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_NameLeft,	"NameLeft"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Name,	"Name"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_NameRight,	"NameRight"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_AddressLeft,	"AddressLeft"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Address,	"Address"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_AddressRight,	"AddressRight"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_TelephoneLeft,	"TelephoneLeft"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Telephone,	"Telephone"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_TelephoneRight,	"TelephoneRight"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_DetermineLeft,	"DetermineLeft"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Determine,	"Determine"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_DetermineRight,	"DetermineRight"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_NameLabel,	"NameLabel"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_AddressLabel,	"AddressLabel"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_TelephoneLabel,	"TelephoneLabel"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_CancelLeft,	"CancelLeft"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_Cancel,	"Cancel"));
  m_ctrlNames.insert(GuiName::value_type(createteamhook_CancelRight,	"CancelRight"));
}

void CCreateTeamHook::MakeControls()
{
  m_addressCtrl.SetCenterElement(GetGuiElement(createteamhook_Address));
  m_addressLabelCtrl.SetCenterElement(GetGuiElement(createteamhook_AddressLabel));
  m_addressCtrl.SetLeftElement(GetGuiElement(createteamhook_AddressLeft));
  m_addressCtrl.SetRightElement(GetGuiElement(createteamhook_AddressRight));
  m_blackGroundCtrl.SetCenterElement(GetGuiElement(createteamhook_BlackGround));
  m_cancelCtrl.SetCenterElement(GetGuiElement(createteamhook_Cancel));
  m_cancelCtrl.SetLeftElement(GetGuiElement(createteamhook_CancelLeft));
  m_cancelCtrl.SetRightElement(GetGuiElement(createteamhook_CancelRight));
  m_determineCtrl.SetCenterElement(GetGuiElement(createteamhook_Determine));
  m_determineCtrl.SetLeftElement(GetGuiElement(createteamhook_DetermineLeft));
  m_determineCtrl.SetRightElement(GetGuiElement(createteamhook_DetermineRight));
  m_headLabelCtrl.SetCenterElement(GetGuiElement(createteamhook_HeadLabel));
  m_mapCtrl.SetCenterElement(GetGuiElement(createteamhook_Map));
  m_nameCtrl.SetCenterElement(GetGuiElement(createteamhook_Name));
  m_nameLabelCtrl.SetCenterElement(GetGuiElement(createteamhook_NameLabel));
  m_nameCtrl.SetLeftElement(GetGuiElement(createteamhook_NameLeft));
  m_nameCtrl.SetRightElement(GetGuiElement(createteamhook_NameRight));
  m_previousCtrl.SetCenterElement(GetGuiElement(createteamhook_Previous));
  m_telephoneCtrl.SetCenterElement(GetGuiElement(createteamhook_Telephone));
  m_telephoneLabelCtrl.SetCenterElement(GetGuiElement(createteamhook_TelephoneLabel));
  m_telephoneCtrl.SetLeftElement(GetGuiElement(createteamhook_TelephoneLeft));
  m_telephoneCtrl.SetRightElement(GetGuiElement(createteamhook_TelephoneRight));
}

short CCreateTeamHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case createteamhook_AddressLabel:
    {
      m_addressLabelCtrl.MouseDown();
    }
    break;
  case createteamhook_Address:
  case createteamhook_AddressLeft:
  case createteamhook_AddressRight:
    {
      m_addressCtrl.MouseDown();
    }
    break;
  case createteamhook_BlackGround:
    {
      m_blackGroundCtrl.MouseDown();
    }
    break;
  case createteamhook_Cancel:
  case createteamhook_CancelLeft:
  case createteamhook_CancelRight:
    {
      m_cancelCtrl.MouseDown();
    }
    break;
  case createteamhook_Determine:
  case createteamhook_DetermineLeft:
  case createteamhook_DetermineRight:
    {
      m_determineCtrl.MouseDown();
    }
    break;
  case createteamhook_HeadLabel:
    {
      m_headLabelCtrl.MouseDown();
    }
    break;
  case createteamhook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case createteamhook_NameLabel:
    {
      m_nameLabelCtrl.MouseDown();
    }
    break;
  case createteamhook_Name:
  case createteamhook_NameLeft:
  case createteamhook_NameRight:
    {
      m_nameCtrl.MouseDown();
    }
    break;
  case createteamhook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case createteamhook_TelephoneLabel:
    {
      m_telephoneLabelCtrl.MouseDown();
    }
    break;
  case createteamhook_Telephone:
  case createteamhook_TelephoneLeft:
  case createteamhook_TelephoneRight:
    {
      m_telephoneCtrl.MouseDown();
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

short CCreateTeamHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCreateTeamHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case createteamhook_AddressLabel:
    {
      m_addressLabelCtrl.MouseUp();
    }
    break;
  case createteamhook_Address:
  case createteamhook_AddressLeft:
  case createteamhook_AddressRight:
    {
      m_addressCtrl.MouseUp();
      DoMouseUp(OT_ADDRESS);
    }
    break;
  case createteamhook_BlackGround:
    {
      m_blackGroundCtrl.MouseUp();
    }
    break;
  case createteamhook_Cancel:
  case createteamhook_CancelLeft:
  case createteamhook_CancelRight:
    {
      m_cancelCtrl.MouseUp();
      /*m_prevHookType = m_curHookType;
      m_curHookType = CViewHook::DHT_FleetManagementHook;*/
      CViewHook::SkipToHook(DHT_FleetManagementHook);
    }
    break;
  case createteamhook_Determine:
  case createteamhook_DetermineLeft:
  case createteamhook_DetermineRight:
    {
      m_determineCtrl.MouseUp();
      string str(m_nameCtrl.GetCaption());
      m_confirmEvent(str);
      /*m_prevHookType = m_curHookType;
      m_curHookType = CViewHook::DHT_FleetManagementHook;*/
      CViewHook::SkipToHook(DHT_FleetManagementHook);
    }
    break;
  case createteamhook_HeadLabel:
    {
      m_headLabelCtrl.MouseUp();
    }
    break;
  case createteamhook_Map:
    {
      m_mapCtrl.MouseUp();
      /*m_prevHookType = m_curHookType;
      m_curHookType = CViewHook::DHT_MapHook;*/
      CViewHook::GoToMapHook();
      ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
    }
    break;
    break;
  case createteamhook_NameLabel:
    {
      m_nameLabelCtrl.MouseUp();
    }
    break;
  case createteamhook_Name:
  case createteamhook_NameLeft:
  case createteamhook_NameRight:
    {
      m_nameCtrl.MouseUp();
      DoMouseUp(OT_NAME);
    }
    break;
  case createteamhook_Previous:
    {
      m_previousCtrl.MouseUp();
      /*m_prevHookType = m_curHookType;
      m_curHookType = CViewHook::DHT_FleetManagementHook;*/
      CViewHook::ReturnPrevHook();
    }
    break;
  case createteamhook_TelephoneLabel:
    {
      m_telephoneLabelCtrl.MouseUp();
    }
    break;
  case createteamhook_Telephone:
  case createteamhook_TelephoneLeft:
  case createteamhook_TelephoneRight:
    {
      m_telephoneCtrl.MouseUp();
      DoMouseUp(OT_TELEPHONE);
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

bool CCreateTeamHook::operator ()()
{
  return false;
}

void CCreateTeamHook::SetConfirmEvent(TeamConfirm tc) 
{
  m_confirmEvent = tc;
}


void CCreateTeamHook::DoMouseUp(HandleType type)
{
  CViewHook::m_prevHookType = CViewHook::m_curHookType;
 
  switch(type) 
  {
    case OT_NAME:
      {
        CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
          CViewHook::m_curHookType,this,EditHookNameCallBack,"编辑信息",m_nameCtrl.GetCenterElement()->m_label);
        break;
      }
    case OT_ADDRESS:
      {
        CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
          CViewHook::m_curHookType,this,EditHookAddressCallBack,"编辑信息",m_addressCtrl.GetCenterElement()->m_label);
        break;
      }
    case OT_TELEPHONE:
      {
        CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
          CViewHook::m_curHookType,this,EditHookTelephoneCallBack,"编辑信息",m_telephoneCtrl.GetCenterElement()->m_label);
        break;
      }
    default:
      break;
  }
}





//编辑名称的回调函数
void CCreateTeamHook::EditHookNameCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CCreateTeamHook *)pDoCallBackObj)->DoEditHookNameCallBack(pResult);
}
void CCreateTeamHook::DoEditHookNameCallBack(const SQLRecord *pResult)
{
  m_nameCtrl.SetCaption(pResult->m_asciiName);
  CViewHook::m_curHookType=CViewHook::DHT_CreateTeamHook;
}

//编辑地址的回调函数
void CCreateTeamHook::EditHookAddressCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CCreateTeamHook *)pDoCallBackObj)->DoEditHookAddressCallBack(pResult);
}
void CCreateTeamHook::DoEditHookAddressCallBack(const SQLRecord *pResult)
{
  m_addressCtrl.SetCaption(pResult->m_asciiName);
  CViewHook::m_curHookType=CViewHook::DHT_CreateTeamHook;
}

//编辑电话的回调函数
void CCreateTeamHook::EditHookTelephoneCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CCreateTeamHook *)pDoCallBackObj)->DoEditHookTelephoneCallBack(pResult);
}

void CCreateTeamHook::DoEditHookTelephoneCallBack(const SQLRecord *pResult)
{
  m_telephoneCtrl.SetCaption(pResult->m_asciiName);
  CViewHook::m_curHookType=CViewHook::DHT_CreateTeamHook;
}

void CCreateTeamHook::ClearInfo() 
{
  m_nameCtrl.ClearCaption();
  m_addressCtrl.ClearCaption();
  m_telephoneCtrl.ClearCaption();
}