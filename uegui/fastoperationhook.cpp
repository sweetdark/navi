#include "fastoperationhook.h"
#include "viewwrapper.h"
#include "maphook.h"

using namespace UeGui;

CFastOperationHook::CFastOperationHook()
{
  m_strTitle = "请选择快捷操作";
  m_vecHookFile.push_back(_T("fastoperationhook.bin"));
}

CFastOperationHook::~CFastOperationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CFastOperationHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_QMarkBtn,	"QMarkBtn"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_QMarkBtnIcon,	"QMarkBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_QMarkLable,	"QMarkLable"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_FavouritBtn,	"FavouritBtn"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_FavouritBtnIcon,	"FavouritBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_FavouritLable,	"FavouritLable"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_ElecEyeBtn,	"ElecEyeBtn"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_ElecEyeBtnIcon,	"ElecEyeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_ElecEyeLable,	"ElecEyeLable"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_BackTrackBtn,	"BackTrackBtn"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_BackTrackBtnIcon,	"BackTrackBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_BackTrackLable,	"BackTrackLable"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_HSplit,	"HSplit"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_FiveBackground,	"FiveBackground"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_FitstSplit,	"FitstSplit"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_SecondSplit,	"SecondSplit"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_ThirdSplit,	"ThirdSplit"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_SixBackground,	"SixBackground"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_SevenBackground,	"SevenBackground"));
  m_ctrlNames.insert(GuiName::value_type(fastoperationhook_EightBackground,	"EightBackground"));
}

void CFastOperationHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_backTrackBtnCtrl.SetCenterElement(GetGuiElement(fastoperationhook_BackTrackBtn));
  m_backTrackBtnCtrl.SetIconElement(GetGuiElement(fastoperationhook_BackTrackBtnIcon));
  m_backTrackLableCtrl.SetLabelElement(GetGuiElement(fastoperationhook_BackTrackLable));
  m_eightBackgroundCtrl.SetCenterElement(GetGuiElement(fastoperationhook_EightBackground));
  m_elecEyeBtnCtrl.SetCenterElement(GetGuiElement(fastoperationhook_ElecEyeBtn));
  m_elecEyeBtnCtrl.SetIconElement(GetGuiElement(fastoperationhook_ElecEyeBtnIcon));
  m_elecEyeLableCtrl.SetLabelElement(GetGuiElement(fastoperationhook_ElecEyeLable));
  m_favouritBtnCtrl.SetCenterElement(GetGuiElement(fastoperationhook_FavouritBtn));
  m_favouritBtnCtrl.SetIconElement(GetGuiElement(fastoperationhook_FavouritBtnIcon));
  m_favouritLableCtrl.SetLabelElement(GetGuiElement(fastoperationhook_FavouritLable));
  m_fitstSplitCtrl.SetCenterElement(GetGuiElement(fastoperationhook_FitstSplit));
  m_fiveBackgroundCtrl.SetCenterElement(GetGuiElement(fastoperationhook_FiveBackground));
  m_hSplitCtrl.SetCenterElement(GetGuiElement(fastoperationhook_HSplit));
  m_qMarkBtnCtrl.SetCenterElement(GetGuiElement(fastoperationhook_QMarkBtn));
  m_qMarkBtnCtrl.SetIconElement(GetGuiElement(fastoperationhook_QMarkBtnIcon));
  m_qMarkLableCtrl.SetLabelElement(GetGuiElement(fastoperationhook_QMarkLable));
  m_secondSplitCtrl.SetCenterElement(GetGuiElement(fastoperationhook_SecondSplit));
  m_sevenBackgroundCtrl.SetCenterElement(GetGuiElement(fastoperationhook_SevenBackground));
  m_sixBackgroundCtrl.SetCenterElement(GetGuiElement(fastoperationhook_SixBackground));
  m_thirdSplitCtrl.SetCenterElement(GetGuiElement(fastoperationhook_ThirdSplit));
}

short CFastOperationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case fastoperationhook_BackTrackBtn:
  case fastoperationhook_BackTrackBtnIcon:
  case fastoperationhook_BackTrackLable:
    {
      m_backTrackBtnCtrl.MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_backTrackBtnCtrl, m_backTrackLableCtrl);
    }
    break;
  case fastoperationhook_ElecEyeBtn:
  case fastoperationhook_ElecEyeBtnIcon:
  case fastoperationhook_ElecEyeLable:
    {
      m_elecEyeBtnCtrl.MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_elecEyeBtnCtrl, m_elecEyeLableCtrl);
    }
    break;
  case fastoperationhook_FavouritBtn:
  case fastoperationhook_FavouritBtnIcon:
  case fastoperationhook_FavouritLable:
    {
      m_favouritBtnCtrl.MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_favouritBtnCtrl, m_favouritLableCtrl);
    }
    break;
  case fastoperationhook_QMarkBtn:
  case fastoperationhook_QMarkBtnIcon:
  case fastoperationhook_QMarkLable:
    {
      m_qMarkBtnCtrl.MouseDown();
      MOUSEDOWN_2RENDERCTRL(m_qMarkBtnCtrl, m_qMarkLableCtrl);
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

short CFastOperationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CFastOperationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case fastoperationhook_QMarkBtn:
  case fastoperationhook_QMarkBtnIcon:
  case fastoperationhook_QMarkLable:
    {
      m_qMarkBtnCtrl.MouseUp();
      TurnTo(DHT_QCodeInputHook);
    }
    break;
  case fastoperationhook_FavouritBtn:
  case fastoperationhook_FavouritBtnIcon:
  case fastoperationhook_FavouritLable:
    {
      m_favouritBtnCtrl.MouseUp();
      TurnTo(DHT_UsuallyHook);
    }
    break;
  case fastoperationhook_ElecEyeBtn:
  case fastoperationhook_ElecEyeBtnIcon:
  case fastoperationhook_ElecEyeLable:
    {
      m_elecEyeBtnCtrl.MouseUp();
      TurnTo(DHT_EEyeListHook);
    }
    break;
  case fastoperationhook_BackTrackBtn:
  case fastoperationhook_BackTrackBtnIcon:
  case fastoperationhook_BackTrackLable:
    {
      m_backTrackBtnCtrl.MouseUp();
      //返程规划
      CViewWrapper &viewWrapper = CViewWrapper::Get();
      CMapHook* mapHook = (CMapHook*)viewWrapper.GetHook(DHT_MapHook);
      if (mapHook)
      {
        m_isNeedRefesh = false;
        mapHook->BackTrackingPlan();
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