#ifndef _UEGUI_SOUNDMENUHOOK_H
#define _UEGUI_SOUNDMENUHOOK_H

#ifndef _UEGUI_BASE_H
#include "uegui.h"
#endif

#ifndef _UEGUI_AGGHOOK_H
#include "agghook.h"
#endif

#include "uilabel.h"
#include "uibutton.h"
#include "uiradiobutton.h"
#include "uicheckbutton.h"
#include "settingwrapper.h"

namespace UeGui
{
  class UEGUI_CLASS CSoundMenuHook : public CAggHook
  {
  public:    
    enum SoundMenuHookCtrlType
    {
      SoundMenuHook_Begin = 0,
      SoundMenuHook_Background,
      SoundMenuHook_SoundBack,
      SoundMenuHook_Title,
      SoundMenuHook_SoundBarBack,
      SoundMenuHook_SoundBarProgress,
      SoundMenuHook_SoundBtn,
      SoundMenuHook_SoundMin,
      SoundMenuHook_SoundMax,
      SoundMenuHook_SoundMute,
      SoundMenuHook_SoundMuteLabel,
      SoundMenuHook_PromptLabel,
      SoundMenuHook_Concision,
      SoundMenuHook_ConsisionLabel,
      SoundMenuHook_General,
      SoundMenuHook_GeneralLabel,
      SoundMenuHook_Detail,
      SoundMenuHook_DetailLabel,
      SoundMenuHook_End
    };
    enum RadioButtonIndex
    {
      RB_Consion,  //简洁
      RB_General,  //一般
      RB_Detail,   //详细
    };
    CSoundMenuHook();

    virtual ~CSoundMenuHook();

    /**
    * \brief 显示界面之前的数据准备
    */
    virtual void Load();

    /**
    * \brief 切换界面时释放数据
    **/
    virtual void UnLoad();

    /**
    * 计时器
    **/
    virtual void Timer();

    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    virtual bool operator ()();

  protected:
    virtual tstring GetBinaryFileName();

    virtual void MakeNames();

    void MakeControls();
  private:
    /**
    * \brief 关闭窗口
    */
    void DoClose();
    /**
    * \brief 设置声音大小
    * param: bIncrease True(增大) False(减少)
    */
    void ChangeVolume(bool bIncrease);
    /**
    * \brief 进度条上选点
    */
    void ClickOnSoundBar(CGeoPoint<short> &scrPoint);
    /**
    * \brief 播报提示选择
    */
    void BroadcastRadioCheck(RadioButtonIndex index);
    /**
    * \brief 传入音量条的宽度来设置音量条
    */
    void SetSoundBar(unsigned short barWidth);
    /**
    * \brief 传入声音值的大小来设置音量条
    * \param volume:大小范围:0..100
    */
    void SetVolume(unsigned short volume);
    /**
    * \brief 设置静音
    */
    void SetMute();
    /**
    * \brief 保存设置到文件中
    */
    void SaveSoundSetting();
    /**
    * \brief 根据音量条的宽度来计算音量值
    */
    unsigned short GetVoume(unsigned short barWidth);
    /**
    * \brief 重新开始界面切换定时器
    */
    void RestarTimer();
    /**
    * \brief 关闭界面切换定时器
    */
    void CloseTimer();
    /**
    * \brief 局部刷新
    */
    void PartRefresh();
  private:
    //消息框背景
    CUiButton m_soundBack;
    //声音加大
    CUiButton m_soundMaxBtn;
    //声音减小
    CUiButton m_soundMinBtn;
    //声音调节按钮
    CUiButton m_soundBtn;
    //声音条背景
    CUiButton m_soundBarBack;
    //声音条
    CUiButton m_soundBarProgress;
    //静音
    CUiCheckButton m_soundMuteBtn;
    //简洁
    CUiRadioButton m_concisionBtn;
    //一般
    CUiRadioButton m_generalBtn;
    //详细
    CUiRadioButton m_detailBtn;
  private:
    //设置对象接口
    CSettingWrapper& m_settingWrapper;
    //声音条的最大长度
    short m_maxSoundBarWidth;
    //当前声音条的长度
    short m_curSoundBarWidth;
    //界面切换定时间隔
    short m_timerInterval;
  };
}
#endif
