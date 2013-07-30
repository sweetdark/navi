#ifndef _UEGUI_HIGHSPEEDBOARDONE_H
#define _UEGUI_HIGHSPEEDBOARDONE_H

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

namespace UeGui
{
  class UEGUI_CLASS ChighspeedboardOne : public CAggHook
  {
  public:
    enum highspeedboardOneCtrlType
    {
      highspeedboardOne_Begin = 0,
      highspeedboardOne_boardone_background,
      highspeedboardOne_boardone_line,
      highspeedboardOne_boardone_roadname,
      highspeedboardOne_boardone_roadexitIdx,
      highspeedboardOne_boardone_roaddist,
      highspeedboardOne_End
    };

    ChighspeedboardOne();

    virtual ~ChighspeedboardOne();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void ShowHighSpeedInfo(char *roaddist,char* roadname,int roadexitId = -1);

    short GetSelfStartX();
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_boardone_backgroundCtrl;
    CUiButton m_boardone_lineCtrl;
    CUiButton m_boardone_roaddistCtrl;
    CUiButton m_boardone_roadexitIdxCtrl;
    CUiButton m_boardone_roadnameCtrl;
  };
}
#endif
