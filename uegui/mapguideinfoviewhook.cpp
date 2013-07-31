#include "mapguideinfoviewhook.h"
#include "maphook.h"
#include "ueroute\routebasic.h"
#include "uebase\vectop.h"

using namespace UeGui;

CMapGuideInfoViewHook::CMapGuideInfoViewHook() : m_parentHook(NULL), m_routeWrapper(CRouteWrapper::Get()),
m_viewWrapper(CViewWrapper::Get()), m_promptDistBarWidth(0)
{
  //地图界面渲染完成后不需要释放图片资源，提高效率
  m_needReleasePic = false;
}

CMapGuideInfoViewHook::~CMapGuideInfoViewHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CMapGuideInfoViewHook::GetBinaryFileName()
{
  return _T("mapguideinfoviewhook.bin");
}

bool CMapGuideInfoViewHook::operator ()()
{
  return false;
}

void UeGui::CMapGuideInfoViewHook::Show( bool show /*= true */ )
{
  CAggHook::Show(show);
  if (show)
  {
    Update(0);
  }
}

void CMapGuideInfoViewHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_RouteInfoBtn,	"RouteInfoBtn"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_RouteInfoIcon,	"RouteInfoIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_RouteInfoLabel,	"RouteInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_CurDirectionBoard,	"CurDirectionBoard"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_CurDirectionBoardIcon,	"CurDirectionBoardIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_DistInfoLabel,	"DistInfoLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_NextDirectionBoard,	"NextDirectionBoard"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_NextDirectionBoardIcon,	"NextDirectionBoardIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_PromptBack,	"PromptBack"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_PromptIcon,	"PromptIcon"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardBack1,	"HighSpeedBoardBack1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardNameLabel1,	"HighSpeedBoardNameLabel1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardTypeLabel1,	"HighSpeedBoardTypeLabel1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardDistLabel1,	"HighSpeedBoardDistLabel1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardBack2,	"HighSpeedBoardBack2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardNameLabel2,	"HighSpeedBoardNameLabel2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardTypeLabel2,	"HighSpeedBoardTypeLabel2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardDistLabel2,	"HighSpeedBoardDistLabel2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardBack3,	"HighSpeedBoardBack3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardNameLabel3,	"HighSpeedBoardNameLabel3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardTypeLabel3,	"HighSpeedBoardTypeLabel3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_HighSpeedBoardDistLabel3,	"HighSpeedBoardDistLabel3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_PromptDistBar,	"PromptDistBar"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_ShowGuideViewBack,	"ShowGuideViewBack"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_ShowGuideViewLabel,	"ShowGuideViewLabel"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType1_1,	"IconType1_1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType1_2,	"IconType1_2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType1_3,	"IconType1_3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType1_4,	"IconType1_4"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType1_5,	"IconType1_5"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_1,	"IconType2_1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_2,	"IconType2_2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_3,	"IconType2_3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_4,	"IconType2_4"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_5,	"IconType2_5"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_6,	"IconType2_6"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_7,	"IconType2_7"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_8,	"IconType2_8"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_9,	"IconType2_9"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_10,	"IconType2_10"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_11,	"IconType2_11"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_12,	"IconType2_12"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_13,	"IconType2_13"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_14,	"IconType2_14"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_15,	"IconType2_15"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_16,	"IconType2_16"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_17,	"IconType2_17"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_18,	"IconType2_18"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_19,	"IconType2_19"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_20,	"IconType2_20"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_21,	"IconType2_21"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_22,	"IconType2_22"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_23,	"IconType2_23"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_24,	"IconType2_24"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_25,	"IconType2_25"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_26,	"IconType2_26"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_27,	"IconType2_27"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType2_28,	"IconType2_28"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_1,	"IconType3_1"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_2,	"IconType3_2"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_3,	"IconType3_3"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_4,	"IconType3_4"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_5,	"IconType3_5"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_6,	"IconType3_6"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_7,	"IconType3_7"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_8,	"IconType3_8"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_9,	"IconType3_9"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_10,	"IconType3_10"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_11,	"IconType3_11"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_12,	"IconType3_12"));
  m_ctrlNames.insert(GuiName::value_type(MapGuideInfoViewHook_IconType3_13,	"IconType3_13"));
}

