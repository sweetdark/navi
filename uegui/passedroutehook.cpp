/**
* Copyright (c) 2012-2013 Guangdong Ritu Information System Co.,Ltd.
*/ 
#include "passedroutehook.h"
#include "routewrapper.h"
#include "maphook.h"
#include "messagedialoghook.h"
using namespace UeGui;

CPassedRouteHook::CPassedRouteHook()
{
  m_strTitle = "经过道路";
  m_vecHookFile.push_back(_T("passedroutehook.bin"));
}

CPassedRouteHook::~CPassedRouteHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void CPassedRouteHook::Init()
{
  m_rowOneBtnCtrl.SetVisible(true);
  m_rowTwoBtnCtrl.SetVisible(true);
  m_rowThreeBtnCtrl.SetVisible(true);
  m_rowFourBtnCtrl.SetVisible(true);
  m_rowFiveBtnCtrl.SetVisible(true);

  m_flagFivePicCtrl.SetVisible(false);
  m_flagFourPicCtrl.SetVisible(false);
  m_flagThreePicCtrl.SetVisible(false);
  m_flagTwoPicCtrl.SetVisible(false);
  m_totalDistLableCtrl.ClearCaption();
  m_totalTimeLableCtrl.ClearCaption();
  m_normalRouteLableCtrl.ClearCaption();
  m_highWayDistLableCtrl.ClearCaption();

  m_rowFiveAvoidBtnCtrl.SetVisible(false);
  m_rowFourAvoidBtnCtrl.SetVisible(false);
  m_rowOneAvoidBtnCtrl.SetVisible(false);
  m_rowThreeAvoidBtnCtrl.SetVisible(false);
  m_rowTwoAvoidBtnCtrl.SetVisible(false);
  //
  InitRowList();
}


void CPassedRouteHook::Load()
{
  ClearAll();

  UeRoute::PlanResultDesc desc;
  //路线类型
  unsigned char planType = GetPlanType();
  if (m_route->GetPlanState() != UeRoute::PS_None && GetResultDesc(planType,desc))
  {
    SetPlanInfo(desc);
    LaodRouteList();
    LoadPageTuring();
    ShowRouteData();
    ShowPageInfo();
  }
}


void CPassedRouteHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_Lable1,	"Lable1"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_TotalDistLable,	"TotalDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_Lable2,	"Lable2"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_TotalTimeLable,	"TotalTimeLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_Lable3,	"Lable3"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_HighWayDistLable,	"HighWayDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_Lable4,	"Lable4"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_NormalRouteLable,	"NormalRouteLable:"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowOneBtn,	"RowOneBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowTwoBtn,	"RowTwoBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowThreeBtn,	"RowThreeBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFourBtn,	"RowFourBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFiveBtn,	"RowFiveBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowOneNameLable,	"RowOneNameLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowTwoNameLable,	"RowTwoNameLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowThreeNameLable,	"RowThreeNameLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFourNameLable,	"RowFourNameLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFiveNameLable,	"RowFiveNameLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RouteTypeBtn,	"RouteTypeBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RouteDownBtn,	"RouteDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowOneDistLable,	"RowOneDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowOneSplitLable,	"RowOneSplitLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowOneAvoidBtn,	"RowOneAvoidBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowTwoDistLable,	"RowTwoDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowTwoSplitLable,	"RowTwoSplitLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowTwoAvoidBtn,	"RowTwoAvoidBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowThreeDistLable,	"RowThreeDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowThreeSplitLable,	"RowThreeSplitLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowThreeAvoidBtn,	"RowThreeAvoidBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFourDistLable,	"RowFourDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFourSplitLable,	"RowFourSplitLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFourAvoidBtn,	"RowFourAvoidBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFiveDistLable,	"RowFiveDistLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFiveSplitLable,	"RowFiveSplitLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowFiveAvoidBtn,	"RowFiveAvoidBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_ColSplit,	"ColSplit"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowUpBtn,	"RowUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowDownBtn,	"RowDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowDownArrowBtn,	"RowDownArrowBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_RowUpArrowBtn,	"RowUpArrowBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_CurrentPageLable,	"CurrentPageLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_SplitBtn,	"SplitBtn"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_TotalPageLable,	"TotalPageLable"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_FlagOnePic,	"FlagOnePic"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_FlagTwoPic,	"FlagTwoPic"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_FlagThreePic,	"FlagThreePic"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_FlagFourPic,	"FlagFourPic"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_FlagFivePic,	"FlagFivePic"));
  m_ctrlNames.insert(GuiName::value_type(passedroutehook_FlagPic,	"FlagPic"));
}

void CPassedRouteHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_currentPageLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_CurrentPageLable));
  m_flagFivePicCtrl.SetCenterElement(GetGuiElement(passedroutehook_FlagFivePic));
  m_flagFourPicCtrl.SetCenterElement(GetGuiElement(passedroutehook_FlagFourPic));
  m_flagThreePicCtrl.SetCenterElement(GetGuiElement(passedroutehook_FlagThreePic));
  m_flagTwoPicCtrl.SetCenterElement(GetGuiElement(passedroutehook_FlagTwoPic));
  m_highWayDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_HighWayDistLable));
  m_normalRouteLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_NormalRouteLable));
  m_routeDownBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RouteDownBtn));
  m_routeTypeBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RouteTypeBtn));
  m_rowDownArrowBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowDownArrowBtn));
  m_rowDownBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowDownBtn));
  m_rowFiveAvoidBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFiveAvoidBtn));
  m_rowFiveBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFiveBtn));
  m_rowFiveDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFiveDistLable));
  m_rowFiveNameLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFiveNameLable));
  m_rowFourAvoidBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFourAvoidBtn));
  m_rowFourBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFourBtn));
  m_rowFourDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFourDistLable));
  m_rowFourNameLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowFourNameLable));
  m_rowOneAvoidBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowOneAvoidBtn));
  m_rowOneBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowOneBtn));
  m_rowOneDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowOneDistLable));
  m_rowOneNameLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowOneNameLable));
  m_rowThreeAvoidBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowThreeAvoidBtn));
  m_rowThreeBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowThreeBtn));
  m_rowThreeDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowThreeDistLable));
  m_rowThreeNameLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowThreeNameLable));
  m_rowTwoAvoidBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowTwoAvoidBtn));
  m_rowTwoBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowTwoBtn));
  m_rowTwoDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowTwoDistLable));
  m_rowTwoNameLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowTwoNameLable));
  m_rowUpArrowBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowUpArrowBtn));
  m_rowUpBtnCtrl.SetCenterElement(GetGuiElement(passedroutehook_RowUpBtn));
  m_flagOnePicCtrl.SetCenterElement(GetGuiElement(passedroutehook_FlagOnePic));
  m_totalDistLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_TotalDistLable));
  m_totalPageLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_TotalPageLable));
  m_totalTimeLableCtrl.SetCenterElement(GetGuiElement(passedroutehook_TotalTimeLable));
}

short CPassedRouteHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case passedroutehook_RouteDownBtn:
  case passedroutehook_RouteTypeBtn:
    {
      DoButtonMouseDown(m_routeTypeBtnCtrl);
    }
    break;
  case passedroutehook_RowDownArrowBtn:
  case passedroutehook_RowDownBtn:
    {
      m_rowDownBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowFiveAvoidBtn:
    {
      m_rowFiveAvoidBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowFiveBtn:
  case passedroutehook_RowFiveDistLable:
  case passedroutehook_RowFiveNameLable:
  case passedroutehook_FlagFivePic:
    {
      m_rowFiveBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowFourAvoidBtn:
    {
      m_rowFourAvoidBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowFourBtn:
  case passedroutehook_RowFourDistLable:
  case passedroutehook_RowFourNameLable:
  case passedroutehook_FlagFourPic:
    {
      m_rowFourBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowOneAvoidBtn:
    {
      m_rowOneAvoidBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowOneBtn:
  case passedroutehook_RowOneDistLable:
  case passedroutehook_RowOneNameLable:
  case passedroutehook_FlagOnePic:
    {
      m_rowOneBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowThreeAvoidBtn:
    {
      m_rowThreeAvoidBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowThreeBtn:
  case passedroutehook_RowThreeDistLable:
  case passedroutehook_RowThreeNameLable:
  case passedroutehook_FlagThreePic:
    {
      m_rowThreeBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowTwoAvoidBtn:
    {
      m_rowTwoAvoidBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowTwoBtn:
  case passedroutehook_RowTwoDistLable:
  case passedroutehook_RowTwoNameLable:
  case passedroutehook_FlagTwoPic:
    {
      m_rowTwoBtnCtrl.MouseDown();
    }
    break;
  case passedroutehook_RowUpArrowBtn:
  case passedroutehook_RowUpBtn:
    {
      m_rowUpBtnCtrl.MouseDown();
    }
    break;
  default:
    return CMenuBackgroundHook::MouseDown(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CPassedRouteHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CPassedRouteHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case passedroutehook_RouteDownBtn:
  case passedroutehook_RouteTypeBtn:
    {
      m_routeTypeBtnCtrl.MouseUp();
      TurnTo(DHT_RouteTypeSwitchHook);
    }
    break;
  case passedroutehook_RowFiveAvoidBtn:
    {
      if (IsNeedRefresh(m_rowFiveAvoidBtnCtrl))
      {
        m_rowFiveAvoidBtnCtrl.MouseUp();
        AvoidRoute(kROW5);
      }
    }
    break;
  case passedroutehook_RowFiveBtn:
  case passedroutehook_RowFiveDistLable:
  case passedroutehook_RowFiveNameLable:
  case passedroutehook_FlagFivePic:
    {
      ClickRowByMouseUp(m_rowFiveBtnCtrl, kROW5);
    }
    break;
  case passedroutehook_RowFourAvoidBtn:
    {
      if (IsNeedRefresh(m_rowFourAvoidBtnCtrl))
      {
        m_rowFourAvoidBtnCtrl.MouseUp();
        AvoidRoute(kROW4);
      } 
    }
    break;
  case passedroutehook_RowFourBtn:
  case passedroutehook_RowFourDistLable:
  case passedroutehook_RowFourNameLable:
  case passedroutehook_FlagFourPic:
    {
      ClickRowByMouseUp(m_rowFourBtnCtrl, kROW4);
    }
    break;
  case passedroutehook_RowThreeAvoidBtn:
    {
      if (IsNeedRefresh(m_rowThreeAvoidBtnCtrl))
      {
        m_rowThreeAvoidBtnCtrl.MouseUp();
        AvoidRoute(kROW3);
      } 
    }
    break;
  case passedroutehook_RowThreeBtn:
  case passedroutehook_RowThreeDistLable:
  case passedroutehook_RowThreeNameLable:
  case passedroutehook_FlagThreePic:
    {
      ClickRowByMouseUp(m_rowThreeBtnCtrl, kROW3);
    }
    break;
  case passedroutehook_RowTwoAvoidBtn:
    {
      if (IsNeedRefresh(m_rowTwoAvoidBtnCtrl))
      {
        m_rowTwoAvoidBtnCtrl.MouseUp();
        AvoidRoute(kROW2);
      } 
    }
    break;
  case passedroutehook_RowTwoBtn:
  case passedroutehook_RowTwoDistLable:
  case passedroutehook_RowTwoNameLable:
  case passedroutehook_FlagTwoPic:
    {
      ClickRowByMouseUp(m_rowTwoBtnCtrl, kROW2);
    }
    break;
  case passedroutehook_RowOneAvoidBtn:
    {
      if (IsNeedRefresh(m_rowOneAvoidBtnCtrl))
      {
        m_rowOneAvoidBtnCtrl.MouseUp();
        AvoidRoute(kROW1);
      } 
    }
    break;
  case passedroutehook_RowOneBtn:
  case passedroutehook_RowOneDistLable:
  case passedroutehook_RowOneNameLable:
  case passedroutehook_FlagOnePic:
    {
      ClickRowByMouseUp(m_rowOneBtnCtrl, kROW1);
    }
    break;
  case passedroutehook_RowUpArrowBtn:
  case passedroutehook_RowUpBtn:
    {
      if (!m_pageController.IsFirstPage())
      {
        m_rowUpBtnCtrl.MouseUp();
        m_pageController.PreviousPage();
        ShowRouteData();
        SetPageCtrlStatus();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case passedroutehook_RowDownArrowBtn:
  case passedroutehook_RowDownBtn:
    {
      if (!m_pageController.IsLastPage())
      {
        m_rowDownBtnCtrl.MouseUp();
        m_pageController.NextPage();
        SetPageCtrlStatus();
        ShowRouteData();
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  default:
    return CMenuBackgroundHook::MouseUp(scrPoint);
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void CPassedRouteHook::ClearAll()
{
  m_pageController.Clear();
  m_totalDistLableCtrl.ClearCaption();
  m_totalTimeLableCtrl.ClearCaption();
  m_normalRouteLableCtrl.ClearCaption();
  m_highWayDistLableCtrl.ClearCaption();
  ClearRows();
  
}

void CPassedRouteHook::ClearRows()
{
  for (int i = 0; i < m_rowList.size(); ++i)
  {
    m_rowList[i].Clear();
  }
}


float CPassedRouteHook::GetCurIndicatorLeftDist(const GuidanceStatus& dirInfo) const
{
  float returnValue;
  if (dirInfo.m_curOrderForSnd == dirInfo.m_curIndicator)
  {
    UeRoute::GuidanceIndicator *curIndicator = m_route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
    returnValue = curIndicator->m_curDist;
  }
  else
  {
    returnValue = dirInfo.m_curDistForSnd;
    for (int i = dirInfo.m_curIndicator - 1; i >= dirInfo.m_curOrderForSnd; --i)
    {
      UeRoute::GuidanceIndicator *curIndicator = m_route->GetIndicator(dirInfo.m_curPair, i);
      returnValue -= curIndicator->m_curDist;
    }
  }
  return returnValue; 
}


bool CPassedRouteHook::GetResultDesc(const unsigned char curPlan, PlanResultDesc &desc) const
{
  GuidanceStatus dirInfo;
  //获取当前车的位置的导航信息
  m_route->GetCurrent(dirInfo);
  int curPair = dirInfo.m_curPair;
  //有多个经由点，就有多段
  int totalPairs = m_route->GetPairs();
  for (; curPair < totalPairs; curPair++)
  {
    int total = (curPair == dirInfo.m_curPair) ? dirInfo.m_curIndicator : (m_route->GetIndicatorNum(curPair) - 1);
    for (int i = total; i >= 0; i--)
    {
      UeRoute::GuidanceIndicator *curIndicator = NULL;
      curIndicator = m_route->GetIndicator(curPair, i);
      if (curPair == dirInfo.m_curPair && i == total)
      {
        desc.totalLength += GetCurIndicatorLeftDist(dirInfo);
        if(curIndicator->m_roadClass == RC_MotorWay)
        {
          desc.highwayLen += GetCurIndicatorLeftDist(dirInfo);
        }
        else
        {
          desc.normalLen += GetCurIndicatorLeftDist(dirInfo);
        }
      }
      else
      {
        desc.totalLength += curIndicator->m_curDist;
        if(curIndicator->m_roadClass == RC_MotorWay)
        {
          desc.highwayLen += curIndicator->m_curDist;
        }
        else
        {
          desc.normalLen += curIndicator->m_curDist;
        }
      }
    }
  }

  if(desc.totalLength > 0)
  {
    double highwaySpeed = 110000.0/60.0;
    double normalSpeed = 60000.0/60.0;
    desc.costTime = desc.highwayLen / highwaySpeed + desc.normalLen / normalSpeed;
    return true;
  }
  else
  {
    return false;
  }
}

unsigned char CPassedRouteHook::GetPlanType() const
{
  unsigned int routeType = m_route->GetMethod();
  unsigned char planType = -1;
  if (routeType & RW_Fast) 
  {
    //高速优先
    planType = UeRoute::MT_Fast;
  }
  else if (routeType & RW_Short)
  {
    //最短路径
    planType = UeRoute::MT_Short;
  }
  else if (routeType & RW_Economic)
  {
    //经济路线
    planType = UeRoute::MT_Economic;
  }
  else if (routeType & RW_Optimal)
  {
    //推荐路线
    planType = UeRoute::MT_Optimal;
  }
  return planType;
}

void CPassedRouteHook::SetPlanInfo( UeRoute::PlanResultDesc& planResult)
{
  char cValue[12] = {};
  //路线总里程
  ::sprintf(cValue, "%0.1fkm", planResult.totalLength / 1000);
  m_totalDistLableCtrl.SetCaption(cValue);
  //高速里程
  ::sprintf(cValue, "%0.1fkm", planResult.highwayLen / 1000);
  m_highWayDistLableCtrl.SetCaption(cValue);
  //一般道路
  ::sprintf(cValue, "%0.1fkm", planResult.normalLen / 1000);
  m_normalRouteLableCtrl.SetCaption(cValue);
  //路线用时
  int time = planResult.costTime;
  ::sprintf(cValue, "%d小时%d分", (int)planResult.costTime / 60, (int)planResult.costTime % 60);
  m_totalTimeLableCtrl.SetCaption(cValue);

  //路线类型
  unsigned int routeType = m_route->GetMethod();
  if (routeType & RW_Fast) 
  {
    /// 高速优先
    m_routeTypeBtnCtrl.SetCaption("高速优先");
  }
  else if (routeType & RW_Short)
  {
    /// 最短路径
    m_routeTypeBtnCtrl.SetCaption("最短路径");
  }
  else if (routeType & RW_Economic)
  {
    /// 经济路线
    m_routeTypeBtnCtrl.SetCaption("经济路线");
  }
  else if (routeType & RW_Optimal)
  {
    /// 推荐路线
    m_routeTypeBtnCtrl.SetCaption("推荐路线");
  }
}

void CPassedRouteHook::LaodRouteList()
{
  CRouteWrapper &routeWrapper = CRouteWrapper::Get();
  routeWrapper.GetPassedRouteList(m_routeList);
  //合并相同道路
  routeWrapper.MergeRoad(m_routeList);
}

void CPassedRouteHook::LoadPageTuring()
{
  m_pageController.SetTotal(m_routeList.size());
  m_pageController.SetQuantityOfOnePage(kROWEnd - 1);
}

void CPassedRouteHook::ShowRouteData()
{
  ClearRows();
  ShowPageInfo();

  int startPosition = m_pageController.GetPageStartPosition() - 1;
  int endPosition = m_pageController.GetPageEndPosition() - 1;
  if ((startPosition < 0) || (endPosition < 0))
  {
    return;
  }

  int row = kROW1;
  for (int i = startPosition; i <= endPosition; ++i)
  {
    if (row >= kROWEnd)
    {
      break;
    }
    //计算meters
    char cMileages[10] = {};
    short length = 0;

    if (m_routeList[i].m_mileages < 1000)
    {
      ::sprintf(cMileages, "%dm", (int)m_routeList[i].m_mileages);
    }
    else
    {
      ::sprintf(cMileages, "%.1fkm", m_routeList[i].m_mileages / 1000.00);
    } 

    m_rowList[row - 1].Show(m_routeList[i].m_routeName, cMileages);

    ++row;
  }
}


void CPassedRouteHook::SetPageCtrlStatus()
{
  if (m_pageController.IsFirstPage())
  {
    m_rowUpBtnCtrl.SetEnable(false);
  }
  else
  {
    m_rowUpBtnCtrl.SetEnable(true);
  }
  if (m_pageController.IsLastPage())
  {
    m_rowDownBtnCtrl.SetEnable(false);
  }
  else
  {
    m_rowDownBtnCtrl.SetEnable(true);
  }
}

void CPassedRouteHook::ShowPageInfo()
{
  m_pageController.ShowPageInfo(&m_currentPageLableCtrl, &m_totalPageLableCtrl);
}

void CPassedRouteHook::InitRowList()
{
  int startFlag = GetGuiElement(passedroutehook_FlagPic)->m_bkNormal;
  int endFlag = GetGuiElement(passedroutehook_FlagPic)->m_bkFocus;

  CRouteRow row1(&m_flagOnePicCtrl, &m_rowOneNameLableCtrl
    , &m_rowOneDistLableCtrl, &m_rowOneAvoidBtnCtrl,
    &m_pageController, kROW1);
  row1.SetStartFlag(startFlag);
  row1.SetEndFlag(endFlag);

  CRouteRow row2(&m_flagTwoPicCtrl, &m_rowTwoNameLableCtrl
    , &m_rowTwoDistLableCtrl, &m_rowTwoAvoidBtnCtrl,
    &m_pageController, kROW2);
  row2.SetStartFlag(startFlag);
  row2.SetEndFlag(endFlag);

  CRouteRow row3(&m_flagThreePicCtrl, &m_rowThreeNameLableCtrl
    , &m_rowThreeDistLableCtrl, &m_rowThreeAvoidBtnCtrl,
    &m_pageController, kROW3);
  row3.SetStartFlag(startFlag);
  row3.SetEndFlag(endFlag);

  CRouteRow row4(&m_flagFourPicCtrl, &m_rowFourNameLableCtrl
    , &m_rowFourDistLableCtrl, &m_rowFourAvoidBtnCtrl,
    &m_pageController, kROW4);
  row4.SetStartFlag(startFlag);
  row4.SetEndFlag(endFlag);

  CRouteRow row5(&m_flagFivePicCtrl, &m_rowFiveNameLableCtrl
    , &m_rowFiveDistLableCtrl, &m_rowFiveAvoidBtnCtrl,
    &m_pageController, kROW5);
  row5.SetStartFlag(startFlag);
  row5.SetEndFlag(endFlag);

  m_rowList.push_back(row1);
  m_rowList.push_back(row2);
  m_rowList.push_back(row3);
  m_rowList.push_back(row4);
  m_rowList.push_back(row5);
}

void CPassedRouteHook::SelectRow(unsigned int row)
{
  if (row >= kROWBegin && row < kROWEnd)
  {
    CMapHook* hook = (CMapHook*)m_view->GetHook(DHT_MapHook);
    if (hook)
    {
      TurnTo(DHT_MapHook);
      hook->SetPickPos(GetCurrentPagePointList(), row - 1);
    }
  }
}


int CPassedRouteHook::GetDataIndex(unsigned int row)
{
  if (m_pageController.GetCurrentPage() >= 1 && row > kROWBegin && row < kROWEnd)
  {
    return m_pageController.GetPageStartPosition() + row - 2;
  }
  return -1;
}

PointList CPassedRouteHook::GetCurrentPagePointList()
{
  PointList resultList;
  PointInfo pointInfo;
  for (int i = m_pageController.GetPageStartPosition() - 1; i < m_pageController.GetPageEndPosition(); ++i)
  {
    pointInfo.m_point = m_routeList[i].m_point;
    ::strcpy(pointInfo.m_name, m_routeList[i].m_routeName);
    resultList.push_back(pointInfo);
  }
  return resultList;
}


bool CPassedRouteHook::IsNeedRefresh(CUiButton &button)
{
  if (button.IsEnable())
  {
    m_isNeedRefesh = true;
    return true;
  }
  else
  {
    m_isNeedRefesh = false;
    return false;
  }
}

void CPassedRouteHook::ClickRowByMouseUp(CUiButton &button, unsigned int row)
{
  if (IsNeedRefresh(button))
  {
    button.MouseUp();
    SelectRow(row);
  }
}

void CPassedRouteHook::DoButtonMouseDown(CUiButton &button)
{
  if (IsNeedRefresh(button))
  {
    button.MouseDown();
  }
}

void CPassedRouteHook::AvoidRoute(unsigned int row)
{
  if (row > kROWBegin && row < kROWEnd)
  {
    int index = GetDataIndex(row);
    if (index >= 0)
    {
      CMemVector blocks(sizeof(UeRoute::BlockElement));
      UeRoute::GuidanceIndicator *curIndicator = NULL;
      for (int j = m_routeList[index].m_begin; j >= m_routeList[index].m_end; --j)
      {
        curIndicator = m_route->GetRoute()->GetIndicator(0, j);
        if (curIndicator)
        {
          BlockElement oneBlock;
          oneBlock.m_parcelIdx = curIndicator->m_parcelIdx;
          oneBlock.m_linkIdx = curIndicator->m_linkIdx;
          blocks.Add(&oneBlock);
        }
      }

      if (blocks.GetCount() > 0)
      {
        m_route->SetBlock(blocks);
      }       
      m_route->SetPlanState(PS_None);
      GoToMapHook();
      CMessageDialogEvent dialogEvent(this, DHT_MapHook, NULL);
      CMessageDialogHook::ShowMessageDialog(MB_NONE, "规划中，请稍候...", dialogEvent);
      if (m_route->RoutePlan() == PEC_Success)
      {
        ::Sleep(10000);
        CMessageDialogHook::CloseMessageDialog();
      }
	    else
	    {
        CMessageDialogEvent dialogEvent(this, DHT_MapHook, NULL);
        CMessageDialogHook::ShowMessageDialog(MB_NONE, "回避规划失败！", dialogEvent);
        CMessageDialogHook::CloseMessageDialog();
	    }
    } 
  }
}
///////////////////////////CRouteRow Implemention////////////////////
bool CPassedRouteHook::CRouteRow::IsValid()
{
  if (m_rowFlagBtn && m_rowNameBtn && m_rowDistBtn 
    && m_rowAvoidBtn  && m_startFlag && m_endFlag
    && m_pageController)
  {
    return true;
  }
  return false;
}

void CPassedRouteHook::CRouteRow::Show(const char* routeName, const char* mileages)
{
  if (!IsValid() || !routeName || !mileages)
  {
    return;
  }
  //起点和终点 不能回避
  if (m_pageController->IsFirstPage() && m_rowNum == 1)
  {
    //第一页第一行显示起点
    m_rowFlagBtn->GetCenterElement()->m_backStyle = m_startFlag;
    m_rowFlagBtn->SetVisible(true);
    m_rowAvoidBtn->SetVisible(true);
    m_rowAvoidBtn->SetEnable(false);
  }
  else if (m_pageController->IsLastPage() && m_pageController->GetLastPageEndRow() == m_rowNum)
  {
    //最后一页最后一行显示目的地
    m_rowFlagBtn->GetCenterElement()->m_backStyle = m_endFlag;
    m_rowFlagBtn->SetVisible(true);
    m_rowAvoidBtn->SetVisible(true);
    m_rowAvoidBtn->SetEnable(false);
  }
  else
  {
    m_rowFlagBtn->SetVisible(false);
    m_rowAvoidBtn->SetVisible(true);
    m_rowAvoidBtn->SetEnable(true);
  }
  m_rowNameBtn->SetCaption(routeName);
  m_rowDistBtn->SetCaption(mileages);
}



void CPassedRouteHook::CRouteRow::Clear()
{
  if (!IsValid())
  {
    return;
  }

  m_rowFlagBtn->SetVisible(false);
  m_rowAvoidBtn->SetVisible(false);
  m_rowNameBtn->ClearCaption();
  m_rowDistBtn->ClearCaption();
}

