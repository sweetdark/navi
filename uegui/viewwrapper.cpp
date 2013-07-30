#include "viewwrapper.h"
#include "uemap/viewcanvas.h"

using namespace UeGui;

UeGui::CViewWrapper::CViewWrapper() : m_view(IView::GetView()), m_route(IRoute::GetRoute()),
  m_query(IQuery::GetQuery()), m_net(IRoadNetwork::GetNetwork()), m_gps(IMapMatch::GetMapMatch())
{
}

UeGui::CViewWrapper::~CViewWrapper()
{

}

CViewWrapper& UeGui::CViewWrapper::Get()
{
  static CViewWrapper m_viewWrapper;
  return m_viewWrapper;
}

void UeGui::CViewWrapper::HideWindow()
{
  if (m_view)
  {
    ::ShowWindow(reinterpret_cast<HWND>(m_view->GetWnd()), SW_HIDE);
  }
}

CViewState* UeGui::CViewWrapper::GetMainViewState()
{
  if (m_view)
  {
    CViewState* mainView = m_view->GetState(VT_North);
    if(!mainView)
    {
      mainView = m_view->GetState(VT_Heading);
      if(!mainView)
      {
        mainView = m_view->GetState(VT_Perspective);
      }
    }
    return mainView;
  }
  return NULL;
}


void UeGui::CViewWrapper::NextState()
{
  if (m_view)
  {
    m_view->NextState();
  }
}

void UeGui::CViewWrapper::MoveToStart()
{
  if (m_view)
  {
    GpsCar gpsInfo;
    PlanPosition startPos;
    startPos.m_type = UeRoute::PT_Start;
    m_route->GetPosition(startPos); 
    if ((startPos.m_type == UeRoute::PT_Invalid) || (startPos.m_pos.m_x <= 0 && startPos.m_pos.m_y <= 0)) 
    {
      const GpsCar &gpsPos = m_view->GetGpsPosInfo();
      gpsInfo.m_curPos.m_x = gpsPos.m_curPos.m_x;
      gpsInfo.m_curPos.m_y = gpsPos.m_curPos.m_y;
    }
    else
    {
      gpsInfo.m_curPos.m_x = startPos.m_pos.m_x;
      gpsInfo.m_curPos.m_y = startPos.m_pos.m_y;
    }
    m_view->SetViewOpeMode(VM_Guidance);
    m_view->SetGpsCar(gpsInfo);
    m_view->GetCommand(UeMap::CT_Select)->MoveTo(gpsInfo);
  }
}

void UeGui::CViewWrapper::MoveToCar()
{
  if (m_view)
  {
    // 移动地图位置
    m_view->SetViewOpeMode(VM_Guidance);
    const GpsCar &carInfo = m_view->GetGpsCar();
    m_view->GetCommand(UeMap::CT_Select)->MoveTo(carInfo);    
  }
}

void UeGui::CViewWrapper::MoveToGPS()
{
  //有信号时回到信号处
  //bool isCaptureGps = false;
  GpsCar gpsInfo;
  //PosBasic gpsPos;
  //MatchResult matchedPos;
  //unsigned int rt = m_gps->GetPos(gpsPos, matchedPos);
  //if (rt == GEC_Success)
  //{
  //  if (!m_gps->IsInitialSignals())
  //  {      
  //    gpsInfo.m_curPos.m_x = matchedPos.m_x;
  //    gpsInfo.m_curPos.m_y = matchedPos.m_y;
  //    isCaptureGps = true;
  //  }
  //}
  const GpsCar &gpsPos = m_view->GetGpsPosInfo();
  gpsInfo.m_curPos.m_x = gpsPos.m_curPos.m_x;
  gpsInfo.m_curPos.m_y = gpsPos.m_curPos.m_y;

  m_view->SetViewOpeMode(VM_Guidance);
  m_view->SetGpsCar(gpsInfo);
  m_view->GetCommand(UeMap::CT_Select)->MoveTo(gpsInfo);
}

bool UeGui::CViewWrapper::AddViewIcon( ViewIconType iconType, short icon )
{
  if (m_view)
  {
    return m_view->AddViewIcon(iconType, icon);
  }
  return false;
}

void UeGui::CViewWrapper::GetScale( short &level, int &scale )
{
  if (m_view)
  {
    CViewState* mainView = GetMainViewState();
    if (mainView)
    {
      return mainView->GetScale(level, scale);
    }
  }
}

unsigned short UeGui::CViewWrapper::GetMaxScaleLevel()
{
  if (m_view)
  {
    return m_view->GetMaxScale() - 1;
  }
  return 0;
}

