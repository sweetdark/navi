#include "adjustroutehook.h"
#include "viewwrapper.h"
#include "routewrapper.h"
#include "messagedialoghook.h"
#include "maphook.h"
#include "querywrapper.h"
#include "routetypeswitchhook.h"
#include "inputswitchhook.h"
#include "selectpointcallbackctrl.h"
using namespace UeGui;

#define ADDPOIMACRO(num) do                 \
{                                           \
  if (IsNeedRefresh(m_addPOI##num##Ctrl))   \
  {                                         \
    m_addPOI##num##Ctrl.MouseUp();          \
    DoEditOrAddPoi(kROW##num##);            \
  }                                         \
} while (0)

#define MOUSEDOWN_REFRESH(ctrl) do            \
{                                           \
  if (IsNeedRefresh(ctrl))                  \
  {                                         \
    ctrl.MouseDown();                       \
  }                                         \
} while (0)

CAdjustRouteHook::CAdjustRouteHook() : m_selectRowTag(kROWBegin) , m_dataFrom(kFromRoute)
{
  m_strTitle = "编辑路线";
  m_vecHookFile.push_back(_T("adjustroutehook.bin"));
}

CAdjustRouteHook::~CAdjustRouteHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CAdjustRouteHook::Init()
{
  ClearAllRow();
}


void CAdjustRouteHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI1Btn,	"POI1Btn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI1BtnIcon,	"POI1BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI1Name,	"POI1Name"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI2Btn,	"POI2Btn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI2BtnIcon,	"POI2BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI2Name,	"POI2Name"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI3Btn,	"POI3Btn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI3BtnIcon,	"POI3BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI3Name,	"POI3Name"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI4Btn,	"POI4Btn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI4BtnIcon,	"POI4BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI4Name,	"POI4Name"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI5Btn,	"POI5Btn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI5BtnIcon,	"POI5BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI5Name,	"POI5Name"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI6Btn,	"POI6Btn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI6BtnIcon,	"POI6BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI6Name,	"POI6Name"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI1,	"AddPOI1"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI1Icon,	"AddPOI1Icon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI2,	"AddPOI2"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI2Icon,	"AddPOI2Icon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI3,	"AddPOI3"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI3Icon,	"AddPOI3Icon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI4,	"AddPOI4"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI4Icon,	"AddPOI4Icon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI5,	"AddPOI5"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI5Icon,	"AddPOI5Icon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI6,	"AddPOI6"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOI6Icon,	"AddPOI6Icon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI1Gps,	"POI1Gps"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI1GpsIcon,	"POI1GpsIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI2Delete,	"POI2Delete"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI2DeleteIcon,	"POI2DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI3DeleteIcon,	"POI3DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI3Delete,	"POI3Delete"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI4Delete,	"POI4Delete"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI4DeleteIcon,	"POI4DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI5Delete,	"POI5Delete"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI5DeleteIcon,	"POI5DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI6Delete,	"POI6Delete"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_POI6DeleteIcon,	"POI6DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_StartGuidanceBtn,	"StartGuidanceBtn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_BottomBack,	"BottomBack"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_DemoGuidanceBtn,	"DemoGuidanceBtn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_HSeparator,	"HSeparator"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_Separator1,	"Separator1"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_Separator2,	"Separator2"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_Separator3,	"Separator3"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_Separator4,	"Separator4"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_SaveLineBtn,	"SaveLineBtn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_Separator5,	"Separator5"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_Separator6,	"Separator6"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_RouteTypeBtn,	"RouteTypeBtn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_RouteTypeLabel,	"RouteTypeLabel"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_RouteTypeBtnIcon,	"RouteTypeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_PlainLineBtn,	"PlainLineBtn"));
  m_ctrlNames.insert(GuiName::value_type(adjustroutehook_AddPOIIconBak,	"AddPOIIconBak"));
}

void CAdjustRouteHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_addPOI1Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOI1));
  m_addPOI1Ctrl.SetIconElement(GetGuiElement(adjustroutehook_AddPOI1Icon));
  m_addPOI2Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOI2));
  m_addPOI2Ctrl.SetIconElement(GetGuiElement(adjustroutehook_AddPOI2Icon));
  m_addPOI3Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOI3));
  m_addPOI3Ctrl.SetIconElement(GetGuiElement(adjustroutehook_AddPOI3Icon));
  m_addPOI4Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOI4));
  m_addPOI4Ctrl.SetIconElement(GetGuiElement(adjustroutehook_AddPOI4Icon));
  m_addPOI5Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOI5));
  m_addPOI5Ctrl.SetIconElement(GetGuiElement(adjustroutehook_AddPOI5Icon));
  m_addPOI6Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOI6));
  m_addPOI6Ctrl.SetIconElement(GetGuiElement(adjustroutehook_AddPOI6Icon));
  m_addPOIIconBakCtrl.SetCenterElement(GetGuiElement(adjustroutehook_AddPOIIconBak));
  m_bottomBackCtrl.SetCenterElement(GetGuiElement(adjustroutehook_BottomBack));
  m_demoGuidanceBtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_DemoGuidanceBtn));
  m_hSeparatorCtrl.SetCenterElement(GetGuiElement(adjustroutehook_HSeparator));
  m_pOI1BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI1Btn));
  m_pOI1BtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI1BtnIcon));
  m_pOI1BtnCtrl.SetLabelElement(GetGuiElement(adjustroutehook_POI1Name));

  m_pOI1GpsCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI1Gps));
  m_pOI1GpsCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI1GpsIcon));
  //m_pOI1NameCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI1Name));
  m_pOI2BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI2Btn));
  m_pOI2BtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI2BtnIcon));
  m_pOI2BtnCtrl.SetLabelElement(GetGuiElement(adjustroutehook_POI2Name));

  m_pOI2DeleteCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI2Delete));
  m_pOI2DeleteCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI2DeleteIcon));
  //m_pOI2NameCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI2Name));
  m_pOI3BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI3Btn));
  m_pOI3BtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI3BtnIcon));
  m_pOI3BtnCtrl.SetLabelElement(GetGuiElement(adjustroutehook_POI3Name));

  m_pOI3DeleteCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI3Delete));
  m_pOI3DeleteCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI3DeleteIcon));
  //m_pOI3NameCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI3Name));
  m_pOI4BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI4Btn));
  m_pOI4BtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI4BtnIcon));
  m_pOI4BtnCtrl.SetLabelElement(GetGuiElement(adjustroutehook_POI4Name));

  m_pOI4DeleteCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI4Delete));
  m_pOI4DeleteCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI4DeleteIcon));
  //m_pOI4NameCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI4Name));
  m_pOI5BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI5Btn));
  m_pOI5BtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI5BtnIcon));
  m_pOI5BtnCtrl.SetLabelElement(GetGuiElement(adjustroutehook_POI5Name));

  m_pOI5DeleteCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI5Delete));
  m_pOI5DeleteCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI5DeleteIcon));
  //m_pOI5NameCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI5Name));
  m_pOI6BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI6Btn));
  m_pOI6BtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI6BtnIcon));
  m_pOI6BtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI6Name));

  m_pOI6DeleteCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI6Delete));
  m_pOI6DeleteCtrl.SetIconElement(GetGuiElement(adjustroutehook_POI6DeleteIcon));
  //m_pOI6NameCtrl.SetCenterElement(GetGuiElement(adjustroutehook_POI6Name));
  m_plainLineBtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_PlainLineBtn));
  m_routeTypeBtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_RouteTypeBtn));
  m_routeTypeBtnCtrl.SetIconElement(GetGuiElement(adjustroutehook_RouteTypeBtnIcon));
  m_routeTypeLabelCtrl.SetCenterElement(GetGuiElement(adjustroutehook_RouteTypeLabel));
  m_saveLineBtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_SaveLineBtn));
  m_separator1Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_Separator1));
  m_separator2Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_Separator2));
  m_separator3Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_Separator3));
  m_separator4Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_Separator4));
  m_separator5Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_Separator5));
  m_separator6Ctrl.SetCenterElement(GetGuiElement(adjustroutehook_Separator6));
  m_startGuidanceBtnCtrl.SetCenterElement(GetGuiElement(adjustroutehook_StartGuidanceBtn));
}

