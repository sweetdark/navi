#include "messagedialoghook.h"

using namespace UeGui;

CMessageDialogHook::CMessageDialogHook() : m_interval(0), m_lineTextCount(0)
{
  m_setting.MakeProperties();
}

CMessageDialogHook::~CMessageDialogHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMessageDialogHook::MakeGUI()
{
  ClearElements();
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CMessageDialogHook::GetBinaryFileName()
{
  return _T("messagedialoghook.bin");
}

void CMessageDialogHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_DialogeBackground,	"DialogeBackground"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_DialogeIcon,	"DialogeIcon"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_DialogeText,	"DialogeText"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_DialogeText2,	"DialogeText2"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmButtonLeft,	"AffirmButtonLeft"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmButtonCenter,	"AffirmButtonCenter"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmLabel,	"AffirmLabel"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_Delimiter,	"Delimiter"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_CancelButtonCenter,	"CancelButtonCenter"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_CancelLabel,	"CancelLabel"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_CancelButtonRight,	"CancelButtonRight"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmButton2Left,	"AffirmButton2Left"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmButton2Center,	"AffirmButton2Center"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmButton2Right,	"AffirmButton2Right"));
  m_ctrlNames.insert(GuiName::value_type(MessageDialogHook_AffirmLabel2,	"AffirmLabel2"));
}

void CMessageDialogHook::MakeControls()
{
  m_dialogeIcon.SetCenterElement(GetGuiElement(MessageDialogHook_DialogeIcon));
  m_affirmButton.SetLeftElement(GetGuiElement(MessageDialogHook_AffirmButtonLeft));
  m_affirmButton.SetCenterElement(GetGuiElement(MessageDialogHook_AffirmButtonCenter));  
  m_affirmButton.SetLabelElement(GetGuiElement(MessageDialogHook_AffirmLabel));  
  m_cancelButton.SetCenterElement(GetGuiElement(MessageDialogHook_CancelButtonCenter));
  m_cancelButton.SetRightElement(GetGuiElement(MessageDialogHook_CancelButtonRight));
  m_cancelButton.SetLabelElement(GetGuiElement(MessageDialogHook_CancelLabel));  
  m_delimiter.SetCenterElement(GetGuiElement(MessageDialogHook_Delimiter));
  m_affirmButton2.SetLeftElement(GetGuiElement(MessageDialogHook_AffirmButton2Left));
  m_affirmButton2.SetCenterElement(GetGuiElement(MessageDialogHook_AffirmButton2Center));    
  m_affirmButton2.SetRightElement(GetGuiElement(MessageDialogHook_AffirmButton2Right));
  m_affirmButton2.SetLabelElement(GetGuiElement(MessageDialogHook_AffirmLabel));

  GuiElement* element = GetGuiElement(MessageDialogHook_DialogeText);
  if (element)
  {    
    const MapsText &textProp = m_setting.GetTextProp(element->m_textStyle);
    if (textProp.m_width > 0)
    {
      m_lineTextCount = element->m_width / textProp.m_width;
    }
    else
    {
      m_lineTextCount = 0;
    }    
  }
  m_dialogText1.SetLabelElement(element);
  m_dialogText1.ClearCaption();
  m_dialogText2.SetLabelElement(GetGuiElement(MessageDialogHook_DialogeText2));
  m_dialogText2.ClearCaption();
  m_cancelButton.SetVisible(false);
  m_affirmButton.SetVisible(false);
  m_delimiter.SetVisible(false);
  m_affirmButton2.SetVisible(false);
}

short CMessageDialogHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);

  bool needRefresh = false;
  switch(ctrlType)
  {
  case MessageDialogHook_AffirmButtonLeft:
  case MessageDialogHook_AffirmButtonCenter:
  case MessageDialogHook_AffirmLabel:
    {
      needRefresh = true;
      m_affirmButton.MouseDown();
      AddRenderUiControls(&m_affirmButton);
      
    }
    break;
  case MessageDialogHook_CancelButtonCenter:
  case MessageDialogHook_CancelButtonRight:
  case MessageDialogHook_CancelLabel:
    {
      needRefresh = true;
      m_cancelButton.MouseDown();
      AddRenderUiControls(&m_cancelButton);
    }
    break;
  case MessageDialogHook_AffirmButton2Left:
  case MessageDialogHook_AffirmButton2Center:
  case MessageDialogHook_AffirmButton2Right:
  case MessageDialogHook_AffirmLabel2:
    {
      needRefresh = true;
      m_affirmButton2.MouseDown();
      AddRenderUiControls(&m_affirmButton2);
    }
    break;
  }
  if (needRefresh)
  {
    this->Refresh();
  }  
  return ctrlType;
}

short CMessageDialogHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMessageDialogHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);

  bool needRefresh = false;
  switch(m_downElementType)
  {
  case MessageDialogHook_AffirmButtonLeft:
  case MessageDialogHook_AffirmButtonCenter:
  case MessageDialogHook_AffirmLabel:
    {
      needRefresh = true;
      m_affirmButton.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(MR_OK);
      }      
    }
    break;
  case MessageDialogHook_CancelButtonCenter:
  case MessageDialogHook_CancelButtonRight:
  case MessageDialogHook_CancelLabel:
    {
      needRefresh = true;
      m_cancelButton.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(MR_CANCEL);
      }      
    }
    break;
  case MessageDialogHook_AffirmButton2Left:
  case MessageDialogHook_AffirmButton2Center:
  case MessageDialogHook_AffirmButton2Right:
  case MessageDialogHook_AffirmLabel2:
    {
      needRefresh = true;
      m_affirmButton2.MouseUp();
      if (ctrlType == m_downElementType)
      {
        DoMouseUp(MR_OK);
      }      
    }
    break;
  }
  if (needRefresh)
  {
    Refresh();
  }  
  return ctrlType;
}

bool CMessageDialogHook::operator ()()
{
  return false;
}

void UeGui::CMessageDialogHook::SetMessageDialogInfo( MessageBoxType type, const char* caption,const CMessageDialogEvent& dialogEvent, unsigned short interval )
{
  m_interval = interval;  
  DoShowMessageText(caption);
  m_messageDialogEvent = dialogEvent;
  //根据消息类型显示窗口
  CViewHook::GuiElement* guiElement = GetGuiElement(MessageDialogHook_DialogeIcon);
  if (guiElement)
  {
    switch (type)
    {
    case  MB_NONE :
      {
        guiElement->m_backStyle = guiElement->m_bkNormal;
        m_affirmButton.SetVisible(false);
        m_cancelButton.SetVisible(false);
        m_delimiter.SetVisible(false);
        m_affirmButton2.SetVisible(false);
        break;
      }
    case MB_INFORMATION :
      {
        guiElement->m_backStyle = guiElement->m_bkNormal;
        m_affirmButton2.SetVisible(false);
        m_affirmButton.SetVisible(true);
        m_cancelButton.SetVisible(true);
        m_delimiter.SetVisible(true);
        break;
      }
    case  MB_WARNING :
      {
        guiElement->m_backStyle = guiElement->m_bkFocus;
        m_affirmButton2.SetVisible(false);
        m_affirmButton.SetVisible(true);
        m_cancelButton.SetVisible(true);
        m_delimiter.SetVisible(true);
        break;
      }
    case  MB_CONFIRM :
      {
        guiElement->m_backStyle = guiElement->m_bkNormal;
        m_affirmButton.SetVisible(false);
        m_cancelButton.SetVisible(false);
        m_delimiter.SetVisible(false);
        m_affirmButton2.SetVisible(true);
        break;
      }
    }
  }
}

void UeGui::CMessageDialogHook::ShowMessageDialog( MessageBoxType type, const char* caption, const CMessageDialogEvent& dialogEvent, unsigned short interval)
{
  IView *view = IView::GetView();
  CMessageDialogHook* dialogHook = dynamic_cast<CMessageDialogHook*>(view->GetHook(CViewHook::DHT_MessageDialogeHook));  
  dialogHook->SetMessageDialogInfo(type, caption, dialogEvent, interval);
  dialogHook->Show();
  CViewHook::m_curHookType = CViewHook::DHT_MessageDialogeHook;  
  //QJW
  view->RefreshUI();
}

