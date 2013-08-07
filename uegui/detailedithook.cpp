#include "detailedithook.h"
//#include "InputHook.h"
#include "myaddressbookhook.h"
//#include "InputSwitchHook.h"
#include "myinformationhook.h"
#include "settingwrapper.h"
#include "messagedialoghook.h"

using namespace UeGui;

CDetailEditHook::CDetailEditHook():m_userWrapper(CUserDataWrapper::Get())
{
  m_strTitle = "详细";
  m_vecHookFile.push_back(_T("detailedithook.bin"));
}

CDetailEditHook::~CDetailEditHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

//void CDetailEditHook::MakeGUI()
//{
//  ClearElements();
//  FetchWithBinary();
//  MakeNames();
//  MakeControls();
//}

//tstring CDetailEditHook::GetBinaryFileName()
//{
//  return _T("detailedithook.bin");
//}

void CDetailEditHook::Load()
{

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
    }
    break;
  case DetailEditHook_Address:
    {
      m_addressCtrl.MouseDown();
    }
    break;
  case DetailEditHook_Telephone:
    {
      m_telephoneCtrl.MouseDown();
    }
    break;
  case DetailEditHook_Mark:
  case DetailEditHook_MarkPic:
  case DetailEditHook_MarkLabel:
    {
      m_markCtrl.MouseDown();
      AddRenderUiControls(&m_markCtrl);
      AddRenderUiControls(&m_markPicCtrl);
    }
    break;
  case DetailEditHook_Determine:
    {
      m_determineCtrl.MouseDown();
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
        DoMouseUp(OT_NAME);
      }
    }
    break;
  case DetailEditHook_Address:
    {
      m_addressCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(OT_ADDRESS);
      }
    }
    break;
  case DetailEditHook_Telephone:
    {
      m_telephoneCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(OT_TELEPHONE);
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

//bool CDetailEditHook::operator ()()
//{
//  return false;
//}

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
            const FavoriteEntry& curEntry1 = curEntry;
           /* UeQuery::SQLSetting setting;
            m_query->ConnectTo(UeQuery::DT_Favorite, setting);
            m_query->RemoveFavorite(recpos);*/
            //如果新添加的地址薄中信息设置了开机位置，则去掉旧的开机位置信息设置
            m_userWrapper.RemoveFavorite(recpos);
            int icount = m_userWrapper.GetFavoriteCount();
            if(curEntry1.m_kind & 0x1)
            {
              const FavoriteEntry *curFavor;
              FavoriteEntry curFavorite;
              for(int order = 0;order < icount;order++)
              {
                curFavor = m_userWrapper.GetFavorite(order);
                if(curFavor->m_kind & 0x1)
                {
                  curFavorite = *curFavor;
                  curFavorite.m_kind--;
                  m_userWrapper.UpdateFavorite(curFavorite, order);
                  break;
                }
              }
            }
            //
            m_userWrapper.AddFavorite(curEntry1);
            CMyAddressBookHook::MyAddressBookCallBack();
          }
          CAggHook::Return();
          Refresh();
        }
        else if (CViewHook::DHT_DetailMessageHook == m_detailEditEvent.m_senderHookType)
        {
          FavoriteEntry curEntry;
          if (CUserDataWrapper::Get().EditData2FavoriteEntry(&eData, &curEntry))
          {
            if (SaveFavoriteEntryData(curEntry))
            {
              CViewHook::m_curHookType = CViewHook::DHT_MapHook;
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
          m_userWrapper.UpdateHistoryRecord(curEntry, recpos);
          m_userWrapper.DisconnectHistoryRecord();
          CAggHook::Return();
        }        
      }

      break;
    }
  case OT_PREVIOUS:
    {
      if (CViewHook::DHT_Unknown != m_detailEditEvent.m_senderHookType)
      {
        CViewHook::m_curHookType = m_detailEditEvent.m_senderHookType;
        if (m_detailEditEvent.m_senderHookType == CViewHook::DHT_DetailMessageHook)
        {
          CViewHook::m_curHookType = CViewHook::DHT_MapHook;
        }
        Refresh();
      }
      break;
    }
  case OT_GOTOMAP:
    {
      /*CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
      CAggHook::GoToMapHook();
      Refresh();
      break;
    }
  case OT_NAME:
    {
      //CInputSwitchHook *inputHook = dynamic_cast<CInputSwitchHook*>(m_view->GetHook(CViewHook::DHT_InputSwitchHook));
      //inputHook->SetInputMethod(CInputSwitchHook::IM_SpellingMethod);
      //CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
      //  CViewHook::DHT_DetailEditHook,this,EditHookNameCallBack,"编辑名称",m_nameCtrl.GetCenterElement()->m_label);
      break;
    }
  case OT_ADDRESS:
    {
      //CInputSwitchHook *inputHook = dynamic_cast<CInputSwitchHook*>(m_view->GetHook(CViewHook::DHT_InputSwitchHook));
      //inputHook->SetInputMethod(CInputSwitchHook::IM_SpellingMethod);
      //CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
      //  CViewHook::DHT_DetailEditHook,this,EditHookAddressCallBack,"编辑地址",m_addressCtrl.GetCenterElement()->m_label);
      break;
    }
  case OT_TELEPHONE:
    {
      //CInputSwitchHook *inputHook = dynamic_cast<CInputSwitchHook*>(m_view->GetHook(CViewHook::DHT_InputSwitchHook));
      //inputHook->SetInputMethod(CInputSwitchHook::IM_CharMethod);
      //CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
      //  CViewHook::DHT_DetailEditHook,this,EditHookTelephoneCallBack,"编辑电话",m_telephoneCtrl.GetCenterElement()->m_label);
      break;
    }
  case OT_MARKER:
    {
      CViewHook::m_curHookType = CViewHook::DHT_MarkerEditHook;
      //CAggHook::SkipToHook(CViewHook::DHT_MarkerEditHook);
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
  CViewHook::m_curHookType=CViewHook::DHT_DetailEditHook;
  //CAggHook::SkipToHook(CViewHook::DHT_DetailEditHook);
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
  //CViewHook::m_curHookType=CViewHook::DHT_DetailEditHook;
  //CAggHook::SkipToHook(CViewHook::DHT_DetailEditHook);
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

  /*m_mapMarkCtrl.SetCheck(caption->m_isMapshow);
  m_voiceBroadCastCtrl.SetCheck(caption->m_isVoice);
  m_bootPositionCtrl.SetCheck(caption->m_isStartpos);*/
  return true;
}
//void UeGui::CDetailEditHook::Init()
//{
//  return;
//}

