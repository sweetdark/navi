#include "roundtypeselecthook.h"

#include "routewrapper.h"

using namespace UeGui;

CRoundTypeSelectHook::CRoundTypeSelectHook() : m_curRoundType(RT_MapCenter)
{
}

CRoundTypeSelectHook::~CRoundTypeSelectHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CRoundTypeSelectHook::GetBinaryFileName()
{
  return _T("roundtypeselecthook.bin");
}

void CRoundTypeSelectHook::Load()
{
  ClearSelection();
  SetBtnEnable();
  switch(m_curRoundType)
  {
  case RT_MapCenter:
    m_mapCenterBtn.SetCheck(true);
    break;
  case RT_CurPos:
    m_curPosBtn.SetCheck(true);
    break;
  case RT_EndPoint:
    m_endPointBtn.SetCheck(true);
    break;
  case RT_Route:
    m_routeBtn.SetCheck(true);
    break;
  }
}

void CRoundTypeSelectHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_MapCenterBtn,	"MapCenterBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_MapCenterBtnIcon,	"MapCenterBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_CurPosBtn,	"CurPosBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_CurPosBtnIcon,	"CurPosBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_EndPointBtn,	"EndPointBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_EndPointBtnIcon,	"EndPointBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_RouteBtn,	"RouteBtn"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_RouteBtnIcon,	"RouteBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(RoundTypeSelectHook_GrayBack,	"GrayBack"));
}

void CRoundTypeSelectHook::MakeControls()
{
  m_mapCenterBtn.SetCenterElement(GetGuiElement(RoundTypeSelectHook_MapCenterBtn));
  m_mapCenterBtn.SetIconElement(GetGuiElement(RoundTypeSelectHook_MapCenterBtnIcon));

  m_curPosBtn.SetCenterElement(GetGuiElement(RoundTypeSelectHook_CurPosBtn));
  m_curPosBtn.SetIconElement(GetGuiElement(RoundTypeSelectHook_CurPosBtnIcon));

  m_endPointBtn.SetCenterElement(GetGuiElement(RoundTypeSelectHook_EndPointBtn));
  m_endPointBtn.SetIconElement(GetGuiElement(RoundTypeSelectHook_EndPointBtnIcon));

  m_routeBtn.SetCenterElement(GetGuiElement(RoundTypeSelectHook_RouteBtn));
  m_routeBtn.SetIconElement(GetGuiElement(RoundTypeSelectHook_RouteBtnIcon));

  m_backBtn.SetCenterElement(GetGuiElement(RoundTypeSelectHook_GrayBack));  
}

short CRoundTypeSelectHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RoundTypeSelectHook_MapCenterBtn:
  case RoundTypeSelectHook_MapCenterBtnIcon:
    {
      if (m_mapCenterBtn.IsEnable())
      {
        m_mapCenterBtn.MouseDown();
        AddRenderUiControls(&m_mapCenterBtn);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_CurPosBtn:
  case RoundTypeSelectHook_CurPosBtnIcon:
    {
      if (m_curPosBtn.IsEnable())
      {
	      m_curPosBtn.MouseDown();
        AddRenderUiControls(&m_curPosBtn);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_EndPointBtn:
  case RoundTypeSelectHook_EndPointBtnIcon:
    {
      if (m_endPointBtn.IsEnable())
      {
        m_endPointBtn.MouseDown();
        AddRenderUiControls(&m_endPointBtn);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_RouteBtn:
  case RoundTypeSelectHook_RouteBtnIcon:
    {
      if (m_routeBtn.IsEnable())
      {
        m_routeBtn.MouseDown();
        AddRenderUiControls(&m_routeBtn);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_GrayBack:
    {
      m_backBtn.MouseDown();
      AddRenderUiControls(&m_backBtn);
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

short CRoundTypeSelectHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRoundTypeSelectHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RoundTypeSelectHook_MapCenterBtn:
  case RoundTypeSelectHook_MapCenterBtnIcon:
    {
      if (m_mapCenterBtn.IsEnable())
      {
        m_mapCenterBtn.MouseUp();
        SetCurRoundType(RT_MapCenter);
        Return();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_CurPosBtn:
  case RoundTypeSelectHook_CurPosBtnIcon:
    {
      if (m_curPosBtn.IsEnable())
      {
        m_curPosBtn.MouseUp();
        SetCurRoundType(RT_CurPos);
        Return();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_EndPointBtn:
  case RoundTypeSelectHook_EndPointBtnIcon:
    {
      if (m_endPointBtn.IsEnable())
      {
        m_endPointBtn.MouseUp();
        SetCurRoundType(RT_EndPoint);
        Return();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_RouteBtn:
  case RoundTypeSelectHook_RouteBtnIcon:
    {
      if (m_routeBtn.IsEnable())
      {
        m_routeBtn.MouseUp();
        SetCurRoundType(RT_Route);
        Return();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case RoundTypeSelectHook_GrayBack:
    {
      m_backBtn.MouseUp();
      Return(false);
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

bool CRoundTypeSelectHook::operator ()()
{
  return false;
}

void CRoundTypeSelectHook::ClearSelection()
{
  m_mapCenterBtn.SetCheck(false);
  m_curPosBtn.SetCheck(false);
  m_endPointBtn.SetCheck(false);
  m_routeBtn.SetCheck(false);
}

void CRoundTypeSelectHook::SetBtnEnable()
{
  m_mapCenterBtn.SetEnable(true);
  m_curPosBtn.SetEnable(true);
  m_endPointBtn.SetEnable(true);
  m_routeBtn.SetEnable(true);

  if (m_gps == 0 || !m_gps->IsLive())
  {
    m_curPosBtn.SetEnable(false);
    m_routeBtn.SetEnable(false);
    if (m_curRoundType == RT_CurPos || m_curRoundType == RT_Route)
    {
      m_curRoundType = RT_MapCenter;
    }
  }

  PlanPosition endPos;
  endPos.m_type = UeRoute::PT_End;
  unsigned int rt = CRouteWrapper::Get().GetPosition(endPos);
  if ((UeRoute::PT_Invalid == endPos.m_type) || (!endPos.IsValid()))
  {
    m_endPointBtn.SetEnable(false);
    if (m_curRoundType == RT_EndPoint)
    {
      m_curRoundType = RT_MapCenter;
    }
  }
}

void CRoundTypeSelectHook::SetCurRoundType(RoundType roundType)
{
  m_curRoundType = roundType;
}

CRoundTypeSelectHook::RoundType CRoundTypeSelectHook::GetCurRoundType()
{
  return m_curRoundType;
}