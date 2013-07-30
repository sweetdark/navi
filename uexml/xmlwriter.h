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
#ifndef _UEXML_XMLWRITER_H
#define _UEXML_XMLWRITER_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEXML_BASE_H
#include "uexml.h"
#endif

// Refer to tstring etc 
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

/// UeXml namespace
namespace UeXml
{
  // Forward classes
  class CXmlElement;
  class CXmlDocument;

  /*! \class CXmlWriter
  *
  * \brief XML编辑接口，用于写入节点数据到XML文件
  *
  * This class mainly benefits from XML part of CPPUNIT test framework
  */
  class UEXML_CLASS CXmlWriter
  {
    /// Disallow implicitly creation

  public:
    //
    //
    //
    /*!
    * \brief 构造函数
    *
    * \param fileName xml文件名
    * \param encoding xml编码
    */
    CXmlWriter(const tstring &fileName, std::string &encoding = std::string("ISO-8859-1"));

    /*!
    * \brief 析构函数
    */
    ~CXmlWriter();

  public:
    //
    // 设置
    //
    /*!
    * \brief 设置输出流
    *
    * \param filename 写入的xml文件名
    */
    void SetStream(const tstring &fileName);


    /*!
    * \brief 设置编码
    *
    * \param encoding xml编码
    */
    void SetEndcoding(std::string &encoding);

    /*!
    * \brief Sets the XSL style sheet used.
    *
    * \param styleSheet
    * Name of the style sheet used. If empty, then no style sheet is used (default).
    */
    void SetStyleSheet(const std::string &styleSheet);

    //
    // Element操作
    //
    /*!
    * \brief 设置根节点
    *
    * \param rootElement 指向根节点的指针
    */
    void SetRootElement(CXmlElement *rootElement);

    /*!
    * \brief 添加节点
    *
    * \param oneElement 指向添加节点的指针
    */
    void AddElement(CXmlElement *oneElement);

    /*!
    * \brief 删除节点
    *
    * \param idx 要删除节点的标识
    */
    void RemoveElement(int idx);

    /*!
    * \brief 获取指定的节点
    *
    * \param idx 节点标识
    */
    CXmlElement *GetElement(int idx);

    //
    //
    //
    /*!
    * \brief 执行写入操作
    */
    void Write();

  private:
    /// Delgate XML format to below object
    CXmlDocument *m_xml;
  };
}
#endif