#include "searchaddressbookhook.h"
#include "maphook.h";
using namespace UeGui;

void CSearchAddressBookHook::GotoAddressBookQueryHook(unsigned uPreHookType,
													  void *pDoCallBackObj,
													  Query_CallBack *pCallBackEvent)
{
	CSearchAddressBookHook *searchaddressHook = (CSearchAddressBookHook *)IView::GetView()->GetHook(CViewHook::DHT_SearchAddressBookHook);
	if (searchaddressHook!=0)
	{
		searchaddressHook->Init();
		searchaddressHook->m_uPrevHookType = uPreHookType;
		searchaddressHook->m_pDoCallBackObj = pDoCallBackObj;
		searchaddressHook->m_pCallBackEvent = pCallBackEvent;
		CViewHook::m_curHookType = CViewHook::DHT_SearchAddressBookHook;
	}
}
CSearchAddressBookHook::~CSearchAddressBookHook()
{
	m_elements.clear();
	m_ctrlNames.clear();
	m_imageNames.clear();
}

void CSearchAddressBookHook::MakeGUI()
{
	FetchWithBinary();
	MakeNames();
	MakeControls();
}

void CSearchAddressBookHook::Init()
{
	favorDeque.clear();
	UeQuery::SQLSetting setting;
	if (m_query->ConnectTo(UeQuery::DT_Favorite,setting) == UeQuery::SQL_Success)
	{
		for(int i = 0; i < m_query->GetFavoriteCount(); i++)
		{
			const FavoriteEntry *curEntry = m_query->GetFavorite(i);
			favorDeque.push_back(*curEntry);
		}
		m_query->Disconnect(UeQuery::DT_Favorite);
	}
	SetAddressBookPageInfo();
	ShowAddressBookData();
}

tstring CSearchAddressBookHook::GetBinaryFileName()
{
	return _T("searchaddressbookhook.bin");
}

void CSearchAddressBookHook::MakeNames()
{
	m_ctrlNames.erase(m_ctrlNames.begin(), m_ctrlNames.end());
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_BackGround,	"BackGround"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_BackGroundText,	"BackGroundText"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_GotoMapBtn,	"GotoMapBtn"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_BackButton,	"BackButton"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_PageText,	"PageText"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_OneCtrlCenter,	"OneCtrlCenter"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_OneCtrlLeft,	"OneCtrlLeft"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_OneCtrlRight,	"OneCtrlRight"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_TwoCtrlCenter,	"TwoCtrlCenter"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_TwoCtrlLeft,	"TwoCtrlLeft"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_TwoCtrlRight,	"TwoCtrlRight"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_ThreeCtrlCenter,	"ThreeCtrlCenter"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_ThreeCtrlLeft,	"ThreeCtrlLeft"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_ThreeCtrlRight,	"ThreeCtrlRight"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_FourCtrlCenter,	"FourCtrlCenter"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_FourCtrlLeft,	"FourCtrlLeft"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_FourCtrlRight,	"FourCtrlRight"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_FiveCtrlCenter,	"FiveCtrlCenter"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_FiveCtrlLeft,	"FiveCtrlLeft"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_FiveCtrlRight,	"FiveCtrlRight"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_SixCtrlCenter,	"SixCtrlCenter"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_SixCtrlLeft,	"SixCtrlLeft"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_SixCtrlRight,	"SixCtrlRight"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_UpBtn,	"UpBtn"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_UpIconBtn,	"UpIconBtn"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_DownBtn,	"DownBtn"));
	m_ctrlNames.insert(GuiName::value_type(SearchAddressBookHook_DownIconBtn,	"DownIconBtn"));
}

