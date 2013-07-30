#ifndef _UEGUI_ADJUSTROUTEHOOK_H
#define _UEGUI_ADJUSTROUTEHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
#include "ueroute/routebasic.h"
#include "uequery/querybasic.h"
#include "routetypeselecthook.h"
#include "upwardordownwardhook.h"

namespace UeGui
{
  class UEGUI_CLASS CAdjustRouteHook : public CAggHook
  { 
  public:    
    const static int kMAXNAME = 128;
    typedef UeRoute::PlanPosition POIItem;
    typedef std::vector<POIItem> POIDataList;

    typedef void (*EditRouteEvent)(CAggHook* sender, const char *journeyName, 
      const unsigned int routeType,
      const POIDataList &poiList);

    //窗口显示模式
    enum WindowModel
    {
      WDefaultModel, //默认模式：WAppendModal模式
      WAppendModel,  //新增行程模式
      WEditModel,    //编辑行程模式
      WBrowseModel   //浏览行程模式
    };

    enum AdjustRouteHookCtrlType
    {
      AdjustRouteHook_Begin = 0,
      AdjustRouteHook_Background,
      AdjustRouteHook_ViewMap,
      AdjustRouteHook_ViewPrevious,
      AdjustRouteHook_NavigationTitle,
      AdjustRouteHook_Line1Left,
      AdjustRouteHook_Line1Center,      
      AdjustRouteHook_Line1Right,
      AdjustRouteHook_Line2Left,
      AdjustRouteHook_Line2Center,      
      AdjustRouteHook_Line2Right,
      AdjustRouteHook_Line3Left,
      AdjustRouteHook_Line3Center,      
      AdjustRouteHook_Line3Right,
      AdjustRouteHook_Line4Left,
      AdjustRouteHook_Line4Center,      
      AdjustRouteHook_Line4Right,
      AdjustRouteHook_Line5Left,
      AdjustRouteHook_Line5Center,      
      AdjustRouteHook_Line5Right,
      AdjustRouteHook_Line6Left,
      AdjustRouteHook_Line6Center,      
      AdjustRouteHook_Line6Right,
      AdjustRouteHook_AddPOI1,
      AdjustRouteHook_AddPOI1Icon,
      AdjustRouteHook_AddPOI2,
      AdjustRouteHook_AddPOI2Icon,
      AdjustRouteHook_AddPOI3,
      AdjustRouteHook_AddPOI3Icon,
      AdjustRouteHook_AddPOI4,
      AdjustRouteHook_AddPOI4Icon,
      AdjustRouteHook_AddPOI5,
      AdjustRouteHook_AddPOI5Icon,
      AdjustRouteHook_POI1Icon,
      AdjustRouteHook_POI1Name,
      AdjustRouteHook_POI1DeleteBack,
      AdjustRouteHook_POI1DeleteIcon,
      AdjustRouteHook_POI2Icon,
      AdjustRouteHook_POI2Name,
      AdjustRouteHook_POI2DeleteBack,
      AdjustRouteHook_POI2DeleteIcon,
      AdjustRouteHook_POI4Icon,
      AdjustRouteHook_POI3Name,
      AdjustRouteHook_POI3DeleteBack,
      AdjustRouteHook_POI3DeleteIcon,
      AdjustRouteHook_POI3Icon,
      AdjustRouteHook_POI4Name,
      AdjustRouteHook_POI4DeleteBack,
      AdjustRouteHook_POI4DeleteIcon,
      AdjustRouteHook_POI5Icon,
      AdjustRouteHook_POI5Name,
      AdjustRouteHook_POI5DeleteBack,
      AdjustRouteHook_POI5DeleteIcon,
      AdjustRouteHook_POI6Icon,
      AdjustRouteHook_POI6Name,
      AdjustRouteHook_POI6EditStartBack,
      AdjustRouteHook_POI6EditStartIcon,
      AdjustRouteHook_ClearLineLeft,
      AdjustRouteHook_ClearLineCenter,
      AdjustRouteHook_ClearLineRight,
      AdjustRouteHook_SaveLineLeft,
      AdjustRouteHook_SaveLineCenter,
      AdjustRouteHook_SaveLineRight,
      AdjustRouteHook_RouteType1,
      AdjustRouteHook_RouteType2,
      AdjustRouteHook_RouteType3,
      AdjustRouteHook_RouteType4,
      AdjustRouteHook_RouteTypeLeft,
      AdjustRouteHook_RouteTypeCenter,
      AdjustRouteHook_RouteTypeRight,
      AdjustRouteHook_RouteTypeLabel,
      AdjustRouteHook_RouteTypeIcon,
      AdjustRouteHook_PlainLineLeft,
      AdjustRouteHook_PlainLineCenter,
      AdjustRouteHook_PlainLineRight,
      AdjustRouteHook_EditLineLeft,
      AdjustRouteHook_EditLineCenter,
      AdjustRouteHook_EditLineRight,
      AdjustRouteHook_AcceptLineLeft,
      AdjustRouteHook_AcceptLineCenter,
      AdjustRouteHook_AcceptLineRight,
      AdjustRouteHook_End
    };

