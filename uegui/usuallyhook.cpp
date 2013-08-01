#include "usuallyhook.h"
#include "usuallyfile.h"
#include "maphook.h"
#include "messagedialoghook.h"
using namespace UeGui;

CUsuallyHook::CUsuallyHook()
{
  m_strTitle = "常用点";
  m_vecHookFile.push_back(_T("usuallyhook.bin"));
  m_usuallyFile = new CUsuallyFile();
}

CUsuallyHook::~CUsuallyHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
  m_usuallRecords.clear();
  if (m_usuallyFile)
  {
    delete m_usuallyFile;
    m_usuallyFile = NULL;
  }
}


void CUsuallyHook::MakeNames()
{
  CMenuBackgroundHook::MakeNames();
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_HomeBtn,	"HomeBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_HomeBtnIcon,	"HomeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_HomeName,	"HomeName"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteHomeBtn,	"DeleteHomeBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteHomeBtnIcon,	"DeleteHomeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_WorkBtn,	"WorkBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_WorkBtnIcon,	"WorkBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_WorkName,	"WorkName"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteWorkBtn,	"DeleteWorkBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteWorkBtnIcon,	"DeleteWorkBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit1Btn,	"Favourit1Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit1BtnIcon,	"Favourit1BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit1Name,	"Favourit1Name"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteFavourit1Btn,	"DeleteFavourit1Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteFavourit1BtnIcon,	"DeleteFavourit1BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit2Btn,	"Favourit2Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit2BtnIcon,	"Favourit2BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit2Name,	"Favourit2Name"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteFavourit2Btn,	"DeleteFavourit2Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteFavourit2BtnIcon,	"DeleteFavourit2BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit3Btn,	"Favourit3Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit3Name,	"Favourit3Name"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Favourit3BtnIcon,	"Favourit3BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteFavourit3Btn,	"DeleteFavourit3Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_DeleteFavourit3BtnIcon,	"DeleteFavourit3BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_BottomBack,	"BottomBack"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_ClearCenter,	"ClearCenter"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditHomeBtn,	"EditHomeBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditHomeBtnIcon,	"EditHomeBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditWorkBtn,	"EditWorkBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditWorkBtnIcon,	"EditWorkBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditFavourit1Btn,	"EditFavourit1Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditFavourit1BtnIcon,	"EditFavourit1BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditFavourit2Btn,	"EditFavourit2Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditFavourit2BtnIcon,	"EditFavourit2BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditFavourit3Btn,	"EditFavourit3Btn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_EditFavourit3BtnIcon,	"EditFavourit3BtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Split1,	"Split1"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Split2,	"Split2"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Split3,	"Split3"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Split4,	"Split4"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Split5,	"Split5"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Split6,	"Split6"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_Row6Center,	"Row6Center"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_UnuseBtn,	"UnuseBtn"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_UnuseBtnIcon,	"UnuseBtnIcon"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_UnuseBtn1,	"UnuseBtn1"));
  m_ctrlNames.insert(GuiName::value_type(usuallyhook_UnuseBtn1Icon,	"UnuseBtn1Icon"));
}