void CAdjustRouteHook::Load()
{
  if (GetPrevHookType() != DHT_RouteTypeSwitchHook)
  {
    if (m_dataFrom == kFromRoute)
    {
      GetRouteData();
    }
    InitButtonState();
    m_planMethod = CRouteWrapper::Get().GetMethod();
    SetRouteTypeCaption(m_planMethod);
    ShowRouteData();
  }
}
short CAdjustRouteHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case adjustroutehook_AddPOI1:
  case adjustroutehook_AddPOI1Icon:
    {
      MOUSEDOWN_REFRESH(m_addPOI1Ctrl);
    }
    break;
  case adjustroutehook_AddPOI2:
  case adjustroutehook_AddPOI2Icon:
    {
      MOUSEDOWN_REFRESH(m_addPOI2Ctrl);
    }
    break;
  case adjustroutehook_AddPOI3:
  case adjustroutehook_AddPOI3Icon:
    {
      MOUSEDOWN_REFRESH(m_addPOI3Ctrl);
    }
    break;
  case adjustroutehook_AddPOI4:
  case adjustroutehook_AddPOI4Icon:
    {
      MOUSEDOWN_REFRESH(m_addPOI4Ctrl);
    }
    break;
  case adjustroutehook_AddPOI5:
  case adjustroutehook_AddPOI5Icon:
    {
      MOUSEDOWN_REFRESH(m_addPOI5Ctrl);
    }
    break;
  case adjustroutehook_AddPOI6:
  case adjustroutehook_AddPOI6Icon:
    {
      MOUSEDOWN_REFRESH(m_addPOI6Ctrl);
    }
    break;
  case adjustroutehook_DemoGuidanceBtn:
    {
      MOUSEDOWN_REFRESH(m_demoGuidanceBtnCtrl);
    }
    break;
  case adjustroutehook_POI1Btn:
  case adjustroutehook_POI1BtnIcon:
  case adjustroutehook_POI1Name:
    {
      if (IsNeedRefresh(m_pOI1BtnCtrl))
      {
        m_pOI1BtnCtrl.MouseDown();
        MOUSEDOWN_3RENDERCTRL(m_pOI1BtnCtrl, m_addPOI1Ctrl, m_pOI1GpsCtrl); 
      }
    }
    break;
  case adjustroutehook_POI1Gps:
  case adjustroutehook_POI1GpsIcon:
    {
      MOUSEDOWN_REFRESH(m_pOI1GpsCtrl);
    }
    break;
  case adjustroutehook_POI2Btn:
  case adjustroutehook_POI2BtnIcon:
  case adjustroutehook_POI2Name:
    {
      if (IsNeedRefresh(m_pOI2BtnCtrl))
      {
        m_pOI2BtnCtrl.MouseDown();
        MOUSEDOWN_3RENDERCTRL(m_pOI2BtnCtrl, m_addPOI2Ctrl, m_pOI2DeleteCtrl); 
      }
    }
    break;
  case adjustroutehook_POI2Delete:
  case adjustroutehook_POI2DeleteIcon:
    {
      MOUSEDOWN_REFRESH(m_pOI2DeleteCtrl);
    }
    break;
  case adjustroutehook_POI3Btn:
  case adjustroutehook_POI3BtnIcon:
  case adjustroutehook_POI3Name:
    {
      if (IsNeedRefresh(m_pOI3BtnCtrl))
      {
        m_pOI3BtnCtrl.MouseDown();
        MOUSEDOWN_3RENDERCTRL(m_pOI3BtnCtrl, m_addPOI3Ctrl, m_pOI3DeleteCtrl);
      }
    }
    break;
  case adjustroutehook_POI3Delete:
  case adjustroutehook_POI3DeleteIcon:
    {
      MOUSEDOWN_REFRESH(m_pOI3DeleteCtrl);
    }
    break;
  case adjustroutehook_POI4Btn:
  case adjustroutehook_POI4BtnIcon:
  case adjustroutehook_POI4Name:
    {
      if (IsNeedRefresh(m_pOI4BtnCtrl))
      {
        m_pOI4BtnCtrl.MouseDown();
        MOUSEDOWN_3RENDERCTRL(m_pOI4BtnCtrl, m_addPOI4Ctrl, m_pOI4DeleteCtrl);
      }
    }
    break;
  case adjustroutehook_POI4Delete:
  case adjustroutehook_POI4DeleteIcon:
    {
      MOUSEDOWN_REFRESH(m_pOI4DeleteCtrl);
    }
    break;
  case adjustroutehook_POI5Btn:
  case adjustroutehook_POI5BtnIcon:
  case adjustroutehook_POI5Name:
    {
      if (IsNeedRefresh(m_pOI5BtnCtrl))
      {
        m_pOI5BtnCtrl.MouseDown();
        MOUSEDOWN_3RENDERCTRL(m_pOI5BtnCtrl, m_addPOI5Ctrl, m_pOI5DeleteCtrl);
      }
    }
    break;
  case adjustroutehook_POI5Delete:
  case adjustroutehook_POI5DeleteIcon:
    {
      MOUSEDOWN_REFRESH(m_pOI5DeleteCtrl);
    }
    break;
  case adjustroutehook_POI6Btn:
  case adjustroutehook_POI6BtnIcon:
  case adjustroutehook_POI6Name:
    {
      if (IsNeedRefresh(m_pOI6BtnCtrl))
      {
        m_pOI6BtnCtrl.MouseDown();
        MOUSEDOWN_3RENDERCTRL(m_pOI6BtnCtrl, m_addPOI6Ctrl, m_pOI6DeleteCtrl);
      }
    }
    break;
  case adjustroutehook_POI6Delete:
  case adjustroutehook_POI6DeleteIcon:
    {
      MOUSEDOWN_REFRESH(m_pOI6DeleteCtrl);
    }
    break;
  case adjustroutehook_PlainLineBtn:
    {
      MOUSEDOWN_REFRESH(m_plainLineBtnCtrl);
    }
    break;
  case adjustroutehook_RouteTypeBtn:
  case adjustroutehook_RouteTypeBtnIcon:
    {
      MOUSEDOWN_REFRESH(m_routeTypeBtnCtrl);
    }
    break;
  case adjustroutehook_RouteTypeLabel:
    {
      MOUSEDOWN_REFRESH(m_routeTypeLabelCtrl);
    }
    break;
  case adjustroutehook_SaveLineBtn:
    {
      MOUSEDOWN_REFRESH(m_saveLineBtnCtrl);
    }
    break;
  case adjustroutehook_StartGuidanceBtn:
    {
      MOUSEDOWN_REFRESH(m_startGuidanceBtnCtrl);
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

short CAdjustRouteHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CAdjustRouteHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case adjustroutehook_AddPOI1:
  case adjustroutehook_AddPOI1Icon:
    {
      ADDPOIMACRO(1);
    }
    break;
  case adjustroutehook_AddPOI2:
  case adjustroutehook_AddPOI2Icon:
    {
      ADDPOIMACRO(2);
    }
    break;
  case adjustroutehook_AddPOI3:
  case adjustroutehook_AddPOI3Icon:
    {
      ADDPOIMACRO(3);
    }
    break;
  case adjustroutehook_AddPOI4:
  case adjustroutehook_AddPOI4Icon:
    {
      ADDPOIMACRO(4);
    }
    break;
  case adjustroutehook_AddPOI5:
  case adjustroutehook_AddPOI5Icon:
    {
      ADDPOIMACRO(5);
    }
    break;
  case adjustroutehook_AddPOI6:
  case adjustroutehook_AddPOI6Icon:
    {
      ADDPOIMACRO(6);
    }
    break;
  case adjustroutehook_DemoGuidanceBtn:
    {
      if (IsNeedRefresh(m_demoGuidanceBtnCtrl))
      {
        m_demoGuidanceBtnCtrl.MouseUp();
        StartDemo();
      }
    }
    break;
  case adjustroutehook_POI1Btn:
  case adjustroutehook_POI1BtnIcon:
  case adjustroutehook_POI1Name:
    {
      m_pOI1BtnCtrl.MouseUp();
    }
    break;
  case adjustroutehook_POI1Gps:
  case adjustroutehook_POI1GpsIcon:
    {
      if (IsNeedRefresh(m_pOI1BtnCtrl))
      {
        m_pOI1GpsCtrl.MouseUp();
        SetGpsPos();
      }
    }
    break;
  case adjustroutehook_POI2Btn:
  case adjustroutehook_POI2BtnIcon:
  case adjustroutehook_POI2Name:
    {
      m_pOI2BtnCtrl.MouseUp();
    }
    break;
  case adjustroutehook_POI2Delete:
  case adjustroutehook_POI2DeleteIcon:
    {
      if (IsNeedRefresh(m_pOI2DeleteCtrl))
      {
        m_pOI2DeleteCtrl.MouseUp();
        DoDeletePOI(kROW2);
      }
    }
    break;
  case adjustroutehook_POI3Btn:
  case adjustroutehook_POI3BtnIcon:
  case adjustroutehook_POI3Name:
    {
      m_pOI3BtnCtrl.MouseUp();
    }
    break;
  case adjustroutehook_POI3Delete:
  case adjustroutehook_POI3DeleteIcon:
    {
      if (IsNeedRefresh(m_pOI3DeleteCtrl))
      {
        m_pOI3DeleteCtrl.MouseUp();
        DoDeletePOI(kROW3);
      }
    }
    break;
  case adjustroutehook_POI4Btn:
  case adjustroutehook_POI4BtnIcon:
  case adjustroutehook_POI4Name:
    {
      m_pOI4BtnCtrl.MouseUp();
    }
    break;
  case adjustroutehook_POI4Delete:
  case adjustroutehook_POI4DeleteIcon:
    {
      if (IsNeedRefresh(m_pOI4DeleteCtrl))
      {
        m_pOI4DeleteCtrl.MouseUp();
        DoDeletePOI(kROW4);
      }
    }
    break;
  case adjustroutehook_POI5Btn:
  case adjustroutehook_POI5BtnIcon:
  case adjustroutehook_POI5Name:
    {
      m_pOI5BtnCtrl.MouseUp();
    }
    break;
  case adjustroutehook_POI5Delete:
  case adjustroutehook_POI5DeleteIcon:
    {
      if (IsNeedRefresh(m_pOI5DeleteCtrl))
      {
        m_pOI5DeleteCtrl.MouseUp();
        DoDeletePOI(kROW5);
      }
    }
    break;
  case adjustroutehook_POI6Btn:
  case adjustroutehook_POI6BtnIcon:
  case adjustroutehook_POI6Name:
    {
      m_pOI6BtnCtrl.MouseUp();
    }
    break;
  case adjustroutehook_POI6Delete:
  case adjustroutehook_POI6DeleteIcon:
    {
      if (IsNeedRefresh(m_pOI6DeleteCtrl))
      {
        m_pOI6DeleteCtrl.MouseUp();
        DoDeletePOI(kROW6);
      }
    }
    break;
  case adjustroutehook_PlainLineBtn:
    {
      if (IsNeedRefresh(m_plainLineBtnCtrl))
      {
        m_plainLineBtnCtrl.MouseUp();
        PlanRoute();        
      }
    }
    break;
  case adjustroutehook_RouteTypeBtn:
  case adjustroutehook_RouteTypeBtnIcon:
  case adjustroutehook_RouteTypeLabel:
    {
      if (IsNeedRefresh(m_routeTypeBtnCtrl))
      {
        m_routeTypeBtnCtrl.MouseUp();
        CRouteTypeSwitchHook::SetRouteTypeCallBackFun(this, &CAdjustRouteHook::OnRouteTypeSelect);
        TurnTo(DHT_RouteTypeSwitchHook);
      }
    }
    break;
  case adjustroutehook_SaveLineBtn:
    {
      if (IsNeedRefresh(m_saveLineBtnCtrl))
      {
        m_saveLineBtnCtrl.MouseUp();
      }
    }
    break;
  case adjustroutehook_StartGuidanceBtn:
    {
      if (IsNeedRefresh(m_startGuidanceBtnCtrl))
      {
        m_startGuidanceBtnCtrl.MouseUp();
        StartGuidance();
      }
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}
void CAdjustRouteHook::ClearPOIList()
{
  m_POIList.clear();
}

void CAdjustRouteHook::AddPOIData( const POIItem& poi )
{
  //超过数量不允许再添加
  if (m_POIList.size() < kMaxPOIListCount)
  {
    m_POIList.push_back(poi);
  }
}

void CAdjustRouteHook::SetPOIDataList( const POIDataList& poiList )
{
  m_POIList.clear();
  m_dataFrom = kFromOutside;
  POIDataList::const_iterator iter = poiList.begin();
  for (; iter != poiList.end(); ++iter)
  {
    AddPOIData(*iter);
  }
}

void CAdjustRouteHook::InsertPOIData( int position, const UeQuery::SQLRecord* data )
{ 
  m_dataFrom = kFromOutside;
  //在指定位置添加数据,超过最大数目后不允许再添加
  if (m_POIList.size() >= 6)
  {
    return;
  }
  if (position < 0)
  {
    return;
  }
  //将查询数据赋值给POI节点
  POIItem poi;
  poi.m_type = PT_Middle;
  if (data->m_uniName)
  {
    ::strcpy(poi.m_name, data->m_uniName);
  }
  int len = ::strlen(poi.m_name);
  if(poi.m_name[len] == 0)
  {
    poi.m_name[len] = '\n';
  }
  poi.m_pos.m_x = data->m_x;
  poi.m_pos.m_y = data->m_y;

  if ((m_POIList.size() == 0) || (position > m_POIList.size() - 1))
  {
    m_POIList.push_back(poi);
  }
  else
  {
    if (0 == position)
    {
      //如果下标是0，则表示修改起点
      m_POIList[0] = poi;
    }
    else
    {
      POIDataList::iterator iter = m_POIList.begin();
      m_POIList.insert(iter + position, poi);
    }
  }
}


void CAdjustRouteHook::ClearAllRow()
{
  ClearRow(kROW1);
  ClearRow(kROW2);
  ClearRow(kROW3);
  ClearRow(kROW4);
  ClearRow(kROW5);
  ClearRow(kROW6);
  SetRowEnbale(kROW1, false);
  SetRowEnbale(kROW2, false);
  SetRowEnbale(kROW3, false);
  SetRowEnbale(kROW4, false);
  SetRowEnbale(kROW5, false);
  SetRowEnbale(kROW6, false);
}


void UeGui::CAdjustRouteHook::ClearRow( RowTag row )
{
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1BtnCtrl.ClearCaption();
      break;
    }
  case kROW2 : 
    {
      m_pOI2BtnCtrl.ClearCaption();
      break;
    }
  case kROW3 :
    {
      m_pOI3BtnCtrl.ClearCaption();
      break;
    }
  case kROW4 :
    {
      m_pOI4BtnCtrl.ClearCaption();
      break;
    }
  case kROW5 :
    {
      m_pOI5BtnCtrl.ClearCaption();
      break;
    }
  case kROW6 :
    {
      m_pOI6BtnCtrl.ClearCaption();     
      break;
    }
  }
}

