#include "itemselecthook.h"
using namespace UeGui;

CItemSelectHook::CItemSelectHook() : m_senderHook(NULL), m_itemSelectEvent(NULL)
{
  ::memset(m_coordinates, 0, ItemSelectHook_End * sizeof(Coordinate));
}

CItemSelectHook::~CItemSelectHook()
{
  m_elements.clear();
  m_ctrlNames.clear();
  m_imageNames.clear();
}

tstring CItemSelectHook::GetBinaryFileName()
{
  return _T("itemselecthook.bin");
}

bool CItemSelectHook::operator ()()
{
  return false;
}

void CItemSelectHook::MakeNames()
{
  m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_BackGround,	"BackGround"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_PopupListTop,	"PopupListTop"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_PopupListBottom,	"PopupListBottom"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item1,	"Item1"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item1_Icon,	"Item1_Icon"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item2,	"Item2"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item2_Icon,	"Item2_Icon"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item3,	"Item3"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item3_Icon,	"Item3_Icon"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item4,	"Item4"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Item4_Icon,	"Item4_Icon"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Line1,	"Line1"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Line2,	"Line2"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Line3,	"Line3"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Position1,	"Position1"));
  m_ctrlNames.insert(GuiName::value_type(ItemSelectHook_Position2,	"Position2"));
}

void CItemSelectHook::MakeControls()
{
  m_backGround.SetCenterElement(GetGuiElement(ItemSelectHook_BackGround));
  m_item1.SetCenterElement(GetGuiElement(ItemSelectHook_Item1));
  m_item1_Icon.SetCenterElement(GetGuiElement(ItemSelectHook_Item1_Icon));
  m_item2.SetCenterElement(GetGuiElement(ItemSelectHook_Item2));
  m_item2_Icon.SetCenterElement(GetGuiElement(ItemSelectHook_Item2_Icon));
  m_item3.SetCenterElement(GetGuiElement(ItemSelectHook_Item3));
  m_item3_Icon.SetCenterElement(GetGuiElement(ItemSelectHook_Item3_Icon));
  m_item4.SetCenterElement(GetGuiElement(ItemSelectHook_Item4));
  m_item4_Icon.SetCenterElement(GetGuiElement(ItemSelectHook_Item4_Icon));
  m_item1.ClearCaption();
  m_item2.ClearCaption();
  m_item3.ClearCaption();
  m_item4.ClearCaption();
  m_item1_Icon.SetVisible(false);
  m_item2_Icon.SetVisible(false);
  m_item3_Icon.SetVisible(false);
  m_item4_Icon.SetVisible(false);

  //初始化界面坐标
  GuiElement* guielemment = NULL;
  guielemment = GetGuiElement(ItemSelectHook_Position1);
  if (guielemment)
  {
    m_dlgDefaultCoordinate1.x = guielemment->m_startX;
    m_dlgDefaultCoordinate1.y = guielemment->m_startY;
  }
  guielemment = GetGuiElement(ItemSelectHook_Position2);
  if (guielemment)
  {
    m_dlgDefaultCoordinate2.x = guielemment->m_startX;
    m_dlgDefaultCoordinate2.y = guielemment->m_startY;
  }

  for (short i = ItemSelectHook_Begin + 1; i < ItemSelectHook_End; ++i)
  {
    guielemment = GetGuiElement(i);
    if (guielemment)
    {
      m_coordinates[i].x = guielemment->m_startX;
      m_coordinates[i].y = guielemment->m_startY;
    }
  }
}

