#ifndef _UEGUI_MAPGUIDEINFOVIEWHOOK_H
#define _UEGUI_MAPGUIDEINFOVIEWHOOK_H

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
#include "routewrapper.h"
#include "viewwrapper.h"

namespace UeGui
{
  class UEGUI_CLASS CMapGuideInfoViewHook : public CAggHook
  {
  public:
    enum MapGuideInfoViewHookCtrlType
    {
      MapGuideInfoViewHook_Begin = 0,
      MapGuideInfoViewHook_RouteInfoBtn,
      MapGuideInfoViewHook_RouteInfoIcon,
      MapGuideInfoViewHook_RouteInfoLabel,
      MapGuideInfoViewHook_CurDirectionBoard,
      MapGuideInfoViewHook_CurDirectionBoardIcon,
      MapGuideInfoViewHook_DistInfoLabel,
      MapGuideInfoViewHook_NextDirectionBoard,
      MapGuideInfoViewHook_NextDirectionBoardIcon,
      MapGuideInfoViewHook_HighSpeedBoardBack1,
      MapGuideInfoViewHook_HighSpeedBoardNameLabel1,
      MapGuideInfoViewHook_HighSpeedBoardTypeLabel1,
      MapGuideInfoViewHook_HighSpeedBoardDistLabel1,
      MapGuideInfoViewHook_HighSpeedBoardBack2,
      MapGuideInfoViewHook_HighSpeedBoardNameLabel2,
      MapGuideInfoViewHook_HighSpeedBoardTypeLabel2,
      MapGuideInfoViewHook_HighSpeedBoardDistLabel2,
      MapGuideInfoViewHook_HighSpeedBoardBack3,
      MapGuideInfoViewHook_HighSpeedBoardNameLabel3,
      MapGuideInfoViewHook_HighSpeedBoardTypeLabel3,
      MapGuideInfoViewHook_HighSpeedBoardDistLabel3,
      MapGuideInfoViewHook_ShowGuideViewBack,
      MapGuideInfoViewHook_ShowGuideViewLabel,
      MapGuideInfoViewHook_IconType1_1,
      MapGuideInfoViewHook_IconType1_2,
      MapGuideInfoViewHook_IconType1_3,
      MapGuideInfoViewHook_IconType1_4,
      MapGuideInfoViewHook_IconType1_5,
      MapGuideInfoViewHook_IconType2_1,
      MapGuideInfoViewHook_IconType2_2,
      MapGuideInfoViewHook_IconType2_3,
      MapGuideInfoViewHook_IconType2_4,
      MapGuideInfoViewHook_IconType2_5,
      MapGuideInfoViewHook_IconType2_6,
      MapGuideInfoViewHook_IconType2_7,
      MapGuideInfoViewHook_IconType2_8,
      MapGuideInfoViewHook_IconType2_9,
      MapGuideInfoViewHook_IconType2_10,
      MapGuideInfoViewHook_IconType2_11,
      MapGuideInfoViewHook_IconType2_12,
      MapGuideInfoViewHook_IconType2_13,
      MapGuideInfoViewHook_IconType2_14,
      MapGuideInfoViewHook_IconType2_15,
      MapGuideInfoViewHook_IconType2_16,
      MapGuideInfoViewHook_IconType2_17,
      MapGuideInfoViewHook_IconType2_18,
      MapGuideInfoViewHook_IconType2_19,
      MapGuideInfoViewHook_IconType2_20,
      MapGuideInfoViewHook_IconType2_21,
      MapGuideInfoViewHook_IconType2_22,
      MapGuideInfoViewHook_IconType2_23,
      MapGuideInfoViewHook_IconType2_24,
      MapGuideInfoViewHook_IconType2_25,
      MapGuideInfoViewHook_IconType2_26,
      MapGuideInfoViewHook_IconType2_27,
      MapGuideInfoViewHook_IconType2_28,
      MapGuideInfoViewHook_End
    };

    CMapGuideInfoViewHook();

    virtual ~CMapGuideInfoViewHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
    /**
    * \设置当前hook是否显示
    */
    virtual void Show( bool show = true );
    /**
    * \brief 更新Hook
    */
    virtual void Update(short type);
    /*
    * 设置父类hook
    */
    void SetParentHook(CAggHook* parentHook);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    /**
    * \brief 隐藏所有引导图标
    */
    void HideAllCtrl();
    /**
    * \brief 改变两个元素的图片
    */
    bool ChangeElementIcon(GuiElement* destElement, GuiElement* srcElement);
    /**
    * \brief 显示指定路口转向图控件
    */
    void ShowGuidanceIconCtrl(CUiBitButton &iconCtrl, bool isShown = true);
    /**
    * \brief 根据当前引导信息显示特定的图标提示(拐弯处图片)
    * \param isShow 显示还是隐藏
    */
    void ShowCurGuidanceIcon(bool isShow, int sndCode = 0, unsigned char infoCode = 0, int curPair = -1);
    /**
    * \brief 根据下一引导信息显示特定的图标提示(拐弯处图片)
    * \param isShow 显示还是隐藏
    */
    void ShowNextGuidanceIcon(bool isShow, int sndCode = 0);
    /**
    * \brief 显示路口放大图
    */
    void ShowGuideView();
  private:
    // 父hook
    CAggHook* m_parentHook;
    //下一道路名称
    CUiBitButton m_routeInfoBtn;
    //当前路口方向看板
    CUiBitButton m_curDirectionBoard;
    //下一路口方向看板
    CUiBitButton m_nextDirectionBoard;
    //高速看板
    CUiBitButton m_highSpeedBoard1;
    CUiLabel m_highSpeedBoardTypeLabel1;
    CUiLabel m_highSpeedBoardDistLabel1;
    CUiBitButton m_highSpeedBoard2;
    CUiLabel m_highSpeedBoardTypeLabel2;
    CUiLabel m_highSpeedBoardDistLabel2;
    CUiBitButton m_highSpeedBoard3;
    CUiLabel m_highSpeedBoardTypeLabel3;
    CUiLabel m_highSpeedBoardDistLabel3;
    //隐藏路口放大图
    CUiBitButton m_shwoGuideViewBtn;
    //路径规划访问接口
    CRouteWrapper& m_routeWrapper;
    //地图访问接口
    CViewWrapper& m_viewWrapper;
  };
}
#endif
