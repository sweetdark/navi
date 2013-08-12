#include "ddtservicehook.h"
using namespace UeGui;

CDdtServiceHook::CDdtServiceHook()
{
  m_strTitle = "·þÎñ";
  m_vecHookFile.push_back(_T("ddtservicehook.bin"));
}

CDdtServiceHook::~CDdtServiceHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDdtServiceHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_OperationHelpBtn,	"OperationHelpBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_OperationLabel,	"OperationLabel"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_OperationBack,	"OperationBack"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_FunctionUpdateBtn,	"FunctionUpdateBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_FunctionUpdateLabel,	"FunctionUpdateLabel"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_FunctionUpdateBack,	"FunctionUpdateBack"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_UpdateServiceBack,	"UpdateServiceBack"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_UpdateServiceLabel,	"UpdateServiceLabel"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_UpdateServiceBtn,	"UpdateServiceBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_VersionCheckBack,	"VersionCheckBack"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_VersionCheckLabel,	"VersionCheckLabel"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_VersionCheckBtn,	"VersionCheckBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_LongSplit,	"LongSplit"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_VerticalSplit1,	"VerticalSplit1"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_VerticalSplit2,	"VerticalSplit2"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_VerticalSplit3,	"VerticalSplit3"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_Back1,	"Back1"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_Back2,	"Back2"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_Back3,	"Back3"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehook_Back4,	"Back4"));
}

void CDdtServiceHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_functionUpdateBtnCtrl.SetCenterElement(GetGuiElement(ddtservicehook_FunctionUpdateBtn));
  m_functionUpdateLabelCtrl.SetCenterElement(GetGuiElement(ddtservicehook_FunctionUpdateLabel));
  m_functionUpdateBackCtrl.SetCenterElement(GetGuiElement(ddtservicehook_FunctionUpdateBack));

  m_operationHelpBtnCtrl.SetCenterElement(GetGuiElement(ddtservicehook_OperationHelpBtn));
  m_operationLabelCtrl.SetCenterElement(GetGuiElement(ddtservicehook_OperationLabel));
  m_operationBackCtrl.SetCenterElement(GetGuiElement(ddtservicehook_OperationBack));

  m_updateServiceBtnCtrl.SetCenterElement(GetGuiElement(ddtservicehook_UpdateServiceBtn));
  m_updateServiceLabelCtrl.SetCenterElement(GetGuiElement(ddtservicehook_UpdateServiceLabel));
  m_updateServiceBackCtrl.SetCenterElement(GetGuiElement(ddtservicehook_UpdateServiceBack));

  m_versionCheckBtnCtrl.SetCenterElement(GetGuiElement(ddtservicehook_VersionCheckBtn));
  m_versionCheckLabelCtrl.SetCenterElement(GetGuiElement(ddtservicehook_VersionCheckLabel));
  m_versionCheckBackCtrl.SetCenterElement(GetGuiElement(ddtservicehook_VersionCheckBack));
}

short CDdtServiceHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case ddtservicehook_FunctionUpdateBtn:
  case ddtservicehook_FunctionUpdateBack:
  case ddtservicehook_FunctionUpdateLabel:
    {
      m_functionUpdateBackCtrl.MouseDown();
      AddRenderElements(GetGuiElement(ddtservicehook_FunctionUpdateBack));
      AddRenderUiControls(&m_functionUpdateBtnCtrl);
      AddRenderUiControls(&m_functionUpdateLabelCtrl);      
    }
    break;
  case ddtservicehook_OperationHelpBtn:
  case ddtservicehook_OperationBack:
  case ddtservicehook_OperationLabel:
    {
      m_operationBackCtrl.MouseDown();
      AddRenderElements(GetGuiElement(ddtservicehook_OperationBack));
      AddRenderUiControls(&m_operationHelpBtnCtrl);
      AddRenderUiControls(&m_operationLabelCtrl);
    }
    break;
  case ddtservicehook_UpdateServiceBack:
  case ddtservicehook_UpdateServiceBtn:
  case ddtservicehook_UpdateServiceLabel:
    {
      m_updateServiceBackCtrl.MouseDown();
      AddRenderElements(GetGuiElement(ddtservicehook_UpdateServiceBack));
      AddRenderUiControls(&m_updateServiceLabelCtrl);
      AddRenderUiControls(&m_updateServiceBtnCtrl);
    }
    break;
  case ddtservicehook_VersionCheckBtn:
  case ddtservicehook_VersionCheckBack:
  case ddtservicehook_VersionCheckLabel:
    {
      m_versionCheckBackCtrl.MouseDown();
      AddRenderElements(GetGuiElement(ddtservicehook_VersionCheckBack));
      AddRenderUiControls(&m_versionCheckBtnCtrl);
      AddRenderUiControls(&m_versionCheckLabelCtrl);
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

short CDdtServiceHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool canDo = (ctrlType == m_downElementType)?true:false;
  switch(m_downElementType)
  {
  case ddtservicehook_FunctionUpdateBack:
  case ddtservicehook_FunctionUpdateBtn:
  case ddtservicehook_FunctionUpdateLabel:
    {
      m_functionUpdateBackCtrl.MouseUp();
      if (canDo)
      {
        TurnTo(CViewHook::DHT_FunctionUpdateHook);
      }
    }
    break;
  case ddtservicehook_OperationBack:
  case ddtservicehook_OperationHelpBtn:
  case ddtservicehook_OperationLabel:
    {
      m_operationBackCtrl.MouseUp();
      if (canDo)
      {
        TurnTo(DHT_OperationHelpHook);
      }
    }
    break;
  case ddtservicehook_UpdateServiceBack:
  case ddtservicehook_UpdateServiceBtn:
  case ddtservicehook_UpdateServiceLabel:
    {
      m_updateServiceBackCtrl.MouseUp();
      if (canDo)
      {
        TurnTo(DHT_UpdateServiceHook);
      }
    }
    break;
  case ddtservicehook_VersionCheckBack:
  case ddtservicehook_VersionCheckBtn:
  case ddtservicehook_VersionCheckLabel:
    {
      m_versionCheckBackCtrl.MouseUp();
      if (canDo)
      {
        TurnTo(DHT_VersionCheckHook);
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

void CDdtServiceHook::Load()
{}

