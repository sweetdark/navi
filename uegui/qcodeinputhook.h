#ifndef _UEGUI_QCODEINPUTHOOK_H
#define _UEGUI_QCODEINPUTHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

namespace UeGui
{
  class UEGUI_CLASS CQCodeInputHook : public CMenuBackgroundHook
  {
  public:
    enum QCodeInputHookCtrlType
    {
      QCodeInputHook_Begin = MenuBackgroundHook_End,
      QCodeInputHook_Edit1Btn,
      QCodeInputHook_KeyBox1,
      QCodeInputHook_Edit2Btn,
      QCodeInputHook_KeyBox2,
      QCodeInputHook_Edit3Btn,
      QCodeInputHook_KeyBox3,
      QCodeInputHook_TextTip,
      QCodeInputHook_SearchBtn,
      QCodeInputHook_DeleteBtn,
      QCodeInputHook_CodeBack,
      QCodeInputHook_BtnKey1,
      QCodeInputHook_BtnKey2,
      QCodeInputHook_BtnKey3,
      QCodeInputHook_BtnKey4,
      QCodeInputHook_BtnKey5,
      QCodeInputHook_BtnKey6,
      QCodeInputHook_BtnKey7,
      QCodeInputHook_BtnKey8,
      QCodeInputHook_BtnKey9,
      QCodeInputHook_BtnKey10,
      QCodeInputHook_BtnKey11,
      QCodeInputHook_BtnKey12,
      QCodeInputHook_BtnKey13,
      QCodeInputHook_BtnKey14,
      QCodeInputHook_BtnKey15,
      QCodeInputHook_BtnKey16,
      QCodeInputHook_BtnKey17,
      QCodeInputHook_BtnKey18,
      QCodeInputHook_BtnKey19,
      QCodeInputHook_BtnKey20,
      QCodeInputHook_BtnKey21,
      QCodeInputHook_BtnKey22,
      QCodeInputHook_BtnKey23,
      QCodeInputHook_BtnKey24,
      QCodeInputHook_BtnKey25,
      QCodeInputHook_BtnKey26,
      QCodeInputHook_BtnKey27,
      QCodeInputHook_BtnKey28,
      QCodeInputHook_BtnKey29,
      QCodeInputHook_BtnKey30,
      QCodeInputHook_BtnKey31,
      QCodeInputHook_BtnKey32,
      QCodeInputHook_BtnKey33,
      QCodeInputHook_BtnKey34,
      QCodeInputHook_BtnKey35,
      QCodeInputHook_BtnKey36,
      QCodeInputHook_line1,
      QCodeInputHook_line2,
      QCodeInputHook_WordSeparation,
      QCodeInputHook_QCodeLabel,
      QCodeInputHook_End
    };

    CQCodeInputHook();

    virtual ~CQCodeInputHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual void Load();

  protected:

    virtual void MakeNames();

    void MakeControls();
    
    bool EraseOneKeyWord();
    bool AddOneKeyWord(const char *pchLabelText);
    bool doAddOneKeyWord(TCHAR oneWord);
    void ClearKeyWord();
    void SetCursorPosX(int curIndex);

    void ShowInputCode();

    void ConvertQCodeToMap();

  private:
    CUiButton m_inputCode[36];

    CUiButton m_deleteBtn;
    CUiButton m_searchBtn;

    CUiLabel m_qCodeBox[3];
    CUiLabel m_qCodeLabel;

    int m_iCurCursorIndex;
    tstring m_tstrKeyWords;
    std::vector<int> m_vecWordPosX;
    //文字后面的白线
    GuiElement *m_pWordCursor;
    //记录Q码
    unsigned char m_qCode[9];
  };
}
#endif
