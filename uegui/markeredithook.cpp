#include "markeredithook.h"

using namespace UeGui;

CMarkerEditHook::CMarkerEditHook()
{
  m_strTitle = "Ñ¡ÔñÍ¼±ê";
  m_vecHookFile.push_back(_T("markeredithook.bin"));
}

CMarkerEditHook::~CMarkerEditHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CMarkerEditHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark01,	"mark01"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark02,	"mark02"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark03,	"mark03"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark04,	"mark04"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark05,	"mark05"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark06,	"mark06"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark07,	"mark07"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark08,	"mark08"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark09,	"mark09"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark10,	"mark10"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark11,	"mark11"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark12,	"mark12"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark13,	"mark13"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark14,	"mark14"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark15,	"mark15"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark16,	"mark16"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark17,	"mark17"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark18,	"mark18"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark19,	"mark19"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark20,	"mark20"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark21,	"mark21"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark22,	"mark22"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark23,	"mark23"));
  m_ctrlNames.insert(GuiName::value_type(MarkerEditHook_mark24,	"mark24"));
}

void CMarkerEditHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_mark01Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark01));
  m_mark02Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark02));
  m_mark03Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark03));
  m_mark04Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark04));
  m_mark05Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark05));
  m_mark06Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark06));
  m_mark07Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark07));
  m_mark08Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark08));
  m_mark09Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark09));
  m_mark10Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark10));
  m_mark11Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark11));
  m_mark12Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark12));
  m_mark13Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark13));
  m_mark14Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark14));
  m_mark15Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark15));
  m_mark16Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark16));
  m_mark17Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark17));
  m_mark18Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark18));
  m_mark19Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark19));
  m_mark20Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark20));
  m_mark21Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark21));
  m_mark22Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark22));
  m_mark23Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark23));
  m_mark24Ctrl.SetCenterElement(GetGuiElement(MarkerEditHook_mark24));
}

short CMarkerEditHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MarkerEditHook_mark01:
    {
      m_mark01Ctrl.MouseDown();
      AddRenderUiControls(&m_mark01Ctrl);
    }
    break;
  case MarkerEditHook_mark02:
    {
      m_mark02Ctrl.MouseDown();
      AddRenderUiControls(&m_mark02Ctrl);
    }
    break;
  case MarkerEditHook_mark03:
    {
      m_mark03Ctrl.MouseDown();
      AddRenderUiControls(&m_mark03Ctrl);
    }
    break;
  case MarkerEditHook_mark04:
    {
      m_mark04Ctrl.MouseDown();
      AddRenderUiControls(&m_mark04Ctrl);
    }
    break;
  case MarkerEditHook_mark05:
    {
      m_mark05Ctrl.MouseDown();
      AddRenderUiControls(&m_mark05Ctrl);
    }
    break;
  case MarkerEditHook_mark06:
    {
      m_mark06Ctrl.MouseDown();
      AddRenderUiControls(&m_mark06Ctrl);
    }
    break;
  case MarkerEditHook_mark07:
    {
      m_mark07Ctrl.MouseDown();
      AddRenderUiControls(&m_mark07Ctrl);
    }
    break;
  case MarkerEditHook_mark08:
    {
      m_mark08Ctrl.MouseDown();
      AddRenderUiControls(&m_mark08Ctrl);
    }
    break;
  case MarkerEditHook_mark09:
    {
      m_mark09Ctrl.MouseDown();
      AddRenderUiControls(&m_mark09Ctrl);
    }
    break;
  case MarkerEditHook_mark10:
    {
      m_mark10Ctrl.MouseDown();
      AddRenderUiControls(&m_mark10Ctrl);
    }
    break;
  case MarkerEditHook_mark11:
    {
      m_mark11Ctrl.MouseDown();
      AddRenderUiControls(&m_mark11Ctrl);
    }
    break;
  case MarkerEditHook_mark12:
    {
      m_mark12Ctrl.MouseDown();
      AddRenderUiControls(&m_mark12Ctrl);
    }
    break;
  case MarkerEditHook_mark13:
    {
      m_mark13Ctrl.MouseDown();
      AddRenderUiControls(&m_mark13Ctrl);
    }
    break;
  case MarkerEditHook_mark14:
    {
      m_mark14Ctrl.MouseDown();
      AddRenderUiControls(&m_mark14Ctrl);
    }
    break;
  case MarkerEditHook_mark15:
    {
      m_mark15Ctrl.MouseDown();
      AddRenderUiControls(&m_mark15Ctrl);
    }
    break;
  case MarkerEditHook_mark16:
    {
      m_mark16Ctrl.MouseDown();
      AddRenderUiControls(&m_mark16Ctrl);
    }
    break;
  case MarkerEditHook_mark17:
    {
      m_mark17Ctrl.MouseDown();
      AddRenderUiControls(&m_mark17Ctrl);
    }
    break;
  case MarkerEditHook_mark18:
    {
      m_mark18Ctrl.MouseDown();
      AddRenderUiControls(&m_mark18Ctrl);
    }
    break;
  case MarkerEditHook_mark19:
    {
      m_mark19Ctrl.MouseDown();
      AddRenderUiControls(&m_mark19Ctrl);
    }
    break;
  case MarkerEditHook_mark20:
    {
      m_mark20Ctrl.MouseDown();
      AddRenderUiControls(&m_mark20Ctrl);
    }
    break;
  case MarkerEditHook_mark21:
    {
      m_mark21Ctrl.MouseDown();
      AddRenderUiControls(&m_mark21Ctrl);
    }
    break;
  case MarkerEditHook_mark22:
    {
      m_mark22Ctrl.MouseDown();
      AddRenderUiControls(&m_mark22Ctrl);
    }
    break;
  case MarkerEditHook_mark23:
    {
      m_mark23Ctrl.MouseDown();
      AddRenderUiControls(&m_mark23Ctrl);
    }
    break;
  case MarkerEditHook_mark24:
    {
      m_mark24Ctrl.MouseDown();
      AddRenderUiControls(&m_mark24Ctrl);
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CMarkerEditHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMarkerEditHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case MarkerEditHook_mark01:
    {
      m_mark01Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark01Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark02:
    {
      m_mark02Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark02Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark03:
    {
      m_mark03Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark03Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark04:
    {
      m_mark04Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark04Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark05:
    {
      m_mark05Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark05Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark06:
    {
      m_mark06Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark06Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark07:
    {
      m_mark07Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark07Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark08:
    {
      m_mark08Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark08Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark09:
    {
      m_mark09Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark09Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark10:
    {
      m_mark10Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark10Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark11:
    {
      m_mark11Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark11Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark12:
    {
      m_mark12Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark12Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark13:
    {
      m_mark13Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark13Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark14:
    {
      m_mark14Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark14Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark15:
    {
      m_mark15Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark15Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark16:
    {
      m_mark16Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark16Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark17:
    {
      m_mark17Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark17Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark18:
    {
      m_mark18Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark18Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark19:
    {
      m_mark19Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark19Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark20:
    {
      m_mark20Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark20Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark21:
    {
      m_mark21Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark21Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark22:
    {
      m_mark22Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark22Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark23:
    {
      m_mark23Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark23Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  case MarkerEditHook_mark24:
    {
      m_mark24Ctrl.MouseUp();
      if (ctrlType == m_downElementType)
      {
        edithook.SetMark(m_mark24Ctrl.GetCenterElement()->m_backStyle);
        Return();
      }
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}