void UeGui::CAdjustRouteHook::SetRowEnbale( RowTag row, bool enable )
{
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1BtnCtrl.SetEnable(enable);    
      break;
    }
  case kROW2 : 
    {
      m_pOI2BtnCtrl.SetEnable(enable);
      m_pOI2DeleteCtrl.SetEnable(enable);
      ChangeButotnIcon(enable, m_addPOI2Ctrl);
      break;
    }
  case kROW3 :
    {
      m_pOI3BtnCtrl.SetEnable(enable);
      m_pOI3DeleteCtrl.SetEnable(enable);
      ChangeButotnIcon(enable, m_addPOI3Ctrl);
      break;
    }
  case kROW4 :
    {
      m_pOI4BtnCtrl.SetEnable(enable);
      m_pOI4DeleteCtrl.SetEnable(enable);
      ChangeButotnIcon(enable, m_addPOI4Ctrl);
      break;
    }
  case kROW5 :
    {
      m_pOI5BtnCtrl.SetEnable(enable);
      m_pOI5DeleteCtrl.SetEnable(enable);
      ChangeButotnIcon(enable, m_addPOI5Ctrl);
      break;
    }
  case kROW6 :
    {
      m_pOI6BtnCtrl.SetEnable(enable);
      m_pOI6DeleteCtrl.SetEnable(enable);
      ChangeButotnIcon(enable, m_addPOI6Ctrl);
      break;
    }
  }
}