void CSearchAddressBookHook::MakeControls()
{
	m_backButtonCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_BackButton));
	m_backGroundCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_BackGround));
	m_backGroundTextCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_BackGroundText));
	m_downBtnCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_DownBtn));
	m_downIconBtnCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_DownIconBtn));
	m_fiveCtrlCenterCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_FiveCtrlCenter));
	m_fiveCtrlLeftCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_FiveCtrlLeft));
	m_fiveCtrlRightCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_FiveCtrlRight));
	m_fourCtrlCenterCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_FourCtrlCenter));
	m_fourCtrlLeftCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_FourCtrlLeft));
	m_fourCtrlRightCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_FourCtrlRight));
	m_gotoMapBtnCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_GotoMapBtn));
	m_oneCtrlCenterCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_OneCtrlCenter));
	m_oneCtrlLeftCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_OneCtrlLeft));
	m_oneCtrlRightCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_OneCtrlRight));
	m_pageTextCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_PageText));
	m_sixCtrlCenterCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_SixCtrlCenter));
	m_sixCtrlLeftCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_SixCtrlLeft));
	m_sixCtrlRightCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_SixCtrlRight));
	m_threeCtrlCenterCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_ThreeCtrlCenter));
	m_threeCtrlLeftCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_ThreeCtrlLeft));
	m_threeCtrlRightCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_ThreeCtrlRight));
	m_twoCtrlCenterCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_TwoCtrlCenter));
	m_twoCtrlLeftCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_TwoCtrlLeft));
	m_twoCtrlRightCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_TwoCtrlRight));
	m_upBtnCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_UpBtn));
	m_upIconBtnCtrl.SetCenterElement(GetGuiElement(SearchAddressBookHook_UpIconBtn));
}

