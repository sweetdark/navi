#include "routeguidancehook.h"
using namespace UeGui;


#include "uemodel\network.h"
#include "uemodel\netnametable.h"
#include "gui.h"
#include "ueroute\routebasic.h"
using namespace UeModel;

Crouteguidancehook::Crouteguidancehook()
{
  MakeGUI();
  SetButtonAttach();
}

Crouteguidancehook::~Crouteguidancehook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void Crouteguidancehook::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring Crouteguidancehook::GetBinaryFileName()
{
  return _T("routeguidancehook.bin");
}

void Crouteguidancehook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_Splitter1,	"Splitter1"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_Splitter2,	"Splitter2"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_Splitter3,	"Splitter3"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_Splitter4,	"Splitter4"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner1,	"AroundCorner1"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner2,	"AroundCorner2"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner3,	"AroundCorner3"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner4,	"AroundCorner4"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RuningCar,	"RuningCar"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteName1,	"RouteName1"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteName2,	"RouteName2"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteName3,	"RouteName3"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteName4,	"RouteName4"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_CurRouteName,	"CurRouteName"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteLength1,	"RouteLength1"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteLength2,	"RouteLength2"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteLength3,	"RouteLength3"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RouteLength4,	"RouteLength4"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon1,	"AroundCorner_Icon1"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon2,	"AroundCorner_Icon2"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon3,	"AroundCorner_Icon3"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon4,	"AroundCorner_Icon4"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon5,	"AroundCorner_Icon5"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon6,	"AroundCorner_Icon6"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_AroundCorner_Icon7,	"AroundCorner_Icon7"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RoadLink1,	"RoadLink1"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RoadLink2,	"RoadLink2"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RoadLink3,	"RoadLink3"));
  m_ctrlNames.insert(GuiName::value_type(routeguidancehook_RoadLink4,	"RoadLink4"));
}

void Crouteguidancehook::MakeControls()
{
  m_backGroundCtrl.SetCenterElement(GetGuiElement(routeguidancehook_BackGround));
  m_curRouteNameCtrl.SetCenterElement(GetGuiElement(routeguidancehook_CurRouteName));
  m_roadLink1Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RoadLink1));
  m_roadLink2Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RoadLink2));
  m_roadLink3Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RoadLink3));
  m_roadLink4Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RoadLink4));
  m_routeLength1Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteLength1));
  m_routeLength2Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteLength2));
  m_routeLength3Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteLength3));
  m_routeLength4Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteLength4));
  m_routeName1Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteName1));
  m_routeName2Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteName2));
  m_routeName3Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteName3));
  m_routeName4Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_RouteName4));
  m_runingCarCtrl.SetCenterElement(GetGuiElement(routeguidancehook_RuningCar));
  m_splitter1Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_Splitter1));
  m_splitter2Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_Splitter2));
  m_splitter3Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_Splitter3));
  m_splitter4Ctrl.SetCenterElement(GetGuiElement(routeguidancehook_Splitter4));
}

short Crouteguidancehook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case routeguidancehook_AroundCorner1:
    {
     
    }
    break;
  default:
    assert(false);
    break;
  }

  this->Refresh();
  return ctrlType;
}

short Crouteguidancehook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short Crouteguidancehook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case routeguidancehook_AroundCorner1:
    {
      
    }
    break;
  default:
    assert(false);
    break;
  }

  this->Refresh();
  return ctrlType;
}

bool Crouteguidancehook::operator ()()
{
  return false;
}

void Crouteguidancehook::SetButtonAttach()
{
  m_aroundCorner1.SetIconElement(GetGuiElement(routeguidancehook_AroundCorner1));
  m_aroundCorner2.SetIconElement(GetGuiElement(routeguidancehook_AroundCorner2));
  m_aroundCorner3.SetIconElement(GetGuiElement(routeguidancehook_AroundCorner3));
  m_aroundCorner4.SetIconElement(GetGuiElement(routeguidancehook_AroundCorner4));
}


