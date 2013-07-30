#include "viewspotmgr.h"
#include "simplefilereader.h"
#include "ddtservicequeryhook.h"
#include "DistAndTypeHook.h"
#include "uebase\filebasic.h"
#include "maphook.h"
#include "gui.h"
#include "assistant.h"
#include "uebase\pathconfig.h"

using namespace UeGui;
using namespace UeBase;

CViewspotMgr::CViewspotMgr(void)
{
}

CViewspotMgr::~CViewspotMgr(void)
{
  ReleaseData();
}

vector<CViewspotMgr::ViewSpotFileBlock*> CViewspotMgr::m_mainVec;
vector<CViewspotMgr::ViewSpotList*> CViewspotMgr::m_list;

void CViewspotMgr::ReadFile(const unsigned int distCode)
{
  //TCHAR path[CPathBasic::MAXPATHLENGTH] = {0,};
  const CPathBasic& pathBasic = CPathBasic::Get();
  //pathBasic.GetModulePath(path,CPathBasic::MAXPATHLENGTH);
  tstring mainFile = CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath);
  /*pathBasic.GetPathSeperator(mainFile);
  mainFile += _T("attrs");*/
  //pathBasic.GetPathSeperator(mainFile);
  mainFile += _T("viewspot");
  pathBasic.GetPathSeperator(mainFile);
  mainFile += _T("viewspot.dk");

  tstring nameFile = UeBase::CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath);
  /*pathBasic.GetPathSeperator(nameFile);
  nameFile += _T("attrs");*/
  //pathBasic.GetPathSeperator(nameFile);
  nameFile += _T("viewspot");
  pathBasic.GetPathSeperator(nameFile);
  nameFile += _T("viewspotnames.dk");

  ReleaseData();
  ReadMainFile(mainFile,distCode);
  PackData(nameFile);
}

void CViewspotMgr::ReleaseData()
{
  int size = m_mainVec.size();
  for (int i = 0; i < size; i++)
  {
    delete m_mainVec[i];
  }
  m_mainVec.clear();
  size = m_list.size();
  for (int i = 0; i < size; i++)
  {
    delete m_list[i];
  }
  m_list.clear();
}

void CViewspotMgr::ReadMainFile(const tstring& fileName, const unsigned int distCode)
{
  CSimpleFileReader fileReader(fileName,sizeof(ViewSpotFileBlock));
  fileReader.ConnectFile();
  unsigned int size = fileReader.GetDataCount();
  for(int i=0; i < size; i++)
  {
    ViewSpotFileBlock* vsfb = new ViewSpotFileBlock();
    unsigned int readSize = 0;
    ::memcpy(vsfb,fileReader.GetBlockData(i,readSize), sizeof(ViewSpotFileBlock));
    /*long x = vsfb->m_x * 100000;
    long y = vsfb->m_y * 100000;
    unsigned int code = CDistAndTypeHook::GetDistCode(x, y);
    if ((code/100) == (distCode/100))*/
    if (CAssistant::IsBelongTo(vsfb->m_distCode,distCode))
    {
      m_mainVec.push_back(vsfb);
    }
  }
  fileReader.DisconnectFile();
}

void CViewspotMgr::PackData(const tstring& fileName)
{
  const CFileBasic& fileBasic = CFileBasic::Get();
  void* fileHandle = fileBasic.OpenFile(fileName,CFileBasic::UE_FILE_READ);
  int size = m_mainVec.size();
  unsigned char length = 0;
  for (int i = 0; i < size; i++)
  {
    ViewSpotList* vsl = new ViewSpotList;
    fileBasic.SeekFile(fileHandle, m_mainVec[i]->m_offSet, CFileBasic::UE_SEEK_BEGIN);
    int count = 1;
    void *ptr = &length;
    fileBasic.ReadFile(fileHandle, &ptr, sizeof(unsigned char), count);
    count = length;
    ptr = vsl->m_name;
    fileBasic.ReadFile(fileHandle, &ptr, sizeof(unsigned char), count);
    long x = m_mainVec[i]->m_x * 100000;
    long y = m_mainVec[i]->m_y * 100000;
    vsl->m_point.m_x = x;
    vsl->m_point.m_y = y;
    IGui* gui = IGui::GetGui();
    gui->GetDistrictName(vsl->m_point, vsl->m_distName);

    m_list.push_back(vsl);

  }
  fileBasic.CloseFile(fileHandle);
}

void CViewspotMgr::OpenViewSpotHook()
{
  IView* veiw = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(veiw->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  serviceQueryHook->Clear();
  serviceQueryHook->SetTitle("景点查询");
  serviceQueryHook->SetQueryArea("广州市");
  serviceQueryHook->SetTypeContent("的旅游景点");
  serviceQueryHook->SetServiceListQueryEvent(&CViewspotMgr::OnViewSpotListQuery);
  serviceQueryHook->SetSwitchAraeEvent(&CViewspotMgr::OnAreaChange);
  ReadFile(2883840);
  int size = m_list.size();
  for (int i = 0; i < size; ++i)
  {
    serviceQueryHook->AddServiceData(m_list[i]->m_name, m_list[i]->m_distName, m_list[i]->m_point);
  }
  serviceQueryHook->ShowData();
}

void CViewspotMgr::OnViewSpotListQuery( const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point )
{
  IView* view = IView::GetView();
  CMapHook* mapHook = (CMapHook*)(view->GetHook(CViewHook::DHT_MapHook));
  mapHook->GoToPosFromList(point, keyValue1);
}

void CViewspotMgr::OnAreaChange() 
{
  IView* veiw = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(veiw->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  ReadFile(serviceQueryHook->GetQueryAreaCode());
  int size = m_list.size();
  for (int i = 0; i < size; ++i)
  {
    serviceQueryHook->AddServiceData(m_list[i]->m_name, m_list[i]->m_distName, m_list[i]->m_point);
  }
  serviceQueryHook->ShowData();
}