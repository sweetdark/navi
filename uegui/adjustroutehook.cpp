#include "adjustroutehook.h"
//#include "queryMenuHook.h"
#include "messagedialoghook.h"
//#include "myjourneyhook.h"
//#include "InputHook.h"
#include "maphook.h"
#include "uemap\viewcanvas.h"
#include "uemap\viewstate.h"
#include "userdatawrapper.h"

using namespace UeGui;
using namespace UeRoute;

CAdjustRouteHook::CAdjustRouteHook() : m_addRowTag(kROWEnd), m_selectRowTag(kROWBegin), m_enterHookType(DHT_Unknown),
    m_routeType(RW_Optimal), m_editRouteEvent(NULL), m_senderHookType(DHT_Unknown), m_senderHook(NULL), m_windowModel(WDefaultModel)
{
  ::memset(m_JourneyName, 0, sizeof(m_JourneyName));
  ClearPOIList();
}

CAdjustRouteHook::~CAdjustRouteHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CAdjustRouteHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CAdjustRouteHook::GetBinaryFileName()
{
  return _T("adjustroutehook.bin");
}


void UeGui::CAdjustRouteHook::Init()
{
  ClearAllRow();
}

void CAdjustRouteHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_ViewMap,	"ViewMap"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_ViewPrevious,	"ViewPrevious"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_NavigationTitle,	"NavigationTitle"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line1Left,	"Line1Left"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line1Center,	"Line1Center"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line1Right,	"Line1Right"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line2Left,	"Line2Left"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line2Center,	"Line2Center"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line2Right,	"Line2Right"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line3Left,	"Line3Left"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line3Center,	"Line3Center"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line3Right,	"Line3Right"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line4Left,	"Line4Left"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line4Center,	"Line4Center"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line4Right,	"Line4Right"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line5Left,	"Line5Left"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line5Center,	"Line5Center"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line5Right,	"Line5Right"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line6Left,	"Line6Left"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line6Center,	"Line6Center"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_Line6Right,	"Line6Right"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI1,	"AddPOI1"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI1Icon,	"AddPOI1Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI2,	"AddPOI2"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI2Icon,	"AddPOI2Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI3,	"AddPOI3"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI3Icon,	"AddPOI3Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI4,	"AddPOI4"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI4Icon,	"AddPOI4Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI5,	"AddPOI5"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AddPOI5Icon,	"AddPOI5Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI1Icon,	"POI1Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI1Name,	"POI1Name"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI1DeleteBack,	"POI1DeleteBack"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI1DeleteIcon,	"POI1DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI2Icon,	"POI2Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI2Name,	"POI2Name"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI2DeleteBack,	"POI2DeleteBack"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI2DeleteIcon,	"POI2DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI4Icon,	"POI4Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI3Name,	"POI3Name"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI3DeleteBack,	"POI3DeleteBack"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI3DeleteIcon,	"POI3DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI3Icon,	"POI3Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI4Name,	"POI4Name"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI4DeleteBack,	"POI4DeleteBack"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI4DeleteIcon,	"POI4DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI5Icon,	"POI5Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI5Name,	"POI5Name"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI5DeleteBack,	"POI5DeleteBack"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI5DeleteIcon,	"POI5DeleteIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI6Icon,	"POI6Icon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI6Name,	"POI6Name"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI6EditStartBack,	"POI6EditStartBack"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_POI6EditStartIcon,	"POI6EditStartIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_ClearLineLeft,	"ClearLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_ClearLineCenter,	"ClearLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_ClearLineRight,	"ClearLineRight"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_SaveLineLeft,	"SaveLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_SaveLineCenter,	"SaveLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_SaveLineRight,	"SaveLineRight"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteType1,	"RouteType1"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteType2,	"RouteType2"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteType3,	"RouteType3"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteType4,	"RouteType4"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteTypeLeft,	"RouteTypeLeft"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteTypeCenter,	"RouteTypeCenter"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteTypeRight,	"RouteTypeRight"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteTypeLabel,	"RouteTypeLabel"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_RouteTypeIcon,	"RouteTypeIcon"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_PlainLineLeft,	"PlainLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_PlainLineCenter,	"PlainLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_PlainLineRight,	"PlainLineRight"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_EditLineLeft,	"EditLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_EditLineCenter,	"EditLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_EditLineRight,	"EditLineRight"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AcceptLineLeft,	"AcceptLineLeft"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AcceptLineCenter,	"AcceptLineCenter"));
  m_ctrlNames.insert(GuiName::value_type(AdjustRouteHook_AcceptLineRight,	"AcceptLineRight"));
  
}

