#include "guioperationright.h"

using namespace UeGui;

CGuiOperationRight* CGuiOperationRight::m_guiOpeRight = NULL;

UeGui::CGuiOperationRight::CGuiOperationRight()
{
  InitRightList();
}

UeGui::CGuiOperationRight::~CGuiOperationRight()
{

}
void UeGui::CGuiOperationRight::InitRightList()
{
  m_opeRightList.clear();
  //主菜单操作模块
  m_opeRightList.push_back(OR_MAP);
  m_opeRightList.push_back(OR_FINDPLACE);
  m_opeRightList.push_back(OR_ROUTEOPERATION);
  m_opeRightList.push_back(OR_DDTSERVICE);
  m_opeRightList.push_back(OR_MYINFORMATION);
  m_opeRightList.push_back(OR_NAVSETTING);
  m_opeRightList.push_back(OR_SYSSETTING);

  //地图操作模块  
  m_opeRightList.push_back(OR_MAPFASTOPE);
  m_opeRightList.push_back(OR_MAPDIMENSIONS);
  //m_opeRightList.push_back(OR_BACKTRACKING);

  //找地方模块  
  m_opeRightList.push_back(OR_QUERYPLACENAME);
  m_opeRightList.push_back(OR_QUERYROUTE);
  m_opeRightList.push_back(OR_QUERYTYPE);
  m_opeRightList.push_back(OR_QUERYHISTORY);
  m_opeRightList.push_back(OR_QUERYADDRESSBOOK);
  m_opeRightList.push_back(OR_QUERYAROUND);
  m_opeRightList.push_back(OR_QUERYQCODE);
  m_opeRightList.push_back(OR_QUERYDISTRICT);

  //路线操作模块  
  m_opeRightList.push_back(OR_PASSEDROUTE);
  m_opeRightList.push_back(OR_ADJUSTROUTE);
  m_opeRightList.push_back(OR_DETOURRULE);
  m_opeRightList.push_back(OR_NEXTDESTINATION);
  m_opeRightList.push_back(OR_BROWSELINE);
  m_opeRightList.push_back(OR_DEMONAVIGATION);
  m_opeRightList.push_back(OR_DELETEROUTE);

  //道道通服务模块
  m_opeRightList.push_back(OR_FOURSERVICE);
  m_opeRightList.push_back(OR_SCENICSPOTQUERY);
  m_opeRightList.push_back(OR_OPERATIONHELP);
  m_opeRightList.push_back(OR_UPDATESERVICE);

  //操作帮助
  m_opeRightList.push_back(OR_MAPBROWSE);
  m_opeRightList.push_back(OR_MAPNAVIGATION);
  m_opeRightList.push_back(OR_BASICFUCTION);
  m_opeRightList.push_back(OR_DDTSERVICEINFO);
  m_opeRightList.push_back(OR_FASTDO);
  m_opeRightList.push_back(OR_3DSHOW);
  m_opeRightList.push_back(OR_3DCITY);

  //我的信息模块
  m_opeRightList.push_back(OR_HISINFORMATION);
  m_opeRightList.push_back(OR_MYADDRESSBOOK);
  m_opeRightList.push_back(OR_MYJOURNEY);
  m_opeRightList.push_back(OR_MYFAVORITE);
  m_opeRightList.push_back(OR_CAPACITY);

  //历史信息
  m_opeRightList.push_back(OR_HISRECORD);
  m_opeRightList.push_back(OR_HISTRACK);
  m_opeRightList.push_back(OR_HISROUTE);

  //导航设置模块
  m_opeRightList.push_back(OR_MAPSETTING);
  m_opeRightList.push_back(OR_CROSSPIC);
  m_opeRightList.push_back(OR_EEYESETTING);
  m_opeRightList.push_back(OR_PROMPTSETTING);
  m_opeRightList.push_back(OR_ROUTESETTING);
  m_opeRightList.push_back(OR_TRACKSETTING);
  m_opeRightList.push_back(OR_SAFETYSETTING);
  m_opeRightList.push_back(OR_RESTORE);

  //地图显示
  m_opeRightList.push_back(OR_MAPMODEL);
  m_opeRightList.push_back(OR_MAPSTYLE);
  m_opeRightList.push_back(OR_MAPDIRECTION);
  m_opeRightList.push_back(OR_MAPINDICATE);

  //系统设置模块
  m_opeRightList.push_back(OR_VOICESETTING);
  m_opeRightList.push_back(OR_CARICONSETTING);
  m_opeRightList.push_back(OR_TIMECALIBRATION);
  m_opeRightList.push_back(OR_STARTDECLARATION);
  m_opeRightList.push_back(OR_VERSIONINFO);
}

UeGui::CGuiOperationRight* UeGui::CGuiOperationRight::GetGuiOperationRight()
{
  if (NULL == m_guiOpeRight)
  {
    m_guiOpeRight = new CGuiOperationRight;
  }
  return m_guiOpeRight;
}

void UeGui::CGuiOperationRight::Delete()
{
  if (NULL != m_guiOpeRight)
  {
    delete m_guiOpeRight;
    m_guiOpeRight = NULL;
  }
}

bool UeGui::CGuiOperationRight::HaveRight( unsigned int rightID )
{
  UeGui::CGuiOperationRight* guiOpeRight = UeGui::CGuiOperationRight::GetGuiOperationRight();
  return guiOpeRight->ExistsInRightList(rightID);

}

bool UeGui::CGuiOperationRight::ExistsInRightList( unsigned int rightID )
{
  //从权限列表中查找是当前操作有权限
  bool isFind = false;
  for (unsigned int i = 0; i < m_opeRightList.size(); ++i)
  {
    if (rightID == m_opeRightList[i])
    {
      isFind = true;
      break;
    }
  }
  return isFind;
}
