#include "itemselecthook.h"
using namespace UeGui;

CItemSelectHook::CItemSelectHook() : m_senderHook(NULL), m_itemSelectEvent(NULL)
{
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
      DoSelect(ITEM1);
    }
    break;
  case ItemSelectHook_Item2:
  case ItemSelectHook_Item2_Icon:
    {
      m_item2.MouseUp();
      DoSelect(ITEM2);
    }
    break;
  case ItemSelectHook_Item3:
  case ItemSelectHook_Item3_Icon:
    {
      m_item3.MouseUp();
      DoSelect(ITEM3);
    }
    break;
  case ItemSelectHook_Item4:
  case ItemSelectHook_Item4_Icon:
    {
      m_item4.MouseUp();
      DoSelect(ITEM4);
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
    if (m_itemSelectEvent)
    {
      m_itemSelectEvent(m_senderHook, type);
    }
    DoClose();
  }
}

void UeGui::CItemSelectHook::DoClose()
{
  Return(false);
}