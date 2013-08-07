#ifndef _UEGUI_MARKEREDITHOOK_H
#define _UEGUI_MARKEREDITHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"
#include "detailedithook.h"

namespace UeGui
{
  class UEGUI_CLASS CMarkerEditHook : public CMenuBackgroundHook
  {
  public:
    enum MarkerEditHookCtrlType
    {
      MarkerEditHook_Begin = 0,
      MarkerEditHook_BackGround,
      MarkerEditHook_BackGroundText,
      MarkerEditHook_GotoMapBtn,
      MarkerEditHook_BackButton,
      MarkerEditHook_mark01,
      MarkerEditHook_mark02,
      MarkerEditHook_mark03,
      MarkerEditHook_mark04,
      MarkerEditHook_mark05,
      MarkerEditHook_mark06,
      MarkerEditHook_mark07,
      MarkerEditHook_mark08,
      MarkerEditHook_mark09,
      MarkerEditHook_mark10,
      MarkerEditHook_mark11,
      MarkerEditHook_mark12,
      MarkerEditHook_mark13,
      MarkerEditHook_mark14,
      MarkerEditHook_mark15,
      MarkerEditHook_mark16,
      MarkerEditHook_mark17,
      MarkerEditHook_mark18,
      MarkerEditHook_mark19,
      MarkerEditHook_mark20,
      MarkerEditHook_mark21,
      MarkerEditHook_mark22,
      MarkerEditHook_mark23,
      MarkerEditHook_mark24,
      MarkerEditHook_End
    };

    CMarkerEditHook();

    virtual ~CMarkerEditHook();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    //virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_mark01Ctrl;
    CUiButton m_mark02Ctrl;
    CUiButton m_mark03Ctrl;
    CUiButton m_mark04Ctrl;
    CUiButton m_mark05Ctrl;
    CUiButton m_mark06Ctrl;
    CUiButton m_mark07Ctrl;
    CUiButton m_mark08Ctrl;
    CUiButton m_mark09Ctrl;
    CUiButton m_mark10Ctrl;
    CUiButton m_mark11Ctrl;
    CUiButton m_mark12Ctrl;
    CUiButton m_mark13Ctrl;
    CUiButton m_mark14Ctrl;
    CUiButton m_mark15Ctrl;
    CUiButton m_mark16Ctrl;
    CUiButton m_mark17Ctrl;
    CUiButton m_mark18Ctrl;
    CUiButton m_mark19Ctrl;
    CUiButton m_mark20Ctrl;
    CUiButton m_mark21Ctrl;
    CUiButton m_mark22Ctrl;
    CUiButton m_mark23Ctrl;
    CUiButton m_mark24Ctrl;

    CDetailEditHook edithook;
  };
}
#endif