void CAdjustRouteHook::ChangeButotnIcon(bool isEdit, CUiBitButton &button)
{
  GuiElement* element = GetGuiElement(adjustroutehook_AddPOIIconBak);
  if (element)
  {
    if (!isEdit)
    {
      button.GetIconElement()->m_bkNormal = element->m_bkNormal;
      button.GetIconElement()->m_bkFocus = element->m_bkFocus;
      button.GetIconElement()->m_bkDisabled = element->m_bkDisabled;
    }
    else
    {
      button.GetIconElement()->m_bkNormal = m_addPOI1Ctrl.GetIconElement()->m_bkNormal;
      button.GetIconElement()->m_bkFocus = m_addPOI1Ctrl.GetIconElement()->m_bkFocus;
      button.GetIconElement()->m_bkDisabled = m_addPOI1Ctrl.GetIconElement()->m_bkDisabled;
    }
    button.GetIconElement()->m_backStyle = button.GetIconElement()->m_bkNormal;
  }
  
}

void UeGui::CAdjustRouteHook::GetRouteData()
{
  CRouteWrapper &routeWrapper = CRouteWrapper::Get();
  m_POIList.clear();
  //获取起点
  POIItem startPos;
  startPos.m_type = UeRoute::PT_Start;
  routeWrapper.GetPosition(startPos);
  if(startPos.m_type != UeRoute::PT_Invalid)
  {
    m_POIList.push_back(startPos);
  }
  else
  {
    GetGpsItem(startPos);
    m_POIList.push_back(startPos);
  }
  //获取中间经由点
  int posCount = m_route->GetPosCount();
  if(posCount > 2)
  {
    POIItem midPos;
    midPos.m_type = UeRoute::PT_Middle;
    for(int i = 1; i < posCount - 1; i++)
    {
      routeWrapper.GetPosition(midPos, i);
      if(midPos.m_type != UeRoute::PT_Invalid)
      {
        m_POIList.push_back(midPos);
      }
    }
  }
  //获取终点
  POIItem endPos;
  endPos.m_type = UeRoute::PT_End;
  routeWrapper.GetPosition(endPos);
  if(endPos.m_type != UeRoute::PT_Invalid)
  {
    m_POIList.push_back(endPos);
  }

}


