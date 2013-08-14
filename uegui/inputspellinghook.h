#ifndef _UEGUI_INPUTSPELLINGHOOK_H
#define _UEGUI_INPUTSPELLINGHOOK_H

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
#define MAXWORDNUM 20

namespace UeGui
{
  class UEGUI_CLASS CInputSpellingHook : public CMenuBackgroundHook
  {
  public:
    enum InputSpellingHookCtrlType
    {
      InputSpellingHook_Begin = MenuBackgroundHook_End,
      InputSpellingHook_CharBack,
      InputSpellingHook_InputCode1,
      InputSpellingHook_InputCode2,
      InputSpellingHook_InputCode3,
      InputSpellingHook_InputCode4,
      InputSpellingHook_InputCode5,
      InputSpellingHook_InputCode6,
      InputSpellingHook_InputCode7,
      InputSpellingHook_InputCode8,
      InputSpellingHook_InputCode9,
      InputSpellingHook_InputCode10,
      InputSpellingHook_InputCode11,
      InputSpellingHook_InputCode12,
      InputSpellingHook_InputCode13,
      InputSpellingHook_InputCode14,
      InputSpellingHook_InputCode15,
      InputSpellingHook_InputCode16,
      InputSpellingHook_InputCode17,
      InputSpellingHook_InputCode18,
      InputSpellingHook_InputCode19,
      InputSpellingHook_InputCode20,
      InputSpellingHook_InputCode21,
      InputSpellingHook_InputCode22,
      InputSpellingHook_InputCode23,
      InputSpellingHook_InputCode24,
      InputSpellingHook_InputCode25,
      InputSpellingHook_InputCode26,
      InputSpellingHook_InputSwitchBtn,
      InputSpellingHook_InputSwitchBtnIcon,
      InputSpellingHook_PageLeftBtn,
      InputSpellingHook_AssociateBtn1,
      InputSpellingHook_AssociateBtn2,
      InputSpellingHook_AssociateBtn3,
      InputSpellingHook_AssociateBtn4,
      InputSpellingHook_AssociateBtn5,
      InputSpellingHook_AssociateBtn6,
      InputSpellingHook_PageRightBtn,
      InputSpellingHook_PageLeftBtnIcon,
      InputSpellingHook_PageRightBtnIcon,
      InputSpellingHook_PageUpIcon,
      InputSpellingHook_PageDownIcon,
      InputSpellingHook_DistSwitchBtn,
      InputSpellingHook_DistSelectBtn,
      InputSpellingHook_DistSelectBtnIcon,
      InputSpellingHook_EditBox,
      InputSpellingHook_EditSelectBtn,
      InputSpellingHook_EditSelectBtnIcon,
      InputSpellingHook_WordSeparation,
      InputSpellingHook_KeyWordBox,
      InputSpellingHook_OtherSearchBtn,
      InputSpellingHook_OtherSearchIcon,
      InputSpellingHook_DeleteBtn,
      InputSpellingHook_SearchBtn,
      InputSpellingHook_End
    };

    CInputSpellingHook();

    virtual ~CInputSpellingHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void ResetKeyWord(const char *pchKeyWord);

    char* GetKeyWord();

    void SetKeyWord(char* keyword);

    void SetQueryMode();

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

    void ShowKeyWord();

    bool GetAssociateThing();

    void SetAssociateBtnLabels();

    void SetKeyBoardBtnsEnable();

    void ResetSpellingToBtns(int chIndex);
    void ResetSpellingWordToBtns(const char *pchSpelling);
    //判断该拼音组合是否在该区域中存在对应的字
    bool IsKeyWordExist(const char *pchKeyWord);

  private:
    static void DistSwitchCallBack(void *pDoCallBackObj, const SQLRecord *pResult);

    void DoDistSwitchCallBack(const SQLRecord *pResult);

    static void InputSelectCallBack(void *pDoCallBackObj, char *keyword);

    void DoInputSelectCallBack(char *keyword);

  private:
    CUiButton m_distSwitchBtn;
    CUiBitButton m_distSelectBtn;
    CUiBitButton m_editSelectBtn;
    CUiLabel m_keyWordBox;
    CUiBitButton m_otherSearchBtn;
    CUiButton m_searchBtn;
    CUiButton m_deleteBtn;
    CUiBitButton m_inputSwitchBtn;

    CUiButton m_inputCode[INPUTCODENUM];
    CUiButton m_associateBtn[ASSOCIATEBTNNUM];
    CUiBitButton m_pageLeftBtn;
    CUiBitButton m_pageRightBtn;

    CUiBitButton m_returnBtn;

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
    //保存搜索poi和路名用的关键字
    char m_poiKeyWord[128];
    //保存搜索区域用的关键字
    char m_distKeyWord[128];
    //判断是否无联想字
    bool m_isAssociateExist;

    //关键字过长显示不全时, 标识从第几个字开始显示
    int m_wordPosOffset;
    //用于保存截取后用于显示的字符串
    tstring m_cutKeyWords;
    //记录关键字输入框的原始位置
    int m_orinBoxPos;
    //光标可以移动到的最后位置
    int m_limitPoxX;
    //记录当前是否显示完整的关键字
    bool m_isShowFullKeyWords;
  };
}
#endif
