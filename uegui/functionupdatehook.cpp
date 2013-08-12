#include "functionupdatehook.h"
using namespace UeGui;

CFunctionUpdateHook::CFunctionUpdateHook()
{
  m_strTitle = "增量更新";
  m_vecHookFile.push_back(_T("functionupdatehook.bin"));
}

CFunctionUpdateHook::~CFunctionUpdateHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CFunctionUpdateHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_TipText,	"TipText"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_TipText2,	"TipText2"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_TipText3,	"TipText3"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_Text1,	"Text1"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_TextDup1,	"TextDup1"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_Text2,	"Text2"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_Text3,	"Text3"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_Text4,	"Text4"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_Text5,	"Text5"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_ElecEyeUpBtn,	"ElecEyeUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(functionupdatehook_InterestPointUp,	"InterestPointUp"));
}

void CFunctionUpdateHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_elecEyeUpBtnCtrl.SetCenterElement(GetGuiElement(functionupdatehook_ElecEyeUpBtn));
  m_interestPointUpCtrl.SetCenterElement(GetGuiElement(functionupdatehook_InterestPointUp));
  m_text1Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_Text1));
  m_text2Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_Text2));
  m_text3Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_Text3));
  m_text4Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_Text4));
  m_text5Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_Text5));
  m_textDup1Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_TextDup1));
  m_tipTextCtrl.SetCenterElement(GetGuiElement(functionupdatehook_TipText));
  m_tipText2Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_TipText2));
  m_tipText3Ctrl.SetCenterElement(GetGuiElement(functionupdatehook_TipText3));
}

short CFunctionUpdateHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case functionupdatehook_ElecEyeUpBtn:
    {
      m_elecEyeUpBtnCtrl.MouseDown();
      if (!m_elecEyeUpBtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_elecEyeUpBtnCtrl);
    }
    break;
  case functionupdatehook_InterestPointUp:
    {
      m_interestPointUpCtrl.MouseDown();
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

short CFunctionUpdateHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CFunctionUpdateHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case functionupdatehook_ElecEyeUpBtn:
    {
      m_elecEyeUpBtnCtrl.MouseUp();
    }
    break;
  case functionupdatehook_InterestPointUp:
    {
      m_interestPointUpCtrl.MouseUp();
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

void CFunctionUpdateHook::Load()
{
  m_tipTextCtrl.SetCaption("系统为您提供免费的地图增量更新服务，请按如下步骤进行操作:");
  m_tipText2Ctrl.SetCaption("对于不支持上网的设备：");
  m_tipText3Ctrl.SetCaption("1.登陆www.ritu.cn网站,选择打开''道道通专题网''页面,");
  m_text1Ctrl.SetCaption("在''道道通专题网''页面中选择''增量更新''");
  m_textDup1Ctrl.SetCaption("2.在''增量更新''页面中下载相应更新数据包");
  m_text2Ctrl.SetCaption("3.下载完成后,将数据包拷贝至设备中");
  m_text3Ctrl.SetCaption("4.拷贝完毕后,点击本界面底部的相应更新按钮");
  m_text4Ctrl.SetCaption("5.更新完毕后系统会给出提示");
  m_elecEyeUpBtnCtrl.SetEnable(false);
}