void CMapGuideInfoViewHook::MakeControls()
{
  m_routeInfoBtn.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_RouteInfoBtn));
  m_routeInfoBtn.SetIconElement(GetGuiElement(MapGuideInfoViewHook_RouteInfoIcon));
  m_routeInfoBtn.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_RouteInfoLabel));
  m_curDirectionBoard.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_CurDirectionBoard));
  m_curDirectionBoard.SetIconElement(GetGuiElement(MapGuideInfoViewHook_CurDirectionBoardIcon));
  m_curDirectionBoard.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_DistInfoLabel));
  m_nextDirectionBoard.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_NextDirectionBoard));
  m_nextDirectionBoard.SetIconElement(GetGuiElement(MapGuideInfoViewHook_NextDirectionBoardIcon));
  m_promptBtn.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_PromptBack));
  m_promptBtn.SetIconElement(GetGuiElement(MapGuideInfoViewHook_PromptIcon));
  m_promptDistBar.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_PromptDistBar));
  m_highSpeedBoard1.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardBack1));
  m_highSpeedBoard1.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardNameLabel1));
  m_highSpeedBoard2.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardBack2));
  m_highSpeedBoard2.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardNameLabel2));
  m_highSpeedBoard3.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardBack3));
  m_highSpeedBoard3.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardNameLabel3));
  m_highSpeedBoardTypeLabel1.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardTypeLabel1));
  m_highSpeedBoardTypeLabel2.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardTypeLabel2));
  m_highSpeedBoardTypeLabel3.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardTypeLabel3));
  m_highSpeedBoardDistLabel1.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardDistLabel1));
  m_highSpeedBoardDistLabel2.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardDistLabel2));
  m_highSpeedBoardDistLabel3.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_HighSpeedBoardDistLabel3));
  m_shwoGuideViewBtn.SetCenterElement(GetGuiElement(MapGuideInfoViewHook_ShowGuideViewBack));
  m_shwoGuideViewBtn.SetLabelElement(GetGuiElement(MapGuideInfoViewHook_ShowGuideViewLabel));
  GuiElement* guiElement = m_promptDistBar.GetCenterElement();
  if (guiElement)
  {
    m_promptDistBarWidth = guiElement->m_width;
  }
}

void UeGui::CMapGuideInfoViewHook::SetParentHook( CAggHook* parentHook )
{
  m_parentHook = parentHook;
}

short CMapGuideInfoViewHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case MapGuideInfoViewHook_RouteInfoBtn:
  case MapGuideInfoViewHook_RouteInfoIcon:
  case MapGuideInfoViewHook_RouteInfoLabel:
    {
      //下一道路名称
      m_routeInfoBtn.MouseDown();
      needRefresh = true;
    }
    break;
  case MapGuideInfoViewHook_CurDirectionBoard:
  case MapGuideInfoViewHook_CurDirectionBoardIcon:
  case MapGuideInfoViewHook_DistInfoLabel:
    {
      needRefresh = true;
      //m_curDirectionBoard.MouseDown();
    }
    break;
  case MapGuideInfoViewHook_ShowGuideViewBack:
  case MapGuideInfoViewHook_ShowGuideViewLabel:
    {
      m_shwoGuideViewBtn.MouseDown();
      needRefresh = true;
    }
    break;
  }
  if (needRefresh)
  {
    this->Refresh();
  }
  return ctrlType;
}

short CMapGuideInfoViewHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CMapGuideInfoViewHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  //是否需要刷新
  bool needRefresh = false;
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(ctrlType)
  {
  case MapGuideInfoViewHook_RouteInfoBtn:
  case MapGuideInfoViewHook_RouteInfoIcon:
  case MapGuideInfoViewHook_RouteInfoLabel:
    {
      //下一道路名称
      m_routeInfoBtn.MouseUp();
      needRefresh = true;
      TurnTo(DHT_PassedRouteHook);
    }
    break;
  case MapGuideInfoViewHook_CurDirectionBoard:
  case MapGuideInfoViewHook_CurDirectionBoardIcon:
  case MapGuideInfoViewHook_DistInfoLabel:
    {
      needRefresh = true;
      //m_curDirectionBoard.MouseUp();
    }
    break;
  case MapGuideInfoViewHook_ShowGuideViewBack:
  case MapGuideInfoViewHook_ShowGuideViewLabel:
    {
      m_shwoGuideViewBtn.MouseUp();
      needRefresh = true;
      ShowGuideView();
    }
    break;
  }

  if (needRefresh)
  {
    this->Refresh();
  }
  return ctrlType;
}


void UeGui::CMapGuideInfoViewHook::Update( short type )
{
  //根据导航状态更新当前导航图标的显示
  GuidanceStatus dirInfo;
  if (UeRoute::PEC_Success != m_routeWrapper.GetCurrent(dirInfo))
  {
    HideAllCtrl();
    return;
  }

  //显示电子眼
  EEyeProp eyeProp;
  double distance = 0.0;
  if (m_routeWrapper.GetCurElecEye(eyeProp, distance))
  {
    ShowPromptIcon(true, &eyeProp);
    UpdatePromptDistance(distance);
  }
  else
  {
    ShowPromptIcon(false);
  }

  //显示引导图标
  if (dirInfo.m_curIndicator >= 0 && dirInfo.m_curIndicator < m_routeWrapper.GetIndicatorNum(dirInfo.m_curPair))
  {
    //当前道路信息 底部状态栏显示当前道路信息
    GuidanceIndicator *curRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
    if (curRoad->m_roadType == RT_Virtual)   //虚拟路口则取下一道路
    {
      curRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd - 1);
    }
    if (curRoad)
    {
      if (m_parentHook)
      {
        CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
        //显示当前道路名
        char *roadName = NULL;
        short length = 0;
        if (curRoad->m_nameOffset)
        {
          m_net->GetNameTable(UeModel::UNT_Network)->GetContent(curRoad->m_nameOffset, &roadName, length);
          if (roadName)
          {
            unsigned char chLen = roadName[0];
            roadName++;
            roadName[chLen] = 0;
          }
        }
        else
        {
          roadName = "一般道路";
        }
        GpsCar gpsCar = m_viewWrapper.GetGpsCar();
        mapHook->UpdateGuideInfo(roadName, gpsCar.m_speed, curRoad->m_leftDist);
      }    
    }

    //顶部状态栏显示下一条道路信息
    char *roadName = NULL;
    //到目的地的距离，初始化为全程的长度
    UeRoute::PlanResultDesc desc;
    m_routeWrapper.GetPlanResultDesc(desc);
    int leftDist = desc.totalLength;
    if ((dirInfo.m_curIndicator - 1) >= 0)
    {   
      GuidanceIndicator *nextRoad = NULL;
      //////////////////////////////////////////////////////////////////////////////////////////////////
      //  首先取得距离终点距离
      //  nextRoad = m_viewWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator - 1);
      //  leftDist = nextRoad->m_leftDist;
      //  取下一条有名称的且和当前道路名称不同的道路
      //  int count = 1;        
      //  while ((nextRoad = m_viewWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator - count)) && 
      //       (nextRoad->m_nameOffset == 0 || nextRoad->m_nameOffset == curRoad->m_nameOffset))
      //  {
      //    //一直循环找下一条道路，都没有新道路名时，下一条道路名就取“一般道路”
      //    if(dirInfo.m_curIndicator == count)
      //    {
      //      nextRoad->m_nameOffset = 0;
      //      break;
      //    }
      //    count++;
      //  }
      //////////////////////////////////////////////////////////////////////////////////////////////////
      //刚开始导航时会出现这种为0的情况
      if (dirInfo.m_curPos.m_x != 0 && dirInfo.m_curPos.m_y != 0) 
      {
        //计算车道当前路段尾的距离
        curRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator);
        leftDist = CVectOP<long>::Mag(curRoad->m_vtxs[0], dirInfo.m_curPos);
      } 
      nextRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator - 1);
      if (nextRoad)
      {      
        if (dirInfo.m_curPos.m_x != 0 && dirInfo.m_curPos.m_y != 0)
        {
          leftDist += nextRoad->m_leftDist;
          if (::abs(leftDist - dirInfo.m_curDistForSnd) <= 1)  //避免快到终点时，与图标上的显示不一致
          {
            leftDist = dirInfo.m_curDistForSnd;
          }
        }

        if (nextRoad->m_roadType == RT_Virtual)   //虚拟路口则取下一道路
        {
          nextRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curIndicator - 2);
        }
        if (nextRoad && nextRoad->m_nameOffset)
        { 
          short length = 0;
          m_net->GetNameTable(UeModel::UNT_Network)->GetContent(nextRoad->m_nameOffset, &roadName, length);
          if (roadName)
          {
            unsigned char chLen = roadName[0];
            roadName++;
            roadName[chLen] = 0;
          }
        }
        else
        {
          roadName = "一般道路";
        }
      }
    }
    else
    {
      UeRoute::PlanPosition endPos;
      endPos.m_type = PT_End;
      m_routeWrapper.GetPosition(endPos);
      if (dirInfo.m_curPos.m_x != 0 && dirInfo.m_curPos.m_y != 0)
      {
        leftDist = CVectOP<long>::Mag(endPos.m_pos, dirInfo.m_curPos);
      }
      roadName = "接近目的地";
    }

    m_routeInfoBtn.SetCaption(roadName);
    m_routeInfoBtn.SetVisible(true);
 
    // 显示路口放大图及指引图标
    // 当前路口
    curRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd);
    int sndCode = (dirInfo.m_curSndCode <= 0) ? curRoad->m_snd.m_dirCode : dirInfo.m_curSndCode;
    ShowCurGuidanceIcon(true, sndCode, curRoad->m_snd.m_infoCode, dirInfo.m_curPair);

    char buf[10] = {};
    if (dirInfo.m_curIndicator > 0)
    {
      if(dirInfo.m_curDistForSnd > 1000.)
      {
        ::sprintf(buf, "%.1fkm", dirInfo.m_curDistForSnd / 1000.);
      }
      else
      {       
        ::sprintf(buf, "%dm", (dirInfo.m_curDistForSnd <= 0) ? 0 : dirInfo.m_curDistForSnd);
      }
    }
    else
    {
      UeRoute::PlanPosition endPos;
      endPos.m_type = PT_End;
      m_routeWrapper.GetPosition(endPos);
      int turnDist = CVectOP<long>::Mag(endPos.m_pos, dirInfo.m_curPos);
      if (turnDist < 0)
      {
        turnDist = 0;
      }
      if(turnDist > 1000)
      {
        ::sprintf(buf, "%dkm", turnDist / 1000);
      }
      else
      {       
        ::sprintf(buf, "%dm", turnDist);
      }

    }
    m_curDirectionBoard.SetCaption(buf);

    if(dirInfo.m_nextOrderForSnd <= 0 || dirInfo.m_nextOrderForSnd >= m_routeWrapper.GetIndicatorNum(dirInfo.m_curPair))
    {
      ShowNextGuidanceIcon(false);
    }
    else
    {
      //下一路口
      curRoad = m_routeWrapper.GetIndicator(dirInfo.m_curPair, dirInfo.m_nextOrderForSnd);
      sndCode = ((int)(dirInfo.m_nextSndCode) <= 0) ? curRoad->m_snd.m_dirCode : dirInfo.m_nextSndCode;
      // 图标提示
      ShowNextGuidanceIcon(true, sndCode);
    }
  }

  //如果当前显示路口放大图
  if (m_viewWrapper.IsGuidanceViewShown())
  {
    m_shwoGuideViewBtn.SetVisible(false);
    m_routeInfoBtn.SetVisible(false);
    ShowCurGuidanceIcon(false);
    ShowNextGuidanceIcon(false);
  }
  else
  {
    if (m_viewWrapper.IsNeedRenderGuidanceView())
    {
      m_shwoGuideViewBtn.SetVisible(true);
    }
    else
    {
      m_shwoGuideViewBtn.SetVisible(false);
    }
  }
}