    enum RowTag
    {
      kROWBegin = 0,
      kROW1,
      kROW2,
      kROW3,
      kROW4,
      kROW5,
      kROW6,
      kROWEnd
    };

    CAdjustRouteHook();

    virtual ~CAdjustRouteHook();

    virtual void Init();

    virtual void MakeGUI();

    virtual void DynamicShow();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();
  
    //根据执行方式显示窗口
    void SpecialShow(WindowModel model = WDefaultModel);
    //删除POI
    void DeletePOI();
    //清空所有POI
    void ClearAll();
    //清空经由点
    void ClearPOIList();
    //添加经由点
    void AddPOIData(const POIItem& poi);
    void AddPOIData(const POIDataList& poiList);
    //添加经由点
    void InsertPOIData(int position, const UeQuery::SQLRecord* data);
    //读取要添加的POI位置,下标从0开始
    unsigned int GetInsertPosition();
    //设置行程名称
    void SetJourneyName(const char *journeyName);
    //选择设置的路线类型:推荐路线,高速优先,最短路径,经济路线
    void SetSelectRouteType(unsigned int routeType);
    //设置路线类型，这个类型不只包括高速优先，推荐，最短，还包括规避类型
    void SetRouteType(unsigned int routeType);
    //处理数据移动事件
    void DataMove(MoveType moveType);
    //显示所有导航点
    void ShowRouteData();
    //保存行程数据
    void SaveJourneyData(const char* journeyName);
    //设置编辑行程事件
    void SetEditEvent(HookType senderHookType, CAggHook* sender, const char *journeyName, 
      const unsigned int routeType, EditRouteEvent editRouteEvent);
    //重新规划路线
    void PlanRoute();
  public:
    //处理删除指定POI
    static void OnDeletePOI(CAggHook* sender, ModalResultType modalResult);
    //清空所有POI
    static void OnClearAll(CAggHook* sender, ModalResultType modalResult);
    //添加POI
    static void OnInsertPOIData(void* sender, const UeQuery::SQLRecord * data);
    //路线类型选择
    static void OnRouteTypeSelect(CAggHook* sender, unsigned int routeType);
    //数据上下移动事件
    static void OnDataMove(CAggHook* sender, MoveType moveType);
    //获取路线名称
    static void OnGetRouteName(void* sender, const UeQuery::SQLRecord * data);
    //规划路线选择提示
    static void OnPlanRouteQuestion(CAggHook* sender, ModalResultType modalResult);

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    //设置窗口显示模式
    void SetWindowModel(WindowModel model);
    //读取列表最大数据容量
    unsigned int MaxPOIListSize();
    //根据行号获取数据下标
    unsigned int GetDataIndex(RowTag row);
    //删除POI
    void DoDeletePOI(RowTag row);
    // 清空所有POI
    void DoClearAll();
    //添加POI节点
    void DoAddPOI(RowTag row);
    //处理数据移动事件
    void DoDataMove(RowTag row);
    //处理保存事件
    void DoSaveNewData();
    //处理编辑保存事件
    void DoSaveEditData();
    //清空行内容
    void ClearRow(RowTag row);
    //清空所有行数据
    void ClearAllRow();
    //设置行有效性
    void SetRowEnbale( RowTag row, bool enable);
    //将数据显示到某行上
    void ShowRowData(RowTag row, const char* value);
    //获取路线数据
    void GetRouteData();
    //根据下标删除数据
    void DeleteRouteData(int dataIndex);
    //规划路线
    void DoPlanRoute();
    //确定
    void DoAcceptSave();
    //

