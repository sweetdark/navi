#include "mapnavigationhook.h"
#include "mapnavigationpic1hook.h"
using namespace UeGui;

CMapNavigationHook::CMapNavigationHook()
{
  m_strTitle = "µØÍ¼µ¼º½";
  m_vecHookFile.push_back(_T("mapnavigationhook.bin"));  
  m_showPic = 1;
}

CMapNavigationHook::~CMapNavigationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
  if (m_picOne)
  {
    delete m_picOne;
  }
}

void CMapNavigationHook::Load()
{
  //m_picOne->Show(true);
  m_picOne->ShowPicOne(true);
  SwitchPage();
}
void CMapNavigationHook::Init()
{
  m_picOne = new CMapNavigationPic1Hook();
  m_picOne->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_picOne->Show(true);
  AddChildHook(CViewHook::DHT_MapNavigationPic1Hook, m_picOne);
}
void CMapNavigationHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(mapnavigationhook_LeftBtnCenter,	"LeftBtnCenter"));
  m_ctrlNames.insert(GuiName::value_type(mapnavigationhook_RightBtnCenter,	"RightBtnCenter"));
}

void CMapNavigationHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_leftBtnCenterCtrl.SetCenterElement(GetGuiElement(mapnavigationhook_LeftBtnCenter));
  m_rightBtnCenterCtrl.SetCenterElement(GetGuiElement(mapnavigationhook_RightBtnCenter));
}

short CMapNavigationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case mapnavigationhook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseDown();
      AddRenderUiControls(&m_leftBtnCenterCtrl);
    }
    break;
  case mapnavigationhook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseDown();
      AddRenderUiControls(&m_rightBtnCenterCtrl);
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

short CMapNavigationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapNavigationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case mapnavigationhook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseUp();
    }
    break;
  case mapnavigationhook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseUp();
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

void CMapNavigationHook::SwitchPage()
{
  if (m_showPic == 1)
  {
    m_leftBtnCenterCtrl.SetEnable(false);
  }
  else 
  {
    m_leftBtnCenterCtrl.SetEnable(true);
  }
  if (m_showPic == 1)
  {
    m_rightBtnCenterCtrl.SetEnable(false);
  }
  else
  {
    m_rightBtnCenterCtrl.SetEnable(true);
  }
}