void UeGui::CMapGuideInfoViewHook::HideAllCtrl()
{
  m_routeInfoBtn.SetVisible(false);
  m_curDirectionBoard.SetVisible(false);
  m_nextDirectionBoard.SetVisible(false);
  m_promptBtn.SetVisible(false);
  m_promptDistBar.SetVisible(false);
  m_highSpeedBoard1.SetVisible(false);
  m_highSpeedBoard1.SetVisible(false);
  m_highSpeedBoardTypeLabel1.SetVisible(false);
  m_highSpeedBoardDistLabel1.SetVisible(false);
  m_highSpeedBoard2.SetVisible(false);
  m_highSpeedBoardTypeLabel2.SetVisible(false);
  m_highSpeedBoardDistLabel2.SetVisible(false);
  m_highSpeedBoard3.SetVisible(false);
  m_highSpeedBoardTypeLabel3.SetVisible(false);
  m_highSpeedBoardDistLabel3.SetVisible(false);
  m_shwoGuideViewBtn.SetVisible(false);
}

bool UeGui::CMapGuideInfoViewHook::ChangeElementIcon( GuiElement* destElement, GuiElement* srcElement )
{
  if (destElement && srcElement)
  {
    destElement->m_backStyle = srcElement->m_backStyle;
    return true;
  }
  return false;
}


