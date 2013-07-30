#include "detailmessagehook.h"
#include "detailedithook.h"
#include "uemap/viewcanvas.h"
#include "gui.h"
#include "messagedialoghook.h"
#include "userdatawrapper.h"
using namespace UeGui;

CDetailMessageHook::CDetailMessageHook()
{
}

CDetailMessageHook::~CDetailMessageHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CDetailMessageHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  SetMessageVisble(false,CT_begin);
  SetCommonlyUsedVisiable(false);
}

void CDetailMessageHook::Init()
{
  SetMessageVisble(false,CT_begin);
  SetCommonlyUsedVisiable(false);
}

tstring CDetailMessageHook::GetBinaryFileName()
{
  return _T("detailmessagehook.bin");
}

void CDetailMessageHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AddressInfoLabel,	"AddressInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AddressLabel,	"AddressLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_BlackGround,	"BlackGround"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed1,	"CommonlyUsed1"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed1Left,	"CommonlyUsed1Left"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed1Right,	"CommonlyUsed1Right"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed2,	"CommonlyUsed2"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed2Left,	"CommonlyUsed2Left"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed2Right,	"CommonlyUsed2Right"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed3,	"CommonlyUsed3"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed3Left,	"CommonlyUsed3Left"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CommonlyUsed3Right,	"CommonlyUsed3Right"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_Company,	"Company"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CompanyLeft,	"CompanyLeft"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_CompanyRight,	"CompanyRight"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_HeadInfoLabel,	"HeadInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_HeadInfoLabel1,	"HeadInfoLabel1"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_HeadInfoLabel2,	"HeadInfoLabel2"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_Home,	"Home"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_HomeLeft,	"HomeLeft"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_HomeRight,	"HomeRight"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_QCodeInfoLabel,	"QCodeInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_QCodeLabel,	"QCodeLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SaveInfo,	"SaveInfo"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SaveInfoLeft,	"SaveInfoLeft"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SaveInfoRight,	"SaveInfoRight"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SetCommonlyUsed,	"SetCommonlyUsed"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SetCommonlyUsedLeft,	"SetCommonlyUsedLeft"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_SetCommonlyUsedRight,	"SetCommonlyUsedRight"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_TelephoneInfoLabel,	"TelephoneInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_TelephoneLabel,	"TelephoneLabel"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_MessageBoxBtn,"MessageBoxBtn"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_MessageBoxText,"MessageBoxText"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_RejectCtrlCenter,"RejectCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_RejectCtrlLeft,"RejectCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_RejectCtrlRight,"RejectCtrlRight"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AcceptCtrlCenter,"AcceptCtrlCenter"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AcceptCtrlLeft,"AcceptCtrlLeft"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_AcceptCtrlRight,"AcceptCtrlRight"));
  m_ctrlNames.insert(GuiName::value_type(DetailMessageHook_IconTip,"IconTip"));
}

