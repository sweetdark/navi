#include "ddtservicequeryhook.h"
#include "assistant.h"
#include "uebase\filebasic.h"
#include "maphook.h"
#include "uegui\gui.h"
#include "uebase\pathconfig.h"
#include "simplefilereader.h"
#include "districtselectionhook.h"
#include "querywrapper.h"
#include "messagedialoghook.h"
#include "distselecthook.h"

using namespace UeGui;
using namespace UeBase;
using namespace std;

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

  for (int i =0; i != m_list.size(); ++i)
  {
    CDdtServiceQueryHook::AddServiceData(m_list[i]->m_name, m_list[i]->m_distName, m_list[i]->m_point);
  }
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
  /*serviceQueryHook->Clear();
  serviceQueryHook->SetTitle("景点查询");
  serviceQueryHook->SetQueryArea("广州市");
  serviceQueryHook->SetTypeContent("的旅游景点");
  serviceQueryHook->SetServiceListQueryEvent(&CViewspotMgr::OnViewSpotListQuery);
  serviceQueryHook->SetSwitchAraeEvent(&CViewspotMgr::OnAreaChange);*/
  ReadFile(2883840);
  int size = m_list.size();
  for (int i = 0; i < size; ++i)
  {
    //serviceQueryHook->AddServiceData(m_list[i]->m_name, m_list[i]->m_distName, m_list[i]->m_point);
  }
  //serviceQueryHook->ShowData();
}

void CViewspotMgr::OnViewSpotListQuery( const char* keyValue1, const char* keyValue2, const CGeoPoint<long>& point )
{
  IView* view = IView::GetView();
  CMapHook* mapHook = (CMapHook*)(view->GetHook(CViewHook::DHT_MapHook));
  //mapHook->GoToPosFromList(point, keyValue1);
}

void CViewspotMgr::OnAreaChange() 
{
  IView* veiw = IView::GetView();
  CDdtServiceQueryHook* serviceQueryHook = dynamic_cast<CDdtServiceQueryHook*>(veiw->GetHook(CViewHook::DHT_DdtServiceQueryHook));
  //ReadFile(serviceQueryHook->GetQueryAreaCode());
  int size = m_list.size();
  for (int i = 0; i < size; ++i)
  {
    //serviceQueryHook->AddServiceData(m_list[i]->m_name, m_list[i]->m_distName, m_list[i]->m_point);
  }
  //serviceQueryHook->ShowData();
}



CDdtServiceQueryHook::CDdtServiceQueryHook():m_code(2883840)
{
  m_strTitle = "请选择一条记录";
  m_vecHookFile.push_back(_T("ddtservicequeryhook.bin"));
  AllCode();
}

void CDdtServiceQueryHook::AllCode()
{
  m_special.push_back("北京市");
  m_special.push_back("天津市");
  m_special.push_back("上海市");
  m_special.push_back("重庆市");
  m_special.push_back("香港特别行政区");
  m_special.push_back("奥盟特别行政区");

  m_pCurItemCtrl = &CCodeIndexCtrl::GetDistCodeCtrl();
  m_pCurItemCtrl->GetComItem(m_vecListItem);
  m_pCurItemCtrl->GetLeve1Item(m_vecListItem);
  std::vector<UeQuery::TCodeEntry>::iterator begin = m_vecListItem.begin();
  std::vector<UeQuery::TCodeEntry>::iterator end = m_vecListItem.end();
  bool tag = true;
  for (;begin != end; ++begin)
  {
    m_vecListItem4.push_back(*begin);
    std::vector<char*>::iterator begin2 = m_special.begin();
    std::vector<char*>::iterator end2 = m_special.end();
    for (;begin2 != end2; ++begin2)
    {
      if (!strcmp(*begin2, (*begin).m_chName))
      {
        m_pCurItemCtrl->GetLeve3Item((*begin).m_uCode, m_vecListItem2);
        std::vector<UeQuery::TCodeEntry>::iterator begin = m_vecListItem2.begin();
        std::vector<UeQuery::TCodeEntry>::iterator end = m_vecListItem2.end();
        for (;begin != end; ++begin)
        {
          m_vecListItem4.push_back(*begin);
        }
        tag = false;
        break;
      }
      else
      {
        tag = true;
      }
    }
    if(tag)
    {
      m_pCurItemCtrl->GetLeve2Item((*begin).m_uCode, m_vecListItem2);
      std::vector<UeQuery::TCodeEntry>::iterator begin = m_vecListItem2.begin();
      std::vector<UeQuery::TCodeEntry>::iterator end = m_vecListItem2.end();
      for (;begin != end; ++begin)
      {
        m_vecListItem4.push_back(*begin);
        m_pCurItemCtrl->GetLeve3Item((*begin).m_uCode, m_vecListItem3);
        if (m_vecListItem3.size() != 0)
        {
          std::vector<UeQuery::TCodeEntry>::iterator begin = m_vecListItem3.begin();
          std::vector<UeQuery::TCodeEntry>::iterator end = m_vecListItem3.end();
          for (;begin != end; ++begin)
          {
            m_vecListItem4.push_back(*begin);
          }
        }
      }
    }
  }
}
CDdtServiceQueryHook::~CDdtServiceQueryHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}
ServiceList CDdtServiceQueryHook::m_dataList;

void CDdtServiceQueryHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_DistSwitchBtn,	"DistSwitchBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_DistSelectBtn,	"DistSelectBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_DistSelectIcon,	"DistSelectIcon"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_DistLabel,	"DistLabel"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_TagBack,	"TagBack"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_TagAllBtn,	"TagAllBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Tag5ABtn,	"Tag5ABtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Tag4ABtn,	"Tag4ABtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Tag3ABtn,	"Tag3ABtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_TagOtherBtn,	"TagOtherBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_List1Btn,	"List1Btn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_List2Btn,	"List2Btn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_List3Btn,	"List3Btn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_List4Btn,	"List4Btn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Search1,	"Search1"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Search2,	"Search2"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Search3,	"Search3"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Search4,	"Search4"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Name1Up,	"Name1Up"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Name2Up,	"Name2Up"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Name3Up,	"Name3Up"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Name4Up,	"Name4Up"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Address1Down,	"Address1Down"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Address2Down,	"Address2Down"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Address3Down,	"Address3Down"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Address4Down,	"Address4Down"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Phone1,	"Phone1"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Phone2,	"Phone2"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Phone3,	"Phone3"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_Phone4,	"Phone4"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_PageCenter,	"PageCenter"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_PageUpBtn,	"PageUpBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_PageDownBtn,	"PageDownBtn"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_PageUpIcon,	"PageUpIcon"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_PageDownIcon,	"PageDownIcon"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_CurrentPage,	"CurrentPage"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_TotalPage,	"TotalPage"));
  m_ctrlNames.insert(GuiName::value_type(ddtservicequeryhook_PageSeparator,	"PageSeparator"));
}

void CDdtServiceQueryHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_address1DownCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Address1Down));
  m_address2DownCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Address2Down));
  m_address3DownCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Address3Down));
  m_address4DownCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Address4Down));

  m_distSelectBtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_DistSelectBtn));
  m_distSelectBtnCtrl.SetIconElement(GetGuiElement(ddtservicequeryhook_DistSelectIcon));
  m_distSwitchBtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_DistSwitchBtn));
  m_list1BtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_List1Btn));
  m_list2BtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_List2Btn));
  m_list3BtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_List3Btn));
  m_list4BtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_List4Btn));
  m_name1UpCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Name1Up));
  m_name2UpCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Name2Up));
  m_name3UpCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Name3Up));
  m_name4UpCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Name4Up));

  m_pageDownBtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_PageDownBtn));
  m_pageDownBtnCtrl.SetIconElement(GetGuiElement(ddtservicequeryhook_PageDownIcon));
  m_pageUpBtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_PageUpBtn));
  m_pageUpBtnCtrl.SetIconElement(GetGuiElement(ddtservicequeryhook_PageUpIcon));
  m_currentPageCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_CurrentPage));
  m_totalPageCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_TotalPage));

  m_phone1Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Phone1));
  m_phone2Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Phone2));
  m_phone3Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Phone3));
  m_phone4Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Phone4));

  m_tag3ABtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Tag3ABtn));
  m_tag4ABtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Tag4ABtn));
  m_tag5ABtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Tag5ABtn));
  m_tagAllBtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_TagAllBtn));

  m_search1Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Search1));
  m_search2Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Search2));
  m_search3Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Search3));
  m_search4Ctrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_Search4));

  m_tagOtherBtnCtrl.SetCenterElement(GetGuiElement(ddtservicequeryhook_TagOtherBtn));
}

short CDdtServiceQueryHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case ddtservicequeryhook_Address1Down:
  case ddtservicequeryhook_List1Btn:
  case ddtservicequeryhook_Name1Up:
  case ddtservicequeryhook_Phone1:
  case ddtservicequeryhook_Search1:
    {
      m_address1DownCtrl.MouseDown();
      m_list1BtnCtrl.MouseDown();
      m_name1UpCtrl.MouseDown();
      m_phone1Ctrl.MouseDown();      
      AddRenderUiControls(&m_list1BtnCtrl);
      AddRenderUiControls(&m_address1DownCtrl);
      AddRenderUiControls(&m_name1UpCtrl);
      AddRenderUiControls(&m_phone1Ctrl);
      AddRenderElements(GetGuiElement(ddtservicequeryhook_Search1));
    }
    break;
  case ddtservicequeryhook_Address2Down:
  case ddtservicequeryhook_List2Btn:
  case ddtservicequeryhook_Name2Up:
  case ddtservicequeryhook_Phone2:
  case ddtservicequeryhook_Search2:
    {
      m_address2DownCtrl.MouseDown();
      m_list2BtnCtrl.MouseDown();
      m_name2UpCtrl.MouseDown();
      m_phone2Ctrl.MouseDown();
      AddRenderUiControls(&m_list2BtnCtrl);
      AddRenderUiControls(&m_address2DownCtrl);
      AddRenderUiControls(&m_name2UpCtrl);
      AddRenderUiControls(&m_phone2Ctrl);
      AddRenderElements(GetGuiElement(ddtservicequeryhook_Search2));
    }
    break;
  case ddtservicequeryhook_Address3Down:
  case ddtservicequeryhook_List3Btn:
  case ddtservicequeryhook_Name3Up:
  case ddtservicequeryhook_Phone3:
  case ddtservicequeryhook_Search3:
    {
      m_address3DownCtrl.MouseDown();
      m_list3BtnCtrl.MouseDown();
      m_name3UpCtrl.MouseDown();
      m_phone3Ctrl.MouseDown();
      AddRenderUiControls(&m_list3BtnCtrl);
      AddRenderUiControls(&m_address3DownCtrl);
      AddRenderUiControls(&m_name3UpCtrl);
      AddRenderUiControls(&m_phone3Ctrl);
      AddRenderElements(GetGuiElement(ddtservicequeryhook_Search3));
    }
    break;
  case ddtservicequeryhook_Address4Down:
  case ddtservicequeryhook_List4Btn:
  case ddtservicequeryhook_Name4Up:
  case ddtservicequeryhook_Phone4:
  case ddtservicequeryhook_Search4:
    {
      m_address4DownCtrl.MouseDown();
      m_list4BtnCtrl.MouseDown();
      m_name4UpCtrl.MouseDown();
      m_phone4Ctrl.MouseDown();
      AddRenderUiControls(&m_list4BtnCtrl);
      AddRenderUiControls(&m_address4DownCtrl);
      AddRenderUiControls(&m_name4UpCtrl);
      AddRenderUiControls(&m_phone4Ctrl);
      AddRenderElements(GetGuiElement(ddtservicequeryhook_Search4));
    }
    break;
  case ddtservicequeryhook_DistSelectBtn:
  case ddtservicequeryhook_DistSelectIcon:
    {
      m_distSelectBtnCtrl.MouseDown();
      AddRenderUiControls(&m_distSelectBtnCtrl);
    }
    break;
  case ddtservicequeryhook_DistSwitchBtn:
    {
      m_distSwitchBtnCtrl.MouseDown();
      AddRenderUiControls(&m_distSwitchBtnCtrl);
    }
    break;
  case ddtservicequeryhook_PageDownBtn:
  case ddtservicequeryhook_PageDownIcon:
    {
      m_pageDownBtnCtrl.MouseDown();
      if (!m_pageDownBtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_pageDownBtnCtrl);
    }
    break;
  case ddtservicequeryhook_PageUpBtn:
  case ddtservicequeryhook_PageUpIcon:
    {
      m_pageUpBtnCtrl.MouseDown();
      if (!m_pageUpBtnCtrl.IsEnable())
      {
        m_isNeedRefesh = false;
        break;
      }
      AddRenderUiControls(&m_pageUpBtnCtrl);
    }
    break;
  case ddtservicequeryhook_Tag3ABtn:
    {
      m_tag3ABtnCtrl.MouseDown();
      AddRenderUiControls(&m_tag3ABtnCtrl);
    }
    break;
  case ddtservicequeryhook_Tag4ABtn:
    {
      m_tag4ABtnCtrl.MouseDown();
      AddRenderUiControls(&m_tag4ABtnCtrl);
    }
    break;
  case ddtservicequeryhook_Tag5ABtn:
    {
      m_tag5ABtnCtrl.MouseDown();
      AddRenderUiControls(&m_tag5ABtnCtrl);
    }
    break;
  case ddtservicequeryhook_TagAllBtn:
    {
      m_tagAllBtnCtrl.MouseDown();
      AddRenderUiControls(&m_tagAllBtnCtrl);
    }
    break;
  case ddtservicequeryhook_TagOtherBtn:
    {
      m_tagOtherBtnCtrl.MouseDown();
      AddRenderUiControls(&m_tagOtherBtnCtrl);
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

short CDdtServiceQueryHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CDdtServiceQueryHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  bool cando = (ctrlType == m_downElementType)?true:false;
  switch(m_downElementType)
  {
  case ddtservicequeryhook_Address1Down:
  case ddtservicequeryhook_List1Btn:
  case ddtservicequeryhook_Name1Up:
  case ddtservicequeryhook_Phone1:
  case ddtservicequeryhook_Search1:
    {
      m_address1DownCtrl.MouseUp();
      m_list1BtnCtrl.MouseUp();
      m_name1UpCtrl.MouseUp();
      m_phone1Ctrl.MouseUp();
      if (cando && m_list1BtnCtrl.IsEnable())
      {
        SelectRecord(kROW1);
      }
    }
    break;
  case ddtservicequeryhook_Address2Down:
  case ddtservicequeryhook_List2Btn:
  case ddtservicequeryhook_Name2Up:
  case ddtservicequeryhook_Phone2:
  case ddtservicequeryhook_Search2:
    {
      m_address2DownCtrl.MouseUp();
      m_list2BtnCtrl.MouseUp();
      m_name2UpCtrl.MouseUp();
      m_phone2Ctrl.MouseUp();
      if (cando && m_list2BtnCtrl.IsEnable())
      {
        SelectRecord(kROW2);
      }
    }
    break;
  case ddtservicequeryhook_Address3Down:
  case ddtservicequeryhook_List3Btn:
  case ddtservicequeryhook_Name3Up:
  case ddtservicequeryhook_Phone3:
  case ddtservicequeryhook_Search3:
    {
      m_address3DownCtrl.MouseUp();
      m_list3BtnCtrl.MouseUp();
      m_name3UpCtrl.MouseUp();
      m_phone3Ctrl.MouseUp();
      if (cando && m_list3BtnCtrl.IsEnable())
      {
        SelectRecord(kROW3);
      }
    }
    break;
  case ddtservicequeryhook_Address4Down:
  case ddtservicequeryhook_List4Btn:
  case ddtservicequeryhook_Name4Up:
  case ddtservicequeryhook_Phone4:
  case ddtservicequeryhook_Search4:
    {
      m_address4DownCtrl.MouseUp();
      m_list4BtnCtrl.MouseUp();
      m_name4UpCtrl.MouseUp();
      m_phone4Ctrl.MouseUp();
      if (cando && m_list4BtnCtrl.IsEnable())
      {
        SelectRecord(kROW4);
      }
    }
    break;
  case ddtservicequeryhook_DistSelectBtn:
  case ddtservicequeryhook_DistSelectIcon:
    {
      m_distSelectBtnCtrl.MouseUp();
      if (cando)
      {
        ClearData();
        CDistSelectHook* hook = (CDistSelectHook*)m_view->GetHook(DHT_DistSelectHook);
        if (hook)
        {
          hook->SetCallBackFun(this, DistSwitchCallBack);
        }
        TurnTo(DHT_DistSelectHook);
      }
    }
    break;
  case ddtservicequeryhook_DistSwitchBtn:
    {
      m_distSwitchBtnCtrl.MouseUp();
      if (cando)
      {
        ClearData();
        CDistrictSelectionHook* hook = (CDistrictSelectionHook*)m_view->GetHook(DHT_DistrictSelectionHook);
        if (hook)
        {
          hook->SetCallBackFun(this, DoDistSwitchCallBack);
        }
        TurnTo(DHT_DistrictSelectionHook);
      }      
    }
    break;
  case ddtservicequeryhook_PageDownBtn:
  case ddtservicequeryhook_PageDownIcon:
    {
      m_pageDownBtnCtrl.MouseUp();
      if (cando && m_pageDownBtnCtrl.IsEnable())
      {
        m_pageTurning.NextPage();
        SwitchPage();
        SetPageInfo();
        ShowData();
      }
    }
    break;
  case ddtservicequeryhook_PageUpBtn:
  case ddtservicequeryhook_PageUpIcon:
    {
      m_pageUpBtnCtrl.MouseUp();
      if (cando && m_pageUpBtnCtrl.IsEnable())
      {
        m_pageTurning.PreviousPage();
        SwitchPage();
        SetPageInfo();
        ShowData();
      }
    }
    break;
  case ddtservicequeryhook_Tag3ABtn:
    {
      m_tag3ABtnCtrl.MouseUp();
      SwitchTabStatus(ddtservicequeryhook_TagOtherBtn,false);
      SwitchTabStatus(ddtservicequeryhook_TagAllBtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag5ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag4ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag3ABtn,true);
    }
    break;
  case ddtservicequeryhook_Tag4ABtn:
    {
      m_tag4ABtnCtrl.MouseUp();
      SwitchTabStatus(ddtservicequeryhook_TagOtherBtn,false);
      SwitchTabStatus(ddtservicequeryhook_TagAllBtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag5ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag4ABtn,true);
      SwitchTabStatus(ddtservicequeryhook_Tag3ABtn,false);
    }
    break;
  case ddtservicequeryhook_Tag5ABtn:
    {
      m_tag5ABtnCtrl.MouseUp();
      SwitchTabStatus(ddtservicequeryhook_TagOtherBtn,false);
      SwitchTabStatus(ddtservicequeryhook_TagAllBtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag5ABtn,true);
      SwitchTabStatus(ddtservicequeryhook_Tag4ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag3ABtn,false);
    }
    break;
  case ddtservicequeryhook_TagAllBtn:
    {
      m_tagAllBtnCtrl.MouseUp();
      SwitchTabStatus(ddtservicequeryhook_TagOtherBtn,false);
      SwitchTabStatus(ddtservicequeryhook_TagAllBtn,true);
      SwitchTabStatus(ddtservicequeryhook_Tag5ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag4ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag3ABtn,false);
    }
    break;
  case ddtservicequeryhook_TagOtherBtn:
    {
      m_tagOtherBtnCtrl.MouseUp();
      SwitchTabStatus(ddtservicequeryhook_TagOtherBtn,true);
      SwitchTabStatus(ddtservicequeryhook_TagAllBtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag5ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag4ABtn,false);
      SwitchTabStatus(ddtservicequeryhook_Tag3ABtn,false);
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
void CDdtServiceQueryHook::Load()
{
  ClearAllRow();
  m_pageUpBtnCtrl.SetEnable(false);
  m_pageDownBtnCtrl.SetEnable(false);
  m_currentPageCtrl.ClearCaption();
  m_totalPageCtrl.ClearCaption();

  const char* distName = NULL;
  distName = CQueryWrapper::Get().GetQueryAdmName();
  m_distSwitchBtnCtrl.SetCaption(distName);
  for (int i= 0; i != m_vecListItem4.size(); ++i)
  {    
    if (!::strcmp(distName, m_vecListItem4[i].m_chName))
    {
      m_code = m_vecListItem4[i].m_uCode;
      break;
    }
  }

  CViewspotMgr::ReadFile(m_code);

  SwitchTabStatus(ddtservicequeryhook_TagAllBtn, true);
  if (m_dataList.size())
  {
    SetPageControl();
    SwitchPage();
    SetPageInfo();
    CMessageDialogEvent dialogEvent(NULL, CViewHook::DHT_DdtServiceQueryHook, NULL);
    CMessageDialogHook::ShowMessageDialog(MB_NONE, "数据加载中，请稍候...", dialogEvent); 
    CMessageDialogHook::CloseMessageDialog();
    ShowData();
  }
}
void CDdtServiceQueryHook::SwitchTabStatus(ddtservicequeryhookCtrlType tag, bool status)
{
  CViewHook::GuiElement* element = NULL;
  switch(tag)
  {
  case ddtservicequeryhook_TagAllBtn:
    {
      element = GetGuiElement(ddtservicequeryhook_TagAllBtn);
    }
    break;
  case ddtservicequeryhook_Tag5ABtn:
    {
      element = GetGuiElement(ddtservicequeryhook_Tag5ABtn);
    }
    break;
  case ddtservicequeryhook_Tag4ABtn:
    {
      element = GetGuiElement(ddtservicequeryhook_Tag4ABtn);
    }
    break;
  case ddtservicequeryhook_Tag3ABtn:
    {
      element = GetGuiElement(ddtservicequeryhook_Tag3ABtn);
    }
    break;
  case ddtservicequeryhook_TagOtherBtn:
    {
      element = GetGuiElement(ddtservicequeryhook_TagOtherBtn);
    }
    break;
  default:
    assert(false);
    break;
  }
  if (element)
  {
    if (status)
    {
      element->m_backStyle = element->m_bkDisabled;
      element->m_textStyle = element->m_disabledTextStype;
    }
    else
    {
      element->m_backStyle = element->m_bkNormal;
      element->m_textStyle = element->m_normalTextStylpe;
    }
  }
}
void CDdtServiceQueryHook::ClearAllRow()
{
  //ClearData();
  CleanRow(kROW1);
  CleanRow(kROW2);
  CleanRow(kROW3);
  CleanRow(kROW4);
  SetRowEnabel(kROW1,false);
  SetRowEnabel(kROW2,false);
  SetRowEnabel(kROW3,false);
  SetRowEnabel(kROW4,false);
}
void CDdtServiceQueryHook::CleanRow(RowTag row)
{
  switch(row)
  {
  case kROW1:
    {
      m_list1BtnCtrl.ClearCaption();
      m_name1UpCtrl.ClearCaption();
      m_address1DownCtrl.ClearCaption();
      m_phone1Ctrl.ClearCaption();
    }
    break;
  case kROW2:
    {
      m_list2BtnCtrl.ClearCaption();
      m_name2UpCtrl.ClearCaption();
      m_address2DownCtrl.ClearCaption();
      m_phone2Ctrl.ClearCaption();
    }
    break;
  case kROW3:
    {
      m_list3BtnCtrl.ClearCaption();
      m_name3UpCtrl.ClearCaption();
      m_address3DownCtrl.ClearCaption();
      m_phone3Ctrl.ClearCaption();
    }
    break;
  case kROW4:
    {
      m_list4BtnCtrl.ClearCaption();
      m_name4UpCtrl.ClearCaption();
      m_address4DownCtrl.ClearCaption();
      m_phone4Ctrl.ClearCaption();
    }
    break;
  default:
    assert(false);
    break;
  }
}
void CDdtServiceQueryHook::SetRowEnabel(RowTag row, bool enable)
{
  switch(row)
  {
  case kROW1:
    {
      m_list1BtnCtrl.SetEnable(enable);
      m_name1UpCtrl.SetEnable(enable);
      m_address1DownCtrl.SetEnable(enable);
      m_phone1Ctrl.SetEnable(enable);
      m_search1Ctrl.SetVisible(enable);
    }
    break;
  case kROW2:
    {
      m_list2BtnCtrl.SetEnable(enable);
      m_name2UpCtrl.SetEnable(enable);
      m_address2DownCtrl.SetEnable(enable);
      m_phone2Ctrl.SetEnable(enable);
      m_search2Ctrl.SetVisible(enable);
    }
    break;
  case kROW3:
    {
      m_list3BtnCtrl.SetEnable(enable);
      m_name3UpCtrl.SetEnable(enable);
      m_address3DownCtrl.SetEnable(enable);
      m_phone3Ctrl.SetEnable(enable);
      m_search3Ctrl.SetVisible(enable);
    }
    break;
  case kROW4:
    {
      m_list4BtnCtrl.SetEnable(enable);
      m_name4UpCtrl.SetEnable(enable);
      m_address4DownCtrl.SetEnable(enable);
      m_phone4Ctrl.SetEnable(enable);  
      m_search4Ctrl.SetVisible(enable);
    }
    break;
  default:
    assert(false);
    break;
  }
}
void CDdtServiceQueryHook::SwitchPage()
{
  if (m_pageTurning.IsFirstPage())
  {
    m_pageUpBtnCtrl.SetEnable(false);
  }
  else
  {
    m_pageUpBtnCtrl.SetEnable(true);
  }
  if (m_pageTurning.IsLastPage())
  {
    m_pageDownBtnCtrl.SetEnable(false);
  }
  else
  {
    m_pageDownBtnCtrl.SetEnable(true);
  }
}
void CDdtServiceQueryHook::SetPageInfo()
{ 
  char current[10] = {};
  ::sprintf(current, "%d", m_pageTurning.GetCurrentPage());
  char total[10] = {};
  ::sprintf(total, "%d", m_pageTurning.GetTotalPage());
  m_currentPageCtrl.SetCaption(current);
  m_totalPageCtrl.SetCaption(total);
}
void CDdtServiceQueryHook::SetPageControl()
{
  m_pageTurning.SetTotal(m_dataList.size());
  m_pageTurning.SetQuantityOfOnePage(kROWEnd-1); 
}

void CDdtServiceQueryHook::DoDistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  CDdtServiceQueryHook* serviceHook = (CDdtServiceQueryHook*)pDoCallBackObj;
  serviceHook->m_distSwitchBtnCtrl.SetCaption(pResult->m_asciiName);
  unsigned int addrCode = serviceHook->GetQueryAreaCode();
  if (addrCode != pResult->m_addrCode)
  {
    serviceHook->SetQueryAreaCode(pResult->m_addrCode);
    CViewspotMgr::ReadFile(pResult->m_addrCode);
    TCodeEntry codeEntry;
    codeEntry.m_uCode = pResult->m_addrCode;
    ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
    CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  }
  CViewHook::m_curHookType = CViewHook::DHT_DdtServiceQueryHook;
}

void CDdtServiceQueryHook::AddServiceData(const char* value1, const char* value2, CGeoPoint<long> point)
{
  ServiceInfo data;
  if (value1)
  {
    strcpy(data.m_value1, value1);
  }
  else
  {
    memset(data.m_value1, 0, sizeof(data.m_value1));
  }
  if (value2)
  {
    strcpy(data.m_value2, value2);
  }  
  else
  {
    memset(data.m_value2, 0, sizeof(data.m_value2));
  }
  data.m_point = point;
  m_dataList.push_back(data);
}
void CDdtServiceQueryHook::ClearData()
{
  m_dataList.clear();
  CViewspotMgr::ReleaseData();
}
void CDdtServiceQueryHook::ShowData()
{
  ClearAllRow();  
  ShowRowData(kROW1);
  ShowRowData(kROW2);
  ShowRowData(kROW3);
  ShowRowData(kROW4);
}
void CDdtServiceQueryHook::ShowRowData(RowTag row)
{
  int n = m_pageTurning.GetCurrentPage();
  if (((n-1)*(kROWEnd-1)+row-1) < m_dataList.size())
  {
    switch(row)
    {
    case kROW1:
      {
        SetRowEnabel(kROW1,true);
        m_name1UpCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW1-1].m_value1);
        m_address1DownCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW1-1].m_value2);
        m_phone1Ctrl.SetCaption("暂无电话");
      }
      break;
    case kROW2:
      {
        SetRowEnabel(kROW2,true);
        m_name2UpCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW2-1].m_value1);
        m_address2DownCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW2-1].m_value2);
        m_phone2Ctrl.SetCaption("暂无电话");
      }
      break;
    case kROW3:
      {
        SetRowEnabel(kROW3,true);
        m_name3UpCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW3-1].m_value1);
        m_address3DownCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW3-1].m_value2);
        m_phone3Ctrl.SetCaption("暂无电话");
      }
      break;
    case kROW4:
      {
        SetRowEnabel(kROW4,true);
        m_name4UpCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW4-1].m_value1);
        m_address4DownCtrl.SetCaption(m_dataList[(n-1)*(kROWEnd-1)+ kROW4-1].m_value2);
        m_phone4Ctrl.SetCaption("暂无电话");
      }
      break;
    default:
      assert(false);
      break;
    }
  } 
}

