#include "roundradiusselecthook.h"

#include "querywrapper.h"


using namespace UeGui;

//默认设置为5千米
CRoundRadiusSelectHook::CRoundRadiusSelectHook() : m_curRadius(RADIUS08)
{
}

CRoundRadiusSelectHook::~CRoundRadiusSelectHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CRoundRadiusSelectHook::GetBinaryFileName()
{
  return _T("roundradiusselecthook.bin");
}

void CRoundRadiusSelectHook::Load()
{
  ClearSelection();
  switch(m_curRadius)
  {
  case RADIUS06:
    m_List1Btn.SetCheck(true);
    break;
  case RADIUS07:
    m_List2Btn.SetCheck(true);
    break;
  case RADIUS08:
    m_List3Btn.SetCheck(true);
    break;
  case RADIUS09:
    m_List4Btn.SetCheck(true);
    break;
  case RADIUS10:
    m_List5Btn.SetCheck(true);
    break;
  }
}

void CRoundRadiusSelectHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List5Btn,	"List5Btn"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_Line4,	"Line4"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_GrayBack,	"GrayBack"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List1BtnIcon,	"List1BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List2BtnIcon,	"List2BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List3BtnIcon,	"List3BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List4BtnIcon,	"List4BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(RoundRadiusSelectHook_List5BtnIcon,	"List5BtnIcon"));
}

void CRoundRadiusSelectHook::MakeControls()
{
  m_List1Btn.SetCenterElement(GetGuiElement(RoundRadiusSelectHook_List1Btn));
  m_List1Btn.SetIconElement(GetGuiElement(RoundRadiusSelectHook_List1BtnIcon));

  m_List2Btn.SetCenterElement(GetGuiElement(RoundRadiusSelectHook_List2Btn));
  m_List2Btn.SetIconElement(GetGuiElement(RoundRadiusSelectHook_List2BtnIcon));

  m_List3Btn.SetCenterElement(GetGuiElement(RoundRadiusSelectHook_List3Btn));
  m_List3Btn.SetIconElement(GetGuiElement(RoundRadiusSelectHook_List3BtnIcon));

  m_List4Btn.SetCenterElement(GetGuiElement(RoundRadiusSelectHook_List4Btn));
  m_List4Btn.SetIconElement(GetGuiElement(RoundRadiusSelectHook_List4BtnIcon));

  m_List5Btn.SetCenterElement(GetGuiElement(RoundRadiusSelectHook_List5Btn));
  m_List5Btn.SetIconElement(GetGuiElement(RoundRadiusSelectHook_List5BtnIcon));

  m_backBtn.SetCenterElement(GetGuiElement(RoundRadiusSelectHook_GrayBack));  
}

short CRoundRadiusSelectHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case RoundRadiusSelectHook_List1Btn:
  case RoundRadiusSelectHook_List1BtnIcon:
    {
      m_List1Btn.MouseDown();
      AddRenderUiControls(&m_List1Btn);
    }
    break;
  case RoundRadiusSelectHook_List2Btn:
  case RoundRadiusSelectHook_List2BtnIcon:
    {
      m_List2Btn.MouseDown();
      AddRenderUiControls(&m_List2Btn);
    }
    break;
  case RoundRadiusSelectHook_List3Btn:
  case RoundRadiusSelectHook_List3BtnIcon:
    {
      m_List3Btn.MouseDown();
      AddRenderUiControls(&m_List3Btn);
    }
    break;
  case RoundRadiusSelectHook_List4Btn:
  case RoundRadiusSelectHook_List4BtnIcon:
    {
      m_List4Btn.MouseDown();
      AddRenderUiControls(&m_List4Btn);
    }
    break;
  case RoundRadiusSelectHook_List5Btn:
  case RoundRadiusSelectHook_List5BtnIcon:
    {
      m_List5Btn.MouseDown();
      AddRenderUiControls(&m_List5Btn);
    }
    break;
  case RoundRadiusSelectHook_GrayBack:
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

short CRoundRadiusSelectHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CRoundRadiusSelectHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case RoundRadiusSelectHook_List1Btn:
  case RoundRadiusSelectHook_List1BtnIcon:
    {
      m_List1Btn.MouseUp();
      m_curRadius = RADIUS06;
      Return();
    }
    break;
  case RoundRadiusSelectHook_List2Btn:
  case RoundRadiusSelectHook_List2BtnIcon:
    {
      m_List2Btn.MouseUp();
      m_curRadius = RADIUS07;
      Return();
    }
    break;
  case RoundRadiusSelectHook_List3Btn:
  case RoundRadiusSelectHook_List3BtnIcon:
    {
      m_List3Btn.MouseUp();
      m_curRadius = RADIUS08;
      Return();
    }
    break;
  case RoundRadiusSelectHook_List4Btn:
  case RoundRadiusSelectHook_List4BtnIcon:
    {
      m_List4Btn.MouseUp();
      m_curRadius = RADIUS09;
      Return();
    }
    break;
  case RoundRadiusSelectHook_List5Btn:
  case RoundRadiusSelectHook_List5BtnIcon:
    {
      m_List5Btn.MouseUp();
      m_curRadius = RADIUS10;
      Return();
    }
    break;
  case RoundRadiusSelectHook_GrayBack:
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

bool CRoundRadiusSelectHook::operator ()()
{
  return false;
}

void CRoundRadiusSelectHook::ClearSelection()
{
  m_List1Btn.SetCheck(false);
  m_List2Btn.SetCheck(false);
  m_List3Btn.SetCheck(false);
  m_List4Btn.SetCheck(false);
  m_List5Btn.SetCheck(false);
}

int CRoundRadiusSelectHook::GetCurSelectedRadius()
{
  return m_curRadius;
}