void CDetailMessageHook::MakeControls()
{
  m_commonlyUsed1Ctrl.SetCenterElement(GetGuiElement(DetailMessageHook_CommonlyUsed1));
  m_commonlyUsed1Ctrl.SetLeftElement(GetGuiElement(DetailMessageHook_CommonlyUsed1Left));
  m_commonlyUsed1Ctrl.SetRightElement(GetGuiElement(DetailMessageHook_CommonlyUsed1Right));
  m_commonlyUsed2Ctrl.SetCenterElement(GetGuiElement(DetailMessageHook_CommonlyUsed2));
  m_commonlyUsed2Ctrl.SetLeftElement(GetGuiElement(DetailMessageHook_CommonlyUsed2Left));
  m_commonlyUsed2Ctrl.SetRightElement(GetGuiElement(DetailMessageHook_CommonlyUsed2Right));
  m_commonlyUsed3Ctrl.SetCenterElement(GetGuiElement(DetailMessageHook_CommonlyUsed3));
  m_commonlyUsed3Ctrl.SetLeftElement(GetGuiElement(DetailMessageHook_CommonlyUsed3Left));
  m_commonlyUsed3Ctrl.SetRightElement(GetGuiElement(DetailMessageHook_CommonlyUsed3Right));
  m_companyCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_Company));
  m_companyCtrl.SetLeftElement(GetGuiElement(DetailMessageHook_CompanyLeft));
  m_companyCtrl.SetRightElement(GetGuiElement(DetailMessageHook_CompanyRight));
  m_homeCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_Home));
  m_homeCtrl.SetLeftElement(GetGuiElement(DetailMessageHook_HomeLeft));
  m_homeCtrl.SetRightElement(GetGuiElement(DetailMessageHook_HomeRight));

  m_qCodeInfoLabelCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_QCodeInfoLabel));
  m_telephoneInfoLabelCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_TelephoneInfoLabel));
  m_addressInfoLabelCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_AddressInfoLabel));

  m_saveInfoCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_SaveInfo));
  m_saveInfoCtrl.SetLeftElement(GetGuiElement(DetailMessageHook_SaveInfoLeft));
  m_saveInfoCtrl.SetRightElement(GetGuiElement(DetailMessageHook_SaveInfoRight));
  m_setCommonlyUsedCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_SetCommonlyUsed));
  m_setCommonlyUsedCtrl.SetLeftElement(GetGuiElement(DetailMessageHook_SetCommonlyUsedLeft));
  m_setCommonlyUsedCtrl.SetRightElement(GetGuiElement(DetailMessageHook_SetCommonlyUsedRight));

  m_messageBoxBtnCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_MessageBoxBtn));
  m_messageBoxBtnCtrl.SetIconElement(GetGuiElement(DetailMessageHook_IconTip));
  m_messageBoxTextCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_MessageBoxText));
  m_rejectCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_RejectCtrlCenter));
  m_rejectCtrl.SetLeftElement(GetGuiElement(DetailMessageHook_RejectCtrlLeft));
  m_rejectCtrl.SetRightElement(GetGuiElement(DetailMessageHook_RejectCtrlRight));
  m_acceptCtrl.SetCenterElement(GetGuiElement(DetailMessageHook_AcceptCtrlCenter));
  m_acceptCtrl.SetLeftElement(GetGuiElement(DetailMessageHook_AcceptCtrlLeft));
  m_acceptCtrl.SetRightElement(GetGuiElement(DetailMessageHook_AcceptCtrlRight));
}

short CDetailMessageHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case DetailMessageHook_CommonlyUsed1:
  case DetailMessageHook_CommonlyUsed1Left:
  case DetailMessageHook_CommonlyUsed1Right:
    {
      m_commonlyUsed1Ctrl.MouseDown();
    }
    break;
  case DetailMessageHook_CommonlyUsed2:
  case DetailMessageHook_CommonlyUsed2Left:
  case DetailMessageHook_CommonlyUsed2Right:
    {
      m_commonlyUsed2Ctrl.MouseDown();
    }
    break;
  case DetailMessageHook_CommonlyUsed3:
  case DetailMessageHook_CommonlyUsed3Left:
  case DetailMessageHook_CommonlyUsed3Right:
    {
      m_commonlyUsed3Ctrl.MouseDown();
    }
    break;
  case DetailMessageHook_Company:
  case DetailMessageHook_CompanyLeft:
  case DetailMessageHook_CompanyRight:
    {
      m_companyCtrl.MouseDown();
    }
    break;
  case DetailMessageHook_Home:
  case DetailMessageHook_HomeLeft:
  case DetailMessageHook_HomeRight:
    {
      m_homeCtrl.MouseDown();
    }
    break;
  case DetailMessageHook_SaveInfo:
  case DetailMessageHook_SaveInfoLeft:
  case DetailMessageHook_SaveInfoRight:
    {
      m_saveInfoCtrl.MouseDown();
    }
    break;
  case DetailMessageHook_SetCommonlyUsed:
  case DetailMessageHook_SetCommonlyUsedLeft:
  case DetailMessageHook_SetCommonlyUsedRight:
    {
      m_setCommonlyUsedCtrl.MouseDown();
    }
    break;
  case DetailMessageHook_AcceptCtrlLeft:
  case DetailMessageHook_AcceptCtrlRight:
  case DetailMessageHook_AcceptCtrlCenter:
    {
      m_acceptCtrl.MouseDown();
    }
    break;
  case DetailMessageHook_RejectCtrlCenter:
  case DetailMessageHook_RejectCtrlLeft:
  case DetailMessageHook_RejectCtrlRight:
    {
      m_rejectCtrl.MouseDown();
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

short CDetailMessageHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDetailMessageHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  //重置倒计时值 因为主计时器是500ms定时，所以这里需要乘以2
  m_statusBarTimeInterval=STATUSBAR_TIMEINTERVAL*2;
  short ctrlType = CAggHook::MouseUp(scrPoint);

  //是否成功设置常用点
  bool isSetting = false;
  //
  switch(m_downElementType)
  {
  case DetailMessageHook_CommonlyUsed1:
  case DetailMessageHook_CommonlyUsed1Left:
  case DetailMessageHook_CommonlyUsed1Right:
    {
      m_commonlyUsed1Ctrl.MouseUp();
      if(SetMessageVisble(IsCommonExist(CT_ONE),CT_ONE))
      {
        m_cType=CT_ONE;
      }
      else
      {
        isSetting = true;
        SetFavoFileContent(CT_ONE);
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      }
    }
    break;
  case DetailMessageHook_CommonlyUsed2:
  case DetailMessageHook_CommonlyUsed2Left:
  case DetailMessageHook_CommonlyUsed2Right:
    {
      m_commonlyUsed2Ctrl.MouseUp();
      if(SetMessageVisble(IsCommonExist(CT_TWO),CT_TWO))
      {
        m_cType=CT_TWO;
      }
      else
      {
        isSetting = true;
        SetFavoFileContent(CT_TWO);
        //m_view->Update(UeRoute::ST_RenderFull);
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
        //CAggHook::SkipToHook(CViewHook::DHT_MapHook);
        //CViewHook::m_prevHookType = CViewHook::DHT_DetailMessageHook;
      }
    }
    break;
  case DetailMessageHook_CommonlyUsed3:
  case DetailMessageHook_CommonlyUsed3Left:
  case DetailMessageHook_CommonlyUsed3Right:
    {
      m_commonlyUsed3Ctrl.MouseUp();
      if(SetMessageVisble(IsCommonExist(CT_THREE),CT_THREE))
      {
        m_cType=CT_THREE;
      }
      else
      {
        isSetting = true;
        SetFavoFileContent(CT_THREE);
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      }
    }
    break;
  case DetailMessageHook_Company:
  case DetailMessageHook_CompanyLeft:
  case DetailMessageHook_CompanyRight:
    {
      m_companyCtrl.MouseUp();
      if(SetMessageVisble(IsCommonExist(CT_COMPANY),CT_COMPANY))
      {
        m_cType=CT_COMPANY;
      }
      else
      {
        isSetting = true;
        SetFavoFileContent(CT_COMPANY);
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      }
    }
    break;
  case DetailMessageHook_AcceptCtrlLeft:
  case DetailMessageHook_AcceptCtrlRight:
  case DetailMessageHook_AcceptCtrlCenter:
    {
      m_acceptCtrl.MouseUp();
      SetMessageVisble(false,m_cType);
      SetFavoFileContent(m_cType);
      CViewHook::m_curHookType = CViewHook::DHT_MapHook;
    }
    break;
  case DetailMessageHook_RejectCtrlCenter:
  case DetailMessageHook_RejectCtrlLeft:
  case DetailMessageHook_RejectCtrlRight:
    {
      m_rejectCtrl.MouseUp();
      SetMessageVisble(false,m_cType);
      CViewHook::m_curHookType = CViewHook::DHT_MapHook;
    }
    break;
  case DetailMessageHook_Home:
  case DetailMessageHook_HomeLeft:
  case DetailMessageHook_HomeRight:
    {
      m_homeCtrl.MouseUp();
      if(SetMessageVisble(IsCommonExist(CT_HOME),CT_HOME))
      {
        m_cType=CT_HOME;
      }
      else
      {
        isSetting = true;
        SetFavoFileContent(CT_HOME);
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      }     
    }
    break;
  case DetailMessageHook_SaveInfo:
  case DetailMessageHook_SaveInfoLeft:
  case DetailMessageHook_SaveInfoRight:
    {
      
      m_saveInfoCtrl.MouseUp();
      //m_view->Update(UeRoute::ST_RenderFull);
      SetDetailEditHookDate();
    }
    break;
  case DetailMessageHook_SetCommonlyUsed:
  case DetailMessageHook_SetCommonlyUsedLeft:
  case DetailMessageHook_SetCommonlyUsedRight:
    {
      m_setCommonlyUsedCtrl.MouseUp();
      SetSaveControlVisiable(false);
      SetCommonlyUsedVisiable(true);
    }
    break;
  default:
    if(!m_messageBoxBtnCtrl.IsVisible())
    {
      assert(false);
      {
        CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      }
    }
    else
    {
      m_isNeedRefesh = false;
      break;
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  if(isSetting)
  {
    CMessageDialogEvent dialogEvent(this, DHT_MapHook, NULL);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "设置成功！", dialogEvent);
    CMessageDialogHook::CloseMessageDialog();
    Sleep(1000);
    Refresh();
  }
  m_isNeedRefesh = true;

  return ctrlType;
}

bool CDetailMessageHook::operator ()()
{
  return false;
}

void CDetailMessageHook::SetDetailInfo(stDetailInfo* pDetailInfo)
{

  //设置定时器。
  needCountDown=true;
  m_statusBarTimeInterval=STATUSBAR_TIMEINTERVAL;

  assert(pDetailInfo);
  char buf[256] = {0, };
  float median = 100000.0;
  dX = pDetailInfo->position.m_x / median;
  dY = pDetailInfo->position.m_y / median;
  nCode = 9;
  QCode_MapToCode(dX,dY,pCode,nCode);
  ::sprintf(buf, "%c%c%c-%c%c%c-%c%c%c",pCode[0],pCode[1],pCode[2],pCode[3],pCode[4],pCode[5],pCode[6],pCode[7],pCode[8]);
  m_qCodeInfoLabelCtrl.SetCaption(buf);
  
  unsigned int slen = ::strlen(pDetailInfo->name);
  char routeName[128] = {};
  if (0 == slen)
  {
    CGeoPoint<long> pickPos;
    pickPos.m_x = pDetailInfo->position.m_x;
    pickPos.m_y = pDetailInfo->position.m_y;
    IGui* gui = IGui::GetGui();
    gui->GetDistrictName(pickPos, m_detailInfo.name);
  } 
  else
  {
    unsigned int size = slen < sizeof(routeName) ? slen : sizeof(routeName);
    ::memcpy(routeName, pDetailInfo->name, size);
//如果最后一个字符是换行符就去掉
    if ( routeName[size-1] == '\n')
    {
          routeName[size -1] = 0;
    }
 //   routeName[size -1] = 0;
    ::sprintf(m_detailInfo.name, "%s", routeName);
  }
  ::sprintf(m_detailInfo.address, "%s", pDetailInfo->address);
  ::sprintf(m_detailInfo.telephone, "%s", pDetailInfo->telephone);
  m_detailInfo.position.m_x = pDetailInfo->position.m_x;
  m_detailInfo.position.m_y = pDetailInfo->position.m_y;  

  CViewHook::GuiElement* pOneElement = NULL;
  pOneElement = GetGuiElement(DetailMessageHook_HeadInfoLabel);
  ::sprintf(pOneElement->m_label, "%s", m_detailInfo.name);

  //判断是否分两行显示
  int ascCharNum = 0;
  for(int i=0; i<32; i++)
  {
    //第一行中出现多少个非中文的单字符
    if(pOneElement->m_label[i] > 0)
      ascCharNum++;
  }

  if (slen > 32)
  {
    pOneElement->m_isVisible =false;

    char labelUp[256], labelDown[256];
    ::strcpy(labelUp,m_detailInfo.name);
    if(ascCharNum%2 == 0)
      labelUp[32] = 0;
    else
      labelUp[31] = 0;
    ::strcpy(labelDown,m_detailInfo.name);
    for(int i= 0; i<slen-32; i++)
    {
      if(ascCharNum%2 == 0)
        labelDown[i] = labelDown[i+32];
      else
        labelDown[i] = labelDown[i+31];
    }
    labelDown[slen-32] = 0;
    pOneElement = GetGuiElement(DetailMessageHook_HeadInfoLabel1);
    ::sprintf(pOneElement->m_label, "%s", labelUp);
    pOneElement->m_isVisible =true;
    pOneElement = GetGuiElement(DetailMessageHook_HeadInfoLabel2);
    ::sprintf(pOneElement->m_label, "%s", labelDown);
    pOneElement->m_isVisible =true;
  }
  else
  {
    pOneElement->m_isVisible = true;

    pOneElement = GetGuiElement(DetailMessageHook_HeadInfoLabel1);
    pOneElement->m_isVisible =false;
    pOneElement = GetGuiElement(DetailMessageHook_HeadInfoLabel2);
    pOneElement->m_isVisible =false;
  }
  pOneElement = GetGuiElement(DetailMessageHook_AddressInfoLabel);
  if(pOneElement) ::sprintf(pOneElement->m_label, "%s", m_detailInfo.address);
  pOneElement = GetGuiElement(DetailMessageHook_TelephoneInfoLabel);
  if(pOneElement) ::sprintf(pOneElement->m_label, "%s", m_detailInfo.telephone);
  SetSaveControlVisiable(true);
  SetCommonlyUsedVisiable(false);
}
/**
* \brief 设置常用点模块可用性
*/
void CDetailMessageHook::SetCommonlyUsedVisiable(bool isVisiabled)
{
  m_homeCtrl.SetVisible(isVisiabled);
  m_companyCtrl.SetVisible(isVisiabled);
  m_commonlyUsed1Ctrl.SetVisible(isVisiabled);
  m_commonlyUsed2Ctrl.SetVisible(isVisiabled);
  m_commonlyUsed3Ctrl.SetVisible(isVisiabled);
}
/**
* \brief 设置保存操作模块可用性
*/
void CDetailMessageHook::SetSaveControlVisiable(bool isVisiabled)
{
  m_setCommonlyUsedCtrl.SetVisible(isVisiabled);
  m_saveInfoCtrl.SetVisible(isVisiabled);
}
void CDetailMessageHook::AsMsgBox(stDetailInfo detailInfo)
{

}

void UeGui::CDetailMessageHook::SetDetailEditHookDate()
{
  CDetailEditEvent editEvent(this,DHT_DetailMessageHook);
  EditData edata;
  CGeoPoint<long> pickPos;
  m_view->GetPickPos(pickPos);
  FavoriteEntry curEntry;
  curEntry.m_teleNumber = 0;
  curEntry.m_addrCode = 0;
  curEntry.m_x=pickPos.m_x;
  curEntry.m_y=pickPos.m_y;
  CViewHook::GuiElement* pOneElement = NULL;
  pOneElement = GetGuiElement(DetailMessageHook_HeadInfoLabel);
  char *tmp=::strstr(pOneElement->m_label,"\n");

  if(tmp!=NULL)
  {
    ::strncpy((char *)curEntry.m_name,CViewCanvas::GetSelectedName(),strlen(pOneElement->m_label)-1);
  }
  else
  {
    ::strcpy((char *)curEntry.m_name,pOneElement->m_label);
  }      

  pOneElement = GetGuiElement(DetailMessageHook_AddressInfoLabel);
  if (pOneElement->m_label != NULL && ::strlen(pOneElement->m_label) > 1)
  {
    ::strcpy((char*)edata.m_addrCode, pOneElement->m_label);
  }

  pOneElement = GetGuiElement(DetailMessageHook_TelephoneInfoLabel);
  if (pOneElement->m_label != NULL && ::strlen(pOneElement->m_label) > 1)
  {
    ::strcpy((char*)edata.m_telephone, pOneElement->m_label);
  }

  edata.m_x=curEntry.m_x;
  edata.m_y=curEntry.m_y;
  edata.m_kind=curEntry.m_kind;
  ::strcpy((char *)edata.m_name,(char *)curEntry.m_name);

  /*if(curEntry.m_teleNumber!=0)
  {
    char *tmp=new char[128];
    ::strcpy((char *)edata.m_telephone,::_itoa(curEntry.m_teleNumber,tmp,10));
    delete []tmp;
  }
  else
  {
    ::strcpy((char *)edata.m_telephone,"");
  }*/

  /*if(curEntry.m_addrCode!=0)
  {
    char *tmp=new char[128];
    ::strcpy((char *)edata.m_addrCode,::_itoa(curEntry.m_addrCode,tmp,10));
    delete []tmp;
  }
  else
  {
    ::strcpy((char *)edata.m_addrCode,"");
  }*/

  if(curEntry.m_kind&0x1)
  {
    edata.m_isStartpos=true;
  }
  else
  {
    edata.m_isStartpos=false;
  }
  curEntry.m_kind=curEntry.m_kind>>1;

  if(curEntry.m_kind&0x1)
  {
    edata.m_isVoice=true;
  }
  else
  {
    edata.m_isVoice=false;
  }
  curEntry.m_kind=curEntry.m_kind>>1;

  if(curEntry.m_kind&0x1)
  {
    edata.m_isMapshow=true;
  }
  else
  {
    edata.m_isMapshow=false;
  }

  curEntry.m_kind=curEntry.m_kind>>1;

  edata.m_kind=curEntry.m_kind;

  CDetailEditHook editHook;
  editHook.ShowDetailEditHook(&edata,editEvent);
}

bool CDetailMessageHook::IsCommonExist(CommonType type)
{
  UsuallyRecord frecord;
  m_cFile.GetRecord(type,&frecord);
  if(frecord.m_y==0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool CDetailMessageHook::SetMessageVisble(bool value,CommonType type)
{
  m_messageBoxBtnCtrl.SetVisible(value);
  m_acceptCtrl.SetVisible(value);
  m_rejectCtrl.SetVisible(value);
  m_messageBoxTextCtrl.SetVisible(value);
  switch (type)
  {
  case CT_HOME:
    {
      m_messageBoxTextCtrl.SetCaption("家已存在，是否覆盖？");
    }
    break;
  case CT_COMPANY:
    {
      m_messageBoxTextCtrl.SetCaption("单位已存在，是否覆盖？");
    }
    break;
  case CT_ONE:
    {
      m_messageBoxTextCtrl.SetCaption("常用点1已存在，是否覆盖？");
    }
    break;
  case CT_TWO:
    {
      m_messageBoxTextCtrl.SetCaption("常用点2已存在，是否覆盖？");
    }
    break;
  case CT_THREE:
    {
      m_messageBoxTextCtrl.SetCaption("常用点3已存在，是否覆盖？");
    }
    break;
  default:
    {
      assert(false);
    }
  }
  return value;
}

void CDetailMessageHook::SetFavoFileContent(CommonType type)
{
  CGeoPoint<long> pickPos;
  m_view->GetPickPos(pickPos);
  UsuallyRecord curRecord;
  curRecord.m_x=pickPos.m_x;
  curRecord.m_y=pickPos.m_y;
  char *tmp=::strstr(CViewCanvas::GetSelectedName(),"\n");

  if(tmp!=NULL)
  {
    ::strncpy((char *)curRecord.m_name,CViewCanvas::GetSelectedName(),strlen(CViewCanvas::GetSelectedName())-1);
  }
  else
  {
    IGui* gui = IGui::GetGui();
    gui->GetDistrictName(pickPos, (char*)curRecord.m_name);
  }
  m_cFile.UpdateRecord(type,&curRecord);
}

void CDetailMessageHook::Timer()
{
  if (needCountDown && m_statusBarTimeInterval >= 0)
  {
    --m_statusBarTimeInterval;
    if (m_statusBarTimeInterval == 0)
    {
      needCountDown = false;
      CViewHook::m_curHookType = CViewHook::DHT_MapHook;
      Refresh();
    }
  }
}