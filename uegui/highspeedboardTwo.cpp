#include "highspeedboardtwo.h"
using namespace UeGui;

ChighspeedboardTwo::ChighspeedboardTwo()
{
  MakeGUI();
}

ChighspeedboardTwo::~ChighspeedboardTwo()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

void ChighspeedboardTwo::MakeGUI()
{
  FetchWithBinary();
  MakeNames();
  MakeControls();
}

tstring ChighspeedboardTwo::GetBinaryFileName()
{
  return _T("highspeedboardtwo.bin");
}

void ChighspeedboardTwo::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(highspeedboardTwo_boardtwo_background,	"boardtwo_background"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardTwo_boardtwo_line,	"boardtwo_line"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardTwo_boardtwo_roadname,	"boardtwo_roadname"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardTwo_boardtwo_roadexitIdx,	"boardtwo_roadexitIdx"));
  m_ctrlNames.insert(GuiName::value_type(highspeedboardTwo_boardtwo_roaddist,	"boardtwo_roaddist"));
}

void ChighspeedboardTwo::MakeControls()
{
  m_boardtwo_backgroundCtrl.SetCenterElement(GetGuiElement(highspeedboardTwo_boardtwo_background));
  m_boardtwo_lineCtrl.SetCenterElement(GetGuiElement(highspeedboardTwo_boardtwo_line));
  m_boardtwo_roaddistCtrl.SetCenterElement(GetGuiElement(highspeedboardTwo_boardtwo_roaddist));
  m_boardtwo_roadexitIdxCtrl.SetCenterElement(GetGuiElement(highspeedboardTwo_boardtwo_roadexitIdx));
  m_boardtwo_roadnameCtrl.SetCenterElement(GetGuiElement(highspeedboardTwo_boardtwo_roadname));
}

short ChighspeedboardTwo::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case highspeedboardTwo_boardtwo_background:
    {
      m_boardtwo_backgroundCtrl.MouseDown();
    }
    break;
  case highspeedboardTwo_boardtwo_line:
    {
      m_boardtwo_lineCtrl.MouseDown();
    }
    break;
  case highspeedboardTwo_boardtwo_roaddist:
    {
      m_boardtwo_roaddistCtrl.MouseDown();
    }
    break;
  case highspeedboardTwo_boardtwo_roadexitIdx:
    {
      m_boardtwo_roadexitIdxCtrl.MouseDown();
    }
    break;
  case highspeedboardTwo_boardtwo_roadname:
    {
      m_boardtwo_roadnameCtrl.MouseDown();
    }
    break;
  default:
    assert(false);
    break;
  }

  //this->Refresh();
  return ctrlType;
}

short ChighspeedboardTwo::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short ChighspeedboardTwo::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case highspeedboardTwo_boardtwo_background:
    {
      m_boardtwo_backgroundCtrl.MouseUp();
    }
    break;
  case highspeedboardTwo_boardtwo_line:
    {
      m_boardtwo_lineCtrl.MouseUp();
    }
    break;
  case highspeedboardTwo_boardtwo_roaddist:
    {
      m_boardtwo_roaddistCtrl.MouseUp();
    }
    break;
  case highspeedboardTwo_boardtwo_roadexitIdx:
    {
      m_boardtwo_roadexitIdxCtrl.MouseUp();
    }
    break;
  case highspeedboardTwo_boardtwo_roadname:
    {
      m_boardtwo_roadnameCtrl.MouseUp();
    }
    break;
  default:
    assert(false);
    break;
  }

  //this->Refresh();
  return ctrlType;
}

bool ChighspeedboardTwo::operator ()()
{
  return false;
}

void ChighspeedboardTwo::ShowHighSpeedInfo(char *roaddist,char* roadname,int roadexitId)
{
  m_boardtwo_roadnameCtrl.SetCaption(roadname);
  m_boardtwo_roaddistCtrl.SetCaption(roaddist);
  if(-1 != roadexitId)
  {
    char roadexitText[128] = {};
    ::sprintf(roadexitText,"µÚ%d³ö¿Ú",roadexitId);
    m_boardtwo_roadexitIdxCtrl.SetCaption(roadexitText);
    m_boardtwo_roadexitIdxCtrl.SetVisible(true);
  }
  else 
  {
    m_boardtwo_roadexitIdxCtrl.SetVisible(false);
  }
}