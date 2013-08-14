#ifndef _UEGUI_MENUBACKGROUNDHOOK_H
#define _UEGUI_MENUBACKGROUNDHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

//#include "uilabel.h"
#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

//用于mousedown时局部刷新的宏，使用方式如下:
//MOUSEDOWN_3RENDERCTRL(m_display3DBackgroundCtrl, m_display3DBtnCtrl, m_display3DLableCtrl)
//PS: 结尾要加分号，因为是函数调用。
//此宏定义支持一次传三个控件名
#define MOUSEDOWN_3RENDERCTRL(ctrl1, ctrl2, ctrl3) \
  do                                               \
  {                                                \
    AddRenderUiControls(&##ctrl1##);               \
    AddRenderUiControls(&##ctrl2##);               \
    AddRenderUiControls(&##ctrl3##);               \
  } while (0)

//此宏定义支持一次传两个控件名
#define MOUSEDOWN_2RENDERCTRL(ctrl1, ctrl2)       \
  do                                              \
  {                                               \
    AddRenderUiControls(&##ctrl1##);              \
    AddRenderUiControls(&##ctrl2##);              \
  } while (0)

//此宏定义支持一次传一个控件名
#define MOUSEDOWN_1RENDERCTRL(ctrl1)              \
  do                                              \
  {                                               \
    AddRenderUiControls(&##ctrl1##);              \
  } while (0)

namespace UeGui
{
  class UEGUI_CLASS CMenuBackgroundHook : public CAggHook
  {
  public:
    enum MenuBackgroundHookCtrlType
    {
      MenuBackgroundHook_Begin = 0,
      MenuBackgroundHook_BackGround,
      MenuBackgroundHook_TitleLable,
      MenuBackgroundHook_ReturnBtn,
      MenuBackgroundHook_ReturnBtnIcon,
      MenuBackgroundHook_GoToMapBtn,
      MenuBackgroundHook_GoToMapBtnIcon,
      MenuBackgroundHook_End
    };

    CMenuBackgroundHook();

    virtual ~CMenuBackgroundHook();

    virtual void FetchWithBinary();

    virtual void MakeNames();

    virtual void MakeControls();

    //初始化界面
    virtual void MakeGUI()
    {
      FetchWithBinary();
      MakeNames();
      MakeControls();
      InitTitle();
    }

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void SetTitle(const char *title)
    {
      if (title)
      {
        ::strcpy(GetGuiElement(MenuBackgroundHook_TitleLable)->m_label, title); 
      }
    }
  protected:
    virtual tstring GetBinaryFileName()
    {
      return m_vecHookFile[m_uCurHookFileIdx];
    };

    /**
    * \brief 根据按钮是否可用来设置是否需要刷新
    * \button 按钮
    * \返回值是按钮是否可用
    **/
    bool IsNeedRefresh(CUiControl &button);

  private:
    void InitTitle()
    {
      ::strcpy(GetGuiElement(MenuBackgroundHook_TitleLable)->m_label, m_strTitle.c_str());
    }

  protected:
    std::vector<tstring> m_vecHookFile;
    string m_strTitle;

  private:
    CUiButton m_backGroundCtrl;
    CUiBitButton m_goToMapBtnCtrl;
    CUiBitButton m_returnBtnCtrl;
    CUiButton m_titleLableCtrl;
    
    
    unsigned int m_uCurHookFileIdx;
  };
}
#endif
