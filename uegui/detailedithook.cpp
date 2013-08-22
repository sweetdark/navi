#include "detailedithook.h"
//#include "InputHook.h"
#include "myaddressbookhook.h"
//#include "InputSwitchHook.h"
#include "myinformationhook.h"
#include "settingwrapper.h"
#include "messagedialoghook.h"
#include "editswitchhook.h"

using namespace UeGui;

CDetailEditHook::CDetailEditHook():m_userWrapper(CUserDataWrapper::Get())
{
  m_strTitle = "详细信息";
  m_vecHookFile.push_back(_T("detailedithook.bin"));
}
int CDetailEditHook::m_recpos = 0;

CDetailEditHook::~CDetailEditHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CDetailEditHook::Load()
{
  if (m_detailEditEvent.m_senderHookType == CViewHook::DHT_HistoryInformationHook)
  {
    m_markCtrl.SetVisible(false);
    m_markPicCtrl.SetVisible(false);
    m_markLabel.SetVisible(false);
  }
};
void CDetailEditHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_Name,	"Name"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_Address,	"Address"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_Telephone,	"Telephone"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_Mark,	"Mark"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_Determine,	"Determine"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_NameLabel,	"NameLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_AddressLabel,	"AddressLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_TelephoneLabel,	"TelephoneLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_MarkLabel,	"MarkLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailEditHook_MarkPic,"MarkPic"));
}

void CDetailEditHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_nameCtrl.SetCenterElement(GetGuiElement(DetailEditHook_Name));
  m_addressCtrl.SetCenterElement(GetGuiElement(DetailEditHook_Address));
  m_telephoneCtrl.SetCenterElement(GetGuiElement(DetailEditHook_Telephone));
  m_markPicCtrl.SetCenterElement(GetGuiElement(DetailEditHook_MarkPic));
  m_markCtrl.SetCenterElement(GetGuiElement(DetailEditHook_Mark));
  m_markLabel.SetCenterElement(GetGuiElement(DetailEditHook_MarkLabel));
  m_determineCtrl.SetCenterElement(GetGuiElement(DetailEditHook_Determine));
}

short CDetailEditHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DetailEditHook_Name:
    {
      m_nameCtrl.MouseDown();
      AddRenderUiControls(&m_nameCtrl);
    }
    break;
  case DetailEditHook_Address:
    {
      m_addressCtrl.MouseDown();
      AddRenderUiControls(&m_addressCtrl);
    }
    break;
  case DetailEditHook_Telephone:
    {
      m_telephoneCtrl.MouseDown();
      AddRenderUiControls(&m_telephoneCtrl);
    }
    break;
  case DetailEditHook_Mark:
  case DetailEditHook_MarkPic:
    {
      m_markCtrl.MouseDown();
      AddRenderUiControls(&m_markCtrl);
      AddRenderUiControls(&m_markPicCtrl);
    }
    break;
  case DetailEditHook_Determine:
    {
      m_determineCtrl.MouseDown();
      AddRenderUiControls(&m_determineCtrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDetailEditHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDetailEditHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case DetailEditHook_Name:
    {
      m_nameCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        CEditSwitchHook* editSwitch = ((CEditSwitchHook*)m_view->GetHook(CAggHook::DHT_EditSwitchHook));
        editSwitch->SetEditCallBackFun(this, "编辑名称", m_nameCtrl.GetCaption(), EditHookNameCallBack);
        TurnTo(editSwitch->GetCurEditHookType());
      }
    }
    break;
  case DetailEditHook_Address:
    {
      m_addressCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        CEditSwitchHook* editSwitch = ((CEditSwitchHook*)m_view->GetHook(CAggHook::DHT_EditSwitchHook));
        editSwitch->SetEditCallBackFun(this, "编辑地址", m_addressCtrl.GetCaption(), EditHookAddressCallBack);
        TurnTo(editSwitch->GetCurEditHookType());
      }
    }
    break;
  case DetailEditHook_Telephone:
    {
      m_telephoneCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        CEditSwitchHook* editSwitch = ((CEditSwitchHook*)m_view->GetHook(CAggHook::DHT_EditSwitchHook));
        editSwitch->SetEditCallBackFun(this, "编辑电话", m_telephoneCtrl.GetCaption(), EditHookTelephoneCallBack);
        TurnTo(editSwitch->GetCurEditHookType());
      }
    }
    break;
  case DetailEditHook_Mark:
  case DetailEditHook_MarkPic:
    {
      m_markCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(OT_MARKER);
      }
    }
    break;
  case DetailEditHook_Determine:
    {
      m_determineCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(OT_CONFIRM);
      }
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void UeGui::CDetailEditHook::DoMouseUp(HandleType resultType)
{
  switch(resultType)
  {
  case OT_CONFIRM:
    {
      if (CViewHook::DHT_Unknown != m_detailEditEvent.m_senderHookType)
      {        
        SetEditData();
        if (CViewHook::DHT_MyAddressBookHook == m_detailEditEvent.m_senderHookType)
        {
          FavoriteEntry curEntry;
          if (CUserDataWrapper::Get().EditData2FavoriteEntry(&eData, &curEntry))
          {
            FavoriteEntry& curEntry1 = curEntry;
            m_userWrapper.RemoveFavorite(m_recpos);
            //
            m_userWrapper.AddFavorite(curEntry1);
            CMyAddressBookHook::MyAddressBookCallBack();
          }
          Return();
          Refresh();
        }
        else if (CViewHook::DHT_DetailMessageHook == m_detailEditEvent.m_senderHookType)
        {
          FavoriteEntry curEntry;
          if (CUserDataWrapper::Get().EditData2FavoriteEntry(&eData, &curEntry))
          {
            if (SaveFavoriteEntryData(curEntry))
            {
              //CViewHook::m_curHookType = CViewHook::DHT_MapHook;
              Return(false);
              Refresh();
            }                        
          }     
        }
        else if (CViewHook::DHT_HistoryInformationHook == m_detailEditEvent.m_senderHookType)
        {
          HistoryRecordEntry curEntry;
          curEntry.m_X = eData.m_x;
          curEntry.m_Y = eData.m_y;
          ::memcpy(&curEntry.m_addrName, &eData.m_name,128);
          m_userWrapper.ConnectToHistoryRecord();
          m_userWrapper.RemoveHistoryRocord(m_recpos);
          m_userWrapper.AddHistoryRecord(curEntry);
          m_userWrapper.DisconnectHistoryRecord();
          Return();
        }        
      }

      break;
    }
  case OT_MARKER:
    {
      TurnTo(DHT_MarkerEditHook);
      Refresh();
      break;
    }
  default:
    break;
  }
}

void UeGui::CDetailEditHook::SetMark(short& n)
{
  IView *view = IView::GetView();
  CDetailEditHook* edithook=(CDetailEditHook *)(view->GetHook(CViewHook::DHT_DetailEditHook));
  edithook->DoSetMark(n);
}
void UeGui::CDetailEditHook::DoSetMark(short& n)
{
  m_markPicCtrl.GetCenterElement()->m_backStyle=n;
  m_markPicCtrl.SetVisible(true);
}

bool UeGui::CDetailEditHook::ShowDetailEditHook(const EditData* caption, CDetailEditEvent& editEvent)
{
  bool retval;
  IView *view = IView::GetView();
  CDetailEditHook* edithook=(CDetailEditHook *)(view->GetHook(CViewHook::DHT_DetailEditHook));

  if(edithook->DoShowDetailEditHook(caption,editEvent))
  {
    retval=true;
  }
  else
  {
    retval=false;
  }
  return retval;
}

bool UeGui::CDetailEditHook::DoShowDetailEditHook(const EditData* caption, CDetailEditEvent& editEvent)
{
  m_detailEditEvent=editEvent;
  eData=*caption;
  m_nameCtrl.SetCaption((char *)caption->m_name);
  m_addressCtrl.SetCaption((char *)caption->m_addrCode);
  m_telephoneCtrl.SetCaption((char *)caption->m_telephone);

  m_markPicCtrl.SetVisible(true);
  m_markLabel.SetVisible(true);
  m_markCtrl.SetVisible(true);
  //在详情-->编辑界面，标记处设置默认的标记图标，而非空显示
  int markPicBegin = MARKPICBEGIN;
  int markPicEnd = MARKPICEND;
  markPicBegin = markPicBegin << 3;
  markPicEnd = markPicEnd << 3;
  if(caption->m_kind < markPicBegin || caption->m_kind > markPicEnd)
  {
    m_markPicCtrl.GetCenterElement()->m_backStyle = MARKPICBEGIN;
  }
  else
  {
    m_markPicCtrl.GetCenterElement()->m_backStyle = caption->m_kind >> 3;
  }
  return true;
}

