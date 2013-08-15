#include "operationhelphook.h"
#include "guioperationright.h"

using namespace UeGui;

COperationHelpHook::COperationHelpHook()
{
  m_strTitle = "²Ù×÷°ïÖú";
  m_vecHookFile.push_back(_T("operationhelphook.bin"));
}

COperationHelpHook::~COperationHelpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void COperationHelpHook::Load()
{}

void COperationHelpHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapScanBtn,	"MapScanBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapScanText,	"MapScanText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapScanBack,	"MapScanBack"));

  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapNavigationBtn,	"MapNavigationBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapNavigationText,	"MapNavigationText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_MapNavigationBack,	"MapNavigationBack"));

  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FunctionBtn,	"FunctionBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FunctionText,	"FunctionText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FunctionBack,	"FunctionBack"));

  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ServiceBtn,	"ServiceBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ServiceText,	"ServiceText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_ServiceBack,	"ServiceBack"));

  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FastBtn,	"FastBtn"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FastText,	"FastText"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_FastBack,	"FastBack"));

  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_LongSplit,	"LongSplit"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_VerticalSplit1,	"VerticalSplit1"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_VerticalSplit2,	"VerticalSplit2"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_VerticalSplit3,	"VerticalSplit3"));

  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_Back2,	"Back2"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_Back3,	"Back3"));
  m_ctrlNames.insert(GuiName::value_type(OperationHelpHook_Back4,	"Back4"));
}

void COperationHelpHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_fastBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_FastBtn));
  m_fastBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_FastText));
  m_fastBackCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_FastBack));

  m_functionBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_FunctionBtn));
  m_functionBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_FunctionText));
  m_functionBackCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_FunctionBack));

  m_mapNavigationBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_MapNavigationBtn));
  m_mapNavigationBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_MapNavigationText));
  m_mapNavigationBackCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_MapNavigationBack));

  m_mapScanBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_MapScanBtn));
  m_mapScanBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_MapScanText));
  m_mapScanBackCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_MapScanBack));

  m_serviceBtnCtrl.SetIconElement(GetGuiElement(OperationHelpHook_ServiceBtn));
  m_serviceBtnCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_ServiceText));
  m_serviceBackCtrl.SetCenterElement(GetGuiElement(OperationHelpHook_ServiceBack));
}

short COperationHelpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case OperationHelpHook_FastBtn:
  case OperationHelpHook_FastText:
  case OperationHelpHook_FastBack:
    {
      m_fastBackCtrl.MouseDown();
      AddRenderUiControls(&m_fastBackCtrl);
      AddRenderUiControls(&m_fastBtnCtrl);
    }
    break;
  case OperationHelpHook_FunctionBtn:
  case OperationHelpHook_FunctionText:
  case  OperationHelpHook_FunctionBack:
    {
      m_functionBackCtrl.MouseDown();
      AddRenderUiControls(&m_functionBackCtrl);
      AddRenderUiControls(&m_functionBtnCtrl);
    }
    break;
  case OperationHelpHook_MapNavigationBtn:
  case OperationHelpHook_MapNavigationText:
  case OperationHelpHook_MapNavigationBack:
    {
      m_mapNavigationBackCtrl.MouseDown();
      AddRenderUiControls(&m_mapNavigationBackCtrl);
      AddRenderUiControls(&m_mapNavigationBtnCtrl);
    }
    break;
  case OperationHelpHook_MapScanBtn:
  case OperationHelpHook_MapScanText:
  case OperationHelpHook_MapScanBack:
    {
      m_mapScanBackCtrl.MouseDown();
      AddRenderUiControls(&m_mapScanBackCtrl);
      AddRenderUiControls(&m_mapScanBtnCtrl);
    }
    break;
  case OperationHelpHook_ServiceBtn:
  case OperationHelpHook_ServiceText:
  case OperationHelpHook_ServiceBack:
    {
      m_serviceBackCtrl.MouseDown();
      AddRenderUiControls(&m_serviceBackCtrl);
      AddRenderUiControls(&m_serviceBtnCtrl);
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

short COperationHelpHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short COperationHelpHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case OperationHelpHook_FastBtn:
  case OperationHelpHook_FastText:
  case OperationHelpHook_FastBack:
    {
      m_fastBackCtrl.MouseUp();
      TurnTo(DHT_FastHandleHelpHook);
    }
    break;
  case OperationHelpHook_FunctionBtn:
  case OperationHelpHook_FunctionText:
  case  OperationHelpHook_FunctionBack:
    {
      m_functionBackCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        TurnTo(DHT_BasicFunctionHook);
      }
    }
    break;
  case OperationHelpHook_MapNavigationBtn:
  case OperationHelpHook_MapNavigationText:
  case OperationHelpHook_MapNavigationBack:
    {
      m_mapNavigationBackCtrl.MouseUp();
      if (ctrlType = m_downElementType)
      {
        TurnTo(DHT_MapNavigationHook);
      }
    }
    break;
  case OperationHelpHook_MapScanBtn:
  case OperationHelpHook_MapScanText:
  case OperationHelpHook_MapScanBack:
    {
      m_mapScanBackCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        TurnTo(DHT_MapScanHook);
      }
    }
    break;
  case OperationHelpHook_ServiceBtn:
  case OperationHelpHook_ServiceText:
  case OperationHelpHook_ServiceBack:
    {
      m_serviceBackCtrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        TurnTo(DHT_DdtServiceHelpHook);
      }
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