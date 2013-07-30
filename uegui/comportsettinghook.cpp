#include "comportsettinghook.h"
#include "guisetting.h"

using namespace UeGui;

CCOMPortSettingHook::CCOMPortSettingHook() : m_COMIndex(0), m_baudRateIndex(0), m_isEdit(false)
{
  m_COMPortList.push_back(1);
  m_COMPortList.push_back(2);
  m_COMPortList.push_back(3);
  m_COMPortList.push_back(4);
  m_COMPortList.push_back(5);
  m_COMPortList.push_back(6);
  m_COMPortList.push_back(7);
  m_COMPortList.push_back(8);
  m_COMPortList.push_back(9);
  m_COMPortList.push_back(10);
  m_COMPortList.push_back(11);

  m_baudRateList.push_back(300);
  m_baudRateList.push_back(600);
  m_baudRateList.push_back(1200);
  m_baudRateList.push_back(1800);
  m_baudRateList.push_back(2400);
  m_baudRateList.push_back(4800);
  m_baudRateList.push_back(7200);
  m_baudRateList.push_back(9600);
  m_baudRateList.push_back(14400);
  m_baudRateList.push_back(19200);
  m_baudRateList.push_back(38400);  
  m_baudRateList.push_back(57600);
  m_baudRateList.push_back(115200);
  m_baudRateList.push_back(128000);
}

CCOMPortSettingHook::~CCOMPortSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CCOMPortSettingHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CCOMPortSettingHook::GetBinaryFileName()
{
  return _T("comportsettinghook.bin");
}

void CCOMPortSettingHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_ViewMap,	"ViewMap"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_Previous,	"Previous"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_Title,	"Title"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMPortLabel,	"COMPortLabel"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMLeft,	"COMLeft"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMLeftIcon,	"COMLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMLeftIconBack,	"COMLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMPort,	"COMPort"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMRight,	"COMRight"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMRightIcon,	"COMRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_COMRightIconBack,	"COMRightIconBack"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateLabel,	"BaudRateLabel"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateLeft,	"BaudRateLeft"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateLeftIcon,	"BaudRateLeftIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateLeftIconBack,	"BaudRateLeftIconBack"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRate,	"BaudRate"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateRight,	"BaudRateRight"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateRightIcon,	"BaudRateRightIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_WarmStartIcon,	"WarmStartIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_BaudRateRightIconBack,	"BaudRateRightIconBack"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_CodeStartLeft,	"CodeStartLeft"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_ColdStartCenter,	"ColdStartCenter"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_CodeStartRight,	"CodeStartRight"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_ColdStartIcon,	"ColdStartIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_ColdStartLabel,	"ColdStartLabel"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_WarmStartLeft,	"WarmStartLeft"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_WarmStartCenter,	"WarmStartCenter"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_WarmStartRight,	"WarmStartRight"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_WarmStartIcon,	"WarmStartIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_WarmStartLabel,	"WarmStartLabel"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_NormalStartLeft,	"NormalStartLeft"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_NormalStartCenter,	"NormalStartCenter"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_NormalStartRight,	"NormalStartRight"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_NormalStartIcon,	"NormalStartIcon"));
  m_ctrlNames.insert(GuiName::value_type(COMPortSettingHook_NormalStartLabel,	"NormalStartLabel"));
}

