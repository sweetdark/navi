#ifndef _UEGUI_LOGONHOOK_H
#define _UEGUI_LOGONHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{  
  class UEGUI_CLASS CLogonHook : public CAggHook
  {
  public:
    //进度条默认最大进度值
    const static unsigned short kMaxProgress = 100;

    enum LogonHookCtrlType
    {
      LogonHook_Begin = 0,
      LogonHook_LogonBackground,
      LogonHook_LogonSetp1,
      LogonHook_LogonSetp2,
      LogonHook_LogonSetp3,
      LogonHook_LogonSetp4,
      LogonHook_LogonSetp5,
      LogonHook_LogonSetp6,
      LogonHook_LogonSetp7,
      LogonHook_LogonSetp8,
      LogonHook_LogonSetp9,
      LogonHook_LogonSetp10,
      LogonHook_LogonSetp11,
      LogonHook_LogonSetp12,
      LogonHook_LogonSetp13,
      LogonHook_LogonSetp14,
      LogonHook_LogonSetp15,
      LogonHook_LogonSetp16,
      LogonHook_LogonSetp17,
      LogonHook_LogonSetp18,
      LogonHook_LogonSetp19,
      LogonHook_LogonSetp20,
      LogonHook_LogonSetp21,
      LogonHook_LogonSetp22,
      LogonHook_LogonSetp23,
      LogonHook_LogonSetp24,
      LogonHook_LogonSetp25,
      LogonHook_LogonSetp26,
      LogonHook_LogonSetp27,
      LogonHook_LogonSetp28,
      LogonHook_LogonSetp29,
      LogonHook_End
    };

    CLogonHook();

    virtual ~CLogonHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    static CLogonHook* GetLogonHook();
  public:
    //GUI元素是否加载完成
    bool GuiElementsLoadDone();
    ///清空进度条
    void ClearProgressBar();
    //设置进度条最大进度数，也就是当进度达到该数值时任务进度完成
    void SetMaxProgress( unsigned short maxProgress);
    //设置进度条进度 0...100 默认值为0 如果传值为默认值则进度自动递增1
    void SetProgress(short progress = 0);
    //进度演示
    void StartDemo();
  protected:
    virtual tstring GetBinaryFileName();
    virtual void MakeNames();
    void MakeControls();
  private:
    void SetPosition(unsigned short position);
    unsigned short GetPosition(short progress);
  private:
    //界面元素是否加载完成
    bool m_loadGuiElementsDone;
    //最大进度数,默认为100
    short m_maxProgress;
    //上一次系统进度位置
    unsigned short m_lastProgress;
    //上一次进度条的位置区间为0-29
    unsigned short m_lastPosition;
    //第一次调用进度条
    bool m_firstSetPosition;
    //静态对象
    static CLogonHook* m_logonHook;
  };
}
#endif