void UeGui::CMapGuideInfoViewHook::ShowGuidanceIconCtrl( CUiBitButton &iconCtrl, bool isShown /*= true*/ )
{
  if (isShown)
  {
    //检查图片是否有效,如果没有图片则不显示
    GuiElement* guiElement = iconCtrl.GetIconElement();
    if (guiElement)
    {
      if (guiElement->m_backStyle > 0)
      {
        iconCtrl.SetVisible(true);
        return;
      }
    }
    iconCtrl.SetVisible(false);
  }
  else
  {
    iconCtrl.SetVisible(false);
  }
}

void UeGui::CMapGuideInfoViewHook::ShowCurGuidanceIcon( bool isShow, int sndCode /*= 0*/, unsigned char infoCode /*= 0*/, int curPair /*= -1*/ )
{
  if (isShow)
  {
    bool rt = false;
    if ((UeRoute::DVT_DirectGo == sndCode) ||
        (UeRoute::DVT_MiddleGo == sndCode) ||
        (UeRoute::IVT_MeetMiddle == infoCode) ||
        (UeRoute::IVT_EnterToll == infoCode) ||
        (UeRoute::IVT_EnterTunnel == infoCode))
    {
      /// 直行,中线
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_1));
      if (IVT_MeetDestination == infoCode)
      {
        /// 到达目的地
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType1_1));
      }
      else if (UeRoute::IVT_MeetMiddle == infoCode)
      {
        /// 到达经由点
        switch(curPair)
        {
        case 0:
          {
            rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(),GetGuiElement(MapGuideInfoViewHook_IconType1_2));
            break;
          }
        case 1:
          {
            rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(),GetGuiElement(MapGuideInfoViewHook_IconType1_3));
            break;
          }
        case 2:
          {
            rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(),GetGuiElement(MapGuideInfoViewHook_IconType1_4));
            break;
          }
        case 3:
          {
            rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(),GetGuiElement(MapGuideInfoViewHook_IconType1_5));
            break;
          }
        }
      }
      else if (IVT_EnterTunnel == infoCode)
      {
        /// 进入隧道
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_23));
      }
      else if (IVT_EnterFerry == infoCode)
      {
        /// 接近轮渡
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_28));
      }
      else if (IVT_EnterFerry == infoCode)
      {
        /// 进入慢速路
        rt = false;
      }
      else if (IVT_EnterFastLane == infoCode)
      {
        /// 进入快速路
        rt = false;
      }
      else if (IVT_EnterHW == infoCode)
      {
        /// 进入高速
        rt = false;
      }
      else if (IVT_ExitHW == infoCode)
      {
        /// 离开高速
        rt = false;
      }
      else if (IVT_EnterSA == infoCode)
      {
        /// 有服务区
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_25));
      }
      else if (IVT_EnterToll == infoCode)
      {
        /// 有收费站
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_24));
      }
      else if (IVT_EnterBridge == infoCode)
      {
        /// 上桥
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_26));
      }
      else if (IVT_EnterOverPass == infoCode)
      {
        /// 上高架
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_26));
      }
      else if (IVT_EnterRoundAbout == infoCode)
      {
        /// 进入环岛
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_22));
      }
      else if (IVT_EnterRoundAbout == infoCode)
      {
        /// 离开环岛
        rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_22));
      }
    }
    else if (UeRoute::DVT_RightDirect == sndCode)
    { 
      /// 靠右直行
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_12));
    }
    else if (UeRoute::DVT_LeftDirect == sndCode)
    {
      /// 靠左直行
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_13));
    }
    else if (UeRoute::DVT_Right == sndCode)
    {
      /// 靠右
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_5));
    }
    else if (UeRoute::DVT_Left == sndCode)
    {
      /// 靠左
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_2));
    }
    else if (UeRoute::DVT_ForkRight == sndCode)
    {
      /// 岔路走右线
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_10));
    }
    else if (UeRoute::DVT_ForkLeft == sndCode)
    {
      /// 岔路走左线
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_11));
    }
    else if (UeRoute::DVT_UTurn == sndCode)
    {
      /// 掉头
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_9));
    }
    else if (UeRoute::DVT_RTurn == sndCode)
    {
      /// 右转
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_6));
    }
    else if (UeRoute::DVT_LTurn == sndCode)
    {
      /// 左转
      rt = ChangeElementIcon(m_curDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_3));
    }
    else
    {
      rt = false;
    }

    if (rt)
    {
      ShowGuidanceIconCtrl(m_curDirectionBoard);
    }
    else
    {
      ShowGuidanceIconCtrl(m_curDirectionBoard, false);
    }
  }
  else
  {
    ShowGuidanceIconCtrl(m_curDirectionBoard, false);
  }
}

