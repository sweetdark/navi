#include "uebase\pathconfig.h"
using namespace UeBase;

#include "productactivationhook.h"
#include "messagedialoghook.h"
using namespace UeGui;

CProduceActivationHook::CProduceActivationHook()
{
  m_seriesNum_Len = 0;
  m_activationCode_Len = 0;
  ::memset(m_seriesNum, 0 ,sizeof(m_seriesNum));
  ::memset(m_activationCode, 0, sizeof(m_activationCode));
}

CProduceActivationHook::~CProduceActivationHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CProduceActivationHook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
  for(unsigned short i = ProduceActivation_Begin; i < ProduceActivation_End; i++)
  {
    AddRenderElements(GetGuiElement(i));
  }
}

tstring CProduceActivationHook::GetBinaryFileName()
{
  return _T("productactivationhook.bin");
}

void CProduceActivationHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Background,	"Background"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Title,	"Title"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Cancel,	"Cancel"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Cancel_qu,	"Cancel_qu"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Cancel_xiao,	"Cancel_xiao"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_VersionLabel,	"VersionLabel"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_VersionCode,	"VersionCode"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_SeriesNumberLabel,	"SeriesNumberLabel"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_SeriesNumberPart1,	"SeriesNumberPart1"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_SeriesNumberPart2,	"SeriesNumberPart2"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_SeriesNumberPart3,	"SeriesNumberPart3"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_SeriesNumberPart4,	"SeriesNumberPart4"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_ActivationCodeLabel,	"ActivationCodeLabel"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_ActivationCodePart1,	"ActivationCodePart1"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_ActivationCodePart2,	"ActivationCodePart2"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_ActivationCodePart3,	"ActivationCodePart3"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_ActivationCodePart4,	"ActivationCodePart4"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode1,	"InputCode1"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode2,	"InputCode2"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode3,	"InputCode3"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode4,	"InputCode4"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode5,	"InputCode5"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode6,	"InputCode6"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode7,	"InputCode7"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode8,	"InputCode8"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode9,	"InputCode9"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode10,	"InputCode10"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode11,	"InputCode11"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode12,	"InputCode12"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode13,	"InputCode13"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode14,	"InputCode14"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode15,	"InputCode15"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_InputCode16,	"InputCode16"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_DeleteChar,	"DeleteChar"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Activation,	"Activation"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Activation_an,	"Activation_an"));
  m_ctrlNames.insert(GuiName::value_type(ProduceActivation_Activation_zhuang,	"Activation_zhuang"));
}

void CProduceActivationHook::MakeControls()
{
  m_versionCodeCtrl.SetCenterElement(GetGuiElement(ProduceActivation_VersionCode));
  m_seriesNumberPart1Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_SeriesNumberPart1));
  m_seriesNumberPart2Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_SeriesNumberPart2));
  m_seriesNumberPart3Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_SeriesNumberPart3));
  m_seriesNumberPart4Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_SeriesNumberPart4));  
  m_activationCodePart1Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_ActivationCodePart1));
  m_activationCodePart2Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_ActivationCodePart2));
  m_activationCodePart3Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_ActivationCodePart3));
  m_activationCodePart4Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_ActivationCodePart4));    
  m_inputCode1Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode1));
  m_inputCode2Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode2));
  m_inputCode3Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode3));
  m_inputCode4Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode4));
  m_inputCode5Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode5));
  m_inputCode6Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode6));
  m_inputCode7Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode7));
  m_inputCode8Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode8));
  m_inputCode9Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode9));
  m_inputCode10Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode10));
  m_inputCode11Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode11));
  m_inputCode12Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode12));
  m_inputCode13Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode13));
  m_inputCode14Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode14));
  m_inputCode15Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode15));
  m_inputCode16Ctrl.SetCenterElement(GetGuiElement(ProduceActivation_InputCode16));
  m_cancelCtrl.SetCenterElement(GetGuiElement(ProduceActivation_Cancel));
  m_cancelCtrl.SetLeftElement(GetGuiElement(ProduceActivation_Cancel_qu));
  m_cancelCtrl.SetRightElement(GetGuiElement(ProduceActivation_Cancel_xiao));
  m_deleteCharCtrl.SetCenterElement(GetGuiElement(ProduceActivation_DeleteChar));
  m_activationCtrl.SetCenterElement(GetGuiElement(ProduceActivation_Activation));
  m_activationCtrl.SetLeftElement(GetGuiElement(ProduceActivation_Activation_an));
  m_activationCtrl.SetRightElement(GetGuiElement(ProduceActivation_Activation_zhuang));
}

short CProduceActivationHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case ProduceActivation_Cancel:
  case ProduceActivation_Cancel_qu:
  case ProduceActivation_Cancel_xiao:
    {
      m_cancelCtrl.MouseDown();
      AddRenderUiControls(&m_cancelCtrl);
    }
    break;
  case ProduceActivation_DeleteChar:
    {
      m_deleteCharCtrl.MouseDown();
      AddRenderUiControls(&m_deleteCharCtrl);
    }
    break;
  case ProduceActivation_Activation:
  case ProduceActivation_Activation_an:
  case ProduceActivation_Activation_zhuang:
    {
      m_activationCtrl.MouseDown();
      AddRenderUiControls(&m_activationCtrl);
    }
    break;
  case ProduceActivation_InputCode1:
    {
      m_inputCode1Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode1Ctrl);
    }
    break;
  case ProduceActivation_InputCode2:
    {
      m_inputCode2Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode2Ctrl);
    }
    break;
  case ProduceActivation_InputCode3:
    {
      m_inputCode3Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode3Ctrl);
    }
    break;
  case ProduceActivation_InputCode4:
    {
      m_inputCode4Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode4Ctrl);
    }
    break;
  case ProduceActivation_InputCode5:
    {
      m_inputCode5Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode5Ctrl);
    }
    break;
  case ProduceActivation_InputCode6:
    {
      m_inputCode6Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode6Ctrl);
    }
    break;
  case ProduceActivation_InputCode7:
    {
      m_inputCode7Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode7Ctrl);
    }
    break;
  case ProduceActivation_InputCode8:
    {
      m_inputCode8Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode8Ctrl);
    }
    break;
  case ProduceActivation_InputCode9:
    {
      m_inputCode9Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode9Ctrl);
    }
    break;

  case ProduceActivation_InputCode10:
    {
      m_inputCode10Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode10Ctrl);
    }
    break;
  case ProduceActivation_InputCode11:
    {
      m_inputCode11Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode11Ctrl);
    }
    break;
  case ProduceActivation_InputCode12:
    {
      m_inputCode12Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode12Ctrl);
    }
    break;
  case ProduceActivation_InputCode13:
    {
      m_inputCode13Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode13Ctrl);
    }
    break;
  case ProduceActivation_InputCode14:
    {
      m_inputCode14Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode14Ctrl);
    }
    break;
  case ProduceActivation_InputCode15:
    {
      m_inputCode15Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode15Ctrl);
    }
    break;
  case ProduceActivation_InputCode16:
    {
      m_inputCode16Ctrl.MouseDown();
      AddRenderUiControls(&m_inputCode16Ctrl);
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      assert(false);
    }
    break;
  }
  if (m_isNeedRefesh)
  {
    this->Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CProduceActivationHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CProduceActivationHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool needDo = ctrlType == m_downElementType;
  switch(m_downElementType)
  {
    case ProduceActivation_Cancel:
    case ProduceActivation_Cancel_qu:
    case ProduceActivation_Cancel_xiao:
      {
        m_cancelCtrl.MouseUp();
        if (needDo)
        {
          DoClose();
        }
      }
      break;
    case ProduceActivation_DeleteChar:
      {
        m_deleteCharCtrl.MouseUp();
        if (needDo)
        {
          DoDeleteChar();
        }
      }
      break;
    case ProduceActivation_Activation:
    case ProduceActivation_Activation_an:
    case ProduceActivation_Activation_zhuang:
      {
        m_activationCtrl.MouseUp();
        if (needDo)
        {
          DoActivation();
        }
      }
      break;
    case ProduceActivation_InputCode1:
      {
        m_inputCode1Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode1Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode2:
      {
        m_inputCode2Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode2Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode3:
      {
        m_inputCode3Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode3Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode4:
      {
        m_inputCode4Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode4Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode5:
      {
        m_inputCode5Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode5Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode6:
      {
        m_inputCode6Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode6Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode7:
      {
        m_inputCode7Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode7Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode8:
      {
        m_inputCode8Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode8Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode9:
      {
        m_inputCode9Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode9Ctrl.GetCaption());
        }
      }
      break;

    case ProduceActivation_InputCode10:
      {
        m_inputCode10Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode10Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode11:
      {
        m_inputCode11Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode11Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode12:
      {
        m_inputCode12Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode12Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode13:
      {
        m_inputCode13Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode13Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode14:
      {
        m_inputCode14Ctrl.MouseUp();
        if (needDo)
        { 
          AddActivationCode(*m_inputCode14Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode15:
      {
        m_inputCode15Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode15Ctrl.GetCaption());
        }
      }
      break;
    case ProduceActivation_InputCode16:
      {
        m_inputCode16Ctrl.MouseUp();
        if (needDo)
        {
          AddActivationCode(*m_inputCode16Ctrl.GetCaption());
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

bool CProduceActivationHook::operator ()()
{
  return false;
}


void UeGui::CProduceActivationHook::Init()
{
  m_seriesNum_Len = 0;
  m_activationCode_Len = 0;
  ClearSeries();
  ClearActivationCode();
#if (__UE_WINCE__)
  //TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
  //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);

  tstring rootPath = CPathConfig::GetRootPath();
  char chPath[CPathBasic::MAXPATHLENGTH] = {};
  ::WideCharToMultiByte(CP_ACP, 0, rootPath.c_str(), -1, chPath, CPathBasic::MAXPATHLENGTH, 0, 0);
  //获取序列号
  char uuid[17] = {};
  int flag = IView::GetView()->GetMediator()->LsGetUUID(chPath, uuid);
  if (0 == flag)
  {
    //序列号获取成功
    for (unsigned int i = 0; i < ::strlen(uuid); ++i)
    {
      DoAddChar(uuid[i]);
    }
  } 
  else
  {
    ShowMessageDialog(MB_INFORMATION, "序列号无效!");
  }
#endif
  m_seriesNum_Len = MAX_NUM_LEN;
}

void UeGui::CProduceActivationHook::DoAddChar( char addChar )
{
  if (m_seriesNum_Len < MAX_NUM_LEN)
  {
    //添加到序列号
    AddSeriesNum(addChar);
  }
  else
  {
    //添加到激活码
    AddActivationCode(addChar);
  }
}

void UeGui::CProduceActivationHook::DoDeleteChar()
{
  if (m_activationCode > 0)
  {
    //删除激活码
    DeleteActivationCode();
  }
  else
  {
    //删除序列号
    //DeleteSeriesNum();
  }
}

void UeGui::CProduceActivationHook::AddSeriesNum( char addChar )
{
  if (m_seriesNum_Len >= MAX_NUM_LEN)
  {
    return;
  }
  m_seriesNum_Len++;
  m_seriesNum[m_seriesNum_Len - 1] = addChar;
  ShowSeries(m_seriesNum, m_seriesNum_Len);
}

void UeGui::CProduceActivationHook::DeleteSeriesNum()
{
  if (m_seriesNum_Len <= 0)
  {
    return;
  }
  m_seriesNum[m_seriesNum_Len - 1] = '\0';
  m_seriesNum_Len--;
  ShowSeries(m_seriesNum, m_seriesNum_Len);
}

void UeGui::CProduceActivationHook::ShowSeries( const char* series, unsigned short seriesLen)
{
  if (seriesLen <= 0)
  {
    return;
  }

  ClearSeries();

  unsigned short partCount = seriesLen / PART_LENT;
  if ((seriesLen % PART_LENT) > 0)
  {
    partCount += 1;
  }

  const char* dataStart = series;
  char partData[5];
  for (int i = 0; i <= partCount - 1; ++i)
  {
    dataStart = series + (i * PART_LENT);
    ::memset(partData, 0, sizeof(partData));
    ::memcpy(partData, dataStart, PART_LENT);
    ShowSeriesPart(i, partData);
  }
}

void UeGui::CProduceActivationHook::ShowSeriesPart( unsigned int partID, char* partData )
{
  switch (partID)
  {
  case kPart1:
    {
      m_seriesNumberPart1Ctrl.SetCaption(partData);
      break;
    }
  case kPart2:
    {
      m_seriesNumberPart2Ctrl.SetCaption(partData);
      break;
    }
  case kPart3:
    {
      m_seriesNumberPart3Ctrl.SetCaption(partData);
      break;
    }
  case kPart4:
    {
      m_seriesNumberPart4Ctrl.SetCaption(partData);
      break;
    }
  }
}
void UeGui::CProduceActivationHook::ShowActivationCode( const char* code, unsigned short codeLen )
{
  ClearActivationCode();

  if (codeLen <= 0)
  {
    return;
  }

  unsigned short partCount = codeLen / PART_LENT;
  unsigned short n = 5 % 6;
  if ((partCount * PART_LENT) < codeLen)
  {
    partCount += 1;
  }

  const char* dataStart = code;
  char partData[5];
  for (int i = 0; i <= partCount - 1; ++i)
  {
    dataStart = code + (i * PART_LENT);
    ::memset(partData, 0, sizeof(partData));
    ::memcpy(partData, dataStart, PART_LENT);
    ShowActivationCodePart(i, partData);
  }
}

void UeGui::CProduceActivationHook::ShowActivationCodePart( unsigned int partID, char* partData )
{
  switch (partID)
  {
  case kPart1:
    {
      m_activationCodePart1Ctrl.SetCaption(partData);
      break;
    }
  case kPart2:
    {
      m_activationCodePart2Ctrl.SetCaption(partData);
      break;
    }
  case kPart3:
    {
      m_activationCodePart3Ctrl.SetCaption(partData);
      break;
    }
  case kPart4:
    {
      m_activationCodePart4Ctrl.SetCaption(partData);
      break;
    }
  }
}

void UeGui::CProduceActivationHook::ClearSeries()
{
  m_seriesNumberPart1Ctrl.ClearCaption();
  m_seriesNumberPart2Ctrl.ClearCaption();
  m_seriesNumberPart3Ctrl.ClearCaption();
  m_seriesNumberPart4Ctrl.ClearCaption();
}

void UeGui::CProduceActivationHook::ClearActivationCode()
{
  m_activationCodePart1Ctrl.ClearCaption();
  m_activationCodePart2Ctrl.ClearCaption();
  m_activationCodePart3Ctrl.ClearCaption();
  m_activationCodePart4Ctrl.ClearCaption();
}

void UeGui::CProduceActivationHook::AddActivationCode( char addChar )
{
  if (m_activationCode_Len >= MAX_NUM_LEN)
  {
    return;
  }
  m_activationCode_Len++;
  m_activationCode[m_activationCode_Len - 1] = addChar;
  ShowActivationCode(m_activationCode, m_activationCode_Len);
}

void UeGui::CProduceActivationHook::DeleteActivationCode()
{  
  if (m_activationCode_Len <= 0)
  {
    return;
  }
  m_activationCode[m_activationCode_Len - 1] = '\0';
  m_activationCode_Len--;
  ShowActivationCode(m_activationCode, m_activationCode_Len);
}

void UeGui::CProduceActivationHook::DoClose()
{
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_ProduceActivationHook, &UeGui::CProduceActivationHook::OnClose);
  CMessageDialogHook::ShowMessageDialog(MB_INFORMATION, "退出软件吗？", dialogEvent);  
}

void UeGui::CProduceActivationHook::DoActivation()
{
  if (::strlen(m_seriesNum) < MAX_NUM_LEN)
  {
    //序列号无效
    ShowMessageDialog(MB_CONFIRM, "序列号无效！");
    return;
  }
  if (::strlen(m_activationCode) < MAX_NUM_LEN)
  {
    //激活码不合法
    ShowMessageDialog(MB_CONFIRM, "请输入16位激活码！");
    return;
  }

  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_ProduceActivationHook, &UeGui::CProduceActivationHook::OnActivateProduct);
  CMessageDialogHook::ShowMessageDialog(MB_INFORMATION, "确定要激活地图？", dialogEvent);

}

void UeGui::CProduceActivationHook::ShowMessageDialog( MessageBoxType type, const char* caption )
{
  CMessageDialogEvent dialogEvent(this, CViewHook::DHT_ProduceActivationHook);
  CMessageDialogHook::ShowMessageDialog(type, caption, dialogEvent);
}

void UeGui::CProduceActivationHook::OnClose( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CProduceActivationHook* activationHook = dynamic_cast<CProduceActivationHook*>(sender);
    activationHook->Close();
  }
}

void UeGui::CProduceActivationHook::Close()
{
  ::SendMessage((HWND)m_view->GetWndHandle(), WM_DESTROY, NULL, NULL);
}

void UeGui::CProduceActivationHook::ActivateProduct()
{
  int flag = -1;
#if (__UE_WINCE__)
  //检测激活码是否有效
  flag = IView::GetView()->GetMediator()->LsCheckActiveID(m_seriesNum, m_activationCode);

  //////////////////////////////////////////////////////////////////////////
  ////测试日志
  //char logMsg[50] = {};
  //::sprintf(logMsg, "uid:[%s] activationCode:[%s] falg:[%d]  ", m_seriesNum, m_activationCode, flag); 
  //LogMessage(logMsg);
  //////////////////////////////////////////////////////////////////////////
  if (0 == flag)
  {
    //TCHAR path[CPathBasic::MAXPATHLENGTH] = {0, };
    //m_pathBasic.GetModulePath(path, CPathBasic::MAXPATHLENGTH);

    tstring rootPath = CPathConfig::GetRootPath();
    char chPath[CPathBasic::MAXPATHLENGTH] = {};
    ::WideCharToMultiByte(CP_ACP, 0, rootPath.c_str(), -1, chPath, CPathBasic::MAXPATHLENGTH, 0, 0);
    //激活码合法，保存成激活文件
    int flag = IView::GetView()->GetMediator()->LsSaveActiveFile(chPath, m_seriesNum, m_activationCode);
    if (0 == flag)
    {
      ShowMessageDialog(MB_NONE, "产品激活成功！");
      ::Sleep(500);
      Close();
    }
    else
    {
      ShowMessageDialog(MB_CONFIRM, "产品激活失败！");
    }
    return;
  }
#endif
  ShowMessageDialog(MB_CONFIRM, "激活码无效！");
}

void UeGui::CProduceActivationHook::OnActivateProduct( CAggHook* sender, ModalResultType modalResult )
{
  if (modalResult == MR_OK)
  {
    CProduceActivationHook* activationHook = dynamic_cast<CProduceActivationHook*>(sender);
    activationHook->ActivateProduct();
  }
}