short CItemSelectHook::MouseDown(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseDown(scrPoint);
  switch(ctrlType)
  {
  case ItemSelectHook_Item1:
  case ItemSelectHook_Item1_Icon:
    {
      m_item1.MouseDown();
    }
    break;
  case ItemSelectHook_Item2:
  case ItemSelectHook_Item2_Icon:
    {
      m_item2.MouseDown();
    }
    break;
  case ItemSelectHook_Item3:
  case ItemSelectHook_Item3_Icon:
    {
      m_item3.MouseDown();
    }
    break;
  case ItemSelectHook_Item4:
  case ItemSelectHook_Item4_Icon:
    {
      m_item4.MouseDown();
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

short CItemSelectHook::MouseMove(CGeoPoint<short> &scrPoint)
{
  return -1;
}

short CItemSelectHook::MouseUp(CGeoPoint<short> &scrPoint)
{
  short ctrlType = CAggHook::MouseUp(scrPoint);
  switch(m_downElementType)
  {
  case ItemSelectHook_BackGround:
    {
      DoClose();
    }
    break;
  case ItemSelectHook_Item1:
  case ItemSelectHook_Item1_Icon:
    {
      m_item1.MouseUp();
      if (m_item1.IsEnable())
      {
        DoSelect(ITEM1);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case ItemSelectHook_Item2:
  case ItemSelectHook_Item2_Icon:
    {
      m_item2.MouseUp();
      if (m_item2.IsEnable())
      {
        DoSelect(ITEM2);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case ItemSelectHook_Item3:
  case ItemSelectHook_Item3_Icon:
    {
      m_item3.MouseUp();
      if (m_item3.IsEnable())
      {
        DoSelect(ITEM3);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  case ItemSelectHook_Item4:
  case ItemSelectHook_Item4_Icon:
    {
      m_item4.MouseUp();
      if (m_item4.IsEnable())
      {
        DoSelect(ITEM4);
      }
      else
      {
        m_isNeedRefesh = false;
      }
    }
    break;
  default:
    {
      m_isNeedRefesh = false;
      break;
    }
  }

  if (m_isNeedRefesh)
  {
    Refresh();
  }
  m_isNeedRefesh = true;
  return ctrlType;
}

void UeGui::CItemSelectHook::SetDlgCoordinateDefault(DFTDlgCoordinateIndex index)
{
  switch (index)
  {
  case DFT_Coordinate1:
    {
      SetDlgCoordinate(m_dlgDefaultCoordinate1.x, m_dlgDefaultCoordinate1.y);
      break;
    }
  case DFT_Coordinate2:
    {
      SetDlgCoordinate(m_dlgDefaultCoordinate2.x, m_dlgDefaultCoordinate2.y);
      break;
    }
  default:
    {
      SetDlgCoordinate(m_dlgDefaultCoordinate1.x, m_dlgDefaultCoordinate1.y);
    }
  }
}

void UeGui::CItemSelectHook::SetDlgCoordinate( unsigned short x, unsigned short y )
{
  short x_Offset = x - m_dlgDefaultCoordinate1.x;
  short y_Offset = y - m_dlgDefaultCoordinate1.y;

  GuiElement* guielemment = NULL;
  for (short i = ItemSelectHook_PopupListTop; i < ItemSelectHook_End; ++i)
  {
    guielemment = GetGuiElement(i);
    if (guielemment)
    {
      guielemment->m_startX = m_coordinates[i].x + x_Offset;
      guielemment->m_startY = m_coordinates[i].y + y_Offset;
    }
  }
}

void UeGui::CItemSelectHook::SetSelectEvent( CAggHook* senderHook, ItemSelectEvent selectEvent, const ItemInfoList& itemInfoList )
{
  m_senderHook = senderHook;
  m_itemSelectEvent = selectEvent;

  m_item1.ClearCaption();
  m_item1.SetEnable(false);
  m_item2.ClearCaption();
  m_item2.SetEnable(false);
  m_item3.ClearCaption();
  m_item3.SetEnable(false);
  m_item4.ClearCaption();
  m_item4.SetEnable(false);

  if (itemInfoList.size() >= 1)
  {
    m_item1.SetCaption(itemInfoList[ITEM1].m_itemName);
    m_item1.SetEnable(itemInfoList[ITEM1].m_enable);
  }
  if (itemInfoList.size() >= 2)
  {
    m_item2.SetCaption(itemInfoList[ITEM2].m_itemName);
    m_item2.SetEnable(itemInfoList[ITEM2].m_enable);
  }
  if (itemInfoList.size() >= 3)
  {
    m_item3.SetCaption(itemInfoList[ITEM3].m_itemName);
    m_item3.SetEnable(itemInfoList[ITEM3].m_enable);
  }
  if (itemInfoList.size() >= 4)
  {
    m_item4.SetCaption(itemInfoList[ITEM4].m_itemName);
    m_item4.SetEnable(itemInfoList[ITEM4].m_enable);
  }
}

void UeGui::CItemSelectHook::DoSelect( ItemType type )
{
  if ((type >= ITEM1) && (type <= ITEM4))
  {
    //先关闭自己然后再执行指定的方法
    DoClose();
    if (m_itemSelectEvent)
    {
      m_itemSelectEvent(m_senderHook, type);
    }
  }
}

void UeGui::CItemSelectHook::DoClose()
{
  Return(false);
}