void CAdjustRouteHook::ShowRouteData()
{
  ClearAllRow();
  //起点
  if (m_POIList.size() > 0)
  {
    ShowRowData(kROW1, m_POIList[0].m_name);
  }
  //目的地
  if (m_POIList.size() > 1)
  {
    ShowRowData(kROW6, m_POIList[m_POIList.size() - 1].m_name);
  }
  //经由点
  if (m_POIList.size() > 2)
  {
    int row = kROW2;
    for (unsigned int i = 1; i < m_POIList.size() - 1; ++i)
    {
      if (row > kROW6)
      {
        break;
      }
      ShowRowData((RowTag)row, m_POIList[i].m_name);
      row++;
    }

  }

}

void UeGui::CAdjustRouteHook::ShowRowData( RowTag row, const char* value )
{
  char routeName[128] = {};
  unsigned int slen = ::strlen(value);
  if (0 == slen)
  {
    ::strcpy(routeName, " 未命名");
  }
  else
  {
    unsigned int size = slen < sizeof(routeName) ? slen : sizeof(routeName);
    if (size > 0)
    {
      ::memcpy(routeName, value, size);
      if (routeName[size - 1] == '\n')
      {
        routeName[size - 1] = 0;
      }
    }                  
  }
  SetRowEnbale(row, true);
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1BtnCtrl.SetCaption(routeName);
      break;
    }
  case kROW2 : 
    {
      m_pOI2BtnCtrl.SetCaption(routeName);
      break;
    }
  case kROW3 : 
    {
      m_pOI3BtnCtrl.SetCaption(routeName);
      break;
    }
  case kROW4 : 
    {
      m_pOI4BtnCtrl.SetCaption(routeName);
      break;
    }
  case kROW5 : 
    {
      m_pOI5BtnCtrl.SetCaption(routeName);
      break;
    }
  case kROW6 : 
    {
      m_pOI6BtnCtrl.SetCaption(routeName);
      break;
    }
  default:
    {
      break;
    }
  }
}

