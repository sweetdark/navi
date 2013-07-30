#ifndef _UEGUI_HIGHSPEEDBOARDTHREE_H
#define _UEGUI_HIGHSPEEDBOARDTHREE_H

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
  class UEGUI_CLASS ChighspeedboardThree : public CAggHook
  {
  public:
    enum highspeedboardThreeCtrlType
    {
      highspeedboardThree_Begin = 0,
      highspeedboardThree_boardthree_background,
      highspeedboardThree_boardthree_line,
      highspeedboardThree_boardthree_roadname,
      highspeedboardThree_boardthree_roadexitIdx,
      highspeedboardThree_boardthree_roaddist,
      highspeedboardThree_End
    };

    ChighspeedboardThree();

    virtual ~ChighspeedboardThree();

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
    CUiButton m_boardthree_backgroundCtrl;
    CUiButton m_boardthree_lineCtrl;
    CUiButton m_boardthree_roaddistCtrl;
    CUiButton m_boardthree_roadexitIdxCtrl;
    CUiButton m_boardthree_roadnameCtrl;
  };
}
#endif