void CAdjustRouteHook::MakeControls()
{
  m_viewMapCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_ViewMap));
  m_viewPreviousCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_ViewPrevious));
  m_addPOI1Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_AddPOI1));
  m_addPOI1Ctrl.SetIconElement(GetGuiElement(AdjustRouteHook_AddPOI1Icon));
  m_addPOI2Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_AddPOI2));
  m_addPOI2Ctrl.SetIconElement(GetGuiElement(AdjustRouteHook_AddPOI2Icon));
  m_addPOI3Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_AddPOI3));
  m_addPOI3Ctrl.SetIconElement(GetGuiElement(AdjustRouteHook_AddPOI3Icon));
  m_addPOI4Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_AddPOI4));
  m_addPOI4Ctrl.SetIconElement(GetGuiElement(AdjustRouteHook_AddPOI4Icon));
  m_addPOI5Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_AddPOI5));
  m_addPOI5Ctrl.SetIconElement(GetGuiElement(AdjustRouteHook_AddPOI5Icon));    
  m_clearCtrl.SetLeftElement(GetGuiElement(AdjustRouteHook_ClearLineLeft));
  m_clearCtrl.SetRightElement(GetGuiElement(AdjustRouteHook_ClearLineRight));
  m_clearCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_ClearLineCenter));

  m_line1Ctrl.SetLeftElement(GetGuiElement(AdjustRouteHook_Line1Left));
  m_line1Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_Line1Center));
  m_line1Ctrl.SetRightElement(GetGuiElement(AdjustRouteHook_Line1Right));
  m_pOI1IconCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI1Icon));
  m_pOI1NameCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_POI1Name));
  m_pOI1DeleteCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_POI1DeleteBack));
  m_pOI1DeleteCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI1DeleteIcon));
     
  m_line2Ctrl.SetLeftElement(GetGuiElement(AdjustRouteHook_Line2Left));
  m_line2Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_Line2Center));  
  m_line2Ctrl.SetRightElement(GetGuiElement(AdjustRouteHook_Line2Right));  
  m_pOI2DeleteCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_POI2DeleteBack));
  m_pOI2DeleteCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI2DeleteIcon));
  m_pOI2IconCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI2Icon));
  m_pOI2NameCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_POI2Name));

  m_line3Ctrl.SetLeftElement(GetGuiElement(AdjustRouteHook_Line3Left));
  m_line3Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_Line3Center));
  m_line3Ctrl.SetRightElement(GetGuiElement(AdjustRouteHook_Line3Right));
  m_pOI3NameCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_POI3Name));
  m_pOI3IconCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI3Icon));
  m_pOI3DeleteCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_POI3DeleteBack));
  m_pOI3DeleteCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI3DeleteIcon));

  m_line4Ctrl.SetLeftElement(GetGuiElement(AdjustRouteHook_Line4Left));
  m_line4Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_Line4Center));
  m_line4Ctrl.SetRightElement(GetGuiElement(AdjustRouteHook_Line4Right));
  m_pOI4NameCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_POI4Name));
  m_pOI4IconCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI4Icon));  
  m_pOI4DeleteCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_POI4DeleteBack));
  m_pOI4DeleteCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI4DeleteIcon));

  m_line5Ctrl.SetLeftElement(GetGuiElement(AdjustRouteHook_Line5Left));
  m_line5Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_Line5Center));
  m_line5Ctrl.SetRightElement(GetGuiElement(AdjustRouteHook_Line5Right));
  m_pOI5NameCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_POI5Name));
  m_pOI5IconCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI5Icon));
  m_pOI5DeleteCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_POI5DeleteBack));
  m_pOI5DeleteCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI5DeleteIcon));

  m_line6Ctrl.SetLeftElement(GetGuiElement(AdjustRouteHook_Line6Left));
  m_line6Ctrl.SetCenterElement(GetGuiElement(AdjustRouteHook_Line6Center));
  m_line6Ctrl.SetRightElement(GetGuiElement(AdjustRouteHook_Line6Right));
  m_pOI6NameCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_POI6Name));
  m_pOI6IconCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI6Icon));
  m_pOI6EditStartCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_POI6EditStartBack));
  m_pOI6EditStartCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_POI6EditStartIcon));
    
  m_plainLineCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_PlainLineCenter));
  m_plainLineCtrl.SetLeftElement(GetGuiElement(AdjustRouteHook_PlainLineLeft));
  m_plainLineCtrl.SetRightElement(GetGuiElement(AdjustRouteHook_PlainLineRight));

  m_routeType1.SetLabelElement(GetGuiElement(AdjustRouteHook_RouteType1));
  m_routeType1.SetVisible(false);
  m_routeType2.SetLabelElement(GetGuiElement(AdjustRouteHook_RouteType2));
  m_routeType2.SetVisible(false);
  m_routeType3.SetLabelElement(GetGuiElement(AdjustRouteHook_RouteType3));
  m_routeType3.SetVisible(false);
  m_routeType4.SetLabelElement(GetGuiElement(AdjustRouteHook_RouteType4));
  m_routeType4.SetVisible(false);

  m_routeTypeCtrl.SetLeftElement(GetGuiElement(AdjustRouteHook_RouteTypeLeft));
  m_routeTypeCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_RouteTypeCenter));  
  m_routeTypeCtrl.SetRightElement(GetGuiElement(AdjustRouteHook_RouteTypeRight));
  m_routeTypeCtrl.SetIconElement(GetGuiElement(AdjustRouteHook_RouteTypeIcon));

  m_routeTypeCtrl.SetCaption(m_routeType4.GetCaption());
    
  m_saveLineCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_SaveLineCenter));
  m_saveLineCtrl.SetLeftElement(GetGuiElement(AdjustRouteHook_SaveLineLeft));
  m_saveLineCtrl.SetRightElement(GetGuiElement(AdjustRouteHook_SaveLineRight));

  m_editLineCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_EditLineCenter));
  m_editLineCtrl.SetLeftElement(GetGuiElement(AdjustRouteHook_EditLineLeft));
  m_editLineCtrl.SetRightElement(GetGuiElement(AdjustRouteHook_EditLineRight));

  m_acceptLineCtrl.SetCenterElement(GetGuiElement(AdjustRouteHook_AcceptLineCenter));
  m_acceptLineCtrl.SetLeftElement(GetGuiElement(AdjustRouteHook_AcceptLineLeft));
  m_acceptLineCtrl.SetRightElement(GetGuiElement(AdjustRouteHook_AcceptLineRight));
}

void UeGui::CAdjustRouteHook::DynamicShow()
{

}

void UeGui::CAdjustRouteHook::SpecialShow( WindowModel model /*= WDefaultModel*/ )
{
  m_windowModel = model;
  //Mario
  /*m_enterHookType = m_prevHookType;*/
  SetWindowModel(model);  
  ShowRouteData();
}

void UeGui::CAdjustRouteHook::SetWindowModel( WindowModel model )
{
  switch (model)
  {
  case WDefaultModel :
  case WAppendModel :
    {
      m_saveLineCtrl.SetVisible(true);
      m_plainLineCtrl.SetVisible(true);
      m_editLineCtrl.SetVisible(false);
      m_acceptLineCtrl.SetVisible(false);
      //新增路线数据中route模块获取
      GetRouteData();
      break;
    }
  case WEditModel :
    {
      m_saveLineCtrl.SetVisible(false);
      m_plainLineCtrl.SetVisible(false);
      m_editLineCtrl.SetVisible(true);
      m_acceptLineCtrl.SetVisible(true);
      //修改数据由外部传入
      break;
    }
  case WBrowseModel:
    {
      m_saveLineCtrl.SetVisible(false);
      m_plainLineCtrl.SetVisible(true);
      m_editLineCtrl.SetVisible(false);
      m_acceptLineCtrl.SetVisible(false);
      //浏览数据由外部传入
      break;
    }
  default:
    {
      //默认和 WAppendModel 同
      m_saveLineCtrl.SetVisible(true);
      m_plainLineCtrl.SetVisible(true);
      m_editLineCtrl.SetVisible(false);
      m_acceptLineCtrl.SetVisible(false);
      GetRouteData();
      break;
    }
  }  
}

short CAdjustRouteHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case AdjustRouteHook_ViewMap:
    {
      m_viewMapCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_ViewPrevious:
    {
      m_viewPreviousCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_AddPOI1:
  case AdjustRouteHook_AddPOI1Icon:
    {
      m_addPOI1Ctrl.MouseDown();
    }
    break;
  case AdjustRouteHook_AddPOI2:
  case AdjustRouteHook_AddPOI2Icon:
    {
      m_addPOI2Ctrl.MouseDown();
    }
    break;
  case AdjustRouteHook_AddPOI3:
  case AdjustRouteHook_AddPOI3Icon:
    {
      m_addPOI3Ctrl.MouseDown();
    }
    break;
  case AdjustRouteHook_AddPOI4:
  case AdjustRouteHook_AddPOI4Icon:
    {
      m_addPOI4Ctrl.MouseDown();
    }
    break;
  case AdjustRouteHook_AddPOI5:
  case AdjustRouteHook_AddPOI5Icon:
    {
      m_addPOI5Ctrl.MouseDown();
    }
    break;
  case AdjustRouteHook_ClearLineCenter:
  case AdjustRouteHook_ClearLineLeft:
  case AdjustRouteHook_ClearLineRight:
    {
      m_clearCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_Line1Left:
  case AdjustRouteHook_Line1Center:    
  case AdjustRouteHook_Line1Right:  
  case AdjustRouteHook_POI1Icon:
  case AdjustRouteHook_POI1Name:
    {
      m_line1Ctrl.MouseDown();
      AddRenderUiControls(&m_line1Ctrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI1Icon));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI1Name));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI1DeleteBack));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI1DeleteIcon));
    }
    break;
  case AdjustRouteHook_Line2Left:
  case AdjustRouteHook_Line2Center:
  case AdjustRouteHook_Line2Right:
  case AdjustRouteHook_POI2Icon:
  case AdjustRouteHook_POI2Name:
    {
      m_line2Ctrl.MouseDown();
      AddRenderUiControls(&m_line2Ctrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI2Icon));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI2Name));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI2DeleteBack));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI2DeleteIcon));
    }
    break;
  case AdjustRouteHook_Line3Center:
  case AdjustRouteHook_Line3Left:
  case AdjustRouteHook_Line3Right:
  case AdjustRouteHook_POI3Icon:
  case AdjustRouteHook_POI3Name:
    {
      m_line3Ctrl.MouseDown();
      AddRenderUiControls(&m_line3Ctrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI3Icon));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI3Name));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI3DeleteBack));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI3DeleteIcon));
    }
    break;
  case AdjustRouteHook_Line4Center:
  case AdjustRouteHook_Line4Left:
  case AdjustRouteHook_Line4Right:
  case AdjustRouteHook_POI4Icon:
  case AdjustRouteHook_POI4Name:
    {
      m_line4Ctrl.MouseDown();
      AddRenderUiControls(&m_line4Ctrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI4Icon));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI4Name));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI4DeleteBack));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI4DeleteIcon));
    }
    break;
  case AdjustRouteHook_Line5Center:
  case AdjustRouteHook_Line5Left:
  case AdjustRouteHook_Line5Right:
  case AdjustRouteHook_POI5Icon:
  case AdjustRouteHook_POI5Name:
    {
      m_line5Ctrl.MouseDown();
      AddRenderUiControls(&m_line5Ctrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI5Icon));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI5Name));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI5DeleteBack));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI5DeleteIcon));
    }
    break;
  case AdjustRouteHook_Line6Center:
  case AdjustRouteHook_Line6Left:
  case AdjustRouteHook_Line6Right:
  case AdjustRouteHook_POI6Icon:
  case AdjustRouteHook_POI6Name:
    {
      m_line6Ctrl.MouseDown();
      AddRenderUiControls(&m_line6Ctrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI6Icon));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI6Name));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI6EditStartBack));
      AddRenderElements(GetGuiElement(AdjustRouteHook_POI6EditStartIcon));
    }
    break;
  case AdjustRouteHook_POI1DeleteBack:
  case AdjustRouteHook_POI1DeleteIcon:
    {
      m_pOI1DeleteCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_POI2DeleteBack:
  case AdjustRouteHook_POI2DeleteIcon:
    {
      m_pOI2DeleteCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_POI3DeleteBack:
  case AdjustRouteHook_POI3DeleteIcon:
    {
      m_pOI3DeleteCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_POI4DeleteBack:
  case AdjustRouteHook_POI4DeleteIcon:
    {
      m_pOI4DeleteCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_POI5DeleteBack:
  case AdjustRouteHook_POI5DeleteIcon:
    {
      m_pOI5DeleteCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_POI6EditStartBack:
  case AdjustRouteHook_POI6EditStartIcon:
    {
      m_pOI6EditStartCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_PlainLineCenter:
  case AdjustRouteHook_PlainLineLeft:
  case AdjustRouteHook_PlainLineRight:
    {
      //规划路线
      m_plainLineCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_RouteTypeCenter:
  case AdjustRouteHook_RouteTypeIcon:
  case AdjustRouteHook_RouteTypeLabel:
  case AdjustRouteHook_RouteTypeRight:
  case AdjustRouteHook_RouteTypeLeft:
    {
      //推荐路线
      m_routeTypeCtrl.MouseDown();
      AddRenderUiControls(&m_routeTypeCtrl);
      AddRenderElements(GetGuiElement(AdjustRouteHook_RouteTypeLabel));
    }
    break;
  case AdjustRouteHook_SaveLineCenter:
  case AdjustRouteHook_SaveLineLeft:
  case AdjustRouteHook_SaveLineRight:
    {
      //保存行程
      m_saveLineCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_EditLineCenter:
  case AdjustRouteHook_EditLineLeft:
  case AdjustRouteHook_EditLineRight:
    {
      //编辑保存到我的行程
      m_editLineCtrl.MouseDown();
    }
    break;
  case AdjustRouteHook_AcceptLineCenter:
  case AdjustRouteHook_AcceptLineLeft:
  case AdjustRouteHook_AcceptLineRight:
    {
      //确定
      m_acceptLineCtrl.MouseDown();
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
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool sameElement = ctrlType == m_downElementType;
  switch(m_downElementType)
  {
  case AdjustRouteHook_ViewMap:
    {
      m_viewMapCtrl.MouseUp();
      if (sameElement)
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
        CAggHook::TurnTo(DHT_MapHook);
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }
    }
    break;
  case AdjustRouteHook_ViewPrevious:
    {
      m_viewPreviousCtrl.MouseUp();
      if (sameElement)
      {
        //CViewHook::m_curHookType = m_enterHookType;
        //if(((CMapHook *)m_view->GetHook(CViewHook::DHT_MapHook))->m_isOpenMenu)
        //{
        //  CViewHook::m_prevHookType = CViewHook::DHT_MainMenuHook/*DHT_AdjustRouteHook*/;  
        //}
        //else
        //{
        //  CViewHook::m_prevHookType = CViewHook::DHT_MapHook;
        //}
        CAggHook::Return();
      } 
    }
    break;
  case AdjustRouteHook_AddPOI1:
  case AdjustRouteHook_AddPOI1Icon:
    {
      m_addPOI1Ctrl.MouseUp();
      if (sameElement && m_addPOI1Ctrl.IsEnable())
      {
        DoAddPOI(kROW1);
      }      
    }
    break;
  case AdjustRouteHook_AddPOI2:
  case AdjustRouteHook_AddPOI2Icon:
    {
      m_addPOI2Ctrl.MouseUp();
      if (sameElement && m_addPOI2Ctrl.IsEnable())
      {
        DoAddPOI(kROW2);
      }      
    }
    break;
  case AdjustRouteHook_AddPOI3:
  case AdjustRouteHook_AddPOI3Icon:
    {
      m_addPOI3Ctrl.MouseUp();
      if (sameElement && m_addPOI3Ctrl.IsEnable())
      {
        DoAddPOI(kROW3);
      }      
    }
    break;
  case AdjustRouteHook_AddPOI4:
  case AdjustRouteHook_AddPOI4Icon:
    {
      m_addPOI4Ctrl.MouseUp();
      if (sameElement && m_addPOI4Ctrl.IsEnable())
      {
        DoAddPOI(kROW4);
      }      
    }
    break;
  case AdjustRouteHook_AddPOI5:
  case AdjustRouteHook_AddPOI5Icon:
    {
      m_addPOI5Ctrl.MouseUp();
      if (sameElement && m_addPOI5Ctrl.IsEnable())
      {
        DoAddPOI(kROW5);
      }      
    }
    break;
  case AdjustRouteHook_ClearLineCenter:
  case AdjustRouteHook_ClearLineLeft:
  case AdjustRouteHook_ClearLineRight:
    {
      m_clearCtrl.MouseUp();
      if (sameElement && m_clearCtrl.IsEnable())
      {
        DoClearAll();
      }      
    }
    break;
  case AdjustRouteHook_Line1Left:
  case AdjustRouteHook_Line1Center:    
  case AdjustRouteHook_Line1Right:  
  case AdjustRouteHook_POI1Icon:
  case AdjustRouteHook_POI1Name:
    {
      m_line1Ctrl.MouseUp();
      if (sameElement && m_line1Ctrl.IsEnable())
      {
        DoDataMove(kROW1);
      }
    }
    break;
  case AdjustRouteHook_Line2Left:
  case AdjustRouteHook_Line2Center:
  case AdjustRouteHook_Line2Right:
  case AdjustRouteHook_POI2Icon:
  case AdjustRouteHook_POI2Name:
    {
      m_line2Ctrl.MouseUp();
      if (sameElement && m_line2Ctrl.IsEnable())
      {
        DoDataMove(kROW2);
      }
    }
    break;
  case AdjustRouteHook_Line3Center:
  case AdjustRouteHook_Line3Left:
  case AdjustRouteHook_Line3Right:
  case AdjustRouteHook_POI3Icon:
  case AdjustRouteHook_POI3Name:
    {
      m_line3Ctrl.MouseUp();
      if (sameElement && m_line3Ctrl.IsEnable())
      {
        DoDataMove(kROW3);
      }
    }
    break;
  case AdjustRouteHook_Line4Center:
  case AdjustRouteHook_Line4Left:
  case AdjustRouteHook_Line4Right:
  case AdjustRouteHook_POI4Icon:
  case AdjustRouteHook_POI4Name:
    {
      m_line4Ctrl.MouseUp();
      if (sameElement && m_line4Ctrl.IsEnable())
      {
        DoDataMove(kROW4);
      }
    }
    break;
  case AdjustRouteHook_Line5Center:
  case AdjustRouteHook_Line5Left:
  case AdjustRouteHook_Line5Right:
  case AdjustRouteHook_POI5Icon:
  case AdjustRouteHook_POI5Name:
    {
      m_line5Ctrl.MouseUp();
      if (sameElement && m_line5Ctrl.IsEnable())
      {
        DoDataMove(kROW5);
      }
    }
    break;
  case AdjustRouteHook_Line6Center:
  case AdjustRouteHook_Line6Left:
  case AdjustRouteHook_Line6Right:
  case AdjustRouteHook_POI6Icon:
  case AdjustRouteHook_POI6Name:
    {
      m_line6Ctrl.MouseUp();
    }
    break;
  case AdjustRouteHook_POI1DeleteBack:
  case AdjustRouteHook_POI1DeleteIcon:
    {
      m_pOI1DeleteCtrl.MouseUp();
      if (sameElement && m_pOI1DeleteCtrl.IsEnable())
      {
        DoDeletePOI(kROW1);
      }
    }
    break;
  case AdjustRouteHook_POI2DeleteBack:
  case AdjustRouteHook_POI2DeleteIcon:
    {
      m_pOI2DeleteCtrl.MouseUp();
      if (sameElement && m_pOI2DeleteCtrl.IsEnable())
      {
        DoDeletePOI(kROW2);
      }
    }
    break;
  case AdjustRouteHook_POI3DeleteBack:
  case AdjustRouteHook_POI3DeleteIcon:
    {
      m_pOI3DeleteCtrl.MouseUp();
      if (sameElement && m_pOI3DeleteCtrl.IsEnable())
      {
        DoDeletePOI(kROW3);
      }
    }
    break;
  case AdjustRouteHook_POI4DeleteBack:
  case AdjustRouteHook_POI4DeleteIcon:
    {
      m_pOI4DeleteCtrl.MouseUp();
      if (sameElement && m_pOI4DeleteCtrl.IsEnable())
      {
        DoDeletePOI(kROW4);
      }
    }
    break;
  case AdjustRouteHook_POI5DeleteBack:
  case AdjustRouteHook_POI5DeleteIcon:
    {
      m_pOI5DeleteCtrl.MouseUp();
      if (sameElement && m_pOI5DeleteCtrl.IsEnable())
      {
        DoDeletePOI(kROW5);
      }
    }
    break;
  case AdjustRouteHook_POI6EditStartBack:
  case AdjustRouteHook_POI6EditStartIcon:
    {
      m_pOI6EditStartCtrl.MouseUp();
      if (sameElement && m_pOI6EditStartCtrl.IsEnable())
      {
        DoAddPOI(kROW6);
      }      
    }
    break;
  case AdjustRouteHook_PlainLineCenter:
  case AdjustRouteHook_PlainLineLeft:
  case AdjustRouteHook_PlainLineRight:
    {
      //规划路线
      m_plainLineCtrl.MouseUp();
      if (sameElement && m_plainLineCtrl.IsEnable())
      {
        DoPlanRoute();
      }
    }
    break;
  case AdjustRouteHook_RouteTypeCenter:
  case AdjustRouteHook_RouteTypeIcon:
  case AdjustRouteHook_RouteTypeLabel:
  case AdjustRouteHook_RouteTypeRight:
  case AdjustRouteHook_RouteTypeLeft:
    {
      //路线类型
      m_routeTypeCtrl.MouseUp();
      if (sameElement && m_routeTypeCtrl.IsEnable())
      {
        CRouteTypeSelectHook::ShowHook(CViewHook::DHT_AdjustRouteHook, this, &OnRouteTypeSelect);
      }      
    }
    break;
  case AdjustRouteHook_SaveLineCenter:
  case AdjustRouteHook_SaveLineLeft:
  case AdjustRouteHook_SaveLineRight:
    {
      //保存到我的行程
      m_saveLineCtrl.MouseUp();
      if (sameElement && m_saveLineCtrl.IsEnable())
      {
        DoSaveNewData();
      }
    }
    break;
  case AdjustRouteHook_EditLineCenter:
  case AdjustRouteHook_EditLineLeft:
  case AdjustRouteHook_EditLineRight:
    {
      //编辑保存到我的行程
      m_editLineCtrl.MouseUp();
      if (sameElement && m_editLineCtrl.IsEnable())
      {
        DoSaveEditData();
      }
    }
    break;
  case AdjustRouteHook_AcceptLineCenter:
  case AdjustRouteHook_AcceptLineLeft:
  case AdjustRouteHook_AcceptLineRight:
    {
      //确定保存修改
      m_acceptLineCtrl.MouseUp();
      if (sameElement && m_acceptLineCtrl.IsEnable())
      {
        //TODO
        DoAcceptSave();
      }
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

bool CAdjustRouteHook::operator ()()
{
  return false;
}


void UeGui::CAdjustRouteHook::ClearRow( RowTag row )
{
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.ClearCaption();
      m_pOI1IconCtrl.SetVisible(false);      
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.ClearCaption();
      m_pOI2IconCtrl.SetVisible(false);
      break;
    }
  case kROW3 :
    {
      m_pOI3NameCtrl.ClearCaption();
      m_pOI3IconCtrl.SetVisible(false);
      break;
    }
  case kROW4 :
    {
      m_pOI4NameCtrl.ClearCaption();
      m_pOI4IconCtrl.SetVisible(false);
      break;
    }
  case kROW5 :
    {
      m_pOI5NameCtrl.ClearCaption();
      m_pOI5IconCtrl.SetVisible(false);
      break;
    }
  case kROW6 :
    {
      //这里不需要隐藏起点图标
      m_pOI6NameCtrl.ClearCaption();     
      break;
    }
  }
}

void UeGui::CAdjustRouteHook::ClearAllRow()
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
  m_addPOI1Ctrl.SetEnable(false);
  m_addPOI2Ctrl.SetEnable(false);
  m_addPOI3Ctrl.SetEnable(false);
  m_addPOI4Ctrl.SetEnable(false);
  m_addPOI5Ctrl.SetEnable(false);
}

void UeGui::CAdjustRouteHook::SetRowEnbale( RowTag row, bool enable )
{
  switch (row)
  {
  case kROW1 : 
    {
      m_line1Ctrl.SetEnable(enable);
      m_pOI1NameCtrl.SetEnable(enable);        
      m_pOI1DeleteCtrl.SetEnable(enable);
      m_pOI1IconCtrl.SetEnable(enable);
      if (enable)
      {
        m_pOI1IconCtrl.SetVisible(true);
      }
      m_addPOI1Ctrl.SetEnable(enable);
      break;
    }
  case kROW2 : 
    {
      m_line2Ctrl.SetEnable(enable);
      m_pOI2NameCtrl.SetEnable(enable);        
      m_pOI2DeleteCtrl.SetEnable(enable);
      m_pOI2IconCtrl.SetEnable(enable);
      if (enable)
      {
        m_pOI2IconCtrl.SetVisible(true);
      }
      m_addPOI2Ctrl.SetEnable(enable);
      break;
    }
  case kROW3 :
    {
      m_line3Ctrl.SetEnable(enable);
      m_pOI3NameCtrl.SetEnable(enable);        
      m_pOI3DeleteCtrl.SetEnable(enable);
      m_pOI3IconCtrl.SetEnable(enable);
      if (enable)
      {
        m_pOI3IconCtrl.SetVisible(enable);
      }
      m_addPOI3Ctrl.SetEnable(enable);
      break;
    }
  case kROW4 :
    {
      m_line4Ctrl.SetEnable(enable);
      m_pOI4NameCtrl.SetEnable(enable);        
      m_pOI4DeleteCtrl.SetEnable(enable);
      m_pOI4IconCtrl.SetEnable(enable);
      if (enable)
      {
        m_pOI4IconCtrl.SetVisible(enable);
      }
      m_addPOI4Ctrl.SetEnable(enable);
      break;
    }
  case kROW5 :
    {
      m_line5Ctrl.SetEnable(enable);
      m_pOI5NameCtrl.SetEnable(enable);        
      m_pOI5DeleteCtrl.SetEnable(enable);
      m_pOI5IconCtrl.SetEnable(enable);
      if (enable)
      {
        m_pOI5IconCtrl.SetVisible(enable);
      }
      break;
    }
  case kROW6 :
    {
      m_line6Ctrl.SetEnable(enable);
      m_pOI6NameCtrl.SetEnable(enable);        
      m_pOI6EditStartCtrl.SetEnable(true);
      m_pOI6IconCtrl.SetEnable(true);
      m_pOI6IconCtrl.SetVisible(true);
      break;
    }
  }
}

void UeGui::CAdjustRouteHook::OnDeletePOI( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CAdjustRouteHook* adjustRoutewHook = dynamic_cast<CAdjustRouteHook*>(sender);
    adjustRoutewHook->DeletePOI();
  }
}

void UeGui::CAdjustRouteHook::OnClearAll( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CAdjustRouteHook* adjustRoutewHook = dynamic_cast<CAdjustRouteHook*>(sender);
    adjustRoutewHook->ClearAll();
  }
}

void UeGui::CAdjustRouteHook::DeletePOI()
{
  int deleteIndex = GetDataIndex(m_selectRowTag);
  DeleteRouteData(deleteIndex);
  //删除POI之后重新显示数据
  ShowRouteData();
}

void UeGui::CAdjustRouteHook::ClearAll()
{
  ClearAllRow();
  ClearPOIList();  
  //定位到地图界面起点
  /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
  CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
  CAggHook::TurnTo(CViewHook::DHT_MapHook);
  ((CMapHook *)m_view->GetHook(CViewHook::DHT_MapHook))->DoEraseRoute();
  m_view->Refresh();
}

void UeGui::CAdjustRouteHook::ClearPOIList()
{
  m_POIList.clear();
}

//根据行号获取数据下标
unsigned int UeGui::CAdjustRouteHook::GetDataIndex( RowTag row )
{
  //默认获取最后一条数据位置
  unsigned int dataIndex = 0;
  switch (row)
  {
  case kROWBegin :
    {
      dataIndex = 0;
      break;
    }
  case kROW1 :
    {
      dataIndex = 5;
      break;
    }
  case kROW2 :
    {
      dataIndex = 4;
      break;
    }
  case kROW3 :
    {
      dataIndex = 3;
      break;
    }
  case kROW4 :
    {
      dataIndex = 2;
      break;
    }
  case kROW5 :
    {
      dataIndex = 1;
      break;
    }
  case kROW6 :
    {
      dataIndex = 0;
      break;
    }
  default:
    {
      dataIndex = 0;
      break;
    }
  }
  return dataIndex;
}

void UeGui::CAdjustRouteHook::AddPOIData( const POIItem& poi )
{
  //超过数量不允许再添加
  if (m_POIList.size() < MaxPOIListSize())
  {
    m_POIList.push_back(poi);
  }
}

void UeGui::CAdjustRouteHook::AddPOIData( const POIDataList& poiList )
{
  POIDataList::const_iterator iter = poiList.begin();
  for (; iter != poiList.end(); ++iter)
  {
    AddPOIData(*iter);
  }
}

void UeGui::CAdjustRouteHook::InsertPOIData( int position, const UeQuery::SQLRecord* data )
{ 
  //在指定位置添加数据,超过最大数目后不允许再添加
  if (m_POIList.size() >= MaxPOIListSize())
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
  ::strcpy(poi.m_name, data->m_uniName);
  int len = ::strlen(poi.m_name);
  if(poi.m_name[len] == 0)
  {
    poi.m_name[len] = '\n';
  }
  poi.m_pos.m_x = data->m_x;
  poi.m_pos.m_y = data->m_y;
  //poi.m_linkIdx = data->m_linkIdx;
  //poi.m_parcelIdx = data->m_parcelIdx;


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
  m_addRowTag = kROWEnd;
}

//读取要添加的POI位置,下标从0开始
unsigned int UeGui::CAdjustRouteHook::GetInsertPosition()
{
  return GetDataIndex(m_addRowTag);
}

void UeGui::CAdjustRouteHook::DoDeletePOI( RowTag row )
{
  m_selectRowTag = row;
  ////弹出消息对话框确认是否删除该POI
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_AdjustRouteHook, &UeGui::CAdjustRouteHook::OnDeletePOI);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "确定要删除该导航点?", dialogEvent);
}

void UeGui::CAdjustRouteHook::DoClearAll()
{
  ////弹出消息对话框确认是否删除所有POI
  m_isNeedRefesh = false;
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_AdjustRouteHook, &UeGui::CAdjustRouteHook::OnClearAll);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "当前路线将被删除", dialogEvent);
}

void UeGui::CAdjustRouteHook::ShowRowData( RowTag row, const char* value )
{
  SetRowEnbale(row, true);
  switch (row)
  {
  case kROW1 : 
    {
      m_pOI1NameCtrl.SetCaption(value);
      break;
    }
  case kROW2 : 
    {
      m_pOI2NameCtrl.SetCaption(value);
      m_addPOI1Ctrl.SetEnable(true);
      break;
    }
  case kROW3 : 
    {
      m_pOI3NameCtrl.SetCaption(value);
      m_addPOI2Ctrl.SetEnable(true);
      break;
    }
  case kROW4 : 
    {
      m_pOI4NameCtrl.SetCaption(value);
      m_addPOI3Ctrl.SetEnable(true);
      break;
    }
  case kROW5 : 
    {
      m_pOI5NameCtrl.SetCaption(value);
      m_addPOI4Ctrl.SetEnable(true);
      break;
    }
  case kROW6 : 
    {
      m_pOI6NameCtrl.SetCaption(value);
      m_addPOI5Ctrl.SetEnable(true);
      break;
    }
  default:
    {
      break;
    }
  }
}

void UeGui::CAdjustRouteHook::OnInsertPOIData( void* sender, const UeQuery::SQLRecord * data )
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

void UeGui::CAdjustRouteHook::OnRouteTypeSelect( CAggHook* sender, unsigned int routeType )
{
  if (NULL == sender)
  {
    return;
  }
  CAdjustRouteHook* adjustRouteHook = static_cast<CAdjustRouteHook*>(sender);
  adjustRouteHook->SetSelectRouteType(routeType);
}

void UeGui::CAdjustRouteHook::OnDataMove( CAggHook* sender, MoveType moveType )
{
  if (NULL == sender)
  {
    return;
  }
  CAdjustRouteHook* adjustRouteHook = static_cast<CAdjustRouteHook*>(sender);
  adjustRouteHook->DataMove(moveType);
  adjustRouteHook->ShowRouteData();
}

void UeGui::CAdjustRouteHook::OnGetRouteName( void* sender, const UeQuery::SQLRecord * data )
{
  if (NULL == sender)
  {
    return;
  }

  CViewHook::m_curHookType = CViewHook::DHT_AdjustRouteHook;
  IView *view = IView::GetView();
  view->RefreshUI();

  CAdjustRouteHook* adjustRouteHook = static_cast<CAdjustRouteHook*>(sender);
  adjustRouteHook->SaveJourneyData(data->m_asciiName);  
}

void UeGui::CAdjustRouteHook::SaveJourneyData( const char* journeyName )
{
  CMessageDialogEvent dialogEvent;
  dialogEvent.m_senderHookType = CViewHook::DHT_AdjustRouteHook;
  dialogEvent.m_senderHook = this;

  if ((NULL == journeyName) || (0 == ::strlen(journeyName)))
  {
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "行程名称不能为空 !", dialogEvent);
    //TODO: 暂时先用Sleep，将来要移除掉，定时功能由消息框自己完成
    Sleep(1000);
    CMessageDialogHook::CloseMessageDialog();
    return;
  }
  if (0 == m_POIList.size())
  {
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "行程路线不能为空 !", dialogEvent);
    Sleep(1000);
    CMessageDialogHook::CloseMessageDialog();
    return;
  }

  switch (m_windowModel)
  {
  case WAppendModel :
    {
      //调用我的行程管理hook接口
      CUserDataWrapper::Get().AddJourneyData(journeyName, m_routeType, m_POIList);
      CMessageDialogHook::ShowMessageDialog(MB_NONE, "数据保存成功 !", dialogEvent);
      Sleep(1000);
      CMessageDialogHook::CloseMessageDialog();
      break;
    }
  case WEditModel :
    {
      ::strcpy(m_JourneyName,journeyName);
      break;
    }
  default:
    {
      assert(false);
      break;
    }
  }
}