bool Crouteguidancehook::FillRouteGuidanceInfo(GuidanceStatus dirInfo)
{
 //先清空所有控件里的内容
  ClearNameAndLength();
  HideAllAroundCornerCtrl();//添加清空左右转图标，和链接箭头，小车的箭头图标控件不做处理
  //获得路径的名字和距离
  bool mergeFlag=true;
  if(mergeFlag)
  {
     return MergeDirectRoadGuidanceInfo(dirInfo);
  }
  else
  {
    unsigned int i=0;
    float totalmeter=dirInfo.m_curDistForSnd;
    //int totalraod=m_route->GetRoute()->GetIndicatorNum(0);
    //得到目前小车所的位
    for(;i<5;i++)
    { 
      GuidanceIndicator *Road = m_route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator-i);
      //
      totalmeter+=Road->m_curDist;

      if ((dirInfo.m_curIndicator-i)==0)
      {
        SetRoadName(Road->m_nameOffset,i);
        SetRoadLength(totalmeter,i);
        ShowAroundCornerCtrl(ROAD_END,i);
        break;
      }
      else
      {
        SetRoadName(Road->m_nameOffset,i);
        SetRoadLength(totalmeter,i);
        ShowAroundCornerCtrl(Road->m_snd.m_dirCode,i);
      }
    }
   }
  return true;
}
//将显示路名和距离的控件清空
void Crouteguidancehook::ClearNameAndLength()
{
  m_curRouteNameCtrl.SetCaption("");
  m_routeLength1Ctrl.SetCaption("");
  m_routeLength2Ctrl.SetCaption("");
  m_routeLength3Ctrl.SetCaption("");
  m_routeLength4Ctrl.SetCaption("");
  m_routeName1Ctrl.SetCaption("");
  m_routeName2Ctrl.SetCaption("");
  m_routeName3Ctrl.SetCaption("");
  m_routeName4Ctrl.SetCaption("");
}
//隐藏所有的拐弯控件
void Crouteguidancehook::HideAllAroundCornerCtrl()
{
  m_aroundCorner1.SetVisible(false);
  m_aroundCorner2.SetVisible(false);
  m_aroundCorner3.SetVisible(false);
  m_aroundCorner4.SetVisible(false);
  m_roadLink1Ctrl.SetVisible(false);
  m_roadLink2Ctrl.SetVisible(false);
  m_roadLink3Ctrl.SetVisible(false);
  m_roadLink4Ctrl.SetVisible(false);

}
//设置路名
void Crouteguidancehook::SetRoadName(const int nameOffset, const unsigned short numb)
{

  char *roadName = NULL;
  if (nameOffset)
  {
    short length = 0;
    m_net->GetNameTable(UeModel::UNT_Network)->GetContent(nameOffset, &roadName, length);
    if (roadName)
    {
      unsigned char chLen = roadName[0];
      roadName++;
      roadName[chLen] = 0;
    }
  }
  else
  {
    roadName="一般道路";
  }

  switch (numb)
  {
  case 0:
    m_curRouteNameCtrl.SetCaption(roadName);
    break;
  case 1:
    m_routeName4Ctrl.SetCaption(roadName);
    break;
  case 2:
    m_routeName3Ctrl.SetCaption(roadName);
    break;
  case 3:
    m_routeName2Ctrl.SetCaption(roadName);
    break;
  case 4:
    m_routeName1Ctrl.SetCaption(roadName);
    break;
  default:
    break;
  }


}
//设置距离
void Crouteguidancehook::SetRoadLength(const float length, const unsigned short numb)
{

  char buf[15];
  if(length > 1000.)
  {
    ::sprintf(buf, "%.1fkm", length / 1000.);
  }
  else
  {       
    ::sprintf(buf, "%dm", (int)length);
  }

  switch (numb)
  {
  case 1:
     m_routeLength4Ctrl.SetCaption(buf);
    break;
  case 2:
    m_routeLength3Ctrl.SetCaption(buf);
    break;
  case 3:
    m_routeLength2Ctrl.SetCaption(buf);
    break;
  case 4:
    m_routeLength1Ctrl.SetCaption(buf);
    break;
  default:
    break;
  }
}
//显示拐弯控件
void Crouteguidancehook::ShowAroundCornerCtrl(const int sndCode,const unsigned int numb)
{
 switch(numb)
 {
 case 1:
   {
     ChangeCtrlAroundIcon(m_aroundCorner4,sndCode);
     m_roadLink4Ctrl.SetVisible(true);
   }
   break;
 case 2:
   {
     ChangeCtrlAroundIcon(m_aroundCorner3,sndCode);
      m_roadLink3Ctrl.SetVisible(true);
   }
     break;
 case 3:
   {
     ChangeCtrlAroundIcon(m_aroundCorner2,sndCode);
      m_roadLink2Ctrl.SetVisible(true);
   }
     break;
 case 4:
   {
     ChangeCtrlAroundIcon(m_aroundCorner1,sndCode);
      m_roadLink1Ctrl.SetVisible(true);
   }
     break;
 default:
   break;
 }
}
//改变拐弯图标
void Crouteguidancehook::ChangeCtrlAroundIcon(CUiBitButton &aroundCornerCtrl, const int sndCode)
{
  if ((UeRoute::DVT_DirectGo == sndCode) ||(UeRoute::DVT_Maximum==sndCode)||
    (UeRoute::DVT_MiddleGo == sndCode) ||(UeRoute::DVT_Unknown==sndCode)||
    (UeRoute::DVT_RightDirect == sndCode) ||
    (UeRoute::DVT_LeftDirect == sndCode))
  {
     ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon1));
  }
  else if(UeRoute::DVT_Right == sndCode)
  {
    ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon3));
  }
  else if(UeRoute::DVT_Left == sndCode)
  {
    ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon5));
  }
  else if(UeRoute::DVT_RTurn == sndCode)
  {
    ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon2));
  }
  else if(UeRoute::DVT_LTurn == sndCode)
  {
     ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon4));
  }
  else if(UeRoute::DVT_UTurn == sndCode)
  {
    ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon6));
  }
  else if (ROAD_END==sndCode)
  {
    ChangeElementIcon(aroundCornerCtrl.GetIconElement(), GetGuiElement(routeguidancehook_AroundCorner_Icon7));
  }
  aroundCornerCtrl.SetVisible(true);
}

