#include "guidanceviewhook.h"
using namespace UeGui;
#include "uemodel\network.h"
#include "uemodel\netnametable.h"
using namespace UeModel;


CGuidanceViewHook::CGuidanceViewHook() : m_maxProgressHight(290), m_maxProgressDist(300),m_lastDistForSnd(0)
{
}

CGuidanceViewHook::~CGuidanceViewHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CGuidanceViewHook::GetBinaryFileName()
{
  return _T("guidanceviewhook.bin");
}

void CGuidanceViewHook::MakeNames()
{
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_Delimiter,	"Delimiter"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_HideBack,	"HideBack"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_HideLabel,	"HideLabel"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_ProgressBarBack,	"ProgressBarBack"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_ProgressBar,	"ProgressBar"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_ProgressPoint,	"ProgressPoint"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_DistLabel,	"DistLabel"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_RoadNameBack,	"RoadNameBack"));
  m_ctrlNames.insert(GuiName::value_type(GuidanceViewHook_RoadnameLabel,	"RoadnameLabel"));
}

void CGuidanceViewHook::MakeControls()
{
  m_hideBtn.SetCenterElement(GetGuiElement(GuidanceViewHook_HideBack));
  m_hideBtn.SetLabelElement(GetGuiElement(GuidanceViewHook_HideLabel));
  m_distLabel.SetLabelElement(GetGuiElement(GuidanceViewHook_DistLabel));
  m_RoadNameLabel.SetCenterElement(GetGuiElement(GuidanceViewHook_RoadNameBack));
  m_RoadNameLabel.SetLabelElement(GetGuiElement(GuidanceViewHook_RoadnameLabel));

  GuiElement* guiElement = GetGuiElement(GuidanceViewHook_ProgressBar);
  if (guiElement)
  {
    m_maxProgressHight = guiElement->m_height;
  }  
}

short CGuidanceViewHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case GuidanceViewHook_HideBack:
  case GuidanceViewHook_HideLabel:
    {
      m_hideBtn.MouseDown();
    }
    break;
  default:
    assert(false);
    m_isNeedRefesh = false;
    break;
  }

  if (m_isNeedRefesh)
  {
    Refresh(VT_Guidance);
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CGuidanceViewHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CGuidanceViewHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case GuidanceViewHook_HideBack:
  case GuidanceViewHook_HideLabel:
    {
      m_hideBtn.MouseUp();
      if (m_downElementType == ctrlType)
      {
        m_view->CloseGuidanceView();
      }
    }
    break;
  default:
    assert(false);
    break;
  }

  return ctrlType;
}

bool CGuidanceViewHook::operator ()()
{
  return false;
}

void UeGui::CGuidanceViewHook::DoDraw( const CGeoRect<short> &scrExtent )
{
  RefreshProgressBar();
  CAggHook::DoDraw(scrExtent);
}

void UeGui::CGuidanceViewHook::RefreshProgressBar()
{
  GuidanceStatus dirInfo;
  unsigned int rt = m_route->GetCurrent(dirInfo);
  if (UeRoute::PEC_Success == rt)
  {
    //if (m_lastDistForSnd != dirInfo.m_curDistForSnd)
    //{
    //  m_lastDistForSnd = dirInfo.m_curDistForSnd;
    //}
    //else
    //{
    //  //避免路口放大图，车已经过了拐点(但ueroute没有notiy通知view去update)导致还停留在路口放大图的情况。
    //  if (m_view)
    //  {
    //    m_view->Update(UeRoute::ST_RenderFull);
    //    m_view->GetMediator()->UpdateHooks(CViewHook::UHT_UpdateMapHook);
    //    return;
    //  }
    //}
    if (dirInfo.m_curDistForSnd < 0)
    {
      dirInfo.m_curDistForSnd = 0;
    }
    unsigned int progressHight = m_maxProgressHight * dirInfo.m_curDistForSnd / (double)m_maxProgressDist;
    if (0 >= progressHight)
    {
      progressHight = 1;
    }
    GuiElement* progressBarElement = GetGuiElement(GuidanceViewHook_ProgressBar);
    if (progressBarElement)
    {
      progressBarElement->m_height = progressHight;
      GuiElement* pointElement = GetGuiElement(GuidanceViewHook_ProgressPoint);
      if (pointElement)
      {
        pointElement->m_startY = progressBarElement->m_startY + progressBarElement->m_height - pointElement->m_height / 2;
      }
    }
    //显示距离
    char buf[10] = {}; 
    ::sprintf(buf, "%dm", dirInfo.m_curDistForSnd);
    m_distLabel.SetCaption(buf);


    const char *signs = m_route->GetSideProp(UeRoute::SVT_RealCross);
    if(signs && ::strlen(signs))  //如果存在实际路口放大图则隐藏道路名
    {
      m_RoadNameLabel.SetVisible(false);
    }
    else
    {
      //得到道路的名字
      char *roadName = NULL;
      if ((dirInfo.m_curIndicator - 1) >= 0)
      {
       
        //下一转弯路口的道路名。
        GuidanceIndicator *nextRoad = IRoute::GetRoute()->GetIndicator(dirInfo.m_curPair, dirInfo.m_curOrderForSnd-1);
        if (nextRoad)
        {
          short length = 0;
          if (nextRoad->m_nameOffset)
          {
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
            roadName="一般道路";
          }
        }
        else
        {
          //没有取到下一条道路则隐藏控件
          m_RoadNameLabel.SetVisible(false);
          return;
        }
      }
      else
      {
         roadName="接近目的地";       
      }
      m_RoadNameLabel.SetCaption(roadName);
      m_RoadNameLabel.SetVisible(true);
    }
  }  
}