//编辑界面数据类型赋值
void UeGui::CDetailEditHook::SetEditData()
{
  ::strcpy((char *)eData.m_name, m_nameCtrl.GetCaption());
  ::strcpy((char *)eData.m_addrCode, m_addressCtrl.GetCaption());
  ::strcpy((char *)eData.m_telephone, m_telephoneCtrl.GetCaption());
  eData.m_kind = m_markPicCtrl.GetCenterElement()->m_backStyle;
}

//编辑名称的回调函数
void CDetailEditHook::EditHookNameCallBack(void *pDoCallBackObj,const char *pResult)
{
  ((CDetailEditHook *)pDoCallBackObj)->DoEditHookNameCallBack(pResult);
}
void CDetailEditHook::DoEditHookNameCallBack(const char *pResult)
{
  m_nameCtrl.SetCaption(pResult);
  Return();
}

//编辑地址的回调函数
void CDetailEditHook::EditHookAddressCallBack(void *pDoCallBackObj,const char *pResult)
{
  ((CDetailEditHook *)pDoCallBackObj)->DoEditHookAddressCallBack(pResult);
}
void CDetailEditHook::DoEditHookAddressCallBack(const char *pResult)
{
  m_addressCtrl.SetCaption(pResult);
  Return();
}

//编辑电话的回调函数
void CDetailEditHook::EditHookTelephoneCallBack(void *pDoCallBackObj,const char *pResult)
{
  ((CDetailEditHook *)pDoCallBackObj)->DoEditHookTelephoneCallBack(pResult);
}
void CDetailEditHook::DoEditHookTelephoneCallBack(const char *pResult)
{
  m_telephoneCtrl.SetCaption(pResult);
  Return();
}

//记录被编辑的地址簿数据位置的回调函数
void UeGui::CDetailEditHook::RecordPosition(int n)
{
  IView *view = IView::GetView();
  CDetailEditHook* edithook=(CDetailEditHook *)(view->GetHook(CViewHook::DHT_DetailEditHook));
  edithook->DoRecordPosition(n);
}

void UeGui::CDetailEditHook::DoRecordPosition(int n)
{
  m_recpos = n;
}

bool UeGui::CDetailEditHook::SaveFavoriteEntryData( const FavoriteEntry& data )
{
  int maxDataCount = 0;
  CSettingWrapper &settingWrapper = CSettingWrapper::Get();
  maxDataCount = settingWrapper.GetAddressBookCapacity();
  maxDataCount = 10;
  UeQuery::SQLSetting setting;
  m_query->ConnectTo(UeQuery::DT_Favorite, setting);
  int favoriteDataCount = m_query->GetFavoriteCount();

  if (favoriteDataCount >= maxDataCount)
  {
    m_query->Disconnect(UeQuery::DT_Favorite);
    //弹出消息对话框
    CMessageDialogEvent dialogEvent;
    dialogEvent.m_senderHookType = CViewHook::m_curHookType;
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "地址簿记录已满！", dialogEvent);
    Sleep(2000);
    CMessageDialogHook::CloseMessageDialog();
    return false;
  }
  else
  {
    //如果新添加的地址薄中信息设置了开机位置，则去掉旧的开机位置信息设置
    if (data.m_kind & 0x1)
    {
      const FavoriteEntry *curFavor;
      FavoriteEntry curFavorite;
      for(int order = 0; order < favoriteDataCount; order++)
      {
        curFavor = m_query->GetFavorite(order);
        if(curFavor->m_kind & 0x1)
        {
          curFavorite = *curFavor;
          curFavorite.m_kind--;
          m_query->UpdateFavorite(curFavorite, order);
          break;
        }
      }
    }
    m_query->AddFavorite(data);
    m_query->Disconnect(UeQuery::DT_Favorite);
    return true;
  }
}