unsigned int CAdjustRouteHook::GetDataIndex(RowTag row)
{
  return row - 1;
}

void UeGui::CAdjustRouteHook::DoDeletePOI( RowTag row )
{
  m_selectRowTag = row;
  ////弹出消息对话框确认是否删除该POI
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_AdjustRouteHook, &UeGui::CAdjustRouteHook::OnDeletePOI);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "确定要删除该导航点?", dialogEvent);
}

void CAdjustRouteHook::OnClearAll( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CAdjustRouteHook* adjustRoutewHook = dynamic_cast<CAdjustRouteHook*>(sender);
    adjustRoutewHook->ClearAll();
  }
}

void UeGui::CAdjustRouteHook::DoEditOrAddPoi(RowTag row)
{
  m_selectRowTag = row;

  CInputSwitchHook *inputHook = (CInputSwitchHook *)CViewWrapper::Get().GetHook(DHT_InputSwitchHook);
  if (inputHook)
  {
    CSelectPointCallBackCtrl::Get().SetCallBackFun((void*)this, &CAdjustRouteHook::SelectPointEvent);
    TurnTo(inputHook->GetCurInputHookType());
  }
}

void CAdjustRouteHook::OnInsertPOIData( void* sender, const UeQuery::SQLRecord * data )
{
  if (NULL == sender)
  {
    return;
  }
  CViewHook::m_curHookType = CViewHook::DHT_AdjustRouteHook;
  CAdjustRouteHook* adjustRouteHook = static_cast<CAdjustRouteHook*>(sender);
  adjustRouteHook->InsertPOIData(adjustRouteHook->GetInsertPosition(), data);
  adjustRouteHook->ShowRouteData();
}

