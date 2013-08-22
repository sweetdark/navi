#include "eeyesettinghook.h"

#include "ueroute\routebasic.h"

#include "maphook.h"

#ifndef __Q_CODE_H__
#include "QCode.h"
#pragma comment(lib, "QCode.lib")
#endif

using namespace UeGui;

CEEyeSettingHook::CEEyeSettingHook()
{
  m_strTitle = "编辑电子眼";
  m_vecHookFile.push_back(_T("eeyesettinghook.bin"));
}

CEEyeSettingHook::~CEEyeSettingHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CEEyeSettingHook::Load()
{
  ShowInfo();
  ShowSettings();
}

void CEEyeSettingHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_DistLabel,	"DistLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_AddrText,	"AddrText"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_AddrLabel,	"AddrLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_QCodeText,	"QCodeText"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_QCodeLabel,	"QCodeLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_FixPosBtn,	"FixPosBtn"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeTypeText,	"EeyeTypeText"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnOne,	"EeyeBtnOne"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnOneIcon,	"EeyeBtnOneIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnOneLabel,	"EeyeBtnOneLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnTwo,	"EeyeBtnTwo"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnTwoIcon,	"EeyeBtnTwoIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnTwoLabel,	"EeyeBtnTwoLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnThree,	"EeyeBtnThree"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnThreeIcon,	"EeyeBtnThreeIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnThreeLabel,	"EeyeBtnThreeLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnFour,	"EeyeBtnFour"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnFourIcon,	"EeyeBtnFourIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_EeyeBtnFourLabel,	"EeyeBtnFourLabel"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_LimitSpeedText,	"LimitSpeedText"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_AddSpeedBtn,	"AddSpeedBtn"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_AddSpeedBtnIcon,	"AddSpeedBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_SpeedLabelLeft,	"SpeedLabelLeft"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_SpeedLabelCenter,	"SpeedLabelCenter"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_SpeedLabelRight,	"SpeedLabelRight"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_MinusSpeedBtn,	"MinusSpeedBtn"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_MinusSpeedBtnIcon,	"MinusSpeedBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_SaveBtn,	"SaveBtn"));
  m_ctrlNames.insert(GuiName::value_type(EEyeSettingHook_ButtomBack,	"ButtomBack"));
}

void CEEyeSettingHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_nameLabel.SetLabelElement(GetGuiElement(EEyeSettingHook_DistLabel));
  m_addrLabel.SetLabelElement(GetGuiElement(EEyeSettingHook_AddrLabel));
  m_qCodeLabel.SetLabelElement(GetGuiElement(EEyeSettingHook_QCodeLabel));

  m_fixPosBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_FixPosBtn));

  m_redLightBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_EeyeBtnOne));
  m_redLightBtn.SetIconElement(GetGuiElement(EEyeSettingHook_EeyeBtnOneIcon));
  m_redLightBtn.SetLabelElement(GetGuiElement(EEyeSettingHook_EeyeBtnOneLabel));

  m_speedBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_EeyeBtnTwo));
  m_speedBtn.SetIconElement(GetGuiElement(EEyeSettingHook_EeyeBtnTwoIcon));
  m_speedBtn.SetLabelElement(GetGuiElement(EEyeSettingHook_EeyeBtnTwoLabel));

  m_ruleBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_EeyeBtnThree));
  m_ruleBtn.SetIconElement(GetGuiElement(EEyeSettingHook_EeyeBtnThreeIcon));
  m_ruleBtn.SetLabelElement(GetGuiElement(EEyeSettingHook_EeyeBtnThreeLabel));

  m_channelBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_EeyeBtnFour));
  m_channelBtn.SetIconElement(GetGuiElement(EEyeSettingHook_EeyeBtnFourIcon));
  m_channelBtn.SetLabelElement(GetGuiElement(EEyeSettingHook_EeyeBtnFourLabel));

  m_addSpeedBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_AddSpeedBtn));
  m_addSpeedBtn.SetIconElement(GetGuiElement(EEyeSettingHook_AddSpeedBtnIcon));
  m_minusSpeedBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_MinusSpeedBtn));
  m_minusSpeedBtn.SetIconElement(GetGuiElement(EEyeSettingHook_MinusSpeedBtnIcon));
  m_speedLabel.SetLabelElement(GetGuiElement(EEyeSettingHook_SpeedLabelCenter));

  m_saveBtn.SetCenterElement(GetGuiElement(EEyeSettingHook_SaveBtn));
}

short CEEyeSettingHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case EEyeSettingHook_AddSpeedBtn:
  case EEyeSettingHook_AddSpeedBtnIcon:
    {
      m_addSpeedBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_addSpeedBtn);
    }
    break;
  case EEyeSettingHook_MinusSpeedBtn:
  case EEyeSettingHook_MinusSpeedBtnIcon:
    {
      m_minusSpeedBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_minusSpeedBtn);
    }
    break;
  case EEyeSettingHook_FixPosBtn:
    {
      m_fixPosBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_fixPosBtn);
    }
    break;
  case EEyeSettingHook_SaveBtn:
    {
      m_saveBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_saveBtn);
    }
    break;
  case EEyeSettingHook_EeyeBtnOne:
  case EEyeSettingHook_EeyeBtnOneIcon:
  case EEyeSettingHook_EeyeBtnOneLabel:
    {
      m_redLightBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_redLightBtn);
    }
    break;
  case EEyeSettingHook_EeyeBtnTwo:
  case EEyeSettingHook_EeyeBtnTwoIcon:
  case EEyeSettingHook_EeyeBtnTwoLabel:
    {
      m_speedBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_speedBtn);
    }
    break;
  case EEyeSettingHook_EeyeBtnThree:
  case EEyeSettingHook_EeyeBtnThreeIcon:
  case EEyeSettingHook_EeyeBtnThreeLabel:
    {
      m_ruleBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_ruleBtn);
    }
    break;
  case EEyeSettingHook_EeyeBtnFour:
  case EEyeSettingHook_EeyeBtnFourIcon:
  case EEyeSettingHook_EeyeBtnFourLabel:
    {
      m_channelBtn.MouseDown();
      MOUSEDOWN_1RENDERCTRL(m_channelBtn);
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

short CEEyeSettingHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CEEyeSettingHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CMenuBackgroundHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case EEyeSettingHook_AddSpeedBtn:
  case EEyeSettingHook_AddSpeedBtnIcon:
    {
      m_addSpeedBtn.MouseUp();
      if (m_addSpeedBtn.IsEnable())
      {
        FixSpeed(true);
      }
    }
    break;
  case EEyeSettingHook_MinusSpeedBtn:
  case EEyeSettingHook_MinusSpeedBtnIcon:
    {
      m_minusSpeedBtn.MouseUp();
      if (m_minusSpeedBtn.IsEnable())
      {
        FixSpeed(false);
      }
    }
    break;
  case EEyeSettingHook_FixPosBtn:
    {
      m_fixPosBtn.MouseUp();
      if (m_fixPosBtn.IsEnable())
      {
        FixEEyeDataPos();
      }
    }
    break;
  case EEyeSettingHook_SaveBtn:
    {
      m_saveBtn.MouseUp();
      if (m_saveBtn.IsEnable())
      {
        CUserDataWrapper::Get().EditUserEEyeData(m_index, m_eEyeData);
        Return();
      }
    }
    break;
  case EEyeSettingHook_EeyeBtnOne:
  case EEyeSettingHook_EeyeBtnOneIcon:
  case EEyeSettingHook_EeyeBtnOneLabel:
    {
      m_redLightBtn.MouseUp();
      if (m_redLightBtn.IsEnable())
      {
        m_eEyeData.m_type = TVT_TrafficLights;
        ShowSettings();
      }
    }
    break;
  case EEyeSettingHook_EeyeBtnTwo:
  case EEyeSettingHook_EeyeBtnTwoIcon:
  case EEyeSettingHook_EeyeBtnTwoLabel:
    {
      m_speedBtn.MouseUp();
      if (m_speedBtn.IsEnable())
      {
        m_eEyeData.m_type = TVT_SpeedLimit;
        ShowSettings();
      }
    }
    break;
  case EEyeSettingHook_EeyeBtnThree:
  case EEyeSettingHook_EeyeBtnThreeIcon:
  case EEyeSettingHook_EeyeBtnThreeLabel:
    {
      m_ruleBtn.MouseUp();
      if (m_ruleBtn.IsEnable())
      {
        m_eEyeData.m_type = TVT_NormalCamera;
        ShowSettings();
      }
    }
    break;
  case EEyeSettingHook_EeyeBtnFour:
  case EEyeSettingHook_EeyeBtnFourIcon:
  case EEyeSettingHook_EeyeBtnFourLabel:
    {
      m_channelBtn.MouseUp();
      if (m_channelBtn.IsEnable())
      {
        m_eEyeData.m_type = TVT_TunnelPort;
        ShowSettings();
      }
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

void CEEyeSettingHook::ShowSettings()
{
  m_redLightBtn.SetCheck(false);
  m_speedBtn.SetCheck(false);
  m_ruleBtn.SetCheck(false);
  m_channelBtn.SetCheck(false);

  m_addSpeedBtn.SetEnable(false);
  m_minusSpeedBtn.SetEnable(false);

  if (m_eEyeData.m_type == TVT_TrafficLights)
  {
    m_redLightBtn.SetCheck(true);
  }
  else if (m_eEyeData.m_type == TVT_SpeedLimit)
  {
    m_speedBtn.SetCheck(true);
    m_addSpeedBtn.SetEnable(true);
    m_minusSpeedBtn.SetEnable(true);
    SetSpeedBtnEnable();
  }
  else if (m_eEyeData.m_type == TVT_NormalCamera)
  {
    m_ruleBtn.SetCheck(true);
  }
  else if (m_eEyeData.m_type == TVT_TunnelPort)
  {
    m_channelBtn.SetCheck(true);
  }
}

void CEEyeSettingHook::ShowInfo()
{
  //读取Q码
  float median = 100000.0;
  double dX = m_eEyeData.m_x / median;
  double dY = m_eEyeData.m_y / median;
  QCode_MapToCode(dX, dY, m_QCode, 9);
  //显示基本信息
  char name[128];
  ::memcpy(name, m_eEyeData.m_name, sizeof(name));
  m_nameLabel.SetCaption(name);
  ::memcpy(name, m_eEyeData.m_address, sizeof(name));
  m_addrLabel.SetCaption(name);
  char buf[12] = {};
  ::sprintf(buf, "%c%c%c-%c%c%c-%c%c%c",m_QCode[0], m_QCode[1], m_QCode[2], m_QCode[3], m_QCode[4], m_QCode[5], m_QCode[6], m_QCode[7], m_QCode[8]);
  m_qCodeLabel.SetCaption(buf);
  //显示设置信息
  ::sprintf(m_speedLabel.GetCaption(), "%dkm/h", m_eEyeData.m_speed);
  SetSpeedBtnEnable();
}

void CEEyeSettingHook::FixSpeed(bool isAddSpeed)
{
  int speed = m_eEyeData.m_speed;
  isAddSpeed ? speed+=10 : speed-=10;
  if (speed < 40)
  {
    speed = 40;
  }
  if (speed > 120)
  {
    speed = 120;
  }
  m_eEyeData.m_speed = speed;
  ::sprintf(m_speedLabel.GetCaption(), "%dkm/h", m_eEyeData.m_speed);
  SetSpeedBtnEnable();
}

void CEEyeSettingHook::SetSpeedBtnEnable()
{
  m_addSpeedBtn.SetEnable(true);
  m_minusSpeedBtn.SetEnable(true);
  if (m_eEyeData.m_speed <= 40)
  {
    m_minusSpeedBtn.SetEnable(false);
  }
  else if (m_eEyeData.m_speed >= 120)
  {
    m_addSpeedBtn.SetEnable(false);
  }
}

void CEEyeSettingHook::FixEEyeDataPos()
{
  PointInfo pointInfo;
  pointInfo.m_point.m_x = m_eEyeData.m_x;
  pointInfo.m_point.m_y = m_eEyeData.m_y;
  ::memcpy(&pointInfo.m_name, m_eEyeData.m_name, sizeof(pointInfo.m_name));
  TurnTo(DHT_MapHook);
  CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
  pMapHook->SelectPoint(pointInfo.m_point, pointInfo.m_name, this, OnFixEEyeDataPos);
}

void CEEyeSettingHook::OnFixEEyeDataPos(void *callBackObj, const UeQuery::SQLRecord* data)
{
  CEEyeSettingHook *pEEyeSettingHook = static_cast<CEEyeSettingHook*>(callBackObj);
  if (pEEyeSettingHook)
  {
    pEEyeSettingHook->DoFixEEyeDataPos(data);
    pEEyeSettingHook->Load();
  }
}

void CEEyeSettingHook::DoFixEEyeDataPos(const UeQuery::SQLRecord* data)
{
  m_eEyeData.m_x = data->m_x;
  m_eEyeData.m_y = data->m_y;
  ::memcpy(m_eEyeData.m_name, data->m_uniName, sizeof(m_eEyeData.m_name));
}

void CEEyeSettingHook::SetEditEEyeData(int index, UserEEyeEntryData *pEEyeData)
{
  m_index = index;
  ::memcpy(&m_eEyeData, pEEyeData, sizeof(UserEEyeEntryData));
  if (m_eEyeData.m_speed == 0)
  {
    m_eEyeData.m_speed = 40;
  }
}