const GpsCar & UeGui::CViewWrapper::GetGpsCar()
{
  if (m_view)
  {
    return m_view->GetGpsCar();
  }
  //自车图标位置
  GpsCar gpsCar;
  return gpsCar;
}

void UeGui::CViewWrapper::SetGpsCar( const GpsCar &gpsCar )
{
  if (m_view)
  {
    m_view->SetGpsCar(gpsCar);
  }
}

CViewHook * UeGui::CViewWrapper::GetHook( short type )
{
  if (m_view)
  {
    return m_view->GetHook(type);
  }
  return NULL;
}

void UeGui::CViewWrapper::Refresh()
{
  if (m_view)
  {
    m_view->Refresh();
  }
}

void UeGui::CViewWrapper::RefreshUI( short type /*= VT_Unknown*/ )
{
  if (m_view)
  {
    m_view->RefreshUI(type);
  }
}

void UeGui::CViewWrapper::SetViewOpeMode( ViewOpeMode opeMode, ViewType type /*= VT_Unknown*/ )
{
  if (m_view)
  {
    m_view->SetViewOpeMode(opeMode, type);
  }
}

UeMap::ViewOpeMode UeGui::CViewWrapper::GetViewOpeMode( ViewType type /*= VT_Unknown*/ )
{
  if (m_view)
  {
    return m_view->GetViewOpeMode(type);
  }
  return VM_Unknown;
}

bool UeGui::CViewWrapper::Pick( const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered /*= false*/ )
{
  if (m_view)
  {
    return m_view->Pick(scrPoint, objects, isRendered);
  }
  return false;
}

void UeGui::CViewWrapper::GetPickPos( CGeoPoint<long> &pickPos )
{
  if (m_view)
  {
    m_view->GetPickPos(pickPos);
  }
}

void UeGui::CViewWrapper::GetPickName( char *pickName )
{
  if (m_view)
  {
    m_view->GetPickName(pickName);
  }
}

char* UeGui::CViewWrapper::GetSelectName()
{
  return CViewCanvas::GetSelectedName();
}

void UeGui::CViewWrapper::ZoomIn( short levelDown, short step )
{
  if (m_view)
  {
    CViewState* mainState = GetMainViewState();
    if (mainState)
    {
      mainState->ZoomIn(levelDown, step);
      int maxScale = m_view->GetMaxScale();
      mainState->Zooming(maxScale, 0, 0);      
    }
  }
}

void UeGui::CViewWrapper::ZoomOut( short levelUp, short step )
{
  if (m_view)
  {
    CViewState* mainState = GetMainViewState();
    if (mainState)
    {
      mainState->ZoomOut(levelUp, step);
      int maxScale = m_view->GetMaxScale();
      mainState->Zooming(0, maxScale, 0);
    }
  }
}

bool UeGui::CViewWrapper::AutoScallingMap( bool isLock /*= false*/ )
{
  if (m_view)
  {
    return m_view->AutoScallingMap(isLock);
  }
  return false;
}

void UeGui::CViewWrapper::SwitchTo( short scaleLevel, double angle, short type /*= VT_Unknown*/ )
{
  if (m_view)
  {
    m_view->SwitchTo(scaleLevel, angle, type);
  }
}

void UeGui::CViewWrapper::SwitchTo( const CGeoRect<long> &extent )
{
  if (m_view)
  {
    m_view->SwitchTo(extent);
  }
}

void UeGui::CViewWrapper::SetCompassIconPos( const CGeoPoint<short> &scrPoint )
{
  if (m_view)
  {
    m_view->SetCompassIconPos(scrPoint);
  }
}

void UeGui::CViewWrapper::SetLanePos( CGeoPoint<short> &scrPoint, short width, short height )
{
  if (m_view)
  {
    m_view->SetLanePos(scrPoint, width, height);
  }
}

bool UeGui::CViewWrapper::IsNeedRenderGuidanceView()
{
  if (m_view)
  {
    return m_view->IsNeedRenderGuidanceView();
  }
  return false;
}

bool UeGui::CViewWrapper::IsGuidanceViewShown()
{
  if (m_view)
  {
    return m_view->IsGuidanceViewShown();
  }
  return false;
}

void UeGui::CViewWrapper::ShowGuidanceView()
{
  if (m_view)
  {
    m_view->ShowGuidanceView();
  }
}

void UeGui::CViewWrapper::SetPickPos( const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint, bool refresh /*= true*/ )
{
  if (m_view)
  {
    //切换地图为可操作状态
    m_view->SetViewOpeMode(VM_Browse);
    m_view->SetPickPos(pickPos, scrPoint);
  }
}