#include "mapoverviewmenuhook.h"
#include "maphook.h"

using namespace UeGui;

CMapOverViewMenuHook::CMapOverViewMenuHook() : m_parentHook(NULL)
{
  //地图界面渲染完成后不需要释放图片资源，提高效率
  m_needReleasePic = false;
}

CMapOverViewMenuHook::~CMapOverViewMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CMapOverViewMenuHook::GetBinaryFileName()
{
  return _T("mapoverviewmenuhook.bin");
}

void UeGui::CMapOverViewMenuHook::Show( bool show /*= true */ )
{
  CAggHook::Show(show);
  if (show)
  {
    Update();
  }    
}

void CMapOverViewMenuHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button1Left,	"Button1Left"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button1,	"Button1"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button1Label,	"Button1Label"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Delimiter1,	"Delimiter1"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button2,	"Button2"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button2Label,	"Button2Label"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Delimiter2,	"Delimiter2"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button3,	"Button3"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button3Label,	"Button3Label"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Delimiter3,	"Delimiter3"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button4,	"Button4"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button4Label,	"Button4Label"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_Button4Right,	"Button4Right"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_DestInfoBtn,	"DestInfoBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_DestInfoLabel,	"DestInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_DistInfoBtn,	"DistInfoBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_DistInfoIcon,	"DistInfoIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_DistInfoLabel,	"DistInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_ReturnBtnBack,	"ReturnBtnBack"));
  m_ctrlNames.insert(GuiName::value_type(MapOverViewMenuHook_ReturnBtnIcon,	"ReturnBtnIcon"));
}

void CMapOverViewMenuHook::MakeControls()
{
  m_button1.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Button1));
  m_button1.SetLabelElement(GetGuiElement(MapOverViewMenuHook_Button1Label));
  m_button1.SetLeftElement(GetGuiElement(MapOverViewMenuHook_Button1Left));
  m_button2.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Button2));
  m_button2.SetLabelElement(GetGuiElement(MapOverViewMenuHook_Button2Label));
  m_button3.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Button3));
  m_button3.SetLabelElement(GetGuiElement(MapOverViewMenuHook_Button3Label));
  m_button4.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Button4));
  m_button4.SetLabelElement(GetGuiElement(MapOverViewMenuHook_Button4Label));
  m_button4.SetRightElement(GetGuiElement(MapOverViewMenuHook_Button4Right));
  m_delimiter1.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Delimiter1));
  m_delimiter2.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Delimiter2));
  m_delimiter3.SetCenterElement(GetGuiElement(MapOverViewMenuHook_Delimiter3));
  m_destInfoBtn.SetCenterElement(GetGuiElement(MapOverViewMenuHook_DestInfoBtn));
  m_destInfoBtn.SetLabelElement(GetGuiElement(MapOverViewMenuHook_DestInfoLabel));
  m_distInfoBtn.SetCenterElement(GetGuiElement(MapOverViewMenuHook_DistInfoBtn));
  m_distInfoBtn.SetIconElement(GetGuiElement(MapOverViewMenuHook_DistInfoIcon));
  m_distInfoBtn.SetLabelElement(GetGuiElement(MapOverViewMenuHook_DistInfoLabel));
  m_returnBtn.SetCenterElement(GetGuiElement(MapOverViewMenuHook_ReturnBtnBack));
  m_returnBtn.SetCenterElement(GetGuiElement(MapOverViewMenuHook_ReturnBtnIcon));
}

short CMapOverViewMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MapOverViewMenuHook_Button1:
  case MapOverViewMenuHook_Button1Label:
  case MapOverViewMenuHook_Button1Left:
    {
      needRefresh = true;
      m_button1.MouseDown();
    }
    break;
  case MapOverViewMenuHook_Button2:
  case MapOverViewMenuHook_Button2Label:
    {
      needRefresh = true;
      m_button2.MouseDown();
    }
    break;
  case MapOverViewMenuHook_Button3:
  case MapOverViewMenuHook_Button3Label:
    {
      needRefresh = true;
      m_button3.MouseDown();
    }
    break;
  case MapOverViewMenuHook_Button4:
  case MapOverViewMenuHook_Button4Label:
  case MapOverViewMenuHook_Button4Right:
    {
      needRefresh = true;
      m_button4.MouseDown();
    }
    break;
  case MapOverViewMenuHook_ReturnBtnBack:
  case MapOverViewMenuHook_ReturnBtnIcon:
    {
      needRefresh = true;
      m_returnBtn.MouseDown();
    }
    break; 
    break;
  default:
    assert(false);
    break;
  }
  if (needRefresh)
  {
    this->Refresh();
  }
  return ctrlType;
}

short CMapOverViewMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapOverViewMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case MapOverViewMenuHook_Button1:
  case MapOverViewMenuHook_Button1Label:
  case MapOverViewMenuHook_Button1Left:
    {
      needRefresh = true;
      m_button1.MouseUp();
    }
    break;
  case MapOverViewMenuHook_Button2:
  case MapOverViewMenuHook_Button2Label:
    {
      needRefresh = true;
      m_button2.MouseUp();
    }
    break;
  case MapOverViewMenuHook_Button3:
  case MapOverViewMenuHook_Button3Label:
    {
      needRefresh = true;
      m_button3.MouseUp();
    }
    break;
  case MapOverViewMenuHook_Button4:
  case MapOverViewMenuHook_Button4Label:
  case MapOverViewMenuHook_Button4Right:
    {
      needRefresh = true;
      m_button4.MouseUp();
    }
    break;
  case MapOverViewMenuHook_ReturnBtnBack:
  case MapOverViewMenuHook_ReturnBtnIcon:
    {
      needRefresh = true;
      m_returnBtn.MouseUp();
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ReturnToPrevGUI();
      }
      Return();
    }
    break; 
  default:
    assert(false);
    break;
  }

  if (needRefresh)
  {
    this->Refresh();
  }
  return ctrlType;
}

bool CMapOverViewMenuHook::operator ()()
{
  return false;
}


void UeGui::CMapOverViewMenuHook::SetParentHook( CAggHook* parentHook )
{
  m_parentHook = parentHook;
}

void UeGui::CMapOverViewMenuHook::Update()
{
  if (m_parentHook)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
    mapHook->HideAllCtrl();
    mapHook->ShowMapScalingBtn(true); 
    mapHook->ShowTimeBtn();
  }
  m_button1.SetVisible(false);
  m_button2.SetVisible(false);
  m_button3.SetVisible(false);
  m_button4.SetVisible(false);
  m_delimiter1.SetVisible(false);
  m_delimiter2.SetVisible(false);
  m_delimiter3.SetVisible(false);
};