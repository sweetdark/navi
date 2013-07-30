/*
* The UrbanEye PND solution is a collection of packages based on native C++ classes
* that implement the fundamental algorithms & operations required to satisfy one kind
* of mobile or portable devices with competitive qualities being a leading one.
*
* Copyright (C) 2008 Xiujie Sheng
*
* This library isn't a free software, so you cann't take the sourcecode without author's permit.
* Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
* if you illegally got it. 
*
* NOTE: 
* AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
* LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
*
* For more information, contact:
*
* China University of Geoseciences(Wuhan)
* Graduate School
*
* Xiujie Sheng
* smartpoint@sohu.com
*/

#ifndef _UEGUI_AGGHOOK_H
#define _UEGUI_AGGHOOK_H

#include "portability.h"
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to its parent
#ifndef _UEMAP_VIEWHOOK_H
#include "uemap\viewhook.h"
#endif

#ifndef _UEGUI_GUIBASIC_H
#include "guibasic.h"
#endif

#ifdef FOR_TRUCK
#define __FOR_TRUCK__ 1
#else
#define __FOR_TRUCK__ 0
#endif

using namespace UeMap;

// Forward class
namespace UeXml
{
  class CXmlReader;
}

// UeGui package
namespace UeGui
{
  /**
  * \brief It mainly embrace AGG some definitions for its children rendering and implement common
  * rendering process for common GUI layout
  *
  * \detail 
  * Note:
  * As far, it doesn't move some implementations of CViewHook to here and they should be here not there since
  * the intention of CViewHook is pure interface class
  **/
  #define ENDKEY 255
  #define DEFAULT_DEMOSPEED 60

  class CFocusTextElement;
  class CAggHook;
  class CUiButton;
  class CUiControl;
  typedef std::map<short, CAggHook*> AggHookMap;
  typedef std::map<short, CFocusTextElement*> FocusTextElementsMap;

  class CAggHook : public CViewHook
  {
  protected:
    /**
    * \brief XML Nodes represent different style and extent etc setting
    */
    enum XmlCtrlDef
    {
      Ctrl_Unknown = -1,
      Ctrl_StartX,
      Ctrl_StartY,
      Ctrl_Width,
      Ctrl_Height,
      Ctrl_ID,
      Ctrl_Style,
      Ctrl_Name,
      Ctrl_FillColor,
      Ctrl_BorderColor,
      Ctrl_BorderStyle,
      Ctrl_BackStyle,
      Ctrl_TextStyle,
      Ctrl_Radius,
      Ctrl_BltType,
      Ctrl_OpType,
      Ctrl_ForegroundPic,
      Ctrl_UpPic,
      Ctrl_DownPic,
      Ctrl_Visible,
      Ctrl_Max,
    };
    /// Names of each XML node
    static const string m_xmlNodes[];

  protected:
    //
    // Only be used for its children not outside
    //
    /*!
    * \brief 构造函数
    */
    CAggHook();

    /*!
    * \brief 析构函数
    */
    virtual ~CAggHook();
  public:
    /**
    * \brief 生成GUI信息
    */
    virtual void MakeGUI();
    /**
    * \brief 初始化hook信息
    */
    virtual void Init();

    /**
    * \brief 显示界面之前的数据准备
    */
    virtual void Load();
    /**
    * \brief 切换界面时释放数据
    **/
    virtual void UnLoad();

    /**
    * \brief 由其它界面返回当前界面
    */
    virtual void ComeBack();

    /**
    * \brief Render each hook in dynamic way
    */
    virtual void DynamicShow();

    /**
    *
    */
    virtual void ExcludeClipRect(void *dc);

    /**
    *
    */
    virtual void DoDraw(const CGeoRect<short> &scrExtent);

