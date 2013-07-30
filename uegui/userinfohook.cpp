#include "userinfohook.h"

using namespace UeGui;

CUserInfoHook::CUserInfoHook()
{
}

CUserInfoHook::~CUserInfoHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CUserInfoHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CUserInfoHook::GetBinaryFileName()
{
  return _T("userinfohook.bin");
}

void CUserInfoHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_Map,	"Map"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_Previous,	"Previous"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_Capacity,	"Capacity"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_CapacityText,	"CapacityText"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_Favorite,	"Favorite"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_FavoriteText,	"FavoriteText"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_History,	"History"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_HistoryText,	"HistoryText"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_Journey,	"Journey"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_JourneyText,	"JourneyText"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_Special,	"Special"));
  m_ctrlNames.insert(GuiName::value_type(UserInfoHook_SpecialText,	"SpecialText"));
}

void CUserInfoHook::MakeControls()
{
  m_mapCtrl.SetCenterElement(GetGuiElement(UserInfoHook_Map));
  m_previousCtrl.SetCenterElement(GetGuiElement(UserInfoHook_Previous));
  m_capacityCtrl.SetCenterElement(GetGuiElement(UserInfoHook_Capacity));
  m_capacityTextCtrl.SetCenterElement(GetGuiElement(UserInfoHook_CapacityText));
  m_favoriteCtrl.SetCenterElement(GetGuiElement(UserInfoHook_Favorite));
  m_favoriteTextCtrl.SetCenterElement(GetGuiElement(UserInfoHook_FavoriteText));
  m_historyCtrl.SetCenterElement(GetGuiElement(UserInfoHook_History));
  m_historyTextCtrl.SetCenterElement(GetGuiElement(UserInfoHook_HistoryText));
  m_journeyCtrl.SetCenterElement(GetGuiElement(UserInfoHook_Journey));
  m_journeyTextCtrl.SetCenterElement(GetGuiElement(UserInfoHook_JourneyText));
  m_specialCtrl.SetCenterElement(GetGuiElement(UserInfoHook_Special));
  m_specialTextCtrl.SetCenterElement(GetGuiElement(UserInfoHook_SpecialText));
}

short CUserInfoHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case UserInfoHook_Map:
    {
      m_mapCtrl.MouseDown();
    }
    break;
  case UserInfoHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case UserInfoHook_Capacity:
    {
      m_capacityCtrl.MouseDown();
    }
    break;
  case UserInfoHook_CapacityText:
    {
      m_capacityTextCtrl.MouseDown();
    }
    break;
  case UserInfoHook_Favorite:
    {
      m_favoriteCtrl.MouseDown();
    }
    break;
  case UserInfoHook_FavoriteText:
    {
      m_favoriteTextCtrl.MouseDown();
    }
    break;
  case UserInfoHook_History:
    {
      m_historyCtrl.MouseDown();
    }
    break;
  case UserInfoHook_HistoryText:
    {
      m_historyTextCtrl.MouseDown();
    }
    break;
  case UserInfoHook_Journey:
    {
      m_journeyCtrl.MouseDown();
    }
    break;
  case UserInfoHook_JourneyText:
    {
      m_journeyTextCtrl.MouseDown();
    }
    break;
  case UserInfoHook_Special:
    {
      m_specialCtrl.MouseDown();
    }
    break;
  case UserInfoHook_SpecialText:
    {
      m_specialTextCtrl.MouseDown();
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
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CUserInfoHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CUserInfoHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case UserInfoHook_Map:
    {
      m_mapCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
    }
    break;
  case UserInfoHook_Previous:
    {
      m_previousCtrl.MouseUp();

      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MainMenuHook;*/
        CAggHook::Return();
      }
    }
    break;
  case UserInfoHook_Capacity:
    {
      m_capacityCtrl.MouseUp();
    }
    break;
  case UserInfoHook_CapacityText:
    {
      m_capacityTextCtrl.MouseUp();
    }
    break;
  case UserInfoHook_Favorite:
    {
      m_favoriteCtrl.MouseUp();
    }
    break;
  case UserInfoHook_FavoriteText:
    {
      m_favoriteTextCtrl.MouseUp();
    }
    break;
  case UserInfoHook_History:
    {
      m_historyCtrl.MouseUp();
    }
    break;
  case UserInfoHook_HistoryText:
    {
      m_historyTextCtrl.MouseUp();
    }
    break;
  case UserInfoHook_Journey:
    {
      m_journeyCtrl.MouseUp();
    }
    break;
  case UserInfoHook_JourneyText:
    {
      m_journeyTextCtrl.MouseUp();
    }
    break;
  case UserInfoHook_Special:
    {
      m_specialCtrl.MouseUp();
    }
    break;
  case UserInfoHook_SpecialText:
    {
      m_specialTextCtrl.MouseUp();
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

bool CUserInfoHook::operator ()()
{
  return false;
}

