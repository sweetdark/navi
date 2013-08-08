#include "mapquerymenuhook.h"
#include "maphook.h"

using namespace UeGui;

CMapQueryMenuHook::CMapQueryMenuHook() : m_parentHook(NULL), m_menuType(QMenu_QueryPoint), m_expandStatus(false)
{
  //地图界面渲染完成后不需要释放图片资源，提高效率
  m_needReleasePic = false;
}

CMapQueryMenuHook::~CMapQueryMenuHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

bool CMapQueryMenuHook::operator ()()
{
  return false;
}

tstring CMapQueryMenuHook::GetBinaryFileName()
{
  return _T("mapquerymenuhook.bin");
}

void CMapQueryMenuHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_ExpandBtn,	"ExpandBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_CloseBtn,	"CloseBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_PreviousBtn,	"PreviousBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_PreviousLabel,	"PreviousLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_Delimiter1,	"Delimiter1"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_BackBtn,	"BackBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_BackLabel,	"BackLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_Delimiter2,	"Delimiter2"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_NextBtn,	"NextBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_NextLabel,	"NextLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_Delimiter3,	"Delimiter3"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_AroundBtn,	"AroundBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_AroundIcon,	"AroundIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_AroundLabel,	"AroundLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_AroundBtnR,	"AroundBtnR"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_ReturnBtnBack,	"ReturnBtnBack"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_ReturnBtnIcon,	"ReturnBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_SelectPointBack,	"SelectPointBack"));
  m_ctrlNames.insert(GuiName::value_type(MapQueryMenuHook_SelectPointLabel,	"SelectPointLabel"));
}

void CMapQueryMenuHook::MakeControls()
{
  m_expandBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_ExpandBtn));
  m_closeBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_CloseBtn));
  m_previousBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_PreviousBtn));
  m_previousBtn.SetLabelElement(GetGuiElement(MapQueryMenuHook_PreviousLabel));
  m_nextBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_NextBtn));
  m_nextBtn.SetLabelElement(GetGuiElement(MapQueryMenuHook_NextLabel));
  m_backBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_BackBtn));
  m_backBtn.SetLabelElement(GetGuiElement(MapQueryMenuHook_BackLabel));
  m_aroundBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_AroundBtn));
  m_aroundBtn.SetRightElement(GetGuiElement(MapQueryMenuHook_AroundBtnR));
  m_aroundBtn.SetIconElement(GetGuiElement(MapQueryMenuHook_AroundIcon));
  m_aroundBtn.SetLabelElement(GetGuiElement(MapQueryMenuHook_AroundLabel));
  m_returnBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_ReturnBtnBack));
  m_returnBtn.SetIconElement(GetGuiElement(MapQueryMenuHook_ReturnBtnIcon));
  m_selectPointBtn.SetCenterElement(GetGuiElement(MapQueryMenuHook_SelectPointBack));
  m_selectPointBtn.SetLabelElement(GetGuiElement(MapQueryMenuHook_SelectPointLabel));  
  m_delimiter1.SetCenterElement(GetGuiElement(MapQueryMenuHook_Delimiter1));
  m_delimiter2.SetCenterElement(GetGuiElement(MapQueryMenuHook_Delimiter2));
  m_delimiter3.SetCenterElement(GetGuiElement(MapQueryMenuHook_Delimiter3));
}

void UeGui::CMapQueryMenuHook::Show( bool show /*= true */ )
{
  CAggHook::Show(show);
  if (show)
  {
    ExpandMenu(true);
    ResetQueryBtnStatus();
  }  
}

void UeGui::CMapQueryMenuHook::SetParentHook( CAggHook* parentHook )
{
  m_parentHook = parentHook;
}

