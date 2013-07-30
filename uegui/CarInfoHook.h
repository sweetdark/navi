#ifndef _UEGUI_CARINFOHOOK_H
#define _UEGUI_CARINFOHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
//#include "uicheckbutton.h"

// Refer to path basic definitions
#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif

// Refer to file basic definition
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif

// Refer to string basic 
#ifndef _UEBASE_STRINGBASIC_H
#include "uebase\stringbasic.h"
#endif

#ifndef _UEBASE_FILEREADER_H
#include "uebase\customerfilereader.h"
#endif
using namespace UeBase;

namespace UeGui
{
#pragma pack(push)
#pragma pack(4)
	typedef struct CarInfoData
	{
		const static unsigned int MAX_LENGTH=32;
		unsigned char carLicence[MAX_LENGTH];	//车牌号
		double width;	//宽
		double height;	//高
		double weight;	//载重

		CarInfoData():width(0.0),height(0.0),weight(0.0)
		{
			memset(&carLicence,0,MAX_LENGTH);
		}
	} CarInfoData;
#pragma pack(pop)

	class UEGUI_CLASS CCarInfoHook : public CAggHook
	{
	public:
		enum CarInfoHookCtrlType
		{
			CarInfoHook_Begin = 0,
			CarInfoHook_BackGround,
			CarInfoHook_BGMenuText,
			CarInfoHook_GotoMapBtn,
			CarInfoHook_LogicBackBtn,
			CarInfoHook_labelTip1,
			CarInfoHook_labelTip2,
			CarInfoHook_LabelCarLicense,
			CarInfoHook_LabelCarWidth,
			CarInfoHook_LabelCarHeight,
			CarInfoHook_LabelCarWeight,
			CarInfoHook_EditLeft1,
			CarInfoHook_EditMid1,
			CarInfoHook_EditLeft2,
			CarInfoHook_EditMid2,
			CarInfoHook_EditLeft3,
			CarInfoHook_EditMid3,
			CarInfoHook_EditLeft4,
			CarInfoHook_EditMid4,
			CarInfoHook_EditBgk1,
			CarInfoHook_EditBgk2,
			CarInfoHook_EditBgk3,
			CarInfoHook_EditBgk4,
			CarInfoHook_End
		};

		CCarInfoHook();
		virtual ~CCarInfoHook();
		virtual void MakeGUI();
		virtual short MouseDown(CGeoPoint<short> &scrPoint);
		virtual short MouseMove(CGeoPoint<short> &scrPoint){ return -1; }
		virtual short MouseUp(CGeoPoint<short> &scrPoint);
		virtual bool operator ()();

		// 		static bool gotoCarInfoHook(unsigned uPreHookType=-1,
		// 			void *pDoCallBackObj=0,Query_CallBack *pCallBackEvent=0);

		CarInfoData &GetCarInfoData() { return m_carInfoData; }
		
	protected:
		virtual tstring GetBinaryFileName()
		{
			return _T("carinfohook.bin");
		}
		virtual void MakeNames();
		void MakeControls();

	private:
		void GetInputs(const char* item);
		static void callInputPanel(void* sender, const UeQuery::SQLRecord * data);
		void ShowCarInfo();
		bool getCarInfoFromFile(void);
		bool saveCarInfoToFile(void);

		tstring m_filename;

	private:
		const CPathBasic &m_pathBasic;
		CarInfoData m_carInfoData;	//车辆信息
		short m_Rowtag;

		//控件变量
		CUiButton m_gotoMapBtnCtrl;
		CUiButton m_logicBackBtnCtrl;

		CUiButton m_bGMenuTextCtrl;
		CUiButton m_backGroundCtrl;
		
		CUiButton m_editLeft1Ctrl;
		CUiButton m_editLeft2Ctrl;
		CUiButton m_editLeft3Ctrl;
		CUiButton m_editLeft4Ctrl;
		CUiButton m_editMid1Ctrl;
		CUiButton m_editMid2Ctrl;
		CUiButton m_editMid3Ctrl;
		CUiButton m_editMid4Ctrl;
		CUiButton m_editBtnCtrl[4];
		//CUiButton m_labelTip1Ctrl;
		//CUiButton m_labelTip2Ctrl;
	};
}
#endif