bool Crouteguidancehook::ChangeElementIcon( GuiElement* destElement, const GuiElement* srcElement )
{
  if (destElement && srcElement)
  {
    destElement->m_backStyle = srcElement->m_backStyle;
    return true;
  }
  return false;
}

short Crouteguidancehook::GetSelfStartX()
{
  CViewHook::GuiElement* guiElement=m_backGroundCtrl.GetCenterElement();
  return guiElement->m_startX;
}

bool Crouteguidancehook::MergeDirectRoadGuidanceInfo(const GuidanceStatus& dirInfo)
{
  int count = 1;
  float totalmeter = dirInfo.m_curDistForSnd;
  GuidanceIndicator *road = m_route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
  SetRoadName(road->m_nameOffset,0);  //第一条记录，第一行显示
  int i = 0;
  for (; dirInfo.m_curIndicator - i >= 0; ++i)
  {
    road = m_route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator-i);
    if ((UeRoute::DVT_DirectGo == road->m_snd.m_dirCode) ||(UeRoute::DVT_MiddleGo == road->m_snd.m_dirCode) 
      ||(UeRoute::DVT_Unknown==road->m_snd.m_dirCode)||(UeRoute::DVT_RightDirect == road->m_snd.m_dirCode) ||
      (UeRoute::DVT_LeftDirect == road->m_snd.m_dirCode))
    {
      totalmeter += road->m_curDist;
    }
    else
    {
      SetRoadName(road->m_nameOffset, count);
      SetRoadLength(totalmeter, count);
      ShowAroundCornerCtrl(road->m_snd.m_dirCode, count);
      if (count++ >= 5) break;
    }
  }

  if (count == 1)   //只显示终点
  {
    PlanPosition  endPos;
    endPos.m_type = UeRoute::PT_End;
    m_route->GetPosition(endPos);
    if (endPos.m_name != "")
    {
      SetRoadName(endPos.m_nameOffset, count);
    }
    SetRoadLength(dirInfo.m_curDistForSnd, count);
    ShowAroundCornerCtrl(ROAD_END, count);
  }
  else
  {
    if (count < 5)
    {
      PlanPosition  endPos;
      endPos.m_type = UeRoute::PT_End;
      m_route->GetPosition(endPos);
      if (endPos.m_name != "")
      {
        SetRoadName(endPos.m_nameOffset, count);
      }
      SetRoadLength(totalmeter, count);
      ShowAroundCornerCtrl(ROAD_END, count);
    }
  }

 
  return true;
}

//bool Crouteguidancehook::MergeDirectRoadGuidanceInfo(const GuidanceStatus& dirInfo)
//{
//  //路径的整合
//  unsigned int i=0;
//  float totalmeter=dirInfo.m_curDistForSnd;
//  bool firstequalflag=false;
//  int k=0;
//  unsigned char snd(1);
//  GuidanceIndicator *Road;
//  int totalRoad = m_route->GetIndicatorNum(dirInfo.m_curPair);
//  while(k<5)
//  { 
//    for (;dirInfo.m_curIndicator-i>0;i++)
//    {
//      Road = m_route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator-i);
//      
//      if ((UeRoute::DVT_DirectGo == Road->m_snd.m_dirCode) ||(UeRoute::DVT_MiddleGo == Road->m_snd.m_dirCode) 
//        ||(UeRoute::DVT_Unknown==Road->m_snd.m_dirCode)||(UeRoute::DVT_RightDirect == Road->m_snd.m_dirCode) ||
//        (UeRoute::DVT_LeftDirect == Road->m_snd.m_dirCode))
//      {
//        if (!firstequalflag)
//        {
//          if (Road->m_snd.m_dirCode==dirInfo.m_curSndCode)
//          {
//            firstequalflag=true;
//          }
//        }
//        else
//        {
//          totalmeter+=Road->m_curDist;
//        }
//      }
//      else
//      {
//        if (!firstequalflag)
//        {
//          if (Road->m_snd.m_dirCode==dirInfo.m_curSndCode)
//          {
//            firstequalflag=true;
//          }
//        }
//        i++;
//         break;
//       }
//     }
//
//    if ((dirInfo.m_curIndicator-i)==0)
//    {
//      SetRoadName(Road->m_nameOffset,k);
//      SetRoadLength(totalmeter,k);
//      ShowAroundCornerCtrl(snd,k);
//      k++;
//      if (k<5)
//      {
//        //得到目前小车离目的地的距离
//        Road = m_route->GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
//        PlanPosition  endPos;
//        endPos.m_type = UeRoute::PT_End;
//        m_route->GetPosition(endPos);
//        if (endPos.m_name != "")
//        {
//          SetRoadName(endPos.m_nameOffset, k);
//        }
//        else
//        {
//          SetRoadName(0, k);
//        }
//        
//        /*if (endPos.m_name=="")
//        {
//          SetRoadName(endPos.m_name,k);
//        }
//        else
//        {
//          SetRoadName("未命名",k);
//        }*/
//        SetRoadLength(Road->m_leftDist,k);
//        ShowAroundCornerCtrl(ROAD_END,k);
//      }
//      break;
//    }
//    else
//    {
//      SetRoadName(Road->m_nameOffset,k);
//      SetRoadLength(totalmeter,k);
//      ShowAroundCornerCtrl(snd,k);
//    }
//    snd=Road->m_snd.m_dirCode;
//    k++;
//  }
//  return true;
//}

bool Crouteguidancehook::DoShow(bool show)
{
  if (show)
  {
    GuidanceStatus dirInfo;
    if (UeRoute::PEC_Success != m_route->GetCurrent(dirInfo))
    {
      show = false;
    }
    else
    {
      if (dirInfo.m_curSndCode>0)
      {
        show = FillRouteGuidanceInfo(dirInfo);
      }
      else
      {
        show = false;
      }
    }
  }
  Show(show);
  return show;
}