void CAdjustRouteHook::OnRouteTypeSelect(CAggHook *sender, unsigned int routeType )
{
  CAdjustRouteHook* hook = dynamic_cast<CAdjustRouteHook*>(sender);
  if (hook)
  {
    hook->SetRouteTypeCaption(routeType);
  }

}

void CAdjustRouteHook::SetRouteTypeCaption(unsigned int routeType)
{//选择规划道路方式
  if(routeType & UeRoute::RW_Optimal)
  {
    m_routeTypeLabelCtrl.SetCaption("推荐路线"); 
  }
  else if(routeType & UeRoute::RW_Short)
  {
    m_routeTypeLabelCtrl.SetCaption("最短路径"); 
  }
  else if(routeType & UeRoute::RW_Fast)
  {
    m_routeTypeLabelCtrl.SetCaption("高速优先"); 
  }
  else if (routeType & UeRoute::RW_Economic)
  {
    m_routeTypeLabelCtrl.SetCaption("经济路线"); 
  }
  m_planMethod = routeType;
  CRouteWrapper::Get().SetMethod(m_planMethod);
}

//读取要添加的POI位置,下标从0开始
unsigned int CAdjustRouteHook::GetInsertPosition()
{
  return GetDataIndex(m_selectRowTag);
}

void CAdjustRouteHook::ClearAll()
{
  ClearAllRow();
  ClearPOIList();  
}

void UeGui::CAdjustRouteHook::OnDeletePOI( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CAdjustRouteHook* adjustRoutewHook = dynamic_cast<CAdjustRouteHook*>(sender);
    adjustRoutewHook->DeletePOI();
  }
}

