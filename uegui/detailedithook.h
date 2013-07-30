#ifndef _UEGUI_DETAILEDITHOOK_H
#define _UEGUI_DETAILEDITHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uibutton.h"
//#include "uiradiobutton.h"
#include "uicheckbutton.h"
//#include "QueryHookBase.h"
#include "userdatawrapper.h"
#include "menubackgroundhook.h"

//标识图标起始ID
#define   MARKPICBEGIN 424 
#define   MARKPICEND 471

namespace UeGui
{
  enum HandleType
  {
    OT_CONFIRM,     //  确定
    OT_PREVIOUS,    //  返回调用的界面
    OT_GOTOMAP,     //  返回地图浏览界面
    OT_NAME,        //  编辑名称
    OT_ADDRESS,     //  编辑地址
    OT_TELEPHONE,   //  编辑电话
    OT_MARKER,      //  编辑标记
    OT_MAPSHOW,     //  选择是否图上显示
    OT_VOICE,       //  选择是否语音播报
    OT_STARTPOS,    //  选择是否开机未知
  };

  struct EditData;
  class CDetailEditEvent
  {
  public:
    CAggHook* m_senderHook;
    short m_senderHookType;

    CDetailEditEvent():m_senderHook(NULL),m_senderHookType(CViewHook::CT_Unknown){}

    CDetailEditEvent(CAggHook* senderhook, short senderhookType):m_senderHook(senderhook),m_senderHookType(senderhookType){}

    virtual ~CDetailEditEvent(){}

    const CDetailEditEvent &operator=(const CDetailEditEvent &other)
    {
      if(this == &other)
      {
        return *this;
      }
      m_senderHook=other.m_senderHook;
      m_senderHookType=other.m_senderHookType;
      return *this;
    }
  };



  class UEGUI_CLASS CDetailEditHook : public CMenuBackgroundHook
  {
  public:
    enum DetailEditHookCtrlType
    {
      DetailEditHook_Begin = MenuBackgroundHook_End,

      DetailEditHook_Name,
      DetailEditHook_NameLabel,

      DetailEditHook_Address,
      DetailEditHook_AddressLabel,

      DetailEditHook_Telephone,
      DetailEditHook_TelephoneLabel,

      DetailEditHook_Mark,
      DetailEditHook_MarkLabel,
      DetailEditHook_MarkPic,

      DetailEditHook_Determine, 
      DetailEditHook_End
    };

    CDetailEditHook();

    virtual ~CDetailEditHook();

    //virtual void MakeGUI();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    /*virtual bool operator ()();

    virtual void Init();*/

    virtual void Load();


    static bool ShowDetailEditHook(const EditData* caption, CDetailEditEvent& editEvent);
    bool DoShowDetailEditHook(const EditData* caption, CDetailEditEvent& editEvent);

    static void SetMark(short& );
    void DoSetMark(short&);

    static void RecordPosition(int);
    void DoRecordPosition(int);
    
    static void EditHookNameCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    void DoEditHookNameCallBack(const SQLRecord *pResult);

    static void EditHookAddressCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    void DoEditHookAddressCallBack(const SQLRecord *pResult);

    static void EditHookTelephoneCallBack(void *pDoCallBackObj,const SQLRecord *pResult);
    void DoEditHookTelephoneCallBack(const SQLRecord *pResult);


  protected:
    //virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();

  private:
    void DoMouseUp(HandleType resultType);

    void SetEditData();

    bool EditData2FavoriteEntry(EditData* edata, FavoriteEntry* fEntry);

    //保存数据
    bool SaveFavoriteEntryData(const FavoriteEntry& data);
  private:
    CUiButton m_nameCtrl;
    CUiButton m_addressCtrl;
    CUiButton m_telephoneCtrl;
    CUiButton m_markCtrl;
    CUiButton m_markPicCtrl;
    CUiButton m_determineCtrl;

    CDetailEditEvent m_detailEditEvent;
    EditData eData;
    int recpos;

    const CUserDataWrapper& m_userWrapper;
  };
}
#endif
