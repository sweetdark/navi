#include "versioncheckhook.h"
using namespace UeGui;

CVersionCheckHook::CVersionCheckHook()
{
  m_strTitle = "正版验证";
  m_vecHookFile.push_back(_T("versioncheckhook.bin"));
}

CVersionCheckHook::~CVersionCheckHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CVersionCheckHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(versioncheckhook_TipText,	"TipText"));
  m_ctrlNames.insert(GuiName::value_type(versioncheckhook_TipText3,	"TipText3"));
  m_ctrlNames.insert(GuiName::value_type(versioncheckhook_Text1,	"Text1"));
  m_ctrlNames.insert(GuiName::value_type(versioncheckhook_TextDup1,	"TextDup1"));
  m_ctrlNames.insert(GuiName::value_type(versioncheckhook_Text2,	"Text2"));
  m_ctrlNames.insert(GuiName::value_type(versioncheckhook_Text3,	"Text3"));
}

void CVersionCheckHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_text1Ctrl.SetCenterElement(GetGuiElement(versioncheckhook_Text1));
  m_text2Ctrl.SetCenterElement(GetGuiElement(versioncheckhook_Text2));
  m_text3Ctrl.SetCenterElement(GetGuiElement(versioncheckhook_Text3));
  m_textDup1Ctrl.SetCenterElement(GetGuiElement(versioncheckhook_TextDup1));
  m_tipTextCtrl.SetCenterElement(GetGuiElement(versioncheckhook_TipText));
  m_tipText3Ctrl.SetCenterElement(GetGuiElement(versioncheckhook_TipText3));
}

short CVersionCheckHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
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

short CVersionCheckHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CVersionCheckHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
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
void CVersionCheckHook::Load()
{
  m_tipTextCtrl.SetCaption("正版验证流程:");
  m_tipText3Ctrl.SetCaption("在电脑上打开网页浏览器,登录http://www.ritu.cn/网站");
  m_text1Ctrl.SetCaption("打开网站后,点击''道道通专题网''链接");
  m_textDup1Ctrl.SetCaption("打开''道道通专题网''页面后,在客户服务部分点击''正版验证''链接");
  m_text2Ctrl.SetCaption("打开''正版验证''页面后,按提示操作即可完成正版验证");
}


