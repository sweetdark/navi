/*
* 翻页控制对象
* 使用方法： 
* 用户调用方法 SetTotal() 告诉数据总量 
* 调用 SetQuantityOfOnePage()告诉类每页显示数量
* 通过接口 NextPage() 翻到下一页
* 通过接口 PreviousPage() 翻到上一页
* GetPageStartPosition() 获取当前页的起始位置
* GetPageEndPosition() 获取当前页的结束位置
*/

#ifndef _UEGUI_PAGECONTROLLER_H
#define _UEGUI_PAGECONTROLLER_H

#include <stdio.h>
#include <iostream>

namespace UeGui
{
  class CUiControl;
  class CPageController
  {
  public:
    CPageController() : m_startPosition(-1), m_quantityOfOnePage(0), m_total(0)
    {
      memset(m_pageInfo, 0, sizeof(m_pageInfo));
    }
    virtual ~CPageController() {}
  public:
    /// 情况数据
    void Clear();
    /// 重置翻页信息及将起始页定位到第一页
    void Reset();
    /// 设置总数量
    void SetTotal(int value);    
    /// 设置单页显示的总数量
    void SetQuantityOfOnePage(int value);
    /// 删除数据时从总数中减去个数个数
    void DeleteRecord(int numbers);
    /// 下一页
    void NextPage();
    /// 前一翻页
    void PreviousPage();
    /// 获取下一页的起始位置
    int GetPageStartPosition();
    /// 获取前一页的起始位置
    int GetPageEndPosition();
    /// 获取总页数
    int GetTotalPage();
    /// 获取当前是第几页
    int GetCurrentPage();
    /// 获取当前页数显示信息
    char* GetPageInfo();
    /// 当前页是否是第一页
    bool IsFirstPage();
    /// 当前页是否是最后一页
    bool IsLastPage(); 
    /// 获取最后一页的最后一行的行数
    int GetLastPageEndRow();
    /// 显示页数信息
    void ShowPageInfo(CUiControl *currentLable, CUiControl *totalLable);
  private:
    void InitPageInfo();
  private:
    /// 数据总数量
    int m_total;
    /// 单页显示总数
    int m_quantityOfOnePage;
    //当前页的起点 从1开始
    int m_startPosition;
    /// 页数显示信息
    char m_pageInfo[10];
  };
}

#endif //_UEGUI_PAGECONTROLLER_H