void CDdtServiceQueryHook::SelectRecord(RowTag row)
{
  CMapHook* mapHook = dynamic_cast<CMapHook*>(m_view->GetHook(CViewHook::DHT_MapHook));
  if (mapHook) 
  {
    int dataIndex = row - 1;
    if (dataIndex < 0)
    {
      return;
    }   
    PointList resultList;
    PointInfo pointInfo;
    int startPos = m_pageTurning.GetPageStartPosition() - 1;
    int endPos = m_pageTurning.GetPageEndPosition() - 1;
    for (int i = startPos; i <= endPos; ++i)
    {  
      const CGeoPoint<long> point = m_dataList[i].m_point;
      char* name = reinterpret_cast<char*>(m_dataList[i].m_value1);  
      pointInfo.m_point = point; 
      ::strcpy(pointInfo.m_name, name);
      resultList.push_back(pointInfo);
    }  
    TurnTo(DHT_MapHook);   
    mapHook->SetPickPos(resultList, dataIndex);        
  }     
}
void CDdtServiceQueryHook::DistSwitchCallBack(void *pDoCallBackObj,const SQLRecord *pResult)
{
  if (pDoCallBackObj)
  {
    ((CDdtServiceQueryHook *)pDoCallBackObj)->DoDistSwitchCallBack(pResult);
  }
}

void CDdtServiceQueryHook::DoDistSwitchCallBack(const SQLRecord *pResult)
{
  TCodeEntry codeEntry;
  codeEntry.m_uCode = pResult->m_addrCode;
  ::strcpy(codeEntry.m_chName, pResult->m_asciiName);
  CQueryWrapper::Get().SetQueryAdmInfo(codeEntry);
  m_distSwitchBtnCtrl.SetCaption(codeEntry.m_chName);
}