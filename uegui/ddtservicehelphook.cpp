#include "ddtservicehelphook.h"
using namespace UeGui;

CDdtServiceHelpHook::CDdtServiceHelpHook():m_showPic(1)
{
  m_strTitle = "服务";
  m_vecHookFile.push_back(_T("ddtservicehelphook.bin"));
}

CDdtServiceHelpHook::~CDdtServiceHelpHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}


void CDdtServiceHelpHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_LeftPageBtn,	"LeftPageBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_RightPageBtn,	"RightPageBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_HelpPicOne,	"HelpPicOne"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneLabelOne,	"PicOneLabelOne"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneLabelTwo,	"PicOneLabelTwo"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneLabelThree,	"PicOneLabelThree"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneLabelFour,	"PicOneLabelFour"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneBtnOne,	"PicOneBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneBtnTwo,	"PicOneBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneBtnThree,	"PicOneBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneBtnFour,	"PicOneBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneShowOne,	"PicOneShowOne"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneShowTwo,	"PicOneShowTwo"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneShowThree,	"PicOneShowThree"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicehelphook_PicOneShowFour,	"PicOneShowFour"));
}

void CDdtServiceHelpHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_backGroundCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_BackGround));
  m_helpPicOneCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_HelpPicOne));
  m_leftPageBtnCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_LeftPageBtn));
  m_picOneBtnFourCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneBtnFour));
  m_picOneBtnOneCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneBtnOne));
  m_picOneBtnThreeCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneBtnThree));
  m_picOneBtnTwoCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneBtnTwo));
  m_picOneLabelFourCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneLabelFour));
  m_picOneLabelOneCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneLabelOne));
  m_picOneLabelThreeCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneLabelThree));
  m_picOneLabelTwoCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneLabelTwo));
  m_picOneShowFourCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneShowFour));
  m_picOneShowOneCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneShowOne));
  m_picOneShowThreeCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneShowThree));
  m_picOneShowTwoCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_PicOneShowTwo));
  m_rightPageBtnCtrl.SetCenterElement(GetGuiElement(ddtservicehelphook_RightPageBtn));

  m_recordBtn = m_picOneBtnOneCtrl;
  m_recordDetail = m_picOneShowOneCtrl;
}

short CDdtServiceHelpHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case ddtservicehelphook_LeftPageBtn:
    {
      m_leftPageBtnCtrl.MouseDown();
      AddRenderUiControls(&m_leftPageBtnCtrl);
    }
    break;
  case ddtservicehelphook_PicOneLabelFour:
  case ddtservicehelphook_PicOneBtnFour:
    {
      m_picOneBtnFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnFourCtrl);
    }
    break;
  case ddtservicehelphook_PicOneLabelOne:
  case ddtservicehelphook_PicOneBtnOne:
    {
      m_picOneBtnOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnOneCtrl);
    }
    break;
  case ddtservicehelphook_PicOneLabelThree:
  case ddtservicehelphook_PicOneBtnThree:
    {
      m_picOneBtnThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnThreeCtrl);
    }
    break;
  case ddtservicehelphook_PicOneLabelTwo:
  case ddtservicehelphook_PicOneBtnTwo:
    {
      m_picOneBtnTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_picOneBtnTwoCtrl);
    }
    break;
  case ddtservicehelphook_PicOneShowFour:
    {
      m_picOneShowFourCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_recordDetail);
    }
    break;
  case ddtservicehelphook_PicOneShowOne:
    {
      m_picOneShowOneCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_recordDetail);
    }
    break;
  case ddtservicehelphook_PicOneShowThree:
    {
      m_picOneShowThreeCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_recordDetail);
    }
    break;
  case ddtservicehelphook_PicOneShowTwo:
    {
      m_picOneShowTwoCtrl.MouseDown();
      m_recordDetail.SetVisible(false);
      AddRenderUiControls(&m_recordDetail);
    }
    break;
  case ddtservicehelphook_RightPageBtn:
    {
      m_rightPageBtnCtrl.MouseDown();
      AddRenderUiControls(&m_rightPageBtnCtrl);
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

short CDdtServiceHelpHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceHelpHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case ddtservicehelphook_LeftPageBtn:
    {
      m_leftPageBtnCtrl.MouseUp();
      if (m_leftPageBtnCtrl.IsEnable())
      {
        ShowPic(m_showPic,false);
        --m_showPic;
        ShowPic(m_showPic,true);
        SwitchPage();
      }
    }
    break;
  case ddtservicehelphook_PicOneBtnFour:
  case ddtservicehelphook_PicOneLabelFour:
    {
      m_picOneBtnFourCtrl.MouseUp();
      m_recordBtn.SetVisible(false);
      ShowDetail(m_picOneBtnFourCtrl, m_picOneShowFourCtrl, true);
      m_picOneShowFourCtrl.SetCaption("通过此功能可以查\n看如何进行产品的\n正版验证");
    }
    break;
  case ddtservicehelphook_PicOneBtnOne:
  case ddtservicehelphook_PicOneLabelOne:
    {
      m_picOneBtnOneCtrl.MouseUp();
      m_recordBtn.SetVisible(false);
      ShowDetail(m_picOneBtnOneCtrl, m_picOneShowOneCtrl, true);
      m_picOneShowOneCtrl.SetCaption("通过此功能可以查\n看操作帮助");
    }
    break;
  case ddtservicehelphook_PicOneLabelThree:
  case ddtservicehelphook_PicOneBtnThree:
    {
      m_picOneBtnThreeCtrl.MouseUp();
      m_recordBtn.SetVisible(false);
      ShowDetail(m_picOneBtnThreeCtrl, m_picOneShowThreeCtrl, true);
      m_picOneShowThreeCtrl.SetCaption("通过此功能可以查看\n升级服务信息和查询\n升级服务网点");
    }
    break;
  case ddtservicehelphook_PicOneLabelTwo:
  case ddtservicehelphook_PicOneBtnTwo:
    {
      m_picOneBtnTwoCtrl.MouseUp();
      m_recordBtn.SetVisible(false);
      ShowDetail(m_picOneBtnTwoCtrl, m_picOneShowTwoCtrl, true);
      m_picOneShowTwoCtrl.SetCaption("通过此功能可以\n进行地图数据的\n增量更新");
    }
    break;
  case ddtservicehelphook_RightPageBtn:
    {
      m_rightPageBtnCtrl.MouseUp();
      if (m_leftPageBtnCtrl.IsEnable())
      {
        ShowPic(m_showPic,false);
        ++m_showPic;
        ShowPic(m_showPic,true);
        SwitchPage();
      }
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    Refresh();
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CDdtServiceHelpHook::Load()
{
  m_showPic = 1;
  ShowPic(1, true);
  SwitchPage();
}

void CDdtServiceHelpHook::ShowPic(int pic, bool show)
{
  switch(pic)
  {
  case 1:
    {
      m_helpPicOneCtrl.SetVisible(show);
      m_picOneLabelFourCtrl.SetVisible(show);
      m_picOneLabelOneCtrl.SetVisible(show);
      m_picOneLabelThreeCtrl.SetVisible(show);
      m_picOneLabelTwoCtrl.SetVisible(show);
    }break;
  default:
    assert(false);
    break;
  }
  m_recordBtn.SetVisible(false);
  m_recordDetail.SetVisible(false);
}

void CDdtServiceHelpHook::SwitchPage()
{
  if (m_showPic == 1)
  {
    m_leftPageBtnCtrl.SetEnable(false);
  }
  else
  {
    m_leftPageBtnCtrl.SetEnable(true);
  }
  if (m_showPic == 1)
  {
    m_rightPageBtnCtrl.SetEnable(false);
  }
  else
  {
    m_rightPageBtnCtrl.SetEnable(true);
  }
}

void CDdtServiceHelpHook::ShowDetail(CUiButton btn, CUiButton detail, bool show)
{
  btn.SetVisible(show);
  detail.SetVisible(show);
  m_recordBtn = btn;
  m_recordDetail = detail;
}
