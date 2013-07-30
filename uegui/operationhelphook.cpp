#include "operationhelphook.h"
#include "guioperationright.h"

using namespace UeGui;

COperationHelpHook::COperationHelpHook()
{
}

COperationHelpHook::~COperationHelpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void COperationHelpHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring COperationHelpHook::GetBinaryFileName()
{
  return _T("operationhelphook.bin");
}

void COperationHelpHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapScanBtn,	"MapScanBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapScanText,	"MapScanText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapNavigationBtn,	"MapNavigationBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapNavigationText,	"MapNavigationText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FunctionBtn,	"FunctionBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FuntionText,	"FuntionText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ServiceBtn,	"ServiceBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ServiceText,	"ServiceText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FastBtn,	"FastBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FastText,	"FastText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ShowBtn,	"ShowBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ShowText,	"ShowText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_CityBtn,	"CityBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_CityText,	"CityText"));
}

void COperationHelpHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_BackButton));
  m_backGroundCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_BackGround));
  m_backGroundTextCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_BackGroundText));
  m_cityBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_CityBtn));
  m_cityBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_CityText));
  m_fastBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_FastBtn));
  m_fastBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_FastText));
  m_functionBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_FunctionBtn));
  m_functionBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_FuntionText));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_GotoMapBtn));
  m_mapNavigationBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_MapNavigationBtn));
  m_mapNavigationBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_MapNavigationText));
  m_mapScanBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_MapScanBtn));
  m_mapScanBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_MapScanText));
  m_serviceBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_ServiceBtn));
  m_serviceBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_ServiceText));
  m_showBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_ShowBtn));
  m_showBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_ShowText));
}

bool COperationHelpHook::operator ()()
{
  return false;
}


void UeGui::COperationHelpHook::Init()
{
  m_mapScanBtnCtrl.SetEnable(false);
  m_mapNavigationBtnCtrl.SetEnable(false);
  m_functionBtnCtrl.SetEnable(false);
  m_serviceBtnCtrl.SetEnable(false);
  m_fastBtnCtrl.SetEnable(false);
  m_showBtnCtrl.SetEnable(false);
  m_cityBtnCtrl.SetEnable(false);

  if (CGuiOperationRight::HaveRight(OR_MAPBROWSE))
  {
    m_mapScanBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_MAPNAVIGATION))
  {
    m_mapNavigationBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_BASICFUCTION))
  {
    m_functionBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_DDTSERVICEINFO))
  {
    m_serviceBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_FASTDO))
  {
    m_fastBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_3DSHOW))
  {
    m_showBtnCtrl.SetEnable(true);
  }
  if (CGuiOperationRight::HaveRight(OR_3DCITY))
  {
    m_cityBtnCtrl.SetEnable(true);
  }
}

short COperationHelpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case OperationHelpHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
      AddRenderUiControls(&m_backButtonCtrl);
    }
    break;
  case OperationHelpHook_BackGround:
  case OperationHelpHook_BackGroundText:
    break;
  case OperationHelpHook_CityBtn:
  case OperationHelpHook_CityText:
    {
      m_cityBtnCtrl.MouseDown();
      AddRenderUiControls(&m_cityBtnCtrl);
    }
    break;
  case OperationHelpHook_FastBtn:
  case OperationHelpHook_FastText:
    {
      m_fastBtnCtrl.MouseDown();
      AddRenderUiControls(&m_fastBtnCtrl);
    }
    break;
  case OperationHelpHook_FunctionBtn:
  case OperationHelpHook_FuntionText:
    {
      m_functionBtnCtrl.MouseDown();
      AddRenderUiControls(&m_functionBtnCtrl);
    }
    break;
  case OperationHelpHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case OperationHelpHook_MapNavigationBtn:
  case OperationHelpHook_MapNavigationText:
    {
      m_mapNavigationBtnCtrl.MouseDown();
      AddRenderUiControls(&m_mapNavigationBtnCtrl);
    }
    break;
  case OperationHelpHook_MapScanBtn:
  case OperationHelpHook_MapScanText:
    {
      m_mapScanBtnCtrl.MouseDown();
      AddRenderUiControls(&m_mapScanBtnCtrl);
    }
    break;
  case OperationHelpHook_ServiceBtn:
  case OperationHelpHook_ServiceText:
    {
      m_serviceBtnCtrl.MouseDown();
     AddRenderUiControls(&m_serviceBtnCtrl);
    }
    break;
  case OperationHelpHook_ShowBtn:
  case OperationHelpHook_ShowText:
    {
      m_showBtnCtrl.MouseDown();
      AddRenderUiControls(&m_showBtnCtrl);
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
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short COperationHelpHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short COperationHelpHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case OperationHelpHook_BackButton:
    {
      m_backButtonCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType = CViewHook::DHT_OperationHelpHook;
        CViewHook::m_curHookType = CViewHook::DHT_DdtserviceHook;*/
        CAggHook::Return();
      }      
    }
    break;
  case OperationHelpHook_BackGround:
  case OperationHelpHook_BackGroundText:
    break;
  case OperationHelpHook_CityBtn:
  case OperationHelpHook_CityText:
    {
      m_cityBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_cityBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_City3DHook;*/
        CAggHook::TurnTo(DHT_City3DHook);
      }
    }
    break;
  case OperationHelpHook_FastBtn:
  case OperationHelpHook_FastText:
    {
      m_fastBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_fastBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_FastHandleHelpHook;*/
        CAggHook::TurnTo(DHT_FastHandleHelpHook);
        ((CAggHook*)m_view->GetHook(CViewHook::DHT_FastHandleHelpHook))->Init();
      }
    }
    break;
  case OperationHelpHook_FunctionBtn:
  case OperationHelpHook_FuntionText:
    {
      m_functionBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_functionBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_BasicFunctionHook;*/
        CAggHook::TurnTo(DHT_BasicFunctionHook);
        ((CAggHook*)m_view->GetHook(CViewHook::DHT_BasicFunctionHook))->Init();
      }
    }
    break;
  case OperationHelpHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseUp();
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
        ((CAggHook *)m_view->GetHook(CViewHook::DHT_MapHook))->ComeBack();
      }      
    }
    break;
  case OperationHelpHook_MapNavigationBtn:
  case OperationHelpHook_MapNavigationText:
    {
      m_mapNavigationBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_mapNavigationBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapNavigationHook;*/
        CAggHook::TurnTo(DHT_MapNavigationHook);
        ((CAggHook*)m_view->GetHook(CViewHook::DHT_MapNavigationHook))->Init();
      }
    }
    break;
  case OperationHelpHook_MapScanBtn:
  case OperationHelpHook_MapScanText:
    {
      m_mapScanBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_mapScanBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_MapScanHook;*/
        CAggHook::TurnTo(DHT_MapScanHook);
        ((CAggHook*)m_view->GetHook(CViewHook::DHT_MapScanHook))->Init();
      }
    }
    break;
  case OperationHelpHook_ServiceBtn:
  case OperationHelpHook_ServiceText:
    {
      m_serviceBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_serviceBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_DdtServiceHelpHook;*/
        CAggHook::TurnTo(DHT_DdtServiceHelpHook);
      }
    }
    break;
  case OperationHelpHook_ShowBtn:
  case OperationHelpHook_ShowText:
    {
      m_showBtnCtrl.MouseUp();
      if ((ctrlType == m_downElementType) && (m_showBtnCtrl.IsEnable()))
      {
        /*CViewHook::m_prevHookType = CViewHook::m_curHookType;
        CViewHook::m_curHookType = CViewHook::DHT_Demonstration3DHook;*/
        CAggHook::TurnTo(DHT_Demonstration3DHook);
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
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}