void UeGui::CMapQueryMenuHook::SetMenuType( QueryMenuType menuType )
{
  m_menuType = menuType;
  switch (menuType)
  {
  case QMenu_QueryPoint:
    {
      //查询POI
      m_closeBtn.SetVisible(true);
      m_expandBtn.SetVisible(false);
      m_previousBtn.SetVisible(true);
      m_nextBtn.SetVisible(true);
      m_backBtn.SetVisible(true);
      m_aroundBtn.SetVisible(true);
      m_returnBtn.SetVisible(true);      
      m_selectPointBtn.SetVisible(false);
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ShowMinimizeBtn(true);
        mapHook->ShowAddElecEyeBtn(true);
        mapHook->ShowMapAzimuthBtn(true);
        mapHook->ShowMapScalingBtn(true);
        mapHook->ShowSetDestPointBtn(true);   
        mapHook->ShowFixedPostionBtn(true);
        mapHook->ShowDetailBtn1(true);
        mapHook->ShowDetailBtn2(false);
        mapHook->ShowGuideInfoBtn(false);
      }
      break;
    }
  case QMenu_SelectPoint:
    {
      //选点
      m_closeBtn.SetVisible(false);
      m_expandBtn.SetVisible(false);
      m_previousBtn.SetVisible(false);
      m_nextBtn.SetVisible(false);
      m_backBtn.SetVisible(false);
      m_aroundBtn.SetVisible(false);
      m_returnBtn.SetVisible(true);      
      m_selectPointBtn.SetVisible(true);
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ShowMinimizeBtn(false);
        mapHook->ShowAddElecEyeBtn(false);
        mapHook->ShowMapAzimuthBtn(true);
        mapHook->ShowMapScalingBtn(true);
        mapHook->ShowSetDestPointBtn(false);
        mapHook->ShowFixedPostionBtn(false);
        mapHook->ShowDetailBtn1(false);
        mapHook->ShowDetailBtn2(true);
        mapHook->ShowGuideInfoBtn(false);        
        mapHook->ShowTimeBtn(true);
      }
      break;
    }
  case QMenu_SelectArea:
    {
      //选区域
      m_closeBtn.SetVisible(false);
      m_expandBtn.SetVisible(false);
      m_previousBtn.SetVisible(false);
      m_nextBtn.SetVisible(false);
      m_backBtn.SetVisible(false);
      m_aroundBtn.SetVisible(false);
      m_returnBtn.SetVisible(true);      
      m_selectPointBtn.SetVisible(true);
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ShowMinimizeBtn(false);
        mapHook->ShowAddElecEyeBtn(false);
        mapHook->ShowMapAzimuthBtn(true);
        mapHook->ShowMapScalingBtn(true);
        mapHook->ShowSetDestPointBtn(false);
        mapHook->ShowFixedPostionBtn(false);
        mapHook->ShowDetailBtn1(false);
        mapHook->ShowDetailBtn2(true);
        mapHook->ShowGuideInfoBtn(false);        
        mapHook->ShowTimeBtn(true);
      }
      break;
    }
  }
}

void UeGui::CMapQueryMenuHook::ExpandMenu( bool bExpand /*= true*/ )
{
  if (QMenu_QueryPoint == m_menuType)
  {
    m_expandStatus = bExpand;

    if (bExpand)
    {
      m_closeBtn.SetVisible(true);
      m_expandBtn.SetVisible(false);
      m_previousBtn.SetVisible(true);
      m_nextBtn.SetVisible(true);
      m_backBtn.SetVisible(true);
      m_aroundBtn.SetVisible(true);
      m_returnBtn.SetVisible(true);      
      m_selectPointBtn.SetVisible(false);
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ShowMinimizeBtn(true);
        mapHook->ShowAddElecEyeBtn(true);
        mapHook->ShowMapAzimuthBtn(true);
        mapHook->ShowMapScalingBtn(true);
        mapHook->ShowSetDestPointBtn(true);   
        mapHook->ShowFixedPostionBtn(true);
        mapHook->ShowDetailBtn1(true);
        mapHook->ShowDetailBtn2(false);
        mapHook->ShowGuideInfoBtn(false);
        mapHook->ShowCompass(false);
        mapHook->ShowElecEye(false);
        //开启界面切换定时器
        mapHook->RestarGuiTimer();
      }
    }
    else
    {
      m_closeBtn.SetVisible(false);
      m_expandBtn.SetVisible(true);
      m_previousBtn.SetVisible(false);
      m_nextBtn.SetVisible(false);
      m_backBtn.SetVisible(false);
      m_aroundBtn.SetVisible(false);
      m_returnBtn.SetVisible(true);      
      m_selectPointBtn.SetVisible(false);
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ShowMinimizeBtn(false);
        mapHook->ShowAddElecEyeBtn(true);
        mapHook->ShowMapAzimuthBtn(true);
        mapHook->ShowMapScalingBtn(true);
        mapHook->ShowSetDestPointBtn(false);   
        mapHook->ShowFixedPostionBtn(true);
        mapHook->ShowDetailBtn1(false);
        mapHook->ShowDetailBtn2(true);
        mapHook->ShowGuideInfoBtn(false);
        mapHook->ShowCompass(false);
        mapHook->ShowElecEye(false);
        //开启界面切换定时器
        mapHook->CloseGuiTimer();
      }
    }
  }
}

