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
#ifndef _UEXML_XMLREADER_H
#define _UEXML_XMLREADER_H

// Refer to EXPORT & IMPORT macro defintions
#ifndef _UEXML_BASE_H
#include "uexml.h"
#endif

// Refer to std::string
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Refer to IrrXmlReader
#ifndef __IRR_XML_H_INCLUDED__
#include "irrxml.h"
#endif
using namespace irr;
using namespace io;

/// UeXml namespace
/// 提供针对XML文档的一系列操作接口
namespace UeXml
{
  /*! \class CXmlReader
  *
  * \brief XML解析器，用于从xml文件中获取节点数据
  *
  * Directly read one XML node to get its attributes and its value and this class directly benefit from 
  * opensource IRRXML project
  */
  class UEXML_CLASS CXmlReader
  {
    ///
    CXmlReader(const CXmlReader &other);
    const CXmlReader &operator=(const CXmlReader &other);

  public:
    //
    // 构造&析构
    //
    /*!
    * \brief 默认构造函数
    */
    CXmlReader() : m_xml(0)
    {
    }

    /*!
    * \brief 构造函数
    *
    * \param xmlFile xml文件名
    */
    CXmlReader(const char *xmlFile);

    /*!
    * \brief 析构函数
    */
    ~CXmlReader();

  public:
    //
    //
    //
    /*!
    * \brief 打开要读取的xml文件
    *
    * \param xmlFile xml文件名
    *
    * \return 打开成功则返回true，否则返回false。
    */
    bool Open(const char *xmlFile);

    /*!
    * \brief 判断xml文件是否打开
    *
    * \return 如果xml已经打开则返回true，否则返回false。
    */
    bool IsOpened()
    {
      return m_xml != 0;
    }

    /*!
    * \brief 判断是否存在指定的节点
    *
    * \param nodeName 节点名称
    *
    * \return 如果存在则返回true，否则返回false。
    */
    bool IsExist(const std::string &nodeName);

    /*!
    * \brief 关闭打开的文件，释放资源。
    */
    void Release()
    {
      if(m_xml)
      {
        delete m_xml;
      }
      m_xml = 0;
    }

    //
    // 获取节点信息
    //
    /*!
    * \brief 获取节点内容
    *
    * \param nodeName 节点名称
    * \param content 用于返回节点内容的字符串
    */
    void GetContent(const std::string &nodeName, std::string &content);

    /*!
    * \brief 获取节点属性
    *
    * \param nodeName 节点名称
    * \param nodeField 属性字段
    * \param value 返回的属性值
    */
    void GetAttribute(const std::string &nodeName, const std::string &nodeField, std::string &value);

  private:
    // Delegate to XML reader
    IrrXMLReader *m_xml;
  };
}
#endif