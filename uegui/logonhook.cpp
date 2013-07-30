#include "logonhook.h"
using namespace UeGui;

CLogonHook* CLogonHook::m_logonHook = NULL;

CLogonHook::CLogonHook() : m_maxProgress(kMaxProgress), m_lastProgress(0), m_lastPosition(0), m_firstSetPosition(true), m_loadGuiElementsDone(false)
{
}

CLogonHook::~CLogonHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CLogonHook::MakeGUI()
{
  if (!m_loadGuiElementsDone)
  {
    FetchWithBinary();
    MakeNames();
    MakeControls();
    m_loadGuiElementsDone = true;
  }
}

tstring CLogonHook::GetBinaryFileName()
{
  return _T("logonhook.bin");
}

void CLogonHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonBackground,	"LogonBackground"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp1,	"LogonSetp1"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp2,	"LogonSetp2"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp3,	"LogonSetp3"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp4,	"LogonSetp4"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp5,	"LogonSetp5"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp6,	"LogonSetp6"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp7,	"LogonSetp7"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp8,	"LogonSetp8"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp9,	"LogonSetp9"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp10,	"LogonSetp10"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp11,	"LogonSetp11"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp12,	"LogonSetp12"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp13,	"LogonSetp13"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp14,	"LogonSetp14"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp15,	"LogonSetp15"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp16,	"LogonSetp16"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp17,	"LogonSetp17"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp18,	"LogonSetp18"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp19,	"LogonSetp19"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp20,	"LogonSetp20"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp21,	"LogonSetp21"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp22,	"LogonSetp22"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp23,	"LogonSetp23"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp24,	"LogonSetp24"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp25,	"LogonSetp25"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp26,	"LogonSetp26"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp27,	"LogonSetp27"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp28,	"LogonSetp28"));
  m_ctrlNames.insert(GuiName::value_type(LogonHook_LogonSetp29,	"LogonSetp29"));
}

void CLogonHook::MakeControls()
{
  ClearProgressBar();
}

short CLogonHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);  
  this->Refresh();
  return ctrlType;
}

short CLogonHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CLogonHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  StartDemo();
  CViewHook::m_curHookType = CViewHook::DHT_LicenseHook;
  this->Refresh();
  return ctrlType;
}

bool CLogonHook::operator ()()
{
  return false;
}

void UeGui::CLogonHook::ClearProgressBar()
{
  CViewHook::GuiElement* guiElement = NULL;
  for (short i = LogonHook_LogonSetp1; i <= LogonHook_LogonSetp29; ++i)
  {
    guiElement = GetGuiElement(i);
    if (guiElement)
    {
      guiElement->m_isVisible = false;
    }
  }
}

void UeGui::CLogonHook::SetMaxProgress( unsigned short maxProgress )
{
  m_maxProgress = maxProgress;
}

void UeGui::CLogonHook::SetProgress( short progress /*= 0*/ )
{
  ////测试代码
  //::Sleep(1000);
  unsigned short position = 0;
  if (progress <= 0)
  {    
    m_lastProgress += 1;
    position = GetPosition(m_lastProgress);
  }
  else
  {
    if (progress < m_lastProgress)
    {
      return;
    }    
    m_lastProgress = progress;
    position = GetPosition(progress);
  }
  SetPosition(position);
}

unsigned short UeGui::CLogonHook::GetPosition( short progress )
{
  //将0-100的进度转换成进度条29格的进度
  if (progress <= 0)
  {
    return 0;
  }
  if (progress > 100)
  {
    progress = 100;
  }
  return progress * (LogonHook_LogonSetp29 - LogonHook_LogonSetp1 + 1) / m_maxProgress;
}

void UeGui::CLogonHook::SetPosition( unsigned short position )
{  
  //进度不能倒退
  if (position <= m_lastPosition)
  {      
    return;
  }
  //position范围必须是0-29
  CViewHook::GuiElement* guiElement = NULL;
  if (m_firstSetPosition)
  {
    ClearProgressBar();
    guiElement = GetGuiElement(LogonHook_LogonBackground);
    if (guiElement)
    {      
      guiElement->m_isVisible = true;
    }
    m_firstSetPosition = false;
  }
  else
  {
    //第二次不画背景  
    guiElement = GetGuiElement(LogonHook_LogonBackground);
    if (guiElement)
    {      
      guiElement->m_isVisible = false;
    }
  }
  //将上一次进度和当前进度之间的所有进度设为可见
  for (short i = m_lastPosition + 1; i <= position; ++i)
  {
    guiElement = GetGuiElement(i + LogonHook_LogonSetp1 - 1);
    if (guiElement)
    {
      guiElement->m_isVisible = true;
    }
  }
  //记录上一次刷新的位置
  m_lastPosition = position;
}

void UeGui::CLogonHook::StartDemo()
{
  for (int i = 0; i <= 100; ++i)
  {
    SetProgress(i);
    Refresh();
    ::Sleep(10);
  }
}

CLogonHook* UeGui::CLogonHook::GetLogonHook()
{
  if (NULL == m_logonHook)
  {
    m_logonHook = new CLogonHook;
  }
  return m_logonHook;
}

bool UeGui::CLogonHook::GuiElementsLoadDone()
{
  return m_loadGuiElementsDone;
}
