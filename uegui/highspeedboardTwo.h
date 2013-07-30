#ifndef _UEGUI_HIGHSPEEDBOARDTWO_H
#define _UEGUI_HIGHSPEEDBOARDTWO_H

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
  class UEGUI_CLASS ChighspeedboardTwo : public CAggHook
  {
  public:
    enum highspeedboardTwoCtrlType
    {
      highspeedboardTwo_Begin = 0,
      highspeedboardTwo_boardtwo_background,
      highspeedboardTwo_boardtwo_line,
      highspeedboardTwo_boardtwo_roadname,
      highspeedboardTwo_boardtwo_roadexitIdx,
      highspeedboardTwo_boardtwo_roaddist,
      highspeedboardTwo_End
    };

    ChighspeedboardTwo();

    virtual ~ChighspeedboardTwo();

    virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

    void ShowHighSpeedInfo(char *roaddist,char* roadname,int roadexitId = -1);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    CUiButton m_boardtwo_backgroundCtrl;
    CUiButton m_boardtwo_lineCtrl;
    CUiButton m_boardtwo_roaddistCtrl;
    CUiButton m_boardtwo_roadexitIdxCtrl;
    CUiButton m_boardtwo_roadnameCtrl;
  };
}
#endif
