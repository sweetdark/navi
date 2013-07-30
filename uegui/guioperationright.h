/*
* \操作权限管理
*/

#ifndef _UEGUI_OPERATIONRIGHT_H
#define _UEGUI_OPERATIONRIGHT_H

#include <stdio.h>
#include <vector>

namespace UeGui
{
  /*
  * \GUI操作权限定义
  */
  //主菜单
  #define OR_MAP               100       //地图操作
  #define OR_FINDPLACE         101       //找地方
  #define OR_ROUTEOPERATION    102       //路线操作
  #define OR_DDTSERVICE        103       //道道通服务
  #define OR_MYINFORMATION     104       //我的信息
  #define OR_NAVSETTING        105       //导航设置
  #define OR_SYSSETTING        106       //系统设置

  //地图操作模块  
  #define OR_MAPFASTOPE        10001     //快捷操作
  #define OR_MAPDIMENSIONS     10002     //2D，3D切换
  #define OR_BACKTRACKING      10003     //返程规划

  //找地方模块  
  #define OR_QUERYPLACENAME    10101     //查地名
  #define OR_QUERYROUTE        10102     //查道路
  #define OR_QUERYTYPE         10103     //找分类查找
  #define OR_QUERYHISTORY      10104     //查历史
  #define OR_QUERYADDRESSBOOK  10105     //查地址簿
  #define OR_QUERYAROUND       10106     //查周边
  #define OR_QUERYQCODE        10107     //查Q码
  #define OR_QUERYDISTRICT     10108     //查区域
  
  //路线操作模块（可用）  
  #define OR_PASSEDROUTE       10201     //经过道路
  #define OR_ADJUSTROUTE       10202     //调整路线
  #define OR_DETOURRULE        10203     //绕路规则
  #define OR_NEXTDESTINATION   10204     //下一目的地
  #define OR_BROWSELINE        10205     //概览路线
  #define OR_DEMONAVIGATION    10206     //模拟导航
  #define OR_DELETEROUTE       10207     //删除路线
  
  //道道通服务模块（可用）  
  #define OR_FOURSERVICE       10301     //4S店查询
  #define OR_SCENICSPOTQUERY   10302     //景点查询  
  #define OR_OPERATIONHELP     10303     //操作帮助
  #define OR_UPDATESERVICE     10304     //升级服务

  //操作帮助（可用）
  #define OR_MAPBROWSE         1030301   //地图浏览
  #define OR_MAPNAVIGATION     1030302   //地图导航
  #define OR_BASICFUCTION      1030303   //基本功能
  #define OR_DDTSERVICEINFO    1030304   //道道通服务
  #define OR_FASTDO            1030305   //快速上手
  #define OR_3DSHOW            1030306   //3D演示
  #define OR_3DCITY            1030307   //3D城市

  //我的信息模块（可用）  
  #define OR_HISINFORMATION    10401     //历史信息
  #define OR_MYADDRESSBOOK     10402     //地址簿
  #define OR_MYJOURNEY         10403     //我的行程
  #define OR_MYFAVORITE        10404     //常用点
  #define OR_CAPACITY          10405     //容量信息

  //历史信息（可用）
  #define OR_HISRECORD         1040101   //历史记录
  #define OR_HISTRACK          1040102   //历史轨迹
  #define OR_HISROUTE          1040103   //历史路线

  //导航设置模块（可用）  
  #define OR_MAPSETTING        10501     //地图显示
  #define OR_CROSSPIC          10502     //路口放大图
  #define OR_EEYESETTING       10503     //电子眼
  #define OR_PROMPTSETTING     10504     //提示设置
  #define OR_ROUTESETTING      10505     //路线设置
  #define OR_TRACKSETTING      10506     //轨迹设置
  #define OR_SAFETYSETTING     10507     //安全设置
  #define OR_RESTORE           10508     //初始化

  //地图显示（可用）
  #define OR_MAPMODEL          1050101   //地图模式      
  #define OR_MAPSTYLE          1050102   //地图风格
  #define OR_MAPDIRECTION      1050103   //导航地图方向
  #define OR_MAPINDICATE       1050104   //导航地图提示

  //系统设置模块 
  #define OR_VOICESETTING      10601     //语音设置
  #define OR_CARICONSETTING    10602     //自车图标
  #define OR_TIMECALIBRATION   10603     //时间校准
  #define OR_STARTDECLARATION  10604     //启动声明
  #define OR_VERSIONINFO       10605     //版本信息  

  //权限管理对象
  class CGuiOperationRight
  {
  public:
    CGuiOperationRight();
    virtual ~CGuiOperationRight();
  public:
		/*!
		* \brief 读取对象
		*/
    static CGuiOperationRight* GetGuiOperationRight();
		/*!
		* \brief 释放对象
		*/
    static void Delete();
		/*!
		* \brief判断当前操作是否有权限
		*/
    static bool HaveRight(unsigned int rightID);
		/*!
		* \brief判断当前操作是否在权限列表中
		*/
    bool ExistsInRightList(unsigned int rightID);
  private:
		/*!
		* \brief初始化权限列表
		*/
    void InitRightList();
  private:
    //权限操作对象
    static CGuiOperationRight* m_guiOpeRight;
    //权限列表
    std::vector<unsigned int> m_opeRightList;
  };
}

#endif
