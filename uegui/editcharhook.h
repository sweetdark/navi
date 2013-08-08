#ifndef _UEGUI_EDITCHARHOOK_H
#define _UEGUI_EDITCHARHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

#define INPUTCODENUM 24
#define ASSOCIATEBTNNUM 6

namespace UeGui
{
  class UEGUI_CLASS CEditCharHook : public CMenuBackgroundHook
  {
  public:
    enum EditCharHookCtrlType
    {
      EditCharHook_Begin = MenuBackgroundHook_End,
      EditCharHook_CharBack,
      EditCharHook_InputCode1,
      EditCharHook_InputCode2,
      EditCharHook_InputCode3,
      EditCharHook_InputCode4,
      EditCharHook_InputCode5,
      EditCharHook_InputCode6,
      EditCharHook_InputCode7,
      EditCharHook_InputCode8,
      EditCharHook_InputCode9,
      EditCharHook_InputCode10,
      EditCharHook_InputCode11,
      EditCharHook_InputCode12,
      EditCharHook_InputCode13,
      EditCharHook_InputCode14,
      EditCharHook_InputCode15,
      EditCharHook_InputCode16,
      EditCharHook_InputCode17,
      EditCharHook_InputCode18,
      EditCharHook_InputCode19,
      EditCharHook_InputCode20,
      EditCharHook_InputCode21,
      EditCharHook_InputCode22,
      EditCharHook_InputCode23,
      EditCharHook_InputCode24,
      EditCharHook_PageUpBtn,
      EditCharHook_PageDownBtn,
      EditCharHook_InputSwitchBtn,
      EditCharHook_InputSwitchBtnIcon,
      EditCharHook_PageLeftBtn,
      EditCharHook_AssociateBtn1,
      EditCharHook_AssociateBtn2,
      EditCharHook_AssociateBtn3,
      EditCharHook_AssociateBtn4,
      EditCharHook_AssociateBtn5,
      EditCharHook_AssociateBtn6,
      EditCharHook_PageRightBtn,
      EditCharHook_PageLeftBtnIcon,
      EditCharHook_PageRightBtnIcon,
      EditCharHook_PageUpIcon,
      EditCharHook_PageDownIcon,
      EditCharHook_EditBox,
      EditCharHook_WordSeparation,
      EditCharHook_KeyWordBox,
      EditCharHook_DeleteBtn,
      EditCharHook_SaveBtn,
      EditCharHook_End
    };

    CEditCharHook();

    virtual ~CEditCharHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void ResetKeyWord(const char *pchKeyWord);

    char* GetKeyWord();

    void SetKeyWord(char* keyword);

  protected:

    virtual void Load();

    virtual void UnLoad();

    virtual void Init();

    void SetKeyBoard();

    bool EraseOneKeyWord();
    bool AddOneKeyWord(const char *pchLabelText);
    bool doAddOneKeyWord(TCHAR oneWord);
    void ClearKeyWord(void);
    void SetCursorPosX(int curIndex);

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiLabel m_keyWordBox;
    CUiButton m_saveBtn;
    CUiButton m_deleteBtn;
    CUiBitButton m_inputSwitchBtn;

    CUiButton m_inputCode[INPUTCODENUM];
    CUiButton m_associateBtn[ASSOCIATEBTNNUM];
    CUiBitButton m_pageLeftBtn;
    CUiBitButton m_pageRightBtn;
    CUiBitButton m_pageUpBtn;
    CUiBitButton m_pageDownBtn;

    int m_iCurCursorIndex;
    tstring m_tstrKeyWords;
    std::vector<int> m_vecWordPosX;
    //文字后面的白线
    GuiElement *m_pWordCursor;

    //当前显示在键盘按钮上的文字索引
    int m_iCurCodeIndex;
    //保存当前的要显示在键盘上的文字
    std::vector<string> m_vecCodesBuf;
    //保存字符串
    char m_keyWord[128];
  };
}
#endif