void UeGui::CAdjustRouteHook::DoAddPOI( RowTag row )
{
  if (m_POIList.size() >= MAXPOSTIONCOUNT)
  {
    CMessageDialogEvent dialogEvent(this, DHT_AdjustRouteHook);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "已经设置了五个目的地", dialogEvent, 2);
    return;
  }
  //记录当前要添加的行标记
  m_addRowTag = row;
  //切换到查询页面查询导航点
  //CQueryMenuHook::gotoQueryMenuHook(CViewHook::DHT_AdjustRouteHook, this, OnInsertPOIData, "查找导航点");
}

void UeGui::CAdjustRouteHook::GetRouteData()
{
  m_POIList.clear();
  //获取起点
  POIItem startPos;
  startPos.m_type = UeRoute::PT_Start;
  m_route->GetPosition(startPos);
  if(startPos.m_type != UeRoute::PT_Invalid)
  {
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
      m_route->GetPosition(midPos, i);
      if(midPos.m_type != UeRoute::PT_Invalid)
      {
        m_POIList.push_back(midPos);
      }
    }
  }
  //获取终点
  POIItem endPos;
  endPos.m_type = UeRoute::PT_End;
  m_route->GetPosition(endPos);
  if(endPos.m_type != UeRoute::PT_Invalid)
  {
    m_POIList.push_back(endPos);
  }
  SetRouteType(m_route->GetMethod());
}