void UeGui::CMapGuideInfoViewHook::ShowNextGuidanceIcon( bool isShow, int sndCode /*= 0*/ )
{
  if (isShow)
  {
    bool rt = false;
    if ((UeRoute::DVT_DirectGo == sndCode) || (UeRoute::DVT_MiddleGo == sndCode))
    {
      /// 直行,中线
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_1));
    }
    else if (UeRoute::DVT_RightDirect == sndCode)
    { 
      /// 靠右直行
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_12));
    }
    else if (UeRoute::DVT_LeftDirect == sndCode)
    {
      /// 靠左直行
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_13));
    }
    else if (UeRoute::DVT_Right == sndCode)
    {
      /// 靠右
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_5));
    }
    else if (UeRoute::DVT_Left == sndCode)
    {
      /// 靠左
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_2));
    }
    else if (UeRoute::DVT_ForkRight == sndCode)
    {
      /// 岔路走右线
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_10));
    }
    else if (UeRoute::DVT_ForkLeft == sndCode)
    {
      /// 岔路走左线
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_11));
    }
    else if (UeRoute::DVT_UTurn == sndCode)
    {
      /// 掉头
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_9));
    }
    else if (UeRoute::DVT_RTurn == sndCode)
    {
      /// 右转
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_6));
    }
    else if (UeRoute::DVT_LTurn == sndCode)
    {
      /// 左转
      rt = ChangeElementIcon(m_nextDirectionBoard.GetIconElement(), GetGuiElement(MapGuideInfoViewHook_IconType2_3));
    }
    else
    {
      rt = false;
    }

    if (rt)
    {
      ShowGuidanceIconCtrl(m_nextDirectionBoard);
    }
    else
    {
      ShowGuidanceIconCtrl(m_nextDirectionBoard, false);
    }
  }
  else
  {
    ShowGuidanceIconCtrl(m_nextDirectionBoard, false);
  }
}

