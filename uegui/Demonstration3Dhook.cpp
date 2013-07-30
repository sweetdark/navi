#include "demonstration3dhook.h"
using namespace UeGui;

CDemonstration3DHook::CDemonstration3DHook()
{
}

CDemonstration3DHook::~CDemonstration3DHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDemonstration3DHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring CDemonstration3DHook::GetBinaryFileName()
{
  return _T("demonstration3dhook.bin");
}

void CDemonstration3DHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_BackGroundText,	"BackGroundText"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_GotoMapBtn,	"GotoMapBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_BackButton,	"BackButton"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_beijingBtn,	"beijingBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_beijingText,	"beijingText"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_shanghaiBtn,	"shanghaiBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_shanghaiText,	"shanghaiText"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_guangzhouBtn,	"guangzhouBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_guangzhouText,	"guangzhouText"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_shenzhenBtn,	"shenzhenBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_shenzhenText,	"shenzhenText"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_chongqingBtn,	"chongqingBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_chongqingText,	"chongqingText"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_foshanBtn,	"foshanBtn"));
  m_ctrlNames.insert(GuiName::value_type(Demonstration3DHook_foshanText,	"foshanText"));
}

void CDemonstration3DHook::MakeControls()
{
  m_backButtonCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_BackButton));
  m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_GotoMapBtn));

  m_beijingBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_beijingBtn));
  m_beijingBtnCtrl.SetLabelElement(GetGuiElement(Demonstration3DHook_beijingText));
  m_chongqingBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_chongqingBtn));
  m_chongqingBtnCtrl.SetLabelElement(GetGuiElement(Demonstration3DHook_chongqingText));
  m_foshanBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_foshanBtn));
  m_foshanBtnCtrl.SetLabelElement(GetGuiElement(Demonstration3DHook_foshanText));
  m_guangzhouBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_guangzhouBtn));
  m_guangzhouBtnCtrl.SetLabelElement(GetGuiElement(Demonstration3DHook_guangzhouText));
  m_shanghaiBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_shanghaiBtn));
  m_shanghaiBtnCtrl.SetLabelElement(GetGuiElement(Demonstration3DHook_shanghaiText));
  m_shenzhenBtnCtrl.SetCenterElement(GetGuiElement(Demonstration3DHook_shenzhenBtn));
  m_shenzhenBtnCtrl.SetLabelElement(GetGuiElement(Demonstration3DHook_shenzhenText));
}

short CDemonstration3DHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case Demonstration3DHook_BackButton:
    {
      m_backButtonCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_GotoMapBtn:
    {
      m_gotoMapBtnCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_beijingBtn:
  case Demonstration3DHook_beijingText:
    {
      m_beijingBtnCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_chongqingBtn:
  case Demonstration3DHook_chongqingText:
    {
      m_chongqingBtnCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_foshanBtn:
  case Demonstration3DHook_foshanText:
    {
      m_foshanBtnCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_guangzhouBtn:
  case Demonstration3DHook_guangzhouText:
    {
      m_guangzhouBtnCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_shanghaiBtn:
  case Demonstration3DHook_shanghaiText:
    {
      m_shanghaiBtnCtrl.MouseDown();
    }
    break;
  case Demonstration3DHook_shenzhenBtn:
  case Demonstration3DHook_shenzhenText:
    {
      m_shenzhenBtnCtrl.MouseDown();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CDemonstration3DHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDemonstration3DHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case Demonstration3DHook_BackButton:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_curHookType = CViewHook::m_prevHookType;*/
        CAggHook::Return();
      }
      m_backButtonCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_GotoMapBtn:
    {
      if(ctrlType == m_downElementType)
      {
        /*CViewHook::m_prevHookType=CViewHook::m_curHookType;
        CViewHook::m_curHookType=CViewHook::DHT_MapHook;*/
        CAggHook::GoToMapHook();
      }
      m_gotoMapBtnCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_beijingBtn:
  case Demonstration3DHook_beijingText:
    {
      m_beijingBtnCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_chongqingBtn:
  case Demonstration3DHook_chongqingText:
    {
      m_chongqingBtnCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_foshanBtn:
  case Demonstration3DHook_foshanText:
    {
      m_foshanBtnCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_guangzhouBtn:
  case Demonstration3DHook_guangzhouText:
    {
      m_guangzhouBtnCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_shanghaiBtn:
  case Demonstration3DHook_shanghaiText:
    {
      m_shanghaiBtnCtrl.MouseUp();
    }
    break;
  case Demonstration3DHook_shenzhenBtn:
  case Demonstration3DHook_shenzhenText:
    {
      m_shenzhenBtnCtrl.MouseUp();
    }
    break;
  default:
    m_isNeedRefesh = false;
    assert(false);
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

bool CDemonstration3DHook::operator ()()
{
  return false;
}