void CAdjustRouteHook::ShowRouteData()
{
  ClearAllRow();
  int row = kROW6;
  POIDataList::iterator iter = m_POIList.begin();
  for (; iter != m_POIList.end(); ++iter)
  {
    unsigned int slen = ::strlen(iter->m_name);
    if (0 == ::strlen(iter->m_name))
    {
      ShowRowData((RowTag)row, "未命名");
    }
    else
    {
      char routeName[128] = {};
      unsigned int size = slen < sizeof(routeName) ? slen : sizeof(routeName);
      if (size > 0)
      {
        ::memcpy(routeName, iter->m_name, size);
        if (routeName[size - 1] == '\n')
        {
          routeName[size - 1] = 0;
        }
      }                  
      ShowRowData((RowTag)row, routeName);
    }    
    --row;
  }
}

unsigned int UeGui::CAdjustRouteHook::MaxPOIListSize()
{
  return kROW6;
}

void UeGui::CAdjustRouteHook::DeleteRouteData( int dataIndex )
{
  if (m_POIList.size() == 0)
  {
    return;
  }
  if ((dataIndex >= 0) && (dataIndex <= m_POIList.size() - 1))
  {
    POIDataList::iterator iter = m_POIList.begin();
    m_POIList.erase(iter + dataIndex);
  }
}

