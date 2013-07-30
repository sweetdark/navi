#include "highspeedboardone.h"
using namespace UeGui;

ChighspeedboardOne::ChighspeedboardOne()
{
  MakeGUI();
}

ChighspeedboardOne::~ChighspeedboardOne()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void ChighspeedboardOne::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring ChighspeedboardOne::GetBinaryFileName()
{
  return _T("highspeedboardone.bin");
}

void ChighspeedboardOne::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(highspeedboardOne_boardone_background,	"boardone_background"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardOne_boardone_line,	"boardone_line"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardOne_boardone_roadname,	"boardone_roadname"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardOne_boardone_roadexitIdx,	"boardone_roadexitIdx"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardOne_boardone_roaddist,	"boardone_roaddist"));
}

void ChighspeedboardOne::MakeControls()
{
  m_boardone_backgroundCtrl.SetCenterElement(GetGuiElement(highspeedboardOne_boardone_background));
  m_boardone_lineCtrl.SetCenterElement(GetGuiElement(highspeedboardOne_boardone_line));
  m_boardone_roaddistCtrl.SetCenterElement(GetGuiElement(highspeedboardOne_boardone_roaddist));
  m_boardone_roadexitIdxCtrl.SetCenterElement(GetGuiElement(highspeedboardOne_boardone_roadexitIdx));
  m_boardone_roadnameCtrl.SetCenterElement(GetGuiElement(highspeedboardOne_boardone_roadname));
}

short ChighspeedboardOne::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case highspeedboardOne_boardone_background:
    {
      m_boardone_backgroundCtrl.MouseDown();
    }
    break;
  case highspeedboardOne_boardone_line:
    {
      m_boardone_lineCtrl.MouseDown();
    }
    break;
  case highspeedboardOne_boardone_roaddist:
    {
      m_boardone_roaddistCtrl.MouseDown();
    }
    break;
  case highspeedboardOne_boardone_roadexitIdx:
    {
      m_boardone_roadexitIdxCtrl.MouseDown();
    }
    break;
  case highspeedboardOne_boardone_roadname:
    {
      m_boardone_roadnameCtrl.MouseDown();
    }
    break;
  default:
    assert(false);
    break;
  }

  //this->Refresh();
  return ctrlType;
}

short ChighspeedboardOne::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short ChighspeedboardOne::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case highspeedboardOne_boardone_background:
    {
      m_boardone_backgroundCtrl.MouseUp();
    }
    break;
  case highspeedboardOne_boardone_line:
    {
      m_boardone_lineCtrl.MouseUp();
    }
    break;
  case highspeedboardOne_boardone_roaddist:
    {
      m_boardone_roaddistCtrl.MouseUp();
    }
    break;
  case highspeedboardOne_boardone_roadexitIdx:
    {
      m_boardone_roadexitIdxCtrl.MouseUp();
    }
    break;
  case highspeedboardOne_boardone_roadname:
    {
      m_boardone_roadnameCtrl.MouseUp();
    }
    break;
  default:
    assert(false);
    break;
  }

  //this->Refresh();
  return ctrlType;
}

bool ChighspeedboardOne::operator ()()
{
  return false;
}

void ChighspeedboardOne::ShowHighSpeedInfo(char *roaddist,char* roadname,int roadexitId)
{
  m_boardone_roadnameCtrl.SetCaption(roadname);
  m_boardone_roaddistCtrl.SetCaption(roaddist);
  if(-1 != roadexitId)
  {
    char roadexitText[128] = {};
    ::sprintf(roadexitText,"µÚ%d³ö¿Ú",roadexitId);
    m_boardone_roadexitIdxCtrl.SetCaption(roadexitText);
    m_boardone_roadexitIdxCtrl.SetVisible(true);
  }
  else 
  {
    m_boardone_roadexitIdxCtrl.SetVisible(false);
  }
}

short ChighspeedboardOne::GetSelfStartX()
{
  CViewHook::GuiElement* guiElement=m_boardone_backgroundCtrl.GetCenterElement();
  return guiElement->m_startX;
}