#ifndef _UEGUI_MAPMENUHOOK_H
#define _UEGUI_MAPMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"

namespace UeGui
{
  class UEGUI_CLASS CMapMenuHook : public CAggHook
  {
  public:
    enum MapMenuHookCtrlType
    {
      MapMenuHook_Begin = 0,
      MapMenuHook_Operation1Left,
      MapMenuHook_Operation1,
      MapMenuHook_Operation1Right,
      MapMenuHook_Delimiter1,
      MapMenuHook_Operation2,
      MapMenuHook_Delimiter2,
      MapMenuHook_Operation3,
      MapMenuHook_Delimiter3,
      MapMenuHook_Operation4,
      MapMenuHook_Delimiter4,
      MapMenuHook_Operation5,
      MapMenuHook_Delimiter5,
      MapMenuHook_Operation6,
      MapMenuHook_Operation6Right,
      MapMenuHook_End
    };

    //菜单类型定义
    enum MapMenuType
    {
      MT_None,            //([    ][        ][        ][        ][        ][      ]) 无操作下状态菜单
      MT_Browse,          //([菜单][周边信息][        ][        ][设 起 点][查地名]) 浏览状态菜单
      MT_Default,         //([菜单][周边信息][        ][设目的地][设 起 点][查地名]) 默认操作状态菜单
      MT_RouteReady,      //([菜单][开始导航][路线操作][多 路 径][模拟导航][取  消]) 路线规划完成时菜单
      MT_RealGuidance,    //([菜单][周边信息][路线操作][鹰 眼 图][停止导航][查地名]) 真实导航时菜单
      MT_RealGuidanceOpe, //([菜单][周边信息][路线操作][设目的地][停止导航][查地名]) 真实导航时菜单，可操作地图
      MT_DemoGuidance,    //([菜单][周边信息][路线操作][鹰 眼 图][停止模拟][  1X  ]) 模拟导航时菜单
      MT_DemoGuidanceOpe, //([菜单][周边信息][路线操作][设目的地][停止模拟][  1X  ]) 模拟导航时菜单，可操作地图
      MT_MultiRoute,      //([菜单][开始导航][路线操作][        ][模拟导航][取  消]) 多路径查看时菜单
      MT_ResetRoute,      //([菜单][周边信息][路线操作][设目的地][设 起 点][查地名]) 已规划路线但没有开始导航也没有开始模拟导航
      MT_OnlyMenu         //([菜单][        ][        ][        ][        ][      ]) 只有菜单按钮
    };
    
    //操作事件类型定义
    enum MapMenuOperation
    {
      MO_None,                 //无操作
      MO_OpenMenu,             //打开菜单
      MO_SurroundInfo,         //周边信息
      MO_RouteOperation,       //路线操作
      MO_MultiPath,            //多路径 
      MO_SetStartingPoint,     //设起点
      MO_SetDestinationPoint,  //设目的地            
      MO_StartRealGuidance,    //开始导航
      MO_StopRealGuidance,     //停止导航
      MO_StartDemoGuidance,    //模拟导航
      MO_StopDemoGuidance,     //停止模拟
      MO_DemoSpeed,            //模拟导航速度选择:1X,2X,4X,暂停
      MO_HawkEyeMap,           //鹰眼图
      MO_FindPlaceName,        //找地方
      MO_Cancel,               //取消
    };

    struct DemoSpeedInfo
    {
      char m_speedText[5];
      short m_speed;
    };
    typedef std::vector<DemoSpeedInfo> DemoSpeedInfoList;

  public:
    CMapMenuHook();

    virtual ~CMapMenuHook();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
  public:
    /*
    * 设置maphook指针
    */
    void SetMapHook(CAggHook* mapHook);
    /*
    * 设置route指针,获取规划状态
    */
    void SetRoute(IRoute *route);
    /*
    * 根据功能设置菜单类型
    */
    void MakeMapMenu(MapMenuType menuType);
    /*
    * 初始化模拟速度
    */
    void InitDemoSpeed();
  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    virtual void MakeControls();
  private:
    //初始化控件状态
    void InitOperationCtrls();
    //执行操作
    void DoOperation(MapMenuOperation operation);
    //变更模拟速度
    void UpdateDemoGuidanceSpeed();
  private:
    DemoSpeedInfoList m_speedList;
    //当前菜单类型
    MapMenuType m_menuType;
    //模拟导航速度索引值 0:暂停 1: 1X 100km/h 2: 2X 200km/h 3: 4X 400km/h
    char m_demoSpeedIndex;
    //maphook指针
    CAggHook* m_mapHook;
    //路线规划对象
    IRoute *m_route;
    //分割线
    CUiButton m_delimiter1Ctrl;
    CUiButton m_delimiter2Ctrl;
    CUiButton m_delimiter3Ctrl;
    CUiButton m_delimiter4Ctrl;
    CUiButton m_delimiter5Ctrl;
    //操作按钮
    CUiButton m_operation1Ctrl;
    CUiButton m_operation1RightCtrl;
    CUiButton m_operation2Ctrl;
    CUiButton m_operation3Ctrl;
    CUiButton m_operation4Ctrl;
    CUiButton m_operation5Ctrl;
    CUiButton m_operation6Ctrl;
    //操作类型
    MapMenuOperation m_operation1;
    MapMenuOperation m_operation2;
    MapMenuOperation m_operation3;
    MapMenuOperation m_operation4;
    MapMenuOperation m_operation5;
    MapMenuOperation m_operation6;
  };
}
#endif