void UeGui::CAdjustRouteHook::SetJourneyName( const char *journeyName )
{
  if (journeyName)
  {
    //注意越界检查
    unsigned int srclen = ::strlen(journeyName);
    unsigned int destlen = sizeof(m_JourneyName) - 1;
    unsigned int copyLen = destlen > srclen ? srclen : destlen;
    ::strncpy(m_JourneyName, journeyName, copyLen);
    m_JourneyName[copyLen] = 0;
  }
}

void UeGui::CAdjustRouteHook::SetSelectRouteType( unsigned int routeType )
{
  unsigned int routeMethod = m_routeType;
  routeMethod &= 0xFFFFFF00;  
  routeMethod |= routeType;
  SetRouteType(routeMethod);
}

void UeGui::CAdjustRouteHook::SetRouteType( unsigned int routeType )
{  
  m_routeType = routeType;
  
  //临时控件 显示控制推荐路线的文字
  CUiBitButton _m_routeTypeCtrl;
  _m_routeTypeCtrl.SetLabelElement(GetGuiElement(AdjustRouteHook_RouteTypeLabel));
  
  if (routeType & RW_Fast) 
  {
    /// 高速优先    
    _m_routeTypeCtrl.SetCaption(m_routeType1.GetCaption());
  }
  else if (routeType & RW_Short)
  {
    /// 最短路径--一般道路
    _m_routeTypeCtrl.SetCaption(m_routeType2.GetCaption());
  }
  else if (routeType & RW_Economic)
  {
    /// 经济路线
    _m_routeTypeCtrl.SetCaption(m_routeType3.GetCaption());
  }
  else if (routeType & RW_Optimal)
  {
    /// 推荐路线
    _m_routeTypeCtrl.SetCaption(m_routeType4.GetCaption());
  }
}