//编辑界面数据类型赋值
void UeGui::CDetailEditHook::SetEditData()
{
  ::strcpy((char *)eData.m_name, m_nameCtrl.GetCaption());
  ::strcpy((char *)eData.m_addrCode, m_addressCtrl.GetCaption());
  ::strcpy((char *)eData.m_telephone, m_telephoneCtrl.GetCaption());
  eData.m_kind = m_markPicCtrl.GetCenterElement()->m_backStyle;
  /*eData.m_isMapshow = m_mapMarkCtrl.Checked();
  eData.m_isVoice = m_voiceBroadCastCtrl.Checked();
  eData.m_isStartpos = m_bootPositionCtrl.Checked();*/
}

//编辑名称的回调函数
void CDetailEditHook::EditHookNameCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CDetailEditHook *)pDoCallBackObj)->DoEditHookNameCallBack(pResult);
}
void CDetailEditHook::DoEditHookNameCallBack(const SQLRecord *pResult)
{
  m_nameCtrl.SetCaption(pResult->m_asciiName);
  CViewHook::m_curHookType=CViewHook::DHT_DetailEditHook;
  //CAggHook::SkipToHook(CViewHook::DHT_DetailEditHook);
}

//编辑地址的回调函数
void CDetailEditHook::EditHookAddressCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CDetailEditHook *)pDoCallBackObj)->DoEditHookAddressCallBack(pResult);
}
void CDetailEditHook::DoEditHookAddressCallBack(const SQLRecord *pResult)
{
  m_addressCtrl.SetCaption(pResult->m_asciiName);
  CViewHook::m_curHookType=CViewHook::DHT_DetailEditHook;
  //CAggHook::SkipToHook(CViewHook::DHT_DetailEditHook);
}

//编辑电话的回调函数
void CDetailEditHook::EditHookTelephoneCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  ((CDetailEditHook *)pDoCallBackObj)->DoEditHookTelephoneCallBack(pResult);
}
void CDetailEditHook::DoEditHookTelephoneCallBack(const SQLRecord *pResult)
{
  m_telephoneCtrl.SetCaption(pResult->m_asciiName);
  CViewHook::m_curHookType=CViewHook::DHT_DetailEditHook;
  //CAggHook::SkipToHook(CViewHook::DHT_DetailEditHook);
}

bool UeGui::CDetailEditHook::EditData2FavoriteEntry(EditData* edata, FavoriteEntry* fEntry)
{
  if (edata == NULL || fEntry == NULL)
  {
    return false;
  }
  fEntry->m_x = edata->m_x;
  fEntry->m_y = edata->m_y;
  ::strcpy((char *)fEntry->m_name, (char *)edata->m_name);
  fEntry->m_teleNumber = ::atoi((char *)edata->m_telephone);
  //fEntry->m_addrCode=::atoi((char *)edata->m_addrCode);
  ::strcpy((char*)fEntry->m_addr, (char *)edata->m_addrCode);
  ::strcpy((char*)fEntry->m_telphone, (char *)edata->m_telephone);

  //三个标志位的增加
  edata->m_kind = edata->m_kind<<3;
  if (edata->m_isMapshow)
  {
    edata->m_kind |= 0x4;
  }
  if(edata->m_isVoice)
  {
    edata->m_kind |= 0x2;
  }
  if(edata->m_isStartpos)
  {
    edata->m_kind |= 0x1;
  }
  fEntry->m_kind = edata->m_kind;
  return true;
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
  recpos = n;
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

    //暂时不主动移除记录
    //if (favoriteDataCount >= maxDataCount)
    //{
    //  //移除最后一条记录，并添加最新记录
    //  m_query->RemoveFavorite(favoriteDataCount - 1);
    //}

    m_query->AddFavorite(data);
    m_query->Disconnect(UeQuery::DT_Favorite);
    return true;
  }
}