//不刷新界面，dialog等到刷新才消失
void UeGui::CMessageDialogHook::CloseMessageDialog( unsigned short senderHookType /*= CViewHook::DHT_Unknown*/ )
{
  IView *view = IView::GetView();
  CMessageDialogHook* dialogHook = dynamic_cast<CMessageDialogHook*>(view->GetHook(CViewHook::DHT_MessageDialogeHook)); 
  if (dialogHook->IsShown())
  {    
    if (CViewHook::DHT_Unknown != senderHookType)
    {
      //返回到指定hook
      CViewHook::m_curHookType = senderHookType;        
    }
    else
    {    
      if (CViewHook::DHT_Unknown != dialogHook->m_messageDialogEvent.m_senderHookType)
      {
        //返回到调用hook
        dialogHook->Show(false);
        CViewHook::m_curHookType = dialogHook->m_messageDialogEvent.m_senderHookType;    
      }
    }
  }
}

void UeGui::CMessageDialogHook::Init()
{
  return;
}

void UeGui::CMessageDialogHook::DoMouseUp(ModalResultType resultType)
{
  Show(false);
  if (CViewHook::DHT_Unknown != m_messageDialogEvent.m_senderHookType)
  {
    //返回到调用hook
    CViewHook::m_curHookType = m_messageDialogEvent.m_senderHookType;    
    Refresh();
  }
  MessageDialogEvent dialogEvent = m_messageDialogEvent.m_messageDialogEvent;
  if (dialogEvent)
  {
    //回调鼠标响应事件
    (*dialogEvent)(m_messageDialogEvent.m_senderHook, resultType);
  }
}

void UeGui::CMessageDialogHook::UpdateCountDown()
{
  if (m_interval >= 1)
  {
    m_interval -= 1;
  }
  if (m_interval == 0)
  {
    //自动关闭窗口
    Show(false);
    if (CViewHook::DHT_Unknown != m_messageDialogEvent.m_senderHookType)
    {
      //返回到调用hook
      CViewHook::m_curHookType = m_messageDialogEvent.m_senderHookType;    
      Refresh();
    }
  }
}

bool UeGui::CMessageDialogHook::NeedCountDown()
{
  return IsShown() && (m_interval > 0);
}

void UeGui::CMessageDialogHook::Timer()
{
  if (NeedCountDown())
  {
    UpdateCountDown();
  }
}

void UeGui::CMessageDialogHook::DoShowMessageText( const char* caption )
{
  //根据文本的宽度和控件的宽度判断是否要显示成2行
  TCHAR uniText[MAX_NAME_LENGTH] = {};
  m_stringBasic.Ascii2Chs(caption, uniText, MAX_NAME_LENGTH);
  int textLen = ::_tcslen(uniText);
  if (textLen <= m_lineTextCount)
  {
    m_dialogText1.SetCaption(caption);
    m_dialogText2.ClearCaption();
  }
  else
  {
    //文字分割索引
    int splitIndex = m_lineTextCount - 1;
    if ((textLen - m_lineTextCount) <= 3)
    {
      //如果第二行少于2个字则至少显示2个字
      splitIndex -= 3;
    }
    TCHAR tc1[MAX_NAME_LENGTH] = {};
    TCHAR tc2[MAX_NAME_LENGTH] = {};
    TCHAR* pc = tc1;
    for (int i = 0; i <= splitIndex; ++i)
    {
      *pc = uniText[i];
      pc++;
    }
    char asciiText[MAX_NAME_LENGTH] = {};
    m_stringBasic.Wcs2Ascii(tc1, asciiText, MAX_NAME_LENGTH);
    m_dialogText1.SetCaption(asciiText);

    pc = tc2;
    for (int i = splitIndex + 1; i < textLen; ++i)
    {
      *pc = uniText[i];
      pc++;
    }
    m_stringBasic.Wcs2Ascii(tc2, asciiText, MAX_NAME_LENGTH);
    m_dialogText2.SetCaption(asciiText);
  }
}