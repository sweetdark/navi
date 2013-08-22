#include "updateservicehook.h"
using namespace UeGui;

CUpdateServiceHook::CUpdateServiceHook()
{
  m_strTitle = "升级服务";
  m_vecHookFile.push_back(_T("updateservicehook.bin"));
}

CUpdateServiceHook::~CUpdateServiceHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CUpdateServiceHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_TipText,	"TipText"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_TipText2,	"TipText2"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_TipText3,	"TipText3"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_Text1,	"Text1"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_TextDup1,	"TextDup1"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_Text2,	"Text2"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_Text3,	"Text3"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_Text4,	"Text4"));
  m_ctrlNames.insert(GuiName::value_type(updateservicehook_ServicePointBtn,	"ServicePointBtn"));
}

void CUpdateServiceHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_servicePointBtnCtrl.SetCenterElement(GetGuiElement(updateservicehook_ServicePointBtn));
  m_text1Ctrl.SetCenterElement(GetGuiElement(updateservicehook_Text1));
  m_text2Ctrl.SetCenterElement(GetGuiElement(updateservicehook_Text2));
  m_text3Ctrl.SetCenterElement(GetGuiElement(updateservicehook_Text3));
  m_text4Ctrl.SetCenterElement(GetGuiElement(updateservicehook_Text4));
  m_textDup1Ctrl.SetCenterElement(GetGuiElement(updateservicehook_TextDup1));
  m_tipTextCtrl.SetCenterElement(GetGuiElement(updateservicehook_TipText));
  m_tipText2Ctrl.SetCenterElement(GetGuiElement(updateservicehook_TipText2));
  m_tipText3Ctrl.SetCenterElement(GetGuiElement(updateservicehook_TipText3));
}

short CUpdateServiceHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case updateservicehook_ServicePointBtn:
    {
      m_servicePointBtnCtrl.MouseDown();
      AddRenderUiControls(&m_servicePointBtnCtrl);
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

short CUpdateServiceHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CUpdateServiceHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case updateservicehook_ServicePointBtn:
    {
      m_servicePointBtnCtrl.MouseUp();
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
void CUpdateServiceHook::Load()
{
  m_tipTextCtrl.SetCaption("如果要获取升级服务,您需要携带道道通正版地图储存卡或道道通正");
  m_tipText2Ctrl.SetCaption("版地图设备到指定的升级服务网点进行升级(点击此界面中的''升级");
  m_tipText3Ctrl.SetCaption("网点''按钮或登录道道通的官方网站：");
  m_text1Ctrl.SetCaption("http://www.ritu.cn/index.aspx,点击网页左下角的''地图升级''就");
  m_textDup1Ctrl.SetCaption("可以查询道道通升级网点信息)。");
  m_text2Ctrl.SetCaption("正版用户每年可享受2次升级服务,具体的升级时间请拨打客服电话");
  m_text3Ctrl.SetCaption("咨询:400-8898-123。");
  m_text4Ctrl.SetCaption("如果需要了解更多升级服务信息,可拨打客服电话咨询");
  m_servicePointBtnCtrl.SetEnable(false);
}

