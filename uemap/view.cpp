/*
 * The UrbanEye PND solution is a collection of packages based on native C++ classes
 * that implement the fundamental algorithms & operations required to satisfy one kind
 * of mobile or portable devices with competitive qualities being a leading one.
 *
 * Copyright (C) 2008 Xiujie Sheng
 *
 * This library isn't a free software, so you cann't take the sourcecode without author's permit.
 * Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
 * if you illegally got it. 
 *
 * NOTE: 
 * AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
 * LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
 *
 * For more information, contact:
 *
 * China University of Geoseciences(Wuhan)
 * Graduate School
 *
 * Xiujie Sheng
 * smartpoint@sohu.com
 */
#include "view.h"
#include "viewimpl.h"
using namespace UeMap;

// Static varible initialization
IView *IView::m_view = 0;

//////////////////////////////////////////////////////////////////////////////////
//
/**
*
**/
IView::IView(CViewImpl *oneImpl) : m_viewImpl(oneImpl), m_garbage(0), m_mediator(0)
{
	assert(m_viewImpl);
}

/**
*
**/
IView::~IView()
{
	if(m_viewImpl)
	{
		delete m_viewImpl;
	}
	m_viewImpl = 0;
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for singletion
/**
*
*/
IView *IView::GetView()
{
	if(!m_view)
	{
		CDynamicLibrary oneLib;

	#ifdef _DEBUG
		tstring fileName = _T("uemapd.dll");
		bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
		assert(rt);
	#else
		tstring fileName = _T("uemap.dll");
		bool rt = oneLib.Load(fileName, CDynamicLibrary::LM_Lazy); 
		assert(rt);
	#endif

		if(rt)
		{
			tstring funcName = _T("GetView");
			tstring type = _T("Default");
			m_view = reinterpret_cast<IView *>(oneLib.ExecuteInterface(funcName, type));
		}
	}

	assert(m_view);
	return m_view;
}

/**
*
*/
IView *IView::GetView(CViewImpl *oneImpl)
{
	if(!m_view)
	{
		// Double check if there has a lock mechanism
		if(!m_view)
		{
			m_view = new IView(oneImpl);
		}
	}

	return m_view;
}

/**
*
*/
void IView::Delete()
{
	if(m_view)
	{
		delete m_view;
	}
	m_view = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Methods for outside specification
/**
*
*/
inline bool IView::Attach(void *oneWnd, ScreenLayout &scrLayout)
{
	return m_viewImpl->Attach(oneWnd, scrLayout);
}


/**
*
*/
inline void IView::Detach()
{
	return m_viewImpl->Detach();
}

/**
*
*/
inline void *IView::GetWndHandle()
{
	return m_viewImpl->GetWndHandle();
}

/**
*
*/
inline void *IView::GetResHandle()
{
	return m_viewImpl->GetResHandle();
}

/**
*
**/
inline void IView::SwitchScrMode()
{
	return m_viewImpl->SwitchScrMode();
}

/**
*
**/
inline short IView::GetScrMode()
{
	return m_viewImpl->GetScrMode();
}


//////////////////////////////////////////////////////////////////////////////////
// Methods for mapping between screen and real map
//
/**
*
**/
inline void IView::SetScrLayout(ScreenLayout &layout, short type)
{
	m_viewImpl->SetScrLayout(layout, type);
}

/**
*
**/
inline const ScreenLayout &IView::GetScrLayout(short type)
{
	return m_viewImpl->GetScrLayout(type);
}

/**
*
**/
inline void IView::SetMapLayout(const MapLayout &layout, short type)
{
	m_viewImpl->SetMapLayout(layout, type);
}

/**
*
**/
inline const MapLayout &IView::GetMapLayout(short type)
{
	return m_viewImpl->GetMapLayout(type);
}

/**
*
*/
inline void IView::GetScale(short &level, int &scale, short type) const
{
	return m_viewImpl->GetScale(level, scale, type);
}

inline int IView::GetMaxScale() const
{
  return m_viewImpl->GetMaxScale();
}


/**
*
**/
inline void IView::SetScale(short level, int scale, short type)
{
	m_viewImpl->SetScale(level, scale, type);
}

/**
*
**/
inline void IView::EraseScale(short level, short type)
{
	m_viewImpl->EraseScale(level, type);
}

/**
*
*/
inline bool IView::Scr2Map(const CGeoPoint<short> &scrPoint, CGeoPoint<long> &mapPoint, short type)
{
	return m_viewImpl->Scr2Map(scrPoint, mapPoint, type);
}

/**
*
*/
inline bool IView::Map2Scr(CGeoPoint<long> &mapPoint, CGeoPoint<short> &scrPoint, short type)
{
	return m_viewImpl->Map2Scr(mapPoint, scrPoint, type);
}

/**
*
**/
inline bool IView::GetScreenCenter(CGeoPoint<long> &mapPt)
{
	return m_viewImpl->GetScreenCenter(mapPt);
}

/**
* 
*/
inline void IView::RenderSidePicture(bool isDraw, short picCode /* = -1 */, short picType /* = -1 */, short arrowCode/*, void *wndHandle*/ /* = 0 */)
{
	return m_viewImpl->RenderSidePicture(isDraw, picCode, picType, arrowCode/*, wndHandle*/);
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for different view states' layout
//
/**
*
*/
inline void IView::NextState()
{
	return m_viewImpl->NextState();
}

/**
*
**/
inline void IView::SetState(short type)
{
	return m_viewImpl->SetState(type);
}

/**
*
*/
inline CViewState *IView::GetState(short type)
{
	return m_viewImpl->GetState(type);
}

/**
*
**/
inline void IView::EraseState(short type)
{
	return m_viewImpl->EraseState(type);
}

/**
*
*/
inline CViewState *IView::MainState()
{
	return m_viewImpl->MainState();
}

/**
*
**/
inline bool IView::FocusOn(short type)
{
	return m_viewImpl->FocusOn(type);
}

/**
*
**/
inline unsigned int IView::FocusOn(const CGeoPoint<short> &scrPoint)
{
	return m_viewImpl->FocusOn(scrPoint);
}

/**
*
**/
inline void IView::SetViewPort(CViewState *curView, const MapLayout &mapLayout, short scaleLevel, short schema)
{
	m_viewImpl->SetViewPort(curView, mapLayout, scaleLevel, schema);
}

/**
*
*/
inline short IView::GetLayoutSchema()
{
	return m_viewImpl->GetLayoutSchema();
}

/**
*
*/
inline void IView::SetLayoutSchema(short schema)
{
	return m_viewImpl->SetLayoutSchema(schema);
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for map operations
//
/**
*
*/
inline void IView::SwitchTo(short scaleLevel, double angle, short type)
{
	return m_viewImpl->SwitchTo(scaleLevel, angle, type);
}

/**
*
*/
inline void IView::SwitchTo(const CGeoRect<long> &extent)
{
	return m_viewImpl->SwitchTo(extent);
}

/**
*
*/
inline void IView::ZoomIn()
{
	return m_viewImpl->ZoomIn();
}

/**
*
*/
inline void IView::ZoomOut()
{
	return m_viewImpl->ZoomOut();
}

/**
*
**/
inline void IView::Rotate(double step, bool isDirect)
{
	return m_viewImpl->Rotate(step, isDirect);
}

/**
*
**/
inline void IView::Rotating(double step, double from, double to)
{
	return m_viewImpl->Rotating(step, from, to);
}

/**
*
**/
inline void IView::Zooming(bool isZoomIn)
{
	return m_viewImpl->Zooming(isZoomIn);
}

/**
*
**/
inline void IView::Zooming(short start, short end, short step)
{
	return m_viewImpl->Zooming(start, end, step);
}

/**
*
**/
inline void IView::Transforming(double diff, bool isOnce)
{
	return m_viewImpl->Transforming(diff, isOnce);
}

/**
*
**/
inline void IView::Transforming(double diff, double to)
{
	return m_viewImpl->Transforming(diff, to);
}

/**
*
*/
inline bool IView::Pick(const CGeoPoint<short> &scrPoint, CMemVector &objects, bool isRendered)
{
	return m_viewImpl->Pick(scrPoint, objects, isRendered);
}

/**
*
*/
inline void IView::RenderSelected()
{
	return m_viewImpl->RenderSelected();
}

/**
*
**/
inline void IView::GetPickPos(CGeoPoint<long> &pickPos)
{
	return m_viewImpl->GetPickPos(pickPos);
}
inline void IView::GetPickName(char *pickName)
{
  return m_viewImpl->GetPickName(pickName);
}
/**
*
**/
inline void IView::SetPickPos(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint)
{
	return m_viewImpl->SetPickPos(pickPos, scrPoint);
}
/**
*
**/
inline void IView::SetPickPos_NoRefresh(const CGeoPoint<long> &pickPos, CGeoPoint<short> &scrPoint)
{
	return m_viewImpl->SetPickPos_NoRefresh(pickPos, scrPoint);
}
/**
*
**/
inline CViewCommand* IView::GetCommand(short type)
{
	return m_viewImpl->GetCommand(type);
}

/**
*
**/
inline const GpsCar &IView::GetGpsCar()
{
	return m_viewImpl->GetGpsCar();
}

/**
*
**/
inline void IView::SetGpsCar(const GpsCar &gpsCar)
{
	return m_viewImpl->SetGpsCar(gpsCar);
}

const GpsCar & UeMap::IView::GetGpsPosInfo()
{
  return m_viewImpl->GetGpsPosInfo();
}

void UeMap::IView::SetGpsPosInfo( const GpsCar &gpsInfo )
{
  return m_viewImpl->SetGpsPosInfo(gpsInfo);
} 

//////////////////////////////////////////////////////////////////////////////////
// Standard methods for OS' APIs
//
/**
*
*/
inline bool IView::MouseDown(const CGeoPoint<short> &scrPoint)
{
	return m_viewImpl->MouseDown(scrPoint);
}

/**
*
*/
inline bool IView::MouseMove(const CGeoPoint<short> &scrPoint)
{
	return m_viewImpl->MouseMove(scrPoint);
}

/**
* Note: Return value to recognize the command of pan and select
*/
inline bool IView::MouseUp(const CGeoPoint<short> &scrPoint)
{
	return m_viewImpl->MouseUp(scrPoint);
}

/**
*
*/
inline bool IView::UpFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end)
{
	return m_viewImpl->UpFlash(start, end);
}

/**
*
*/
inline bool IView::PanFlash(const CGeoPoint<short> &start, const CGeoPoint<short> &end)
{
	return m_viewImpl->PanFlash(start, end);
}

/**
*
*/
inline void IView::OnPaint()
{
	return m_viewImpl->OnPaint();
}

/**
*
*/
inline void IView::OnIdle()
{
	return m_viewImpl->OnIdle();
}

/**
*
**/
inline void IView::SetIdleOP(unsigned int operations)
{
	return m_viewImpl->SetIdleOP(operations);
}

/**
*
**/
inline unsigned int IView::GetIdleOP()
{
	return m_viewImpl->GetIdleOP();
}

/**
* Deprecated function, here it only give outside warning that u shouldn't igonre this message
*/
inline void IView::EraseBkGnd()
{
	return m_viewImpl->EraseBkGnd();
}

/**
*
**/
inline void IView::IsReadyForOperation(bool isReady)
{
	return m_viewImpl->IsReadyForOperation(isReady);
}

/**
*
*/
inline void IView::Refresh()
{
	return m_viewImpl->Refresh();
}

inline void IView::RefreshUI(short type)
{
  return m_viewImpl->RefreshUI(type);
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for data organization
//
/**
*
*/
inline unsigned long IView::GetMapSchema()
{
	return m_viewImpl->GetMapSchema();
}

/**
*
*/
inline void IView::SetMapSchema(unsigned long schema)
{
	return m_viewImpl->SetMapSchema(schema);
}

/**
* TODO: Here still adopt Citus's map configure file, namely, "map.ini"
* and also not forget to remove all magic numbers
*/
inline bool IView::LoadMap(unsigned long schema, const tstring &file, short scaleLevel, double angle)
{
	return m_viewImpl->LoadMap(schema, file, scaleLevel, angle);
}

/**
*
*/
inline bool IView::AddLayer(int scale, std::vector<CViewLayer *> &oneLayer)
{
	return m_viewImpl->AddLayer(scale, oneLayer);
}

/**
*
*/
inline bool IView::AddLayer(int scale, CViewLayer *oneLayer)
{
	return m_viewImpl->AddLayer(scale, oneLayer);
}

/**
*
*/
inline bool IView::RemoveLayer(int scale)
{
	return m_viewImpl->RemoveLayer(scale);
}

/**
*
*/
inline bool IView::RemoveLayer(int scale, const tstring &layerName)
{
	return m_viewImpl->RemoveLayer(scale, layerName);
}

/**
*
**/
inline CViewLayer *IView::GetLayer(int scale, int type)
{
	return m_viewImpl->GetLayer(scale, type);
}

/**
* 
*/
inline void IView::SortLayer()
{
	return m_viewImpl->SortLayer();
}

/**
*
**/
inline bool IView::SetUePicture(short type, CUePicture *navPicture)
{
	return m_viewImpl->SetUePicture(type, navPicture);
}

/**
*
**/
inline const CUePicture *IView::GetUePicture(short type)
{
	return m_viewImpl->GetUePicture(type);
}

/**
* 
*/
inline void IView::SetPlanLogInfo(tstring &version, long sampleID)
{
	m_viewImpl->SetPlanLogInfo(version, sampleID);
}

/**
* 
*/
inline void IView::GetPlanLogInfo(tstring &version, long &sampleID)
{
	m_viewImpl->GetPlanLogInfo(version, sampleID);
}

/**
* 
*/
inline bool IView::IsDrawPlanLayer()
{
	return m_viewImpl->IsDrawPlanLayer();
}

/**
* 
*/
inline void IView::SetDrawPlanLayer(bool isDraw)
{
	m_viewImpl->SetDrawPlanLayer(isDraw);
}

/**
* 
*/
inline void IView::SetRenderSetting(const RenderSettings &settings)
{
	m_viewImpl->SetRenderSetting(settings);
}

/**
* 
*/
inline void IView::GetRenderSetting(RenderSettings &settings)
{
	m_viewImpl->GetRenderSetting(settings);
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for letting outside have chance to extend view's functionalities
//
/**
*
*/
inline bool IView::AddDC(short type, CViewDC *oneDC)
{
	return m_viewImpl->AddDC(type, oneDC);
}

/**
*
*/
inline CViewDC *IView::GetDC(unsigned int type)
{
	return m_viewImpl->GetDC(type);
}

/**
*
**/
inline void IView::EraseDC(unsigned int type)
{
	m_viewImpl->EraseDC(type);
}

/**
*
*/
inline bool IView::AddHook(short type, CViewHook *oneHook)
{
	return m_viewImpl->AddHook(type, oneHook);
}

/**
*
*/
CViewHook *IView::GetHook(short type)
{
	return m_viewImpl->GetHook(type);
}

//////////////////////////////////////////////////////////////////////////////////
// Methods for working with other collegues
//
/**
*
*/
inline void IView::Update(short type, void *para)
{
	return m_viewImpl->Update(type);
}

/**
* 
**/
inline void IView::RegisterGarbage(CMemoryGarbage *garbage)
{
	//
	assert(garbage);
	m_garbage = garbage;
}

/**
*
**/
inline CMemoryGarbage *IView::GetGarbage()
{
	//
	assert(m_garbage);
	return m_garbage;
}

/**
* 
**/
inline void IView::RegisterMediator(CMediator *mediator)
{
	assert(mediator && !m_mediator);
	m_mediator = mediator;
}

/**
*
**/
inline CMediator *IView::GetMediator()
{
	assert(m_mediator);
	return m_mediator;
}

/**
* 
*/
inline void *IView::GetWnd()
{
	return reinterpret_cast<HWND>(m_viewImpl->m_wnd);
}

/**
* \brief 地图中心点是否已经被移动
*/
bool IView::HasMovedMapCenter()
{
  return m_viewImpl->HasMovedMapCenter();
}
/**
* \brief 设置地图中心点被移动标志
*/
void IView::SetMapCenterMoveFlag(bool hasMoved)
{
  m_viewImpl->SetMapCenterMoveFlag(hasMoved);
}

void UeMap::IView::UpdateProgress( short step )
{
  m_viewImpl->UpdateProgress(step);
}

void UeMap::IView::SetCarIcon( short carIcon )
{
  m_viewImpl->SetCarIcon(carIcon);
}

void UeMap::IView::Set3DCarIcon(unsigned short carIcon )
{
  m_viewImpl->Set3DCarIcon(carIcon);
}


short UeMap::IView::GetCarIcon()
{
  return m_viewImpl->GetCarIcon();
}

unsigned short UeMap::IView::Get3DCarIcon()
{
  return m_viewImpl->Get3DCarIcon();
}


void UeMap::IView::SetCompassIconPos( const CGeoPoint<short> &scrPoint )
{
  m_viewImpl->SetCompassIconPos(scrPoint);
}

void UeMap::IView::GetCompassIconPos( CGeoPoint<short> &scrPoint )
{
  m_viewImpl->GetCompassIconPos(scrPoint);
}

void UeMap::IView::SetLanePos( CGeoPoint<short> &scrPoint, short width, short height )
{
  m_viewImpl->SetLanePos(scrPoint, width, height);
}

void UeMap::IView::GetLanePos( CGeoPoint<short> &scrPoint, short &width, short &height )
{
  m_viewImpl->GetLanePos(scrPoint, width, height);
}

bool UeMap::IView::IsNeedRenderGuidanceView()
{
  return m_viewImpl->IsNeedRenderGuidanceView();
}

bool UeMap::IView::IsGuidanceViewShown()
{
  return m_viewImpl->IsGuidanceViewShown();
}

void UeMap::IView::CloseGuidanceView()
{
  m_viewImpl->CloseGuidanceView();
}

void UeMap::IView::ShowGuidanceView()
{
  m_viewImpl->ShowGuidanceView();
}

unsigned int UeMap::IView::GetSelectedViewType( const CGeoPoint<short> &scrPoint )
{
  return m_viewImpl->GetSelectedViewType(scrPoint);
}

bool UeMap::IView::IsMapLayoutChange()
{
  return m_viewImpl->IsMapLayoutChange();
}

void UeMap::IView::SetViewOpeMode( ViewOpeMode opeMode, ViewType type /*= VT_Unknown*/ )
{
  m_viewImpl->SetViewOpeMode(opeMode, type);
}

UeMap::ViewOpeMode UeMap::IView::GetViewOpeMode( ViewType type /*= VT_Unknown*/ )
{
  return m_viewImpl->GetViewOpeMode(type);
}

bool UeMap::IView::AddViewIcon( ViewIconType iconType, short icon )
{
  return m_viewImpl->AddViewIcon(iconType, icon);
}

short UeMap::IView::GetViewIcon( ViewIconType iconType )
{
  return m_viewImpl->GetViewIcon(iconType);
}

bool UeMap::IView::AutoScallingMap(bool isLock)
{
  return m_viewImpl->AutoScallingMap(isLock);
}

void UeMap::IView::UnLockScalling()
{
  m_viewImpl->UnLockScalling();
}