void CAdjustRouteHook::DeletePOI()
{
  
  DeleteRouteData(m_selectRowTag);
  InitButtonState();
  //删除POI之后重新显示数据
  ShowRouteData();
}


void CAdjustRouteHook::DeleteRouteData( RowTag row )
{
  if (m_POIList.size() == 0)
  {
    return;
  }
  if (row == kROW6)
  {
    m_POIList.erase(m_POIList.end() - 1);
  }
  else
  {
     POIDataList::iterator first = m_POIList.begin();
     for (unsigned int i = 1; i < row && first != m_POIList.end(); ++i, ++first);
     m_POIList.erase(first);
  }
}


void UeGui::CAdjustRouteHook::PlanRoute()
{
  SetPosition();
  //规划路线方式,调用maphook接口
  CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    GoToMapHook();
    CRouteWrapper::Get().SetMethod(m_planMethod);
    mapHook->RoutePlan(CMapHook::Plan_Single);
    CViewWrapper::Get().Refresh();
  }
   
}

void UeGui::CAdjustRouteHook::SetPosition()
{
  unsigned int POICount = m_POIList.size();
  if (POICount < 2)
  {
    return;
  }
  
  CRouteWrapper::Get().RemovePosition(true);
  for (unsigned int i = 0; i < m_POIList.size(); ++i)
  {
    CRouteWrapper::Get().SetPosition(m_POIList[i]);
  }
}

void UeGui::CAdjustRouteHook::StartGuidance()
{
  SetPosition();
  //规划路线方式,调用maphook接口
  CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    GoToMapHook();
    CRouteWrapper::Get().SetMethod(m_planMethod);
    if (mapHook->RoutePlan_StartGuidance(CMapHook::Plan_Single))
    {
      m_isNeedRefesh = false;
    }
  }
}
void CAdjustRouteHook::StartDemo()
{
  SetPosition();
  //规划路线方式,调用maphook接口
  CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
  if (mapHook)
  {
    GoToMapHook();
    CRouteWrapper::Get().SetMethod(m_planMethod);
    if (mapHook->RoutePlan_StartDemo(CMapHook::Plan_Single) == PEC_Success)
    {
      m_isNeedRefesh = false;
    }
  }

}

void CAdjustRouteHook::SetGpsPos()
{
  POIItem item1;
  GetGpsItem(item1);
  EditPOIData(kROW1, item1);
  ShowRouteData();
}

void CAdjustRouteHook::GetGpsItem(POIItem &item)
{
  CViewWrapper &viewWrapper = CViewWrapper::Get();
  const GpsCar &gpsCar = viewWrapper.GetGpsCar();
  CGeoPoint<long> mapPos;
  mapPos.m_x = gpsCar.m_curPos.m_x;
  mapPos.m_y = gpsCar.m_curPos.m_y;
  item.m_pos = mapPos;
  item.m_type = PT_Start;
  CQueryWrapper &queryWrapper = CQueryWrapper::Get();
  queryWrapper.GetPlaceName(mapPos, item.m_name);
}

void CAdjustRouteHook::EditPOIData(RowTag row, const POIItem &item)
{
  if (row > kROWBegin && row < kROWEnd)
  {
    m_POIList[row - 1] = item;
  }
}
void CAdjustRouteHook::SelectPointEvent(void *callBackObj, const UeQuery::SQLRecord* data)
{
  CAdjustRouteHook *hook = static_cast<CAdjustRouteHook*>(callBackObj);
  if (hook)
  {
    hook->InsertPOIData(hook->GetInsertPosition(), data);
    hook->Fall(CViewHook::DHT_AdjustRouteHook);
    hook->ShowRouteData();
  }
}

void CAdjustRouteHook::InitButtonState()
{
  if (m_POIList.size() < 2)
  {
    m_startGuidanceBtnCtrl.SetEnable(false);
    m_demoGuidanceBtnCtrl.SetEnable(false);
    m_routeTypeBtnCtrl.SetEnable(false);
    m_routeTypeLabelCtrl.SetEnable(false);
    m_plainLineBtnCtrl.SetEnable(false);
    m_saveLineBtnCtrl.SetEnable(false);
  }
  else
  {
    m_startGuidanceBtnCtrl.SetEnable(true);
    m_demoGuidanceBtnCtrl.SetEnable(true);
    m_routeTypeBtnCtrl.SetEnable(true);
    m_routeTypeLabelCtrl.SetEnable(true);
    m_plainLineBtnCtrl.SetEnable(true);
    m_saveLineBtnCtrl.SetEnable(true);
  }
}