void UeGui::CAdjustRouteHook::DataMove( MoveType moveType )
{
  //根据选择行来获取数据要移动的数据下标索引
  unsigned int dataIndex = GetDataIndex(m_selectRowTag);
  if ((dataIndex < 0) || (dataIndex > m_POIList.size() - 1))
  {
    return;
  }
  //目标移动位置
  unsigned int targetIndex = dataIndex;
  switch (moveType)
  {
  case MT_Up :
    {
      //上移
      targetIndex = dataIndex + 1;

      break;
    }
  case MT_Down :
    {
      //下移
      targetIndex = dataIndex - 1;
      break;
    }
  default:
    {
      break;
    }
  }
  if ((targetIndex == dataIndex) || (targetIndex < 0) || (targetIndex > m_POIList.size() - 1))
  {
    return;
  }
  POIItem temPOIItem = m_POIList[targetIndex];
  m_POIList[targetIndex] = m_POIList[dataIndex];
  m_POIList[dataIndex] = temPOIItem;
}

void UeGui::CAdjustRouteHook::DoDataMove( RowTag row )
{
  m_selectRowTag = row;
  //如果数据量小于2条则不允许移动
  if (m_POIList.size() <= 2)
  {
    return;
  }
  //根据选择行来获取数据下标索引
  unsigned int dataIndex = GetDataIndex(row);
  if (dataIndex > m_POIList.size() - 1)
  {
    return;
  }
  //起点不允许移动
  if (0 == dataIndex)
  {
    return;
  }
  if (1 == dataIndex)
  {
    CUpwardOrDownwardHook::ShowHook(CViewHook::DHT_AdjustRouteHook, this, &OnDataMove, ME_DownEnd);
  }
  else if ((m_POIList.size() - 1) == dataIndex)
  {
    CUpwardOrDownwardHook::ShowHook(CViewHook::DHT_AdjustRouteHook, this, &OnDataMove, ME_UpEnd);
  }
  else
  {
    CUpwardOrDownwardHook::ShowHook(CViewHook::DHT_AdjustRouteHook, this, &OnDataMove, ME_None);
  }  
}