    /**
    *
    */
    virtual short MouseDown(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseUp(CGeoPoint<short> &scrPoint);

    /**
    *
    */
    virtual short MouseMove(CGeoPoint<short> &scrPoint);

    /**
    * 计时器
    **/
    virtual void Timer() {};

    /**
    * \设置当前hook是否显示
    */
    virtual void Show( bool show = true );
#if __FOR_FPC__
    /**
    * 
    */
    virtual void GetPhoto(TCHAR *photoName);
#endif
  public:
    /**
    * \设置hook路径
    */
    static void SetPath(TCHAR* path);

    /**
    * \添加当前hook的子hook
    */
    void AddChildHook(HookType childHookType, CAggHook* aggHook);

    /**
    * \显示当前hook的子hook
    */
    void ShowChildHook(HookType childHookType, bool show = true);

    /**
    * \当前hook是否处于显示状态
    */
    bool IsShown();

    /**
    * 清空子hook
    */
    void ClearChildHookList();

    /**
    * \添加需要对焦点字体变色的控件
    * param elementType 元素类型
    * fkey 要变色的文子位置信息 存放的是变色文字的数组下标 0 - len-1
    */  
    bool AddFocusTextElements(short elementType, const unsigned char* fkey);

    /**
    * \添加需要对焦点字体变色的控件
    * param element 元素
    * fkey 要变色的文子位置信息 存放的是变色文字的数组下标 0 - len-1
    */  
    bool AddFocusTextElements(GuiElement* guielement, const unsigned char* fkey);

    /**
    * \删除指定需要变色的元素
    */    
    void EraseFocusTextElements(short elementType);

    /**
    * \删除指定需要变色的元素
    */    
    void EraseFocusTextElements(GuiElement* guielement);

    /**
    * \添加需要对焦点字体变色的控件
    */    
    void ClearFocusTextElements();
  public:
    //跳转到某一个hook的界面
    void TurnTo(int hookType);
    //返回到前一个hook
    void Return();
    /**
    * 获得前一个hook的类型,没有则返回DHT_Unknown
    */
    int GetPrevHookType();
    //返回地图的按钮调用，并清空栈。
    void GoToMapHook();
  protected:
    //
    // GUI response functions
    //
    /**
    * 获取二进制文件名称，不包括路径，如:licensehook.bin
    * 该方法由子类负责实现，父类调用, 且必须实现，这里应声明为纯虚方法，主要是为了统一读取二进制文件方法
    */
    virtual tstring GetBinaryFileName();

    /**
    * 生成XML配置文件中的事件名称对应的枚举IC    
    * 该方法由子类负责实现，父类调用, 且必须实现，这里应声明为纯虚方法
    */
    virtual void MakeNames();

    /** 
    * 生成界面控件，该方法由子类负责实现，父类调用
    */
    virtual void MakeControls();

    /**
    * 从新结构二进制文件中读取配置
    */
    virtual void FetchWithBinary();

    /**
    * \brief 设置按钮样式(按下或正常状态)
    */
    virtual void SetCursor(short type, bool isfocus = false);
    //
    // Rendering functions
    //
    /**
    * \brief Render background of each GUI hook
    *
    * \param scrExtent the current size of screen for rendering
    */
    virtual void RenderHookWindow(const CGeoRect<short> &scrExtent);

    /**
    * \brief Render GUI outlook for one specified hook, it is a template method called by Draw() function in respective children
    *
    * \param scrExtent the current size of screen for rendering
    * \param oneElement one of GUI elements of current hook which carries size etc information
    **/
    virtual void RenderHookCommands(const CGeoRect<short> &scrExtent, const CViewHook::GuiElement &oneElement, const unsigned char* fKey = NULL, bool needRelesePic = true);

    /*!
    * \brief 从XML资源文件中生成对应的界面元素
    * TODO: 旧版本代码，待删除
    */
    virtual void FetchExtentWithXML(UeXml::CXmlReader& xmlReader, const std::string& nodeName, GuiElement& oneElement);

    /*!
    * \brief 从XML资源文件中生成对应的界面元素属性
    * TODO: 旧版本代码，待删除
    */
    virtual void FetchAttributeWithXML(UeXml::CXmlReader& xmlReader, const std::string& nodeName, GuiElement& oneElement);

    /**
    * TODO: 旧版本代码，待删除
    * \brief 为当前界面添加指定的界面元素
    *
    * \param eleType 指定的界面元素的标识
    * \param oneElement 界面元素对象
    */
    virtual bool AddGuiElement(int eleType, GuiElement &oneElement);

    /**
    * \brief 为当前界面添加指定的界面元素
    * \param eleType 指定的界面元素的标识
    * \param oneElement 界面元素对象
    */
    virtual bool AddGuiElement(GuiElement &oneElement);

    /**
    * TODO: 旧版本代码，待删除
    * \brief 获取当前界面指定的界面元素
    *
    * \param eleType 指定的界面元素的标识
    * \param oneElement 界面元素对象
    */
    virtual void GetGuiElement(int eleType, GuiElement &oneElement);

    /**
    * TODO: 旧版本代码，待删除
    * \brief 更新当前界面指定的界面元素
    *
    * \param eleType 指定的界面元素的标识
    * \param oneElement 界面元素对象
    */
    virtual void UpdateGuiElement(int eleType, GuiElement &oneElement);

    /*
    * 根据鼠标点击的位置获取当前点击控件信息
    */
    CViewHook::GuiElement* GetGuiElement(CGeoPoint<short> &scrPoint);

    /**
    * 清空界面元素
    */
    void ClearElements();

    /**
    * 根据事件的名称来获取对应的整型事件类型
    */
    short GetElementType(CGeoPoint<short> &scrPoint);

    /**
    * 根据事件的名称来获取对应的整型事件类型
    */
    short GetElementType(char* elementName);

    /**
    * 根据事件的名称来获取对应的整型事件类型
    */
    const char* GetElementName(short elementType);

    /**
    * 刷新界面
    */
    void Refresh(short type = VT_Unknown);

    /**
    * 更新指定控件显示类型
    */
    bool SetStyle(short elementType, bool focusStyle);

    /**
    * 根据控件的枚举ID值来设置控件的显示状态

    */
    void SetVisible(short elementType, bool visible);

    /**
    * 输出测试日志
    */
    void LogMessage(const char* msg);
    void LogMessageF(const char* format, ...);

  protected:
    /*
    * \根据事件枚举ID来获取控件信息
    */
    CViewHook::GuiElement* GetGuiElement(const short elementType);

    /*
    * \根据事件名称来获取控件信息
    */
    CViewHook::GuiElement* GetGuiElement(const char* elementName);
 
    /**
    * \只存放要刷新的按钮控件（暂时只是在设计阶段，还没有使用）
    */ 
    void ClearRenderElements();

    /**
    * \只存放要刷新的按钮控件（暂时只是在设计阶段，还没有使用）
    */ 
    void AddRenderElements(GuiElement* element);

    void AddRenderUiControls(CUiControl* control);

    

  private:
    /**
    * \从子hook列表中获取子hook
    */ 
    CAggHook* GetChildHook(HookType hookType);
    //判断是否是局部渲染
    bool IsPartRefresh(const CGeoRect<short> &scrExtent);

    /**
    * 获取当前元素需要进行焦点变色的文字信息
    * 如果读取不到则返回NULL，证明该元素不需要进行焦点文字变色
    */ 
   const unsigned char* GetFocusText(CViewHook::GuiElement* guielement);
  protected:
    // Steps for dynamically rendering GUI elements of each hook
    int m_dynamics;
    int m_curDynamic;    
    /// 记录屏幕坐标信息
    CGeoRect<short> m_scrExtent;
    /// 记录选中的控件对应事件类型
    short m_downElementType; 
    /// 记录鼠标按下时对应的控件信息
    GuiElement* m_downElement;
    /// 记录鼠标弹起时对应控件位置信息
    GuiElement* m_upElement;
    /// 鼠标按下时的子hook类型
    HookType m_downChildHookType;
    /// 鼠标弹起时的子hook类型
    HookType m_upChildHookType;
    //移到ViewHook中，用静态的方式。
    ///// 记录是否是鼠标按下事件
    //bool m_isMouseDown;
    ///// 记录是否是鼠标弹起事件
    //bool m_isMouseUp;
    //hook文件存放路径
    static TCHAR m_hookPath[CPathBasic::MAXPATHLENGTH];
    //记录是否需要刷新,默认为True
    bool m_isNeedRefesh;
    //当前hook渲染完成后是否需要释放渲染时使用的图片资源,默认为True需要释放
    bool m_needReleasePic;
  private: 
    //当前hook是否是显示状态 注：该变量主要是当前hook作为其它hook的子hook时调用。
    bool m_isShown;
    //当前hook的子hook列表，使用map存储
    AggHookMap m_childHookList;
    /// 新的控件存放结构
    GuiElements m_guiElements; 
    /// 只存放要刷新的按钮控件（暂时只是在设计阶段，还没有使用）
    std::vector<GuiElement*> m_renderElements;
    /// 焦点文本
    char m_focusText[MAXELEMENTNAME];
    /// 存放需要焦点变色的控件
    FocusTextElementsMap m_focusTextElements;
    //GUI日志输出对象
    static CDbgLogger m_guiLogger;
   //记录按下的按钮的位置。
    static CGeoPoint<short> m_downScrPoint;
   //保存Hook的堆栈
    static std::vector<int> m_hookTypeStack; 
  };

  //焦点文字变色类封装
  class CFocusTextElement
  {
  public:
    CFocusTextElement();
    CFocusTextElement(CViewHook::GuiElement* focusElement, const unsigned char* fkey);
    virtual ~CFocusTextElement() {}
  public:
    /*
    * 设置需要焦点变色的元素及变色信息
    * focusElement 需要焦点变色的原色
    * 焦点变色文本的位置信息列表
    */
    void SetFocusKey(CViewHook::GuiElement* focusElement, const unsigned char* fkey);
    /*
    * 设置需要焦点变色的元素及变色信息
    * 焦点变色文本的位置信息列表
    */
    void SetFocusKey(const unsigned char* fkey);
    /*
    * 读取焦点文字位置信息
    * focusElement 需要焦点变色的原色
    */
    unsigned char* GetFocusKey();
    /*
    * 判断传入的Element和当前Element是否是同一个
    * otherElement 其它要变色的元素
    */
    bool IsSameElement(const CViewHook::GuiElement* otherElement);
  private:
    //存放需要实现焦点变色的元素信息
    CViewHook::GuiElement* m_focusElement;
    //存放需要焦点变色的文字的位置信息
    unsigned char m_fTextPosList[CViewHook::MAXELEMENTNAME];

  };
}

#endif