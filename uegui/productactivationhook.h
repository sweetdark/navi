#ifndef _UEGUI_PRODUCEACTIVATION_H
#define _UEGUI_PRODUCEACTIVATION_H

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  
  class UEGUI_CLASS CProduceActivationHook : public CAggHook
  {
  public:
    const static unsigned short MAX_NUM_LEN = 16;  
    const static unsigned short PART_LENT = 4; 

    enum ProduceActivationCtrlType
    {
      ProduceActivation_Begin = 0,
      ProduceActivation_Background,
      ProduceActivation_Title,
      ProduceActivation_Cancel,
      ProduceActivation_Cancel_qu,
      ProduceActivation_Cancel_xiao,
      ProduceActivation_VersionLabel,
      ProduceActivation_VersionCode,
      ProduceActivation_SeriesNumberLabel,
      ProduceActivation_SeriesNumberPart1,
      ProduceActivation_SeriesNumberPart2,
      ProduceActivation_SeriesNumberPart3,
      ProduceActivation_SeriesNumberPart4,
      ProduceActivation_ActivationCodeLabel,
      ProduceActivation_ActivationCodePart1,
      ProduceActivation_ActivationCodePart2,
      ProduceActivation_ActivationCodePart3,
      ProduceActivation_ActivationCodePart4,
      ProduceActivation_InputCode1,
      ProduceActivation_InputCode2,
      ProduceActivation_InputCode3,
      ProduceActivation_InputCode4,
      ProduceActivation_InputCode5,
      ProduceActivation_InputCode6,
      ProduceActivation_InputCode7,
      ProduceActivation_InputCode8,
      ProduceActivation_InputCode9,
      ProduceActivation_InputCode10,
      ProduceActivation_InputCode11,
      ProduceActivation_InputCode12,
      ProduceActivation_InputCode13,
      ProduceActivation_InputCode14,
      ProduceActivation_InputCode15,
      ProduceActivation_InputCode16,
      ProduceActivation_DeleteChar,
      ProduceActivation_Activation,
      ProduceActivation_Activation_an,
      ProduceActivation_Activation_zhuang,
      ProduceActivation_End
    };

    enum SectionPart{
      kPart1,
      kPart2,
      kPart3,
      kPart4
    };
  public:
    CProduceActivationHook();

    virtual ~CProduceActivationHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    virtual void Init();
  public:
    //退出系统
    void Close();
    //退出系统事件
    static void OnClose(CAggHook* sender, ModalResultType modalResult);
    //系统激活方法
    void ActivateProduct();
    //系统激活事件
    static void OnActivateProduct(CAggHook* sender, ModalResultType modalResult);
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    //添加字符
    void DoAddChar(char addChar);
    //删除字符
    void DoDeleteChar();

    //添加一个字符到序列号
    void AddSeriesNum(char addChar);
    //删除序列号最后一个字符
    void DeleteSeriesNum();
    //显示序列号
    void ShowSeries(const char* series, unsigned short seriesLen);
    void ShowSeriesPart(unsigned int partID, char* partData);
    //清空序列号
    void ClearSeries();

    //添加一个字符到激活码
    void AddActivationCode(char addChar);
    //删除激活码最后一个字符
    void DeleteActivationCode();
    //显示激活码
    void ShowActivationCode(const char* code, unsigned short codeLen);
    void ShowActivationCodePart(unsigned int partID, char* partData);
    //清空激活码
    void ClearActivationCode();
    //关闭系统
    void DoClose();
    //激活
    void DoActivation();
    //消息对话框
    void ShowMessageDialog(MessageBoxType type, const char* caption);
  private:
    //版本号
    CUiButton m_versionCodeCtrl;
    //序列号    
    CUiButton m_seriesNumberPart1Ctrl;
    CUiButton m_seriesNumberPart2Ctrl;
    CUiButton m_seriesNumberPart3Ctrl;
    CUiButton m_seriesNumberPart4Ctrl;
    //激活码
    CUiButton m_activationCodePart1Ctrl;
    CUiButton m_activationCodePart2Ctrl;
    CUiButton m_activationCodePart3Ctrl;
    CUiButton m_activationCodePart4Ctrl;
    //取消
    CUiButton m_cancelCtrl;
    //删除输入
    CUiButton m_deleteCharCtrl;
    //键盘代码
    CUiButton m_inputCode1Ctrl;
    CUiButton m_inputCode2Ctrl;
    CUiButton m_inputCode3Ctrl;
    CUiButton m_inputCode4Ctrl;
    CUiButton m_inputCode5Ctrl;
    CUiButton m_inputCode6Ctrl;
    CUiButton m_inputCode7Ctrl;
    CUiButton m_inputCode8Ctrl;
    CUiButton m_inputCode9Ctrl;
    CUiButton m_inputCode10Ctrl;
    CUiButton m_inputCode11Ctrl;
    CUiButton m_inputCode12Ctrl;
    CUiButton m_inputCode13Ctrl;
    CUiButton m_inputCode14Ctrl;
    CUiButton m_inputCode15Ctrl;
    CUiButton m_inputCode16Ctrl;
    //激活
    CUiButton m_activationCtrl;

    //序列号
    unsigned char m_seriesNum_Len;
    char m_seriesNum[MAX_NUM_LEN + 1];
    //激活码
    unsigned char m_activationCode_Len;
    char m_activationCode[MAX_NUM_LEN + 1];
  };
}
#endif