void CUsuallyHook::MakeControls()
{
  CMenuBackgroundHook::MakeControls();
  m_clearCenterCtrl.SetCenterElement(GetGuiElement(usuallyhook_ClearCenter));
  m_deleteFavourit1BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_DeleteFavourit1Btn));
  m_deleteFavourit1BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_DeleteFavourit1BtnIcon));
  m_deleteFavourit2BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_DeleteFavourit2Btn));
  m_deleteFavourit2BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_DeleteFavourit2BtnIcon));
  m_deleteFavourit3BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_DeleteFavourit3Btn));
  m_deleteFavourit3BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_DeleteFavourit3BtnIcon));
  m_deleteHomeBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_DeleteHomeBtn));
  m_deleteHomeBtnCtrl.SetIconElement(GetGuiElement(usuallyhook_DeleteHomeBtnIcon));
  m_deleteWorkBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_DeleteWorkBtn));
  m_deleteWorkBtnCtrl.SetIconElement(GetGuiElement(usuallyhook_DeleteWorkBtnIcon));
  m_editFavourit1BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_EditFavourit1Btn));
  m_editFavourit1BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_EditFavourit1BtnIcon));
  m_editFavourit2BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_EditFavourit2Btn));
  m_editFavourit2BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_EditFavourit2BtnIcon));
  m_editFavourit3BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_EditFavourit3Btn));
  m_editFavourit3BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_EditFavourit3BtnIcon));
  m_editHomeBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_EditHomeBtn));
  m_editHomeBtnCtrl.SetIconElement(GetGuiElement(usuallyhook_EditHomeBtnIcon));
  m_editWorkBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_EditWorkBtn));
  m_editWorkBtnCtrl.SetIconElement(GetGuiElement(usuallyhook_EditWorkBtnIcon));
  m_favourit1BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_Favourit1Btn));
  m_favourit1BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_Favourit1BtnIcon));
  m_favourit1NameCtrl.SetLabelElement(GetGuiElement(usuallyhook_Favourit1Name));
  m_favourit2BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_Favourit2Btn));
  m_favourit2BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_Favourit2BtnIcon));
  m_favourit2NameCtrl.SetLabelElement(GetGuiElement(usuallyhook_Favourit2Name));
  m_favourit3BtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_Favourit3Btn));
  m_favourit3BtnCtrl.SetIconElement(GetGuiElement(usuallyhook_Favourit3BtnIcon));
  m_favourit3NameCtrl.SetLabelElement(GetGuiElement(usuallyhook_Favourit3Name));
  m_homeBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_HomeBtn));
  m_homeBtnCtrl.SetIconElement(GetGuiElement(usuallyhook_HomeBtnIcon));
  m_homeNameCtrl.SetLabelElement(GetGuiElement(usuallyhook_HomeName));
  m_row6CenterCtrl.SetCenterElement(GetGuiElement(usuallyhook_Row6Center));
  m_unuseBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_UnuseBtn));
  m_unuseBtn1Ctrl.SetCenterElement(GetGuiElement(usuallyhook_UnuseBtn1));
  m_unuseBtn1Ctrl.SetIconElement(GetGuiElement(usuallyhook_UnuseBtn1Icon));
  m_unuseBtnIconCtrl.SetCenterElement(GetGuiElement(usuallyhook_UnuseBtnIcon));
  m_workBtnCtrl.SetCenterElement(GetGuiElement(usuallyhook_WorkBtn));
  m_workBtnCtrl.SetIconElement(GetGuiElement(usuallyhook_WorkBtnIcon));
  m_workNameCtrl.SetLabelElement(GetGuiElement(usuallyhook_WorkName));
}

void CUsuallyHook::Load()
{
  RefreshData();
}

short CUsuallyHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case usuallyhook_DeleteFavourit1Btn:
  case usuallyhook_DeleteFavourit1BtnIcon:
    {
      m_deleteFavourit1BtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_DeleteFavourit2Btn:
  case usuallyhook_DeleteFavourit2BtnIcon:
    {
      m_deleteFavourit2BtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_DeleteFavourit3Btn:
  case usuallyhook_DeleteFavourit3BtnIcon:
    {
      m_deleteFavourit3BtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_DeleteHomeBtn:
  case usuallyhook_DeleteHomeBtnIcon:
    {
      m_deleteHomeBtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_DeleteWorkBtn:
  case usuallyhook_DeleteWorkBtnIcon:
    {
      m_deleteWorkBtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_EditFavourit1Btn:
  case usuallyhook_EditFavourit1BtnIcon:
    {
      m_editFavourit1BtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_EditFavourit2Btn:
  case usuallyhook_EditFavourit2BtnIcon:
    {
      m_editFavourit2BtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_EditFavourit3Btn:
  case usuallyhook_EditFavourit3BtnIcon:
    {
      m_editFavourit3BtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_EditHomeBtn:
  case usuallyhook_EditHomeBtnIcon:
    {
      m_editHomeBtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_EditWorkBtn:
  case usuallyhook_EditWorkBtnIcon:
    {
      m_editWorkBtnCtrl.MouseDown();
    }
    break;
  case usuallyhook_Favourit1Btn:
  case usuallyhook_Favourit1BtnIcon:
  case usuallyhook_Favourit1Name:
    {
      m_favourit1BtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_favourit1BtnCtrl, m_favourit1NameCtrl);
      MOUSEDONW_2RENDERCTRL(m_editFavourit1BtnCtrl, m_deleteFavourit1BtnCtrl);
    }
    break;
  case usuallyhook_Favourit2Btn:
  case usuallyhook_Favourit2BtnIcon:
  case usuallyhook_Favourit2Name:
    {
      m_favourit2BtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_favourit2BtnCtrl, m_favourit2NameCtrl);
      MOUSEDONW_2RENDERCTRL(m_editFavourit2BtnCtrl, m_deleteFavourit2BtnCtrl);
    }
    break;
  case usuallyhook_Favourit3Btn:
  case usuallyhook_Favourit3BtnIcon:
  case usuallyhook_Favourit3Name:
    {
      m_favourit3BtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_favourit3BtnCtrl, m_favourit3NameCtrl);
      MOUSEDONW_2RENDERCTRL(m_editFavourit3BtnCtrl, m_deleteFavourit3BtnCtrl);
    }
    break;
  case usuallyhook_HomeBtn:
  case usuallyhook_HomeBtnIcon:
  case usuallyhook_HomeName:
    {
      m_homeBtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_homeBtnCtrl, m_homeNameCtrl);
      MOUSEDONW_2RENDERCTRL(m_editHomeBtnCtrl, m_deleteHomeBtnCtrl);
    }
    break;
  case usuallyhook_WorkBtn:
  case usuallyhook_WorkBtnIcon:
  case usuallyhook_WorkName:
    {
      m_workBtnCtrl.MouseDown();
      MOUSEDONW_2RENDERCTRL(m_workBtnCtrl, m_workNameCtrl);
      MOUSEDONW_2RENDERCTRL(m_editWorkBtnCtrl, m_deleteWorkBtnCtrl);
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

short CUsuallyHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CUsuallyHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case usuallyhook_DeleteFavourit1Btn:
  case usuallyhook_DeleteFavourit1BtnIcon:
    {
      if (IsNeedRefresh(m_deleteFavourit1BtnCtrl))
      {
        m_deleteFavourit1BtnCtrl.MouseUp();
        OnClickDeleteBtn(kROW3);
      }
    }
    break;
  case usuallyhook_DeleteFavourit2Btn:
  case usuallyhook_DeleteFavourit2BtnIcon:
    {
      if (IsNeedRefresh(m_deleteFavourit2BtnCtrl))
      {
        m_deleteFavourit2BtnCtrl.MouseUp();
        OnClickDeleteBtn(kROW4);
      }
    }
    break;
  case usuallyhook_DeleteFavourit3Btn:
  case usuallyhook_DeleteFavourit3BtnIcon:
    {
      if (IsNeedRefresh(m_deleteFavourit3BtnCtrl))
      {
        m_deleteFavourit3BtnCtrl.MouseUp();
        OnClickDeleteBtn(kROW5);
      }      
    }
    break;
  case usuallyhook_DeleteHomeBtn:
  case usuallyhook_DeleteHomeBtnIcon:
    {
      if (IsNeedRefresh(m_deleteHomeBtnCtrl))
      {
        m_deleteHomeBtnCtrl.MouseUp();
        OnClickDeleteBtn(kROW1);
      }      
    }
    break;
  case usuallyhook_DeleteWorkBtn:
  case usuallyhook_DeleteWorkBtnIcon:
    {
      if (IsNeedRefresh(m_deleteWorkBtnCtrl))
      {
        m_deleteWorkBtnCtrl.MouseUp();
        OnClickDeleteBtn(kROW2);
      }
    }
    break;
  case usuallyhook_EditFavourit1Btn:
  case usuallyhook_EditFavourit1BtnIcon:
    {
      m_editFavourit1BtnCtrl.MouseUp();
    }
    break;
  case usuallyhook_EditFavourit2Btn:
  case usuallyhook_EditFavourit2BtnIcon:
    {
      m_editFavourit2BtnCtrl.MouseUp();
    }
    break;
  case usuallyhook_EditFavourit3Btn:
  case usuallyhook_EditFavourit3BtnIcon:
    {
      m_editFavourit3BtnCtrl.MouseUp();
    }
    break;
  case usuallyhook_EditHomeBtn:
  case usuallyhook_EditHomeBtnIcon:
    {
      m_editHomeBtnCtrl.MouseUp();
    }
    break;
  case usuallyhook_EditWorkBtn:
  case usuallyhook_EditWorkBtnIcon:
    {
      m_editWorkBtnCtrl.MouseUp();
    }
    break;
  case usuallyhook_Favourit1Btn:
  case usuallyhook_Favourit1BtnIcon:
  case usuallyhook_Favourit1Name:
    {
      if (m_favourit1BtnCtrl.IsEnable())
      {
        SelectRow(kROW3);
        m_favourit1BtnCtrl.MouseUp();
      }
      m_isNeedRefesh = false;
    }
    break;
  case usuallyhook_Favourit2Btn:
  case usuallyhook_Favourit2BtnIcon:
  case usuallyhook_Favourit2Name:
    {
      if (m_favourit2BtnCtrl.IsEnable())
      {
        SelectRow(kROW4);
        m_favourit2BtnCtrl.MouseUp();
      }
      m_isNeedRefesh = false;
    }
    break;
  case usuallyhook_Favourit3Btn:
  case usuallyhook_Favourit3BtnIcon:
  case usuallyhook_Favourit3Name:
    {
      if (m_favourit3BtnCtrl.IsEnable())
      {
        SelectRow(kROW5);
        m_favourit3BtnCtrl.MouseUp();
      }
      m_isNeedRefesh = false;
    }
    break;
  case usuallyhook_HomeBtn:
  case usuallyhook_HomeBtnIcon:
  case usuallyhook_HomeName:
    {
      if (m_homeBtnCtrl.IsEnable())
      {
        SelectRow(kROW1);
        m_homeBtnCtrl.MouseUp();
      }
      m_isNeedRefesh = false;
    }
    break;
  case usuallyhook_WorkBtn:
  case usuallyhook_WorkBtnIcon:
  case usuallyhook_WorkName:
    {
      if (m_workBtnCtrl.IsEnable())
      {
        SelectRow(kROW2);
        m_workBtnCtrl.MouseUp();
      }
      m_isNeedRefesh = false;
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


void CUsuallyHook::PositionTextSetting(const char* prefix , const char* content ,CUiBitButton* const pbutton,
                                               CUiLabel* const plabel,CUiBitButton* const pdelete)
{
  if(prefix!=NULL && content!=NULL && pbutton!=NULL && plabel!=NULL && pdelete!=NULL)
  {
    if(*content=='\0')
    {
      plabel->SetCaption(prefix);
      plabel->SetEnable(false);
      pbutton->SetEnable(false);
      pdelete->SetEnable(false);
    }
    else
    {
      plabel->SetCaption(prefix);
      ::strcat(plabel->GetLabelElement()->m_label,"(");
      ::strcat(plabel->GetLabelElement()->m_label,content);
      ::strcat(plabel->GetLabelElement()->m_label,")"); 
      plabel->SetEnable(true);
      pbutton->SetEnable(true);
      pdelete->SetEnable(true);
    }
  }
  else
  {
    assert(false);
  }
}

void CUsuallyHook::RefreshData()
{
  m_usuallRecords.clear();
  UsuallyRecord fRecord; 
  if(m_usuallyFile && m_usuallyFile->GetRecordCount()==5)
  {
    m_usuallyFile->GetRecord(1,&fRecord);
    m_usuallRecords.push_back(fRecord);
    PositionTextSetting("家",(char *)fRecord.m_name,&m_homeBtnCtrl, &m_homeNameCtrl, &m_deleteHomeBtnCtrl);

    m_usuallyFile->GetRecord(2,&fRecord);
    m_usuallRecords.push_back(fRecord);
    PositionTextSetting("单位",(char *)fRecord.m_name,&m_workBtnCtrl, &m_workNameCtrl, &m_deleteWorkBtnCtrl);

    m_usuallyFile->GetRecord(3,&fRecord);
    m_usuallRecords.push_back(fRecord);
    PositionTextSetting("常用点1",(char *)fRecord.m_name, &m_favourit1BtnCtrl, &m_favourit1NameCtrl, &m_deleteFavourit1BtnCtrl);

    m_usuallyFile->GetRecord(4,&fRecord);
    m_usuallRecords.push_back(fRecord);
    PositionTextSetting("常用点2",(char *)fRecord.m_name, &m_favourit2BtnCtrl, &m_favourit2NameCtrl, &m_deleteFavourit2BtnCtrl);

    m_usuallyFile->GetRecord(5,&fRecord);
    m_usuallRecords.push_back(fRecord);
    PositionTextSetting("常用点3",(char *)fRecord.m_name, &m_favourit3BtnCtrl, &m_favourit3NameCtrl, &m_deleteFavourit3BtnCtrl);
  }
}

PointList CUsuallyHook::GetPointList()
{
  PointList resultList;
  PointInfo pointInfo;

  for (unsigned int i = 0; i < m_usuallRecords.size(); ++i)
  {
    if (m_usuallRecords[i].IsValid())
    {
      pointInfo.m_point.m_x = m_usuallRecords[i].m_x;
      pointInfo.m_point.m_y = m_usuallRecords[i].m_y;
      ::strcpy(pointInfo.m_name, (char*)m_usuallRecords[i].m_name);
      resultList.push_back(pointInfo);
    }
  }
  return resultList;
}

int CUsuallyHook::RowToValidIndexInList(unsigned int row)
{
  int validIndex = -1;
  for (unsigned int i = 0; i < row; ++i)
  {
    if (m_usuallRecords[i].IsValid())
    {
      validIndex++;
    }
  }
  return validIndex;
}

void CUsuallyHook::SelectRow(unsigned int row)
{
  if (row >= kROWBegin && row < kROWEnd)
  {
    CMapHook* hook = (CMapHook*)m_view->GetHook(DHT_MapHook);
    if (hook)
    {
      TurnTo(DHT_MapHook);
      hook->SetPickPos(GetPointList(), RowToValidIndexInList(row));
      if (m_view)
      {
        m_view->Refresh();
      }
    }
  }
}

void CUsuallyHook::OnClickDeleteBtn(unsigned int row)
{
  m_deleteRowNum = row;
  CMessageDialogEvent deleteUsuallyEvt(this, DHT_UsuallyHook, &UeGui::CUsuallyHook::DealDeleteUsuallyEvent);
  CMessageDialogHook::ShowMessageDialog(MB_WARNING, "所选记录将被删除！", deleteUsuallyEvt);
}

void CUsuallyHook::DealDeleteUsuallyEvent(CAggHook *sender, ModalResultType modalResult)
{
  if (modalResult == MR_OK && sender)
  {
    CUsuallyHook* hook = dynamic_cast<CUsuallyHook*>(sender);
    if (hook)
    {
      hook->DeleteUsually();
    }
  }
}

void CUsuallyHook::DeleteUsually()
{
  m_usuallyFile->RemoveRecord(m_deleteRowNum);
  RefreshData();
}