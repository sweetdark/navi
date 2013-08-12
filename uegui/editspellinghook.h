#ifndef _UEGUI_EDITSPELLINGHOOK_H
#define _UEGUI_EDITSPELLINGHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

#define INPUTCODENUM 26
#define ASSOCIATEBTNNUM 6

namespace UeGui
{
  class UEGUI_CLASS CEditSpellingHook : public CMenuBackgroundHook
  {
  public:
    enum EditSpellingHookCtrlType
    {
      EditSpellingHook_Begin = MenuBackgroundHook_End,
      EditSpellingHook_CharBack,
      EditSpellingHook_InputCode1,
      EditSpellingHook_InputCode2,
      EditSpellingHook_InputCode3,
      EditSpellingHook_InputCode4,
      EditSpellingHook_InputCode5,
      EditSpellingHook_InputCode6,
      EditSpellingHook_InputCode7,
      EditSpellingHook_InputCode8,
      EditSpellingHook_InputCode9,
      EditSpellingHook_InputCode10,
      EditSpellingHook_InputCode11,
      EditSpellingHook_InputCode12,
      EditSpellingHook_InputCode13,
      EditSpellingHook_InputCode14,
      EditSpellingHook_InputCode15,
      EditSpellingHook_InputCode16,
      EditSpellingHook_InputCode17,
      EditSpellingHook_InputCode18,
      EditSpellingHook_InputCode19,
      EditSpellingHook_InputCode20,
      EditSpellingHook_InputCode21,
      EditSpellingHook_InputCode22,
      EditSpellingHook_InputCode23,
      EditSpellingHook_InputCode24,
      EditSpellingHook_InputCode25,
      EditSpellingHook_InputCode26,
      EditSpellingHook_InputSwitchBtn,
      EditSpellingHook_InputSwitchBtnIcon,
      EditSpellingHook_PageLeftBtn,
      EditSpellingHook_AssociateBtn1,
      EditSpellingHook_AssociateBtn2,
      EditSpellingHook_AssociateBtn3,
      EditSpellingHook_AssociateBtn4,
      EditSpellingHook_AssociateBtn5,
      EditSpellingHook_AssociateBtn6,
      EditSpellingHook_PageRightBtn,
      EditSpellingHook_PageLeftBtnIcon,
      EditSpellingHook_PageRightBtnIcon,
      EditSpellingHook_EditBox,
      EditSpellingHook_WordSeparation,
      EditSpellingHook_KeyWordBox,
      EditSpellingHook_DeleteBtn,
      EditSpellingHook_SaveBtn,
      EditSpellingHook_End
    };

    CEditSpellingHook();

    virtual ~CEditSpellingHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void ResetKeyWord(const char *pchKeyWord);

    char* GetKeyWord();

    void SetKeyWord(const char* keyword);

    void SetTitle(const char* title);

  protected:

    virtual void Load();

    virtual void UnLoad();

    virtual void Init();

    virtual void MakeNames();

    void MakeControls();

    void InitKeyBoard();

    bool EraseOneKeyWord();
    bool AddOneKeyWord(const char *pchLabelText);
    bool doAddOneKeyWord(TCHAR oneWord);
    void ClearKeyWord(void);
    void SetCursorPosX(int curIndex);

    void SetAssociateBtnLabels();

    void SetKeyBoardBtnsEnable();

    void ResetSpellingToBtns(int chIndex);
    void ResetSpellingWordToBtns(const char *pchSpelling);

  private:
    CUiLabel m_keyWordBox;
    CUiButton m_saveBtn;
    CUiButton m_deleteBtn;
    CUiBitButton m_inputSwitchBtn;

    CUiButton m_inputCode[INPUTCODENUM];
    CUiButton m_associateBtn[ASSOCIATEBTNNUM];
    CUiBitButton m_pageLeftBtn;
    CUiBitButton m_pageRightBtn;

    int m_iCurCursorIndex;
    tstring m_tstrKeyWords;
    std::vector<int> m_vecWordPosX;
    //文字后面的白线
    GuiElement *m_pWordCursor;

    bool m_isWordsReady;

    //当前显示在文字按钮上的文字索引
    int m_iCurWordIndex;
    //保存当前的要显示在按钮上的文字
    std::vector<string> m_vecWordsBuf;

    //目前输入到拼音组合的第几个字母
    int m_curSpellingCursor;
    //用于记录输入过的字母变色
    unsigned char m_posBuffer[10];
    //保存字符串
    char m_keyWord[128];
  };
}
#endif
