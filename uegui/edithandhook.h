#ifndef _UEGUI_EDITHANDHOOK_H
#define _UEGUI_EDITHANDHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#include "menubackgroundhook.h"

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

#define INPUTCODENUM 10

namespace UeGui
{
  class UEGUI_CLASS CEditHandHook : public CMenuBackgroundHook
  {
  public:
    enum EditHandHookCtrlType
    {
      EditHandHook_Begin = MenuBackgroundHook_End,
      EditHandHook_CharBack,
      EditHandHook_InputCode1,
      EditHandHook_InputCode2,
      EditHandHook_InputCode3,
      EditHandHook_InputCode4,
      EditHandHook_InputCode5,
      EditHandHook_InputCode6,
      EditHandHook_InputCode7,
      EditHandHook_InputCode8,
      EditHandHook_InputCode9,
      EditHandHook_InputCode10,
      EditHandHook_WriteBack,
      EditHandHook_WriteBackShade,
      EditHandHook_PageUpIcon,
      EditHandHook_PageDownIcon,
      EditHandHook_PageUpBtn,
      EditHandHook_PageDownBtn,
      EditHandHook_SaveBtn,
      EditHandHook_InputSwitchBtn,
      EditHandHook_InputSwitchBtnIcon,
      EditHandHook_EditBox,
      EditHandHook_WordSeparation,
      EditHandHook_KeyWordBox,
      EditHandHook_DeleteBtn,
      EditHandHook_End
    };

    CEditHandHook();

    virtual ~CEditHandHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    void ResetKeyWord(const char *pchKeyWord);

    char* GetKeyWord();

    void SetKeyWord(char* keyword);

    void SetQueryMode();

    //实时判断当前的手写输入
    void DoHandWriting(int curTime);

  protected:

    virtual void Load();

    virtual void UnLoad();

    virtual void Init();

    virtual void MakeNames();

    void MakeControls();

    bool EraseOneKeyWord();
    bool AddOneKeyWord(const char *pchLabelText);
    bool doAddOneKeyWord(TCHAR oneWord);
    void ClearKeyWord(void);
    void SetCursorPosX(int curIndex);

    bool GetAssociateThing();

    void SetAssociateBtnLabels();

  private:
    //初始化手写输入
    static void InitHandWriting();
    //释放手写输入资源
    static void UninitHandWriting();

  private:
    CUiLabel m_keyWordBox;
    CUiBitButton m_otherSearchBtn;
    CUiButton m_saveBtn;
    CUiButton m_deleteBtn;
    CUiBitButton m_inputSwitchBtn;

    CUiButton m_inputCode[INPUTCODENUM];
    CUiBitButton m_pageDownBtn;
    CUiBitButton m_pageUpBtn;

    int m_iCurCursorIndex;
    tstring m_tstrKeyWords;
    std::vector<int> m_vecWordPosX;
    //文字后面的白线
    GuiElement *m_pWordCursor;

    //当前显示在文字按钮上的文字索引
    int m_iCurWordIndex;
    //保存当前的要显示在按钮上的文字
    std::vector<string> m_vecWordsBuf;

    GuiElement *m_pWrittingArea;

    //因为所有的手写屏幕一样大，且不会同时出现
    // Handwriting library
    //手写输入字库
    static unsigned char *m_hwAddress;
    //用来保存手写输入时的点坐标
    static short *m_writingPts;
    //保存坐标点的缓存大小
    static short m_writingNum;
    //当前可以保存坐标的下标
    static short m_writingCursor;
    //保存字符串
    char m_keyWord[128];
    //
    bool m_isNewChar;
    int m_writingTime;
    short m_prevX;
    short m_prevY;
    bool m_isWriting;

    HPEN m_pen;
    HPEN m_oldPen;
    HDC m_renderingDC;
    //判断当前是准备纠正想要输入的字还是输入下一个联想字
    bool m_isIdentify;
  };
}
#endif
