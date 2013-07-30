#include "highspeedboardthree.h"
using namespace UeGui;

ChighspeedboardThree::ChighspeedboardThree()
{
  MakeGUI();
}

ChighspeedboardThree::~ChighspeedboardThree()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void ChighspeedboardThree::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring ChighspeedboardThree::GetBinaryFileName()
{
  return _T("highspeedboardthree.bin");
}

void ChighspeedboardThree::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(highspeedboardThree_boardthree_background,	"boardthree_background"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardThree_boardthree_line,	"boardthree_line"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardThree_boardthree_roadname,	"boardthree_roadname"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardThree_boardthree_roadexitIdx,	"boardthree_roadexitIdx"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardThree_boardthree_roaddist,	"boardthree_roaddist"));
}

void ChighspeedboardThree::MakeControls()
{
  m_boardthree_backgroundCtrl.SetCenterElement(GetGuiElement(highspeedboardThree_boardthree_background));
  m_boardthree_lineCtrl.SetCenterElement(GetGuiElement(highspeedboardThree_boardthree_line));
  m_boardthree_roaddistCtrl.SetCenterElement(GetGuiElement(highspeedboardThree_boardthree_roaddist));
  m_boardthree_roadexitIdxCtrl.SetCenterElement(GetGuiElement(highspeedboardThree_boardthree_roadexitIdx));
  m_boardthree_roadnameCtrl.SetCenterElement(GetGuiElement(highspeedboardThree_boardthree_roadname));
}

short ChighspeedboardThree::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case highspeedboardThree_boardthree_background:
    {
      m_boardthree_backgroundCtrl.MouseDown();
    }
    break;
  case highspeedboardThree_boardthree_line:
    {
      m_boardthree_lineCtrl.MouseDown();
    }
    break;
  case highspeedboardThree_boardthree_roaddist:
    {
      m_boardthree_roaddistCtrl.MouseDown();
    }
    break;
  case highspeedboardThree_boardthree_roadexitIdx:
    {
      m_boardthree_roadexitIdxCtrl.MouseDown();
    }
    break;
  case highspeedboardThree_boardthree_roadname:
    {
      m_boardthree_roadnameCtrl.MouseDown();
    }
    break;
  default:
    assert(false);
    break;
  }

  //this->Refresh();
  return ctrlType;
}

short ChighspeedboardThree::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short ChighspeedboardThree::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case highspeedboardThree_boardthree_background:
    {
      m_boardthree_backgroundCtrl.MouseUp();
    }
    break;
  case highspeedboardThree_boardthree_line:
    {
      m_boardthree_lineCtrl.MouseUp();
    }
    break;
  case highspeedboardThree_boardthree_roaddist:
    {
      m_boardthree_roaddistCtrl.MouseUp();
    }
    break;
  case highspeedboardThree_boardthree_roadexitIdx:
    {
      m_boardthree_roadexitIdxCtrl.MouseUp();
    }
    break;
  case highspeedboardThree_boardthree_roadname:
    {
      m_boardthree_roadnameCtrl.MouseUp();
    }
    break;
  default:
    assert(false);
    break;
  }

  //this->Refresh();
  return ctrlType;
}

bool ChighspeedboardThree::operator ()()
{
  return false;
}

void ChighspeedboardThree::ShowHighSpeedInfo(char *roaddist,char* roadname,int roadexitId)
{
  m_boardthree_roadnameCtrl.SetCaption(roadname);
  m_boardthree_roaddistCtrl.SetCaption(roaddist);
  if(-1 != roadexitId)
  {
    char roadexitText[128] = {};
    ::sprintf(roadexitText,"µÚ%d³ö¿Ú",roadexitId);
    m_boardthree_roadexitIdxCtrl.SetCaption(roadexitText);
    m_boardthree_roadexitIdxCtrl.SetVisible(true);
  }
  else 
  {
    m_boardthree_roadexitIdxCtrl.SetVisible(false);
  }
}
