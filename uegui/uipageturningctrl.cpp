#include "uipageturningctrl.h"
#include "uictrltypes.h"

using namespace UeGui;

void UeGui::CPageController::Clear()
{
  m_total = 0;
  m_startPosition = -1;
  m_quantityOfOnePage = 0;
  memset(m_pageInfo, 0, sizeof(m_pageInfo));
}

void UeGui::CPageController::Reset()
{
  InitPageInfo();
}

void UeGui::CPageController::InitPageInfo()
{
  //初始化翻页起始信息
  if ((m_total <= 0) || (m_quantityOfOnePage <= 0))
  {
    m_startPosition = -1;
  }
  else
  {
    m_startPosition = 1;
  } 
}

void UeGui::CPageController::SetTotal( int value )
{
  if (value >= 0)
  {
    m_total = value;    
  }
  InitPageInfo();
}

void UeGui::CPageController::SetQuantityOfOnePage( int value )
{
  //只有设置了每页显示数量，才能计算总页数
  if (value > 0)
  {
    m_quantityOfOnePage = value;    
  }
  InitPageInfo();
}

void UeGui::CPageController::DeleteRecord( int numbers )
{
  //数据量减少
  m_total -= numbers;
  if (m_total <= 0)
  {
    m_total = 0;
    InitPageInfo();
    return;
  }
  if (m_startPosition > m_total)
  {
    //向前翻页
    PreviousPage(); 
  }
}

void UeGui::CPageController::NextPage()
{
  int nextPagePosition = m_startPosition + m_quantityOfOnePage;
  if (nextPagePosition > m_total)
  {
    nextPagePosition = m_startPosition;
  }
  m_startPosition = nextPagePosition;

}

void UeGui::CPageController::PreviousPage()
{
  int previousPagePosition = m_startPosition - m_quantityOfOnePage;
  if (previousPagePosition < 0)
  {
    previousPagePosition = m_startPosition;
  }
  m_startPosition = previousPagePosition;
}

int UeGui::CPageController::GetPageStartPosition()
{  
  if (m_total <= 0)
  {
    return -1;
  }
  return m_startPosition;
}

int UeGui::CPageController::GetPageEndPosition()
{
  if (m_startPosition < 0)
  {
    //如果起始位置为-1 则结束位置也为-1
    return -1;
  }
  int pageEndPosition = m_startPosition + m_quantityOfOnePage - 1;
  if (pageEndPosition > m_total)
  {
    pageEndPosition = m_total;
  }
  return pageEndPosition;
}

int UeGui::CPageController::GetTotalPage()
{
  //计算总页数
  if (m_quantityOfOnePage <= 0) 
  {
    return 0;
  }
  int totalPages = m_total / m_quantityOfOnePage;
  if ((m_total % m_quantityOfOnePage) > 0)
  {
    totalPages += 1;
  }
  return totalPages;
}

int UeGui::CPageController::GetCurrentPage()
{
  if (m_startPosition < 0)
  {
    return 0;
  }
  else
  {
    return m_startPosition / m_quantityOfOnePage + 1;
  }
}

char* UeGui::CPageController::GetPageInfo()
{
  ::sprintf(m_pageInfo, "%d/%d", GetCurrentPage(), GetTotalPage());
  return m_pageInfo;
}

bool UeGui::CPageController::IsFirstPage()
{
  if ((m_startPosition - m_quantityOfOnePage) < 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool UeGui::CPageController::IsLastPage()
{
  if ((m_startPosition + m_quantityOfOnePage) > m_total)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int UeGui::CPageController::GetLastPageEndRow()
{
  if (m_total > 0 && m_quantityOfOnePage > 0)
  {
    int result = m_total % m_quantityOfOnePage;
    return (result == 0) ? m_quantityOfOnePage : result;
  }
  return -1;
}

void UeGui::CPageController::ShowPageInfo(CUiControl *currentLable, CUiControl *totalLable)
{
  if (currentLable && totalLable)
  {
    char pageInfo[10] = {0,};
    ::sprintf(pageInfo, "%d", GetCurrentPage());
    currentLable->SetCaption(pageInfo);
    ::sprintf(pageInfo, "%d", GetTotalPage());
    totalLable->SetCaption(pageInfo);
  }
  
}