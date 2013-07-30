#ifndef _UEGUI_MOVEUPANDDOWNHOOK_H
#define _UEGUI_MOVEUPANDDOWNHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  enum MoveType
  {
    MT_UnKnow,
    MT_Down,
    MT_Up
  };
  //上移到结束还是下移到结束
  enum MoveEndType
  {
    ME_None,
    ME_DownEnd,
    ME_UpEnd
  };

  //路线类型选择回调函数类型定义
  typedef void (*UpwardOrDownwardEvent)(CAggHook* senderHook, MoveType moveType);

  class UEGUI_CLASS CUpwardOrDownwardHook : public CAggHook
  {
  public:
    enum UpwardOrDownwardHookCtrlType
    {
      UpwardOrDownwardHook_Begin = 0,
      UpwardOrDownwardHook_BackGround,
      UpwardOrDownwardHook_UpwardLeft,
      UpwardOrDownwardHook_UpwardCenter,
      UpwardOrDownwardHook_UpwardRight,
      UpwardOrDownwardHook_DownwardLeft,
      UpwardOrDownwardHook_DownwardCenter,
      UpwardOrDownwardHook_DownwardRight,
      UpwardOrDownwardHook_End
    };

    CUpwardOrDownwardHook();

    virtual ~CUpwardOrDownwardHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
  public:
    void SetEvent(CViewHook::HookType senderHookType, CAggHook* senderHook, UpwardOrDownwardEvent selectEvent, MoveEndType endType = ME_None);
    //打开路线选择hook
    static void ShowHook(CViewHook::HookType senderHookType, CAggHook* senderHook, UpwardOrDownwardEvent selectEvent, MoveEndType endType = ME_None);
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    void DoMouseUp(MoveType moveType);
  private:
    CUiButton m_backGroundCtrl;
    //向下
    CUiButton m_downwardCtrl;
    //向上
    CUiButton m_upwardCtrl;
    //是否上移到终点或者下移到终点
    MoveEndType m_endType;
    //调用的hook
    CAggHook* m_senderHook;
    //调用的hook类型
    CViewHook::HookType m_senderHookType;
    //路线类型选择事件
    UpwardOrDownwardEvent m_selectEvent;
  };
}
#endif