void UeGui::CAdjustRouteHook::DoSaveNewData()
{
  //打开名称输入对话框
  //CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
  //  CViewHook::DHT_AdjustRouteHook, this, OnGetRouteName, "编辑名称");
}

void UeGui::CAdjustRouteHook::DoSaveEditData()
{
  //打开名称输入对话框
  //CInputHook::gotoCurInputMethod(CInputHook::IM_Edit,
  //  CViewHook::DHT_AdjustRouteHook, this, OnGetRouteName, "编辑名称", m_JourneyName);
}

void UeGui::CAdjustRouteHook::DoPlanRoute()
{
  short planState = m_route->GetPlanState();
  if ((UeRoute::PS_DemoGuidance == planState) || (UeRoute::PS_RealGuidance == planState))
  {    
    m_isNeedRefesh = false;
    CMessageDialogEvent dialogEvent(this, CViewHook::DHT_AdjustRouteHook, &UeGui::CAdjustRouteHook::OnPlanRouteQuestion);
    CMessageDialogHook::ShowMessageDialog(MB_INFORMATION, "路线导航过程将被终止?", dialogEvent);
    return;
  }
  else
  {
    //重新规划路线
    PlanRoute();
  }  
}

void UeGui::CAdjustRouteHook::PlanRoute()
{
  unsigned int POICount = m_POIList.size();
  if (POICount < 2)
  {
    return;
  }

  //规划路线方式,调用maphook接口
  CMapHook* mapHook = (CMapHook*)m_view->GetHook(CViewHook::DHT_MapHook);
  mapHook->IsReSetMethod(false);

  CGeoPoint<short> scrPoint;
  CGeoPoint<long> point;

  //设置路线类型。
  m_route->SetMethod(m_routeType);
  //设起点
  POIItem startPos = m_POIList[0];
  point.m_x = startPos.m_pos.m_x;
  point.m_y = startPos.m_pos.m_y;
  //选择了起点后，保存其名称
  CViewCanvas::SetSelectedName(startPos.m_name);
  if (m_route->SetPosition(startPos) != PEC_Success)
  {
    return;
  }

  //添加经由点
  if (POICount >= 3)
  {
    for (unsigned int i = 1; i <= POICount - 2; ++i)
    {
      POIItem middlePos = m_POIList[i];
      middlePos.m_type = UeRoute::PT_Middle;
      if (m_route->SetPosition(middlePos) != PEC_Success)
      {
        return;
      }
    }
  }

  //设目的地
  POIItem endPos = m_POIList[m_POIList.size() - 1];
  point.m_x = endPos.m_pos.m_x;
  point.m_y = endPos.m_pos.m_y;
  //挑选了终点后，保存其名称
  CViewCanvas::SetSelectedName(endPos.m_name);
  if (m_route->SetPosition(endPos) != PEC_Success)
  {
    return;
  }

  CMessageDialogEvent dialogEvent(this, DHT_AdjustRouteHook, NULL);
  CMessageDialogHook::ShowMessageDialog(MB_NONE, "规划中，请稍候...", dialogEvent); 
  CMessageDialogHook::CloseMessageDialog();
  //设目的地并规划路线
  unsigned int result = m_route->RoutePlan(false);
  if (m_view && m_view->MainState())
  {
    m_view->MainState()->RefreshLayerData();
  }
  if (UeRoute::PEC_TooShortest == result)
  {
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "起点和目的地距离太近", dialogEvent, 2);
  }
  else
  {
    if (result == UeRoute::PEC_Success)
    {
      CAggHook::GoToMapHook();
      //切换到路线规划成功倒计时界面
      mapHook->SwitchToRoutePlanReadyGUI();
      m_view->Refresh();
    }
    else
    {
      CMessageDialogHook::ShowMessageDialog(MB_NONE, "路径规划失败", dialogEvent, 2);
      m_route->RemovePosition();
    }
    m_view->Refresh();
  }
}

void UeGui::CAdjustRouteHook::SetEditEvent( HookType senderHookType, CAggHook* sender, const char *journeyName, const unsigned int routeType, EditRouteEvent editRouteEvent )
{
  m_senderHookType = senderHookType;
  m_senderHook = sender;
  m_editRouteEvent = editRouteEvent;
  SetJourneyName(journeyName);
  SetRouteType(routeType);
}

void UeGui::CAdjustRouteHook::OnPlanRouteQuestion( CAggHook* sender, ModalResultType modalResult )
{
  if (MR_OK == modalResult)
  {
    CAdjustRouteHook* adjustRouteHook = dynamic_cast<CAdjustRouteHook*>(sender);
    adjustRouteHook->PlanRoute();
  }
}

//确定
void UeGui::CAdjustRouteHook::DoAcceptSave()
{
  //由于编辑数据是外部调用，本类为被调，所以使用委托，这里只做回调处理
  if (m_editRouteEvent)
  {
    (*m_editRouteEvent)(m_senderHook, m_JourneyName, m_routeType, m_POIList);
  }
}