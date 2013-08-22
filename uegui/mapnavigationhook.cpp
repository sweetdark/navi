#include "mapnavigationhook.h"
#include "mapnavigationpic1hook.h"
#include "mapnavigationpic2hook.h"
#include "mapnavigationpic3hook.h"
#include "mapnavigationpic4hook.h"
#include "mapnavigationpic5hook.h"
#include "mapnavigationpic6hook.h"
using namespace UeGui;

CMapNavigationHook::CMapNavigationHook()
{
  m_strTitle = "µØÍ¼µ¼º½";
  m_vecHookFile.push_back(_T("mapnavigationhook.bin"));  

  m_showPic = 1;
  m_picOne = NULL;
  m_picTwo = NULL;
  m_picThree = NULL;
  m_picFour = NULL;
  m_picFive = NULL;
  m_picSix = NULL;
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
  if (m_picTwo)
  {
    delete m_picTwo;
  }
  if (m_picThree)
  {
    delete m_picThree;
  }
  if (m_picFour)
  {
    delete m_picFour;
  }
  if (m_picFive)
  {
    delete m_picFive;
  }
  if (m_picSix)
  {
    delete m_picSix;
  }
}

void CMapNavigationHook::Load()
{
  ShowPic(1 ,true);
  ShowPic(2,false);
  ShowPic(3,false);
  ShowPic(4,false);
  ShowPic(5,false);
  ShowPic(6,false);
  SwitchPage();
}
void CMapNavigationHook::Init()
{
  m_picOne = new CMapNavigationPic1Hook();
  m_picOne->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  m_picOne->Show(true);
  AddChildHook(CViewHook::DHT_MapNavigationPic1Hook, m_picOne);

  m_picTwo = new CMapNavigationPic2Hook();
  m_picTwo->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  AddChildHook(CViewHook::DHT_MapNavigationPic2Hook, m_picTwo);

  m_picThree = new CMapNavigationPic3Hook();
  m_picThree->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  AddChildHook(CViewHook::DHT_MapNavigationPic3Hook, m_picThree);

  m_picFour = new CMapNavigationPic4Hook();
  m_picFour->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  AddChildHook(CViewHook::DHT_MapNavigationPic4Hook, m_picFour);

  m_picFive = new CMapNavigationPic5Hook();
  m_picFive->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  AddChildHook(CViewHook::DHT_MapNavigationPic5Hook, m_picFive);

  m_picSix = new CMapNavigationPic6Hook();
  m_picSix->SetHelpers(m_net, m_view, m_route, m_gps, m_query);
  AddChildHook(CViewHook::DHT_MapNavigationPic6Hook, m_picSix);
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
  short ctrlType = CMenuBackgroundHook::MouseDown(scrPoint);
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
    m_isNeedRefesh = false;
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
  short ctrlType = CMenuBackgroundHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case mapnavigationhook_LeftBtnCenter:
    {
      m_leftBtnCenterCtrl.MouseUp();
      if (m_leftBtnCenterCtrl.IsEnable())
      {
        ShowPic(m_showPic, false);
        --m_showPic;
        ShowPic(m_showPic, true);
        SwitchPage();
    }
    }
    break;
  case mapnavigationhook_RightBtnCenter:
    {
      m_rightBtnCenterCtrl.MouseUp();
      if (m_rightBtnCenterCtrl.IsEnable())
      {
        ShowPic(m_showPic, false);
        ++m_showPic;
        ShowPic(m_showPic, true);
        SwitchPage();
    }
    }
    break;
  default:
    m_isNeedRefesh = false;
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
  if (m_showPic == 6)
  {
    m_rightBtnCenterCtrl.SetEnable(false);
  }
  else
  {
    m_rightBtnCenterCtrl.SetEnable(true);
  }
}

void CMapNavigationHook::ShowPic(int showPic, bool show)
{
  switch(showPic)
  {
  case 1:
    {
      m_showPic = show?1:m_showPic;
      m_picOne->Show(show);
      m_picOne->ShowPicOne(show);
    }
    break;
  case 2:
    {
      m_showPic = show?2:m_showPic;
      m_picTwo->Show(show);
      m_picTwo->ShowPicTwo(show);
    }
    break;
  case 3:
    {
      m_showPic = show?3:m_showPic;
      m_picThree->Show(show);
      m_picThree->ShowPicThree(show);
    }
    break;
  case 4:
    {
      m_showPic = show?4:m_showPic;
      m_picFour->Show(show);
      m_picFour->ShowPicFour(show);
    }
    break;
  case 5:
    {
      m_showPic = show?5:m_showPic;
      m_picFive->Show(show);
      m_picFive->ShowPicFive(show);
    }
    break;
  case 6:
    {
      m_showPic = show?6:m_showPic;
      m_picSix->Show(show);
      m_picSix->ShowPicSix(show);
    }
    break;
  default:
    assert(false);
    break;
  }
}