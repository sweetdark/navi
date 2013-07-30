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
#include "viewhook.h"
#include "viewstate.h"
#include "viewlayer.h"
using namespace UeMap;

// Static variable initialization
CViewHook::GuiName CViewHook::m_imageNames;
unsigned int CViewHook::m_curHookType = DHT_Inner + 1;
unsigned int CViewHook::m_prevHookType = DHT_Unknown;
CGeoRect<short> CViewHook::m_hookExtent;
bool CViewHook::m_isMouseDown = false;
bool CViewHook::m_isMouseUp  = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/
CViewHook::CViewHook() : m_pathBasic(CPathBasic::Get()), m_stringBasic(CStringBasic::Get()), m_fileBasic(CFileBasic::Get()),
		m_view(0), m_query(0), m_gps(0), m_route(0), m_districtLayer(0), m_isGUILoadDone(false)
{
	// TODO:
	// Based on automatically code generation then get below map
	if(m_imageNames.empty())
	{
		m_imageNames.insert(GuiName::value_type(IT_StartFlag,					"StartFlagImage"));
		m_imageNames.insert(GuiName::value_type(IT_EndFlag,					"EndFlagImage"));
		m_imageNames.insert(GuiName::value_type(IT_WayFlag,					"WayFlagImage"));
		m_imageNames.insert(GuiName::value_type(IT_POI,							"POIImage"));
		m_imageNames.insert(GuiName::value_type(IT_ZoomIn,					"ZoomInImage"));
		m_imageNames.insert(GuiName::value_type(IT_ZoomOut,					"ZoomOutImage"));
		m_imageNames.insert(GuiName::value_type(IT_HeadUp,					"HeadUpImage"));
		m_imageNames.insert(GuiName::value_type(IT_HeadDown,				"HeadDownImage"));
		m_imageNames.insert(GuiName::value_type(IT_Compass,					"CompassImage"));
		m_imageNames.insert(GuiName::value_type(IT_Sound,						"SoundImage"));
		m_imageNames.insert(GuiName::value_type(IT_GPS,						"GPSImage"));
		m_imageNames.insert(GuiName::value_type(IT_Setting,					"SettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainMap,					"MainMapImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainQuery,				"MainQueryImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainPOI,					"MainPOIImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainAddr,				"MainAddrImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainCross,				"MainCrossImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainFavorite,			"MainFavoriteImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainRecent,				"MainRecentImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainSetting,				"MainSettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_MainExit,					"MainExitImage"));
		m_imageNames.insert(GuiName::value_type(IT_PageDown,				"PageDownImage"));
		m_imageNames.insert(GuiName::value_type(IT_PageUp,					"PageUpImage"));
		m_imageNames.insert(GuiName::value_type(IT_DemoStop,				"DemoStopImage"));
		m_imageNames.insert(GuiName::value_type(IT_DemoResume,			"DemoResumeImage"));
		m_imageNames.insert(GuiName::value_type(IT_DemoPause,				"DemoPauseImage"));
		m_imageNames.insert(GuiName::value_type(IT_DemoStart,				"DemoStartImage"));
		m_imageNames.insert(GuiName::value_type(IT_DistDown,				"DistDownImage"));
		m_imageNames.insert(GuiName::value_type(IT_DistUp,						"DistUpImage"));
		m_imageNames.insert(GuiName::value_type(IT_DistOrder,				"DistOrderImage"));
		m_imageNames.insert(GuiName::value_type(IT_CharOrder,				"CharOrderImage"));
		m_imageNames.insert(GuiName::value_type(IT_CharDown,				"CharDownImage"));
		m_imageNames.insert(GuiName::value_type(IT_CharUp,					"CharUpImage"));
		m_imageNames.insert(GuiName::value_type(IT_Previous,					"PreviousImage"));
		m_imageNames.insert(GuiName::value_type(IT_EraseKey,					"EraseKeyImage"));
		m_imageNames.insert(GuiName::value_type(IT_RouteBlock,				"RouteBlockImage"));
		m_imageNames.insert(GuiName::value_type(IT_Small_Turn,				"SmallTurnImage"));
		m_imageNames.insert(GuiName::value_type(IT_Middle_Turn,			"MiddleTurnImage"));
		m_imageNames.insert(GuiName::value_type(IT_ViewSetting,				"ViewSettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_RouteSetting,			"RouteSettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_QuerySetting,			"QuerySettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_VoiceSetting,			"VoiceSettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_SystemSetting,			"SystemSettingImage"));
		m_imageNames.insert(GuiName::value_type(IT_PortraitBtn_Fore,		"PortraitBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_PortraitBtn_Back,		"PortraitBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_MapBtn,					"MapBtnImage"));
		m_imageNames.insert(GuiName::value_type(IT_SquareBtn_Fore,		"SquareBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_SquareBtn_Back,		"SquareBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_LandBtn_Fore,			"LandBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_LandBtn_Back,			"LandBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_ListBtn_Fore,			"ListBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_ListBtn_Back,			"ListBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_Vehicle_Car,				"VehicleCarImage"));
		m_imageNames.insert(GuiName::value_type(IT_Vehicle_Truck,			"VehicleTruckImage"));
		m_imageNames.insert(GuiName::value_type(IT_Vehicle_Pedistrian,	"VehiclePedistrianImage"));
		m_imageNames.insert(GuiName::value_type(IT_North_Map,				"NorthMapImage"));
		m_imageNames.insert(GuiName::value_type(IT_Head_Map,				"HeadMapImage"));
		m_imageNames.insert(GuiName::value_type(IT_25D_Map,				"25DMapImage"));
		m_imageNames.insert(GuiName::value_type(IT_Auto_Color,				"AutoColorImage"));
		m_imageNames.insert(GuiName::value_type(IT_Day_Color,				"DayColorImage"));
		m_imageNames.insert(GuiName::value_type(IT_Night_Color,				"NightColorImage"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon01,  "ToolIcon01"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon02,  "ToolIcon02"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon03,  "ToolIcon03"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon04,  "ToolIcon04"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon05,  "ToolIcon05"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon06,  "ToolIcon06"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon07,  "ToolIcon07"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon08,  "ToolIcon08"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon09,  "ToolIcon09"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon10,  "ToolIcon10"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon11,  "ToolIcon11"));
		m_imageNames.insert(GuiName::value_type(IT_ToolIcon12,  "ToolIcon12"));
		m_imageNames.insert(GuiName::value_type(IT_SquareBtn_Middle_Fore,		"MiddleSqureBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_SquareBtn_Middle_Back,		"MiddleSqureBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_SquareBtn_Small_Fore,			"SmallSqureBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_SquareBtn_Small_Back,			"SmallSqureBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_KeyBtn_Fore,		"KeyBtnForeImage"));
		m_imageNames.insert(GuiName::value_type(IT_KeyBtn_Back,		"KeyBtnBackImage"));
		m_imageNames.insert(GuiName::value_type(IT_RES76,				"Res76Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES77,				"Res77Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES78,				"Res78Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES79,				"Res79Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES80,				"Res80Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES81,				"Res81Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES82,				"Res82Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES83,				"Res83Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES84,				"Res84Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES85,				"Res85Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES86,				"Res86Image"));
		m_imageNames.insert(GuiName::value_type(IT_RES87,				"Res87Image"));
		m_imageNames.insert(GuiName::value_type(IT_GLOBE,				"GlobeImage"));
		m_imageNames.insert(GuiName::value_type(IT_Declare,				"DeclareImage"));
		m_imageNames.insert(GuiName::value_type(IT_Back,				"BackImage"));
    m_imageNames.insert(GuiName::value_type(IT_CheckIcon,				"CheckIconImage"));
    m_imageNames.insert(GuiName::value_type(IT_CheckBack,				"CheckBackImage"));
    m_imageNames.insert(GuiName::value_type(IT_RadioIcon,				"RadioIconImage"));
    m_imageNames.insert(GuiName::value_type(IT_RadioBack,				"RadioBackImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnBack,				"BtnBackImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnSel,				"BtnSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnGray,				"BtnGrayImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnLeft,				"BtnLeftImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnRight,				"BtnRightImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnLeftSel,			"BtnLeftSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnRightSel,			"BtnRightSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnLeftGray,			"BtnLeftGrayImage"));
    m_imageNames.insert(GuiName::value_type(IT_BtnRightGray,			"BtnRightGrayImage"));
    m_imageNames.insert(GuiName::value_type(IT_ReturnMapIcon,			"ReturnMapIconImage"));
    m_imageNames.insert(GuiName::value_type(IT_ReturnMapIconGray,		 "ReturnMapIconGrayImage"));
    m_imageNames.insert(GuiName::value_type(IT_ReturnIcon,			"ReturnIconImage"));
    m_imageNames.insert(GuiName::value_type(IT_ReturnIconGray,			"ReturnIconGrayImage"));
    m_imageNames.insert(GuiName::value_type(IT_SquareBack,			"SquareBackImage"));
    m_imageNames.insert(GuiName::value_type(IT_SquareBackSel,			"SquareBackSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_Scale,			"ScaleImage"));
    m_imageNames.insert(GuiName::value_type(IT_ScaleSel,			"ScaleSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_ScaleBack,			"ScaleBackImage"));
    m_imageNames.insert(GuiName::value_type(IT_MapText,			"MapTextImage"));
    m_imageNames.insert(GuiName::value_type(IT_Biao,			"BiaoPan"));
    m_imageNames.insert(GuiName::value_type(IT_IntervalV,			"IntervalVImage"));
    m_imageNames.insert(GuiName::value_type(IT_IntervalH,			"IntervalHImage"));
    m_imageNames.insert(GuiName::value_type(IT_NorthSymbolBack,			"NorthSymbolBackImage"));
    m_imageNames.insert(GuiName::value_type(IT_NorthSymbol,			"NorthSymbolImage"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon01,			"MapIcon01Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon02,			"MapIcon02Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon03,			"MapIcon03Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon04,			"MapIcon04Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon05,			"MapIcon05Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon06,			"MapIcon06Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon07,			"MapIcon07Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon08,			"MapIcon08Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon09,			"MapIcon09Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon10,			"MapIcon10Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon11,			"MapIcon11Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon12,			"MapIcon12Image"));
    m_imageNames.insert(GuiName::value_type(IT_MapIcon13,			"MapIcon13Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite01,			"Satellite01Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite02,			"Satellite02Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite03,			"Satellite03Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite04,			"Satellite04Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite05,			"Satellite05Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite06,			"Satellite06Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite07,			"Satellite07Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite08,			"Satellite08Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite09,			"Satellite09Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite10,			"Satellite10Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite11,			"Satellite11Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite12,			"Satellite12Image"));
    m_imageNames.insert(GuiName::value_type(IT_Satellite13,			"Satellite13Image"));
    m_imageNames.insert(GuiName::value_type(IT_ECity,			"ECityImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuCurrent,			"MenuCurrentRoutImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuCurrentRoutGray,			"MenuCurrentRoutGrayImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuCurrentRoutSel,			"MenuCurrentRoutSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuDaoDaoTong,			"MenuDaoDaoTongImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuDaoDaoTongSel,			"MenuDaoDaoTongSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuFind,			"MenuFindImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuFindSel,			"MenuFindSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuMyInfor,			"MenuMyInforImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuMyInforSel,			"MenuMyInforSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuNaviSet,			"MenuNaviSetImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuNaviSetSel,			"MenuNaviSetSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuSystemSet,			"MenuSystemSetImage"));
    m_imageNames.insert(GuiName::value_type(IT_MenuSystemSetSel,			"MenuSystemSetSelImage"));
    m_imageNames.insert(GuiName::value_type(IT_Res159Image,			"Res159Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res160Image,			"Res160Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res161Image,			"Res161Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res162Image,			"Res162Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res163Image,			"Res163Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res164Image,			"Res164Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res165Image,			"Res165Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res166Image,			"Res166Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res167Image,			"Res167Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res168Image,			"Res168Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res169Image,			"Res169Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res170Image,			"Res170Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res171Image,			"Res171Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res172Image,			"Res172Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res173Image,			"Res173Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res174Image,			"Res174Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res175Image,			"Res175Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res176Image,			"Res176Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res177Image,			"Res177Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res178Image,			"Res178Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res179Image,			"Res179Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res180Image,			"Res180Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res181Image,			"Res181Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res182Image,			"Res182Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res183Image,			"Res183Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res184Image,			"Res184Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res185Image,			"Res185Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res186Image,			"Res186Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res187Image,			"Res187Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res188Image,			"Res188Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res189Image,			"Res189Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res190Image,			"Res190Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res191Image,			"Res191Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res192Image,			"Res192Image"));
    m_imageNames.insert(GuiName::value_type(IT_Res193Image,			"Res193Image"));
		m_imageNames.insert(GuiName::value_type(IT_TrafficSign,  "TrafficSignImage"));
		m_imageNames.insert(GuiName::value_type(IT_VirtualCross, "VirtualCrossImage"));
		m_imageNames.insert(GuiName::value_type(IT_RealCross,    "RealCrossImage"));
		m_imageNames.insert(GuiName::value_type(IT_Arrow, "ArrowImage"));
		m_imageNames.insert(GuiName::value_type(IT_LandMark,     "LandMarkImage"));
	}
}

/**
*
*/
CViewHook::~CViewHook()
{
	m_imageNames.erase(m_imageNames.begin(), m_imageNames.end());
}

/**
*
**/
inline int CViewHook::GetDistrictIdx()
{
	//
	if(!m_districtLayer)
	{
		m_districtLayer = m_view->GetLayer(1, LT_Index);
	}
	assert(m_districtLayer);

	if(!m_districtLayer)
	{
		return -1;
	}

	//
	const ScreenLayout &scrLayout = m_view->MainState()->GetScrLayout();
	CGeoPoint<short> scrPt;
	scrPt.m_x = scrLayout.m_extent.m_minX + scrLayout.m_extent.Width()/2;
	scrPt.m_y = scrLayout.m_extent.m_minY + scrLayout.m_extent.Height()/2;
	CGeoPoint<long> mapPt;
	m_view->MainState()->Scr2Map(scrPt, mapPt);

	//
	return m_districtLayer->GetDistrictIdx(mapPt);
}

/**
*
**/
inline short CViewHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short type = CT_Unknown;
  gui_itr guiFirst = m_elements.begin();
  gui_itr guiEnd = m_elements.end();
  for(; guiFirst != guiEnd; guiFirst++)
  {
    // Simple analysis
    CGeoRect<short> oneRect = (*guiFirst).second.m_extent;
    oneRect.m_minX-=5;
    oneRect.m_maxX += 5;
    oneRect.m_minY -= 5;
    oneRect.m_maxY += 5;

    if(((*guiFirst).second.m_opType != OT_Unknown) && 
    oneRect.IsContain(scrPoint))
    {
      // The switch rule differenciating different view modes
      type = (*guiFirst).first;
      break;
    }
  } 
	return type;
}

/**
*
*/
inline short CViewHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short type = CT_Unknown;
  gui_itr guiFirst = m_elements.begin();
  gui_itr guiEnd = m_elements.end();
  for(; guiFirst != guiEnd; guiFirst++)
  {
    // Simple analysis
    CGeoRect<short> oneRect = (*guiFirst).second.m_extent;
    oneRect.m_minX-=5;
    oneRect.m_maxX += 5;
    oneRect.m_minY -= 5;
    oneRect.m_maxY += 5;

    if(((*guiFirst).second.m_opType != OT_Unknown) && 
      oneRect.IsContain(scrPoint))
    {
      // The switch rule differenciating different view modes
      type = (*guiFirst).first;
      break;
    }
  } 
  return type;
	//return -1;
}

void UeMap::CViewHook::LoadGUI()
{
  if (!m_isGUILoadDone)
  {
    MakeGUI();
    Init();
    m_isGUILoadDone = true;
  }
}

bool UeMap::CViewHook::IsGUILoadDone()
{
  return m_isGUILoadDone;
}