void UeGui::CMapGuideInfoViewHook::ShowPromptIcon( bool isShow, EEyeProp* eyeProp /*= NULL*/ )
{
  if (isShow)
  {
    if (eyeProp)
    {
      short promptElementType = MapGuideInfoViewHook_Begin;
      switch (eyeProp->m_type)
      {
      case UeRoute::TVT_TrafficLights:
        {
          //红绿灯
          promptElementType = MapGuideInfoViewHook_IconType3_13;
          break;
        }
      case UeRoute::TVT_SpeedLimit:
        {
          //限速
          if (eyeProp->m_speed <= 25)
          {
            promptElementType = MapGuideInfoViewHook_IconType3_1;
          }
          else if ((eyeProp->m_speed > 25) && (eyeProp->m_speed <= 35))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_2;
          }
          else if ((eyeProp->m_speed > 35) && (eyeProp->m_speed <= 45))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_3;
          }
          else if ((eyeProp->m_speed > 45) && (eyeProp->m_speed <= 55))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_4;
          }
          else if ((eyeProp->m_speed > 55) && (eyeProp->m_speed <= 65))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_5;
          }
          else if ((eyeProp->m_speed > 65) && (eyeProp->m_speed <= 75))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_6;
          }
          else if ((eyeProp->m_speed > 75) && (eyeProp->m_speed <= 85))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_7;
          }
          else if ((eyeProp->m_speed > 85) && (eyeProp->m_speed <= 95))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_8;
          }
          else if ((eyeProp->m_speed > 95) && (eyeProp->m_speed <= 105))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_9;
          }
          else if ((eyeProp->m_speed > 105) && (eyeProp->m_speed <= 115))
          {
            promptElementType = MapGuideInfoViewHook_IconType3_10;
          }
          else
          {
            promptElementType = MapGuideInfoViewHook_IconType3_11;
          }
          break;
        }
      case UeRoute::TVT_NormalCamera:
      case UeRoute::TVT_InTunnel:
      case UeRoute::TVT_TunnelPort:
        {
          //电子眼
          promptElementType = MapGuideInfoViewHook_IconType3_12;
          break;
        }
      default:
        {
          return;
        }
      }
      if (MapGuideInfoViewHook_Begin != promptElementType)
      {
        //先只显示电子眼
        ChangeElementIcon(m_promptBtn.GetIconElement(), GetGuiElement(promptElementType));
        m_promptBtn.SetVisible(true);
        m_promptDistBar.SetVisible(true);
      }
      else
      {
        m_promptBtn.SetVisible(false);
        m_promptDistBar.SetVisible(false);
      }
    }
    else
    {
      m_promptBtn.SetVisible(false);
      m_promptDistBar.SetVisible(false);
    }
  }
  else
  {
    m_promptBtn.SetVisible(false);
    m_promptDistBar.SetVisible(false);
  }
}

void UeGui::CMapGuideInfoViewHook::UpdatePromptDistance( double distance /*= 0*/ )
{
  int promptDist = distance;
  if (promptDist < 0)
  {
    promptDist = 0;
  }
  if (promptDist > MAX_PROPMT_DISTANCE)
  {
    promptDist = MAX_PROPMT_DISTANCE;
  }
  GuiElement* guiElement = m_promptDistBar.GetCenterElement();
  if (guiElement)
  {
    int progress = (m_promptDistBarWidth * promptDist) / MAX_PROPMT_DISTANCE;
    if (progress <= 0)
    {
      progress = 1;
    }
    guiElement->m_width = progress;
  }
}

void UeGui::CMapGuideInfoViewHook::ShowGuideView()
{
  if (m_parentHook)
  {
    CMapHook* mapHook = dynamic_cast<CMapHook*>(m_parentHook);
    mapHook->ShowGuideView();
  }
}