short CSearchAddressBookHook::MouseDown(CGeoPoint<short> &scrPoint)
{
	short ctrlType = CAggHook::MouseDown(scrPoint);
	switch(ctrlType)
	{
	case SearchAddressBookHook_BackButton:
		{
			m_backButtonCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_BackGround:
	case SearchAddressBookHook_BackGroundText:
		break;
	case SearchAddressBookHook_DownBtn:
	case SearchAddressBookHook_DownIconBtn:
		{
			m_downBtnCtrl.MouseDown();
			m_downIconBtnCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_FiveCtrlCenter:
	case SearchAddressBookHook_FiveCtrlLeft:
	case SearchAddressBookHook_FiveCtrlRight:
		{
			m_fiveCtrlCenterCtrl.MouseDown();
			m_fiveCtrlLeftCtrl.MouseDown();
			m_fiveCtrlRightCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_FourCtrlCenter:
	case SearchAddressBookHook_FourCtrlLeft:
	case SearchAddressBookHook_FourCtrlRight:
		{
			m_fourCtrlCenterCtrl.MouseDown();
			m_fourCtrlLeftCtrl.MouseDown();
			m_fourCtrlRightCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_GotoMapBtn:
		{
			m_gotoMapBtnCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_OneCtrlCenter:
	case SearchAddressBookHook_OneCtrlLeft:
	case SearchAddressBookHook_OneCtrlRight:
		{
			m_oneCtrlCenterCtrl.MouseDown();
			m_oneCtrlLeftCtrl.MouseDown();
			m_oneCtrlRightCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_PageText:
		break;
	case SearchAddressBookHook_SixCtrlCenter:
	case SearchAddressBookHook_SixCtrlLeft:
	case SearchAddressBookHook_SixCtrlRight:
		{
			m_sixCtrlCenterCtrl.MouseDown();
			m_sixCtrlLeftCtrl.MouseDown();
			m_sixCtrlRightCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_ThreeCtrlCenter:
	case SearchAddressBookHook_ThreeCtrlLeft:
	case SearchAddressBookHook_ThreeCtrlRight:
		{
			m_threeCtrlCenterCtrl.MouseDown();
			m_threeCtrlLeftCtrl.MouseDown();
			m_threeCtrlRightCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_TwoCtrlCenter:
	case SearchAddressBookHook_TwoCtrlLeft:
	case SearchAddressBookHook_TwoCtrlRight:
		{
			m_twoCtrlCenterCtrl.MouseDown();
			m_twoCtrlLeftCtrl.MouseDown();
			m_twoCtrlRightCtrl.MouseDown();
		}
		break;
	case SearchAddressBookHook_UpBtn:
	case SearchAddressBookHook_UpIconBtn:
		{
			m_upBtnCtrl.MouseDown();
			m_upIconBtnCtrl.MouseDown();
		}
		break;
	default:
		assert(false);
		break;
	}

	this->Refresh();
	return ctrlType;
}

short CSearchAddressBookHook::MouseMove(CGeoPoint<short> &scrPoint)
{
	return -1;
}

short CSearchAddressBookHook::MouseUp(CGeoPoint<short> &scrPoint)
{
	short ctrlType = CAggHook::MouseUp(scrPoint);
	switch(m_downElementType)
	{
	case SearchAddressBookHook_BackButton:
		{
			m_backButtonCtrl.MouseUp();
			if (ctrlType == m_downElementType) 
			{
				CViewHook::m_curHookType = m_uPrevHookType;
			}
		}
		break;
	case SearchAddressBookHook_BackGround:
	case SearchAddressBookHook_BackGroundText:
		break;
	case SearchAddressBookHook_DownBtn:
	case SearchAddressBookHook_DownIconBtn:
		{
			if(m_downBtnCtrl.IsEnable())
			{
				m_downBtnCtrl.MouseUp();
				m_downIconBtnCtrl.MouseUp();
				m_pageTurning.NextPage();
				SetNavigatorStatus();
				ShowAddressBookData();
			}
		}
		break;
	case SearchAddressBookHook_FiveCtrlCenter:
	case SearchAddressBookHook_FiveCtrlLeft:
	case SearchAddressBookHook_FiveCtrlRight:
		{
			if(m_fiveCtrlCenterCtrl.IsEnable())
			{
				m_fiveCtrlCenterCtrl.MouseUp();
				m_fiveCtrlLeftCtrl.MouseUp();
				m_fiveCtrlRightCtrl.MouseUp();
				GoToMapPosition(m_pageTurning.GetPageStartPosition()+kROW5-2);
			}
		}
		break;
	case SearchAddressBookHook_FourCtrlCenter:
	case SearchAddressBookHook_FourCtrlLeft:
	case SearchAddressBookHook_FourCtrlRight:
		{
			if(m_fourCtrlCenterCtrl.IsEnable())
			{
				m_fourCtrlCenterCtrl.MouseUp();
				m_fourCtrlLeftCtrl.MouseUp();
				m_fourCtrlRightCtrl.MouseUp();
				GoToMapPosition(m_pageTurning.GetPageStartPosition()+kROW4-2);
			}
		}
		break;
	case SearchAddressBookHook_GotoMapBtn:
		{
			m_gotoMapBtnCtrl.MouseUp();
			/*CViewHook::m_prevHookType = CViewHook::m_curHookType;
			CViewHook::m_curHookType = CViewHook::DHT_MapHook;*/
      CAggHook::GoToMapHook();
			CMapHook *maphook=(CMapHook *)m_view->GetHook(CViewHook::DHT_MapHook);
      maphook->ComeBack();
			maphook->SetFavoritePickVisble(false);
		}
		break;
	case SearchAddressBookHook_OneCtrlCenter:
	case SearchAddressBookHook_OneCtrlLeft:
	case SearchAddressBookHook_OneCtrlRight:
		{
			if(m_oneCtrlCenterCtrl.IsEnable())
			{
				m_oneCtrlCenterCtrl.MouseUp();
				m_oneCtrlLeftCtrl.MouseUp();
				m_oneCtrlRightCtrl.MouseUp();
				GoToMapPosition(m_pageTurning.GetPageStartPosition()+kROW1-2);
			}
		}
		break;
	case SearchAddressBookHook_PageText:
		break;
	case SearchAddressBookHook_SixCtrlCenter:
	case SearchAddressBookHook_SixCtrlLeft:
	case SearchAddressBookHook_SixCtrlRight:
		{
			if(m_sixCtrlCenterCtrl.IsEnable())
			{
				m_sixCtrlCenterCtrl.MouseUp();
				m_sixCtrlLeftCtrl.MouseUp();
				m_sixCtrlRightCtrl.MouseUp();
				GoToMapPosition(m_pageTurning.GetPageStartPosition()+kROW6-2);
			}
		}
		break;
	case SearchAddressBookHook_ThreeCtrlCenter:
	case SearchAddressBookHook_ThreeCtrlLeft:
	case SearchAddressBookHook_ThreeCtrlRight:
		{
			if(m_threeCtrlCenterCtrl.IsEnable())
			{
				m_threeCtrlCenterCtrl.MouseUp();
				m_threeCtrlLeftCtrl.MouseUp();
				m_threeCtrlRightCtrl.MouseUp();
				GoToMapPosition(m_pageTurning.GetPageStartPosition()+kROW3-2);
			}
		}
		break;
	case SearchAddressBookHook_TwoCtrlCenter:
	case SearchAddressBookHook_TwoCtrlLeft:
	case SearchAddressBookHook_TwoCtrlRight:
		{
			if(m_twoCtrlCenterCtrl.IsEnable())
			{
				m_twoCtrlCenterCtrl.MouseUp();
				m_twoCtrlLeftCtrl.MouseUp();
				m_twoCtrlRightCtrl.MouseUp();
				GoToMapPosition(m_pageTurning.GetPageStartPosition()+kROW2-2);
			}
		}
		break;
	case SearchAddressBookHook_UpBtn:
	case SearchAddressBookHook_UpIconBtn:
		{
			if(m_upBtnCtrl.IsEnable())
			{
				m_upBtnCtrl.MouseUp();
				m_upIconBtnCtrl.MouseUp();
				m_pageTurning.PreviousPage();
				SetNavigatorStatus();
				ShowAddressBookData();
			}
		}
		break;
	default:
		assert(false);
		break;
	}

	this->Refresh();
	return ctrlType;
}

bool CSearchAddressBookHook::operator ()()
{
	return false;
}

void UeGui::CSearchAddressBookHook::DoShowRowData( RowTag tag, const char* value )
{
	SetRowEnbale(tag, true);
	switch (tag)
	{
	case kROW1 : 
		{
			m_oneCtrlCenterCtrl.SetCaption(value);
			break;
		}
	case kROW2 : 
		{
			m_twoCtrlCenterCtrl.SetCaption(value);
			break;
		}
	case kROW3 : 
		{
			m_threeCtrlCenterCtrl.SetCaption(value);
			break;
		}
	case kROW4 : 
		{
			m_fourCtrlCenterCtrl.SetCaption(value);
			break;
		}
	case kROW5:
		{
			m_fiveCtrlCenterCtrl.SetCaption(value);
			break;
		}
	case kROW6:
		{
			m_sixCtrlCenterCtrl.SetCaption(value);
			break;
		}
	default:
		{
			break;
		}
	}
}

void UeGui::CSearchAddressBookHook::SetRowEnbale( RowTag row, bool enable )
{
	switch (row)
	{
	case kROW1 : 
		{
			m_oneCtrlCenterCtrl.SetEnable(enable);
			m_oneCtrlLeftCtrl.SetEnable(enable);
			m_oneCtrlRightCtrl.SetEnable(enable);
			break;
		}
	case kROW2 : 
		{
			m_twoCtrlCenterCtrl.SetEnable(enable);
			m_twoCtrlLeftCtrl.SetEnable(enable);
			m_twoCtrlRightCtrl.SetEnable(enable);
			break;
		}
	case kROW3 : 
		{
			m_threeCtrlCenterCtrl.SetEnable(enable);
			m_threeCtrlLeftCtrl.SetEnable(enable);
			m_threeCtrlRightCtrl.SetEnable(enable);
			break;
		}
	case kROW4 : 
		{
			m_fourCtrlCenterCtrl.SetEnable(enable);
			m_fourCtrlLeftCtrl.SetEnable(enable);
			m_fourCtrlRightCtrl.SetEnable(enable);
			break;
		}
	case kROW5:
		{
			m_fiveCtrlCenterCtrl.SetEnable(enable);
			m_fiveCtrlLeftCtrl.SetEnable(enable);
			m_fiveCtrlRightCtrl.SetEnable(enable);
			break;
		}
	case kROW6:
		{
			m_sixCtrlCenterCtrl.SetEnable(enable);
			m_sixCtrlLeftCtrl.SetEnable(enable);
			m_sixCtrlRightCtrl.SetEnable(enable);
			break;
		}
	default:
		{
			break;
		}
	}
}

void UeGui::CSearchAddressBookHook::SetNavigatorStatus()
{
	if (m_pageTurning.IsFirstPage())
	{
		m_upBtnCtrl.SetEnable(false);
		m_upIconBtnCtrl.SetEnable(false);
	}
	else
	{
		m_upBtnCtrl.SetEnable(true);
		m_upIconBtnCtrl.SetEnable(true);
	}
	if (m_pageTurning.IsLastPage())
	{
		m_downBtnCtrl.SetEnable(false);
		m_downIconBtnCtrl.SetEnable(false);
	}
	else
	{
		m_downBtnCtrl.SetEnable(true);
		m_downIconBtnCtrl.SetEnable(true);
	}
}

void UeGui::CSearchAddressBookHook::SetAddressBookPageInfo()
{
	m_pageTurning.Clear();
	UeQuery::SQLSetting setting;
	unsigned int itm=m_query->ConnectTo(UeQuery::DT_Favorite, setting);
	if(itm != UeQuery::SQL_Success)
		return;
	m_pageTurning.SetTotal(m_query->GetFavoriteCount());
	m_pageTurning.SetQuantityOfOnePage(kROWEnd - 1);
	m_query->Disconnect(UeQuery::DT_Favorite);
	SetNavigatorStatus();
}

void UeGui::CSearchAddressBookHook::ClearAllRow()
{
	ClearRow(kROW1);
	ClearRow(kROW2);
	ClearRow(kROW3);
	ClearRow(kROW4);
	ClearRow(kROW5);
	ClearRow(kROW6);
	SetRowEnbale(kROW1, false);
	SetRowEnbale(kROW2, false);
	SetRowEnbale(kROW3, false);
	SetRowEnbale(kROW4, false);
	SetRowEnbale(kROW5, false);
	SetRowEnbale(kROW6, false);
}

void UeGui::CSearchAddressBookHook::ClearRow( RowTag row )
{
	switch (row)
	{
	case kROW1 : 
		{
			m_oneCtrlCenterCtrl.ClearCaption();
			break;
		}
	case kROW2 : 
		{
			m_twoCtrlCenterCtrl.ClearCaption();
			break;
		}
	case kROW3 : 
		{
			m_threeCtrlCenterCtrl.ClearCaption();
			break;
		}
	case kROW4 : 
		{
			m_fourCtrlCenterCtrl.ClearCaption();
			break;
		}
	case kROW5:
		{
			m_fiveCtrlCenterCtrl.ClearCaption();
			break;
		}
	case kROW6:
		{
			m_sixCtrlCenterCtrl.ClearCaption();
			break;
		}
	default:
		{
			break;
		}
	}
}

void UeGui::CSearchAddressBookHook::ShowAddressBookData()
{
	ClearAllRow();
	m_pageTextCtrl.SetCaption(m_pageTurning.GetPageInfo());
	int startPos = m_pageTurning.GetPageStartPosition() - 1;
	int endPos = m_pageTurning.GetPageEndPosition() - 1;
	if (startPos < 0)
	{
		return;
	}
	//显示数据
	int row = kROW1;
	for(int i = startPos; i <= endPos; i++)
	{
		DoShowRowData((RowTag)row, (char *)favorDeque[i].m_name);
		++row;
	}
}

void CSearchAddressBookHook::GoToMapPosition(int n)
{
	SQLRecord oneRecord;
	oneRecord.m_x = favorDeque[n].m_x;
	oneRecord.m_y = favorDeque[n].m_y;
	//
	unsigned uStrLen(::strlen(reinterpret_cast<char *>(favorDeque[n].m_name))+1);
	if (oneRecord.m_uniName = static_cast<char *>(::malloc(sizeof(char)*uStrLen)))
	{
		::memcpy(oneRecord.m_uniName,favorDeque[n].m_name,uStrLen);
	}
	if (m_pCallBackEvent!=0)
	{
		m_pCallBackEvent(m_pDoCallBackObj,&oneRecord);
		m_pCallBackEvent = 0;
	}
	else
	{
		//进入地图界面
		CMapHook *pMapHook((CMapHook *)(m_view->GetHook(CViewHook::DHT_MapHook)));
		pMapHook->PeripheralInformationSwitch(&oneRecord);
	}
	::free(oneRecord.m_uniName);
}