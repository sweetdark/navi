#ifndef _UEGUI_INPUTACRONYMHOOK_H
#define _UEGUI_INPUTACRONYMHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

#define INPUTCODENUM 26
#define MAXWORDNUM 20

namespace UeGui
{
  class UEGUI_CLASS CInputAcronymHook : public CMenuBackgroundHook
  {
  public:
    enum InputAcronymHookCtrlType
    {
      InputAcronymHook_Begin = MenuBackgroundHook_End,
      InputAcronymHook_CharBack,
      InputAcronymHook_InputCode1,
      InputAcronymHook_InputCode2,
      InputAcronymHook_InputCode3,
      InputAcronymHook_InputCode4,
      InputAcronymHook_InputCode5,
      InputAcronymHook_InputCode6,
      InputAcronymHook_InputCode7,
      InputAcronymHook_InputCode8,
      InputAcronymHook_InputCode9,
      InputAcronymHook_InputCode10,
      InputAcronymHook_InputCode11,
      InputAcronymHook_InputCode12,
      InputAcronymHook_InputCode13,
      InputAcronymHook_InputCode14,
      InputAcronymHook_InputCode15,
      InputAcronymHook_InputCode16,
      InputAcronymHook_InputCode17,
      InputAcronymHook_InputCode18,
      InputAcronymHook_InputCode19,
      InputAcronymHook_InputCode20,
      InputAcronymHook_InputCode21,
      InputAcronymHook_InputCode22,
      InputAcronymHook_InputCode23,
      InputAcronymHook_InputCode24,
      InputAcronymHook_InputCode25,
      InputAcronymHook_InputCode26,
      InputAcronymHook_InputSwitchBtn,
      InputAcronymHook_InputSwitchBtnIcon,
      InputAcronymHook_DistSwitchBtn,
      InputAcronymHook_DistSelectBtn,
      InputAcronymHook_DistSelectBtnIcon,
      InputAcronymHook_EditBox,
      InputAcronymHook_EditSelectBtn,
      InputAcronymHook_EditSelectBtnIcon,
      InputAcronymHook_WordSeparation,
      InputAcronymHook_KeyWordBox,
      InputAcronymHook_OtherSearchBtn,
      InputAcronymHook_OtherSearchIcon,
      InputAcronymHook_DeleteBtn,
      InputAcronymHook_SearchBtn,
      InputAcronymHook_End
    };

    CInputAcronymHook();

    virtual ~CInputAcronymHook();

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
    void ClearKeyWord(void);
    void SetCursorPosX(int curIndex);

    void ShowKeyWord();

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

    CUiBitButton m_returnBtn;

    int m_iCurCursorIndex;
    tstring m_tstrKeyWords;
    std::vector<int> m_vecWordPosX;
    //文字后面的白线
    GuiElement *m_pWordCursor;

    //当前显示在文字按钮上的文字索引
    int m_iCurWordIndex;
    //保存当前的要显示在按钮上的文字
    std::vector<string> m_vecWordsBuf;
    //保存搜索poi和路名用的关键字
    char m_poiKeyWord[128];
    //保存搜索区域用的关键字
    char m_distKeyWord[128];

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