  private:
    //查看地图
    CUiButton m_viewMapCtrl;
    //返回
    CUiButton m_viewPreviousCtrl;
    //添加经由点
    CUiBitButton m_addPOI1Ctrl;
    CUiBitButton m_addPOI2Ctrl;
    CUiBitButton m_addPOI3Ctrl;
    CUiBitButton m_addPOI4Ctrl;
    CUiBitButton m_addPOI5Ctrl;
    //第一行控件
    CUiButton m_line1Ctrl;
    CUiLabel m_pOI1NameCtrl;
    CUiBitButton m_pOI1IconCtrl;
    CUiBitButton m_pOI1DeleteCtrl;
    //第二行控件
    CUiButton m_line2Ctrl;
    CUiLabel m_pOI2NameCtrl;
    CUiBitButton m_pOI2IconCtrl;
    CUiBitButton m_pOI2DeleteCtrl;        
    //第三行控件
    CUiButton m_line3Ctrl;
    CUiLabel m_pOI3NameCtrl;
    CUiBitButton m_pOI3IconCtrl;
    CUiBitButton m_pOI3DeleteCtrl;    
    //第四行控件
    CUiButton m_line4Ctrl;
    CUiLabel m_pOI4NameCtrl;
    CUiBitButton m_pOI4IconCtrl;
    CUiBitButton m_pOI4DeleteCtrl;      
    //第五行控件
    CUiButton m_line5Ctrl;
    CUiLabel m_pOI5NameCtrl;
    CUiBitButton m_pOI5IconCtrl;    
    CUiBitButton m_pOI5DeleteCtrl;
    //第六行控件
    CUiButton m_line6Ctrl;
    CUiLabel m_pOI6NameCtrl;
    CUiBitButton m_pOI6IconCtrl;
    CUiBitButton m_pOI6EditStartCtrl;
    //清空路线
    CUiButton m_clearCtrl;
    //保存行程
    CUiButton m_saveLineCtrl;
    //路线类型
    CUiBitButton m_routeTypeCtrl;
    //规划路线
    CUiButton m_plainLineCtrl;
    //编辑保存路线
    CUiButton m_editLineCtrl;
    //确定
    CUiButton m_acceptLineCtrl;

    //记录要删除的行号
    RowTag m_selectRowTag;
    //记录要添加数据的行号
    RowTag m_addRowTag;
    //路线类型1-高速优先
    CUiLabel m_routeType1;
    //路线类型2-最短路径
    CUiLabel m_routeType2;
    //路线类型3-经济路线
    CUiLabel m_routeType3;
    //路线类型4-推荐路线
    CUiLabel m_routeType4;

    //当前窗口的显示模式
    WindowModel m_windowModel;
    //进入当前hook的前一个hook
    unsigned int m_enterHookType;
    //行程名称
    char m_JourneyName[kMAXNAME];
    //路线类型:推荐路线,高速优先,最短路径,经济路线
    unsigned int m_routeType;
    //路线的经由点下标0为起点，目前加上起点和终点总共不超过6个经由点。
    POIDataList m_POIList;

    //编辑行程事件，由外部传入
    unsigned int m_senderHookType;
    CAggHook* m_senderHook;
    EditRouteEvent m_editRouteEvent;

  };
}
#endif