void CCOMPortSettingHook::MakeControls()
{
  m_previousCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_Previous));
  m_viewMapCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_ViewMap));

  m_COMPortCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_COMPort));
  m_COMLeftCtrl.SetLeftElement(GetGuiElement(COMPortSettingHook_COMLeft));
  m_COMLeftCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_COMLeftIcon));
  m_COMLeftCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_COMLeftIconBack));
  
  m_COMRightCtrl.SetRightElement(GetGuiElement(COMPortSettingHook_COMRight));
  m_COMRightCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_COMRightIcon));
  m_COMRightCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_COMRightIconBack));

  m_baudRateCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_BaudRate));

  m_baudRateLeftCtrl.SetLeftElement(GetGuiElement(COMPortSettingHook_BaudRateLeft));
  m_baudRateLeftCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_BaudRateLeftIcon));
  m_baudRateLeftCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_BaudRateLeftIconBack));

  m_baudRateRightCtrl.SetRightElement(GetGuiElement(COMPortSettingHook_BaudRateRight));
  m_baudRateRightCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_BaudRateRightIcon));
  m_baudRateRightCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_BaudRateRightIconBack));

  m_codeStartCtrl.SetLeftElement(GetGuiElement(COMPortSettingHook_CodeStartLeft));
  m_codeStartCtrl.SetRightElement(GetGuiElement(COMPortSettingHook_CodeStartRight));
  m_codeStartCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_ColdStartCenter));
  m_codeStartCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_ColdStartIcon));

  m_warmStartCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_WarmStartCenter));
  m_warmStartCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_WarmStartIcon));
  m_warmStartCtrl.SetLeftElement(GetGuiElement(COMPortSettingHook_WarmStartLeft));
  m_warmStartCtrl.SetRightElement(GetGuiElement(COMPortSettingHook_WarmStartRight));

  m_normalStartCtrl.SetCenterElement(GetGuiElement(COMPortSettingHook_NormalStartCenter));
  m_normalStartCtrl.SetIconElement(GetGuiElement(COMPortSettingHook_NormalStartIcon));
  m_normalStartCtrl.SetLeftElement(GetGuiElement(COMPortSettingHook_NormalStartLeft));
  m_normalStartCtrl.SetRightElement(GetGuiElement(COMPortSettingHook_NormalStartRight));
}

short CCOMPortSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case COMPortSettingHook_Previous:
    {
      m_previousCtrl.MouseDown();
    }
    break;
  case COMPortSettingHook_ViewMap:
    {
      m_viewMapCtrl.MouseDown();
    }
    break;
  case COMPortSettingHook_COMLeft:
  case COMPortSettingHook_COMLeftIcon:
  case COMPortSettingHook_COMLeftIconBack:
    {
      m_COMLeftCtrl.MouseDown();
    }
    break;
  case COMPortSettingHook_COMRight:
  case COMPortSettingHook_COMRightIcon:
  case COMPortSettingHook_COMRightIconBack:
    {
      m_COMRightCtrl.MouseDown();
    }
    break;
  case COMPortSettingHook_BaudRateLeft:
  case COMPortSettingHook_BaudRateLeftIcon:
  case COMPortSettingHook_BaudRateLeftIconBack:
    {
      m_baudRateLeftCtrl.MouseDown();
    }
    break;  
  case COMPortSettingHook_BaudRateRight:
  case COMPortSettingHook_BaudRateRightIcon:
  case COMPortSettingHook_BaudRateRightIconBack:
    {
      m_baudRateRightCtrl.MouseDown();
    }
    break;
  case COMPortSettingHook_CodeStartLeft:
  case COMPortSettingHook_CodeStartRight:
  case COMPortSettingHook_ColdStartCenter:
  case COMPortSettingHook_ColdStartIcon:
  case COMPortSettingHook_ColdStartLabel:
    {
      m_codeStartCtrl.MouseDown();
      AddRenderUiControls(&m_codeStartCtrl);
      AddRenderElements(GetGuiElement(COMPortSettingHook_ColdStartLabel));
    }
    break;
  case COMPortSettingHook_WarmStartLeft:
  case COMPortSettingHook_WarmStartRight:
  case COMPortSettingHook_WarmStartCenter:
  case COMPortSettingHook_WarmStartIcon:
  case COMPortSettingHook_WarmStartLabel:
    {
      m_warmStartCtrl.MouseDown();
      AddRenderUiControls(&m_warmStartCtrl);
      AddRenderElements(GetGuiElement(COMPortSettingHook_WarmStartLabel));
    }
    break;
  case COMPortSettingHook_NormalStartLeft:
  case COMPortSettingHook_NormalStartCenter:
  case COMPortSettingHook_NormalStartRight:
  case COMPortSettingHook_NormalStartIcon:
  case COMPortSettingHook_NormalStartLabel:
    {
      m_normalStartCtrl.MouseDown();
      AddRenderUiControls(&m_normalStartCtrl);
      AddRenderElements(GetGuiElement(COMPortSettingHook_NormalStartLabel));
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

short CCOMPortSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CCOMPortSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short upElementType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case COMPortSettingHook_Previous:
    {
      m_previousCtrl.MouseUp();
      if (upElementType == m_downElementType)
      {
        UpdateSettings();      
        DoReturn();
      }
    }
    break;
  case COMPortSettingHook_ViewMap:
    {
      m_viewMapCtrl.MouseUp();
      if (upElementType == m_downElementType)
      {
        UpdateSettings();    
        OpenMapHook();
      } 
    }
    break;
  case COMPortSettingHook_COMLeft:
  case COMPortSettingHook_COMLeftIcon:
  case COMPortSettingHook_COMLeftIconBack:
    {
      m_COMLeftCtrl.MouseUp();
      m_isEdit = true; 
      if (m_COMIndex > 0)
      {
        --m_COMIndex;
        SetCOMPort(m_COMPortList[m_COMIndex]);
      }
      SetCOMPortCtrlStatus();
    }
    break;
  case COMPortSettingHook_COMRight:
  case COMPortSettingHook_COMRightIcon:
  case COMPortSettingHook_COMRightIconBack:
    {
      m_COMRightCtrl.MouseUp();
      m_isEdit = true; 
      if (m_COMIndex < m_COMPortList.size() - 1)
      {
        ++m_COMIndex;
        SetCOMPort(m_COMPortList[m_COMIndex]);
      }
      SetCOMPortCtrlStatus();
    }
    break;
  case COMPortSettingHook_BaudRateLeft:
  case COMPortSettingHook_BaudRateLeftIcon:
  case COMPortSettingHook_BaudRateLeftIconBack:
    {
      m_baudRateLeftCtrl.MouseUp();
      m_isEdit = true; 
      if (m_baudRateIndex > 0)
      {
        --m_baudRateIndex;
        SetBaudRate(m_baudRateList[m_baudRateIndex]);
      }
      SetBaudRateCtrlStatus();
    }
    break;  
  case COMPortSettingHook_BaudRateRight:
  case COMPortSettingHook_BaudRateRightIcon:
  case COMPortSettingHook_BaudRateRightIconBack:
    {
      m_baudRateRightCtrl.MouseUp();
      m_isEdit = true; 
      if (m_baudRateIndex < m_baudRateList.size() - 1)
      {
        ++m_baudRateIndex;
        SetBaudRate(m_baudRateList[m_baudRateIndex]);
      }
      SetBaudRateCtrlStatus();
    }
    break;
  case COMPortSettingHook_CodeStartLeft:
  case COMPortSettingHook_CodeStartRight:
  case COMPortSettingHook_ColdStartCenter:
  case COMPortSettingHook_ColdStartIcon:
  case COMPortSettingHook_ColdStartLabel:
    {
      m_codeStartCtrl.MouseUp();
      m_warmStartCtrl.SetCheck(false);
      m_normalStartCtrl.SetCheck(false);
      m_isEdit = true; 
    }
    break;
  case COMPortSettingHook_WarmStartLeft:
  case COMPortSettingHook_WarmStartRight:
  case COMPortSettingHook_WarmStartCenter:
  case COMPortSettingHook_WarmStartIcon:
  case COMPortSettingHook_WarmStartLabel:
    {
      m_warmStartCtrl.MouseUp();
      m_codeStartCtrl.SetCheck(false);
      m_normalStartCtrl.SetCheck(false);
      m_isEdit = true; 
    }
    break;
  case COMPortSettingHook_NormalStartLeft:
  case COMPortSettingHook_NormalStartCenter:
  case COMPortSettingHook_NormalStartRight:
  case COMPortSettingHook_NormalStartIcon:
  case COMPortSettingHook_NormalStartLabel:
    {
      m_normalStartCtrl.MouseUp();
      m_warmStartCtrl.SetCheck(false);
      m_codeStartCtrl.SetCheck(false);
      m_isEdit = true; 
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
  return upElementType;
}

bool CCOMPortSettingHook::operator ()()
{
  return false;
}


void UeGui::CCOMPortSettingHook::ReadSettings()
{
  CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
  if (ueSettings)
  {  
    /// 设置端口
    unsigned char port = ueSettings->GetGPSPort();
    m_COMIndex = FinCOMPortIndex(port);
    SetCOMPort(m_COMPortList[m_COMIndex]);
    SetBaudRateCtrlStatus();
    /// 设置波特率
    unsigned int baudRate = ueSettings->GetGPSBaudRate();    
    m_baudRateIndex = FinBaudRateIndex(baudRate);
    SetBaudRate(m_baudRateList[m_baudRateIndex]);
    SetBaudRateCtrlStatus();
    /// 设置启动方式
    unsigned char startMode = ueSettings->GetGPSStartMode();
    switch (startMode)
    {
    case SystemSettings::SM_Cold :
      {
        m_codeStartCtrl.SetCheck(true);
        m_warmStartCtrl.SetCheck(false);
        m_normalStartCtrl.SetCheck(false);
        break;
      }
    case SystemSettings::SM_Warm :
      {
        m_codeStartCtrl.SetCheck(false);
        m_warmStartCtrl.SetCheck(true);
        m_normalStartCtrl.SetCheck(false);
        break;
      }
    case SystemSettings::SM_Normal :
      {
        m_codeStartCtrl.SetCheck(false);
        m_warmStartCtrl.SetCheck(false);
        m_normalStartCtrl.SetCheck(true);
        break;
      }
    }
  }
}

void UeGui::CCOMPortSettingHook::UpdateSettings()
{
  if (m_isEdit)
  {
    CGuiSettings* ueSettings = CGuiSettings::GetGuiSettings();
    if (ueSettings)
    {  
      if ((m_COMIndex >= 0) && (m_COMIndex < m_COMPortList.size()))
      {
        ueSettings->SetGPSPort(m_COMPortList[m_COMIndex]);
      }
      if ((m_baudRateIndex >= 0) && (m_baudRateIndex < m_baudRateList.size()))
      {
        ueSettings->SetGPSBaudRate(m_baudRateList[m_baudRateIndex]);
      }   
      ueSettings->SaveSystemSettings();
    }
  }
}

unsigned int UeGui::CCOMPortSettingHook::FinCOMPortIndex( unsigned char port )
{
  for (int i = 0; i < m_COMPortList.size(); ++i)
  {
    if (port == m_COMPortList[i])
    {
      return i;
    }
  }
  return 0;
}

unsigned int UeGui::CCOMPortSettingHook::FinBaudRateIndex( unsigned int baudRate )
{
  for (int i = 0; i < m_baudRateList.size(); ++i)
  {
    if (baudRate == m_baudRateList[i])
    {
      return i;
    }
  }
  return 0;
}

void UeGui::CCOMPortSettingHook::Load()
{
  m_isEdit = false;
  ReadSettings();
}

void UeGui::CCOMPortSettingHook::SetCOMPort( unsigned char port )
{
  /// 设置端口
  char strValue[15] = {};
  ::sprintf(strValue, "COM%d", port);
  m_COMPortCtrl.SetCaption(strValue);
}

void UeGui::CCOMPortSettingHook::SetBaudRate( unsigned int baudRate )
{
  /// 设置波特率
  char strValue[15] = {};  
  ::sprintf(strValue, "%d", baudRate);
  m_baudRateCtrl.SetCaption(strValue);
}

void UeGui::CCOMPortSettingHook::OpenMapHook()
{
  /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_MapHook;  */
  CAggHook::TurnTo(CViewHook::DHT_MapHook);
  ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
}

void UeGui::CCOMPortSettingHook::DoReturn()
{
  /*CViewHook::m_curHookType = CViewHook::m_prevHookType;
  CViewHook::m_prevHookType = CViewHook::DHT_COMPortSettingHook;  */
  CAggHook::Return();
}

void UeGui::CCOMPortSettingHook::SetCOMPortCtrlStatus()
{
  if (m_COMIndex <= 0)
  {
    m_COMLeftCtrl.SetEnable(false);
  }
  if (m_COMIndex >= m_COMPortList.size() - 1)
  { 
    m_COMRightCtrl.SetEnable(false);
  }
  if ((m_COMIndex > 0) && (m_COMIndex < m_COMPortList.size() - 1))
  {
    m_COMLeftCtrl.SetEnable(true);
    m_COMRightCtrl.SetEnable(true);
  }
}

void UeGui::CCOMPortSettingHook::SetBaudRateCtrlStatus()
{
  if (m_baudRateIndex <= 0)
  {
    m_baudRateLeftCtrl.SetEnable(false);
  }
  if (m_baudRateIndex >= m_baudRateList.size() - 1)
  { 
    m_baudRateRightCtrl.SetEnable(false);
  }
  if ((m_baudRateIndex > 0) && (m_baudRateIndex < m_baudRateList.size() - 1))
  {
    m_baudRateLeftCtrl.SetEnable(true);
    m_baudRateRightCtrl.SetEnable(true);
  }
}