void UeGui::CMapQueryMenuHook::Update()
{

}

void UeGui::CMapQueryMenuHook::SetQueryPos( short posType )
{
  if (m_parentHook)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
    mapHook->SetQueryPos((CMapHook::PosType)posType);
    if (mapHook->HaveNextQueryPoint(CMapHook::PT_Next))
    {
      m_nextBtn.SetEnable(true);
    }
    else
    {
      m_nextBtn.SetEnable(false);
    }
    if (mapHook->HaveNextQueryPoint(CMapHook::PT_Previous))
    {
      m_previousBtn.SetEnable(true);
    }
    else
    {
      m_previousBtn.SetEnable(false);
    }
  }
}

void UeGui::CMapQueryMenuHook::ResetQueryBtnStatus()
{
  if (m_parentHook)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
    if (mapHook->HaveNextQueryPoint(CMapHook::PT_Next))
    {
      m_nextBtn.SetEnable(true);
    }
    else
    {
      m_nextBtn.SetEnable(false);
    }
    if (mapHook->HaveNextQueryPoint(CMapHook::PT_Previous))
    {
      m_previousBtn.SetEnable(true);
    }
    else
    {
      m_previousBtn.SetEnable(false);
    }
  }
}

short CMapQueryMenuHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MapQueryMenuHook_ExpandBtn:
    {
      needRefresh = true;
      m_expandBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_CloseBtn:
    {
      needRefresh = true;
      m_closeBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_PreviousBtn:
  case MapQueryMenuHook_PreviousLabel:
    {
      needRefresh = true;
      m_previousBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_NextBtn:
  case MapQueryMenuHook_NextLabel:
    {
      needRefresh = true;
      m_nextBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_BackBtn:
  case MapQueryMenuHook_BackLabel:
    {
      needRefresh = true;
      m_backBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_AroundBtn:
  case MapQueryMenuHook_AroundBtnR:
  case MapQueryMenuHook_AroundIcon:
  case MapQueryMenuHook_AroundLabel:
    {
      needRefresh = true;
      m_aroundBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_ReturnBtnBack:
  case MapQueryMenuHook_ReturnBtnIcon:
    {
      needRefresh = true;
      m_returnBtn.MouseDown();
    }
    break;
  case MapQueryMenuHook_SelectPointBack:
  case MapQueryMenuHook_SelectPointLabel:
    {
      needRefresh = true;
      m_selectPointBtn.MouseDown();
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

short CMapQueryMenuHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapQueryMenuHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool canDo = ctrlType == m_downElementType; 
  switch(ctrlType)
  {
  case MapQueryMenuHook_ExpandBtn:
    {
      m_expandBtn.MouseUp();
      needRefresh = true;
      ExpandMenu();
    }
    break;
  case MapQueryMenuHook_CloseBtn:
    {
      m_closeBtn.MouseUp();
      needRefresh = true;
      ExpandMenu(false);
    }
    break;
  case MapQueryMenuHook_PreviousBtn:
  case MapQueryMenuHook_PreviousLabel:
    {
      m_previousBtn.MouseUp();
      needRefresh = true;
      SetQueryPos(CMapHook::PT_Previous);
    }
    break;
  case MapQueryMenuHook_NextBtn:
  case MapQueryMenuHook_NextLabel:
    {      
      m_nextBtn.MouseUp();
      needRefresh = true;
      SetQueryPos(CMapHook::PT_Next);
    }
    break;
  case MapQueryMenuHook_BackBtn:
  case MapQueryMenuHook_BackLabel:
    {      
      m_backBtn.MouseUp();
      needRefresh = true;
      SetQueryPos(CMapHook::PT_Current);
    }
    break;
  case MapQueryMenuHook_AroundBtn:
  case MapQueryMenuHook_AroundBtnR:
  case MapQueryMenuHook_AroundIcon:
  case MapQueryMenuHook_AroundLabel:
    {      
      m_aroundBtn.MouseUp();
      needRefresh = true;
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->OpenAroundSearchMenu();
      }
    }
    break;
  case MapQueryMenuHook_ReturnBtnBack:
  case MapQueryMenuHook_ReturnBtnIcon:
    {      
      m_returnBtn.MouseUp(); 
      needRefresh = true;      
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->ReturnToPrevGUI();
      }
      Return(false);
    }
    break;
  case MapQueryMenuHook_SelectPointBack:
  case MapQueryMenuHook_SelectPointLabel:
    {      
      m_selectPointBtn.MouseUp();
      needRefresh = true;
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        mapHook->Select();
      }
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