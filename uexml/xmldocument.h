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
#ifndef _UEXML_XMLDOCUMENT_H
#define _UEXML_XMLDOCUMENT_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEXML_BASE_H
#include "uexml.h"
#endif

// Refer to std::string etc definition
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

//
#pragma warning(push)
#pragma warning(disable: 4251)  // X needs to have dll-interface to be used by clients of class Z

/// UeXml namespace
namespace UeXml
{
  // Forward declaration
  class CXmlElement;

  /*! \class CXmlDocument
  *
  * \brief XML文档定义，是对xml文件的抽象，一个CXmlDocument对象对应一个xml文件
  *
  * A CXmlDocument represents a XML file. It holds a pointer on the root XmlElement of the document. 
  * It also holds the encoding and style sheet used.   
  */
  class UEXML_CLASS CXmlDocument
  {
  public:
    //
    // Constructors and deconstructor
    //
    /*!
    * \brief Constructs a CXmlDocument object.
    *
    * \param Encoding used in the XML file (default is Latin-1, ISO-8859-1 ).
    * \param Name of the XSL style sheet file used. If empty then no style sheet will be specified in the output.
    */
    CXmlDocument(const tstring &fileName, const std::string &encoding = "", const std::string &styleSheet = "");

  private:
    // Doesn't permit outside declare this class as stack variable except for CXmlOutputter
    friend class CXmlWriter;

    virtual ~CXmlDocument();

  private:
    /**
    * Prevents the use of xml document
    */
    CXmlDocument(const CXmlDocument &copy);

    /**
    * Prevents the use of xml document
    */
    void operator =(const CXmlDocument &copy);

  public:
    /*!
    * \brief 获取xml编码
    */
    std::string Encoding() const;

    /*!
    * \brief 设置xml编码
    *
    * \param encoding XML文档的编码
    */
    void SetEncoding(const std::string &encoding = "");

    /*!
    * \brief 获取StyleSheet
    */
    std::string StyleSheet() const;

    /*!
    * \brief 设置StyleSheet
    *
    * \param styleSheet
    */
    void SetStyleSheet(const std::string &styleSheet = "");

    /*!
    * \brief 设置文件流
    *
    * \param fileName xml文件名
    */
    void SetStream(const tstring &fileName);

    //
    //
    //
    /*!
    * \brief 设置根节点
    *
    * \param rootElement 指向根节点对象的指针
    */
    void SetRootElement(const CXmlElement *rootElement);

    /*!
    * \brief 获取根节点
    */
    CXmlElement &RootElement() const;

    /*!
    * \brief 添加节点
    *
    * \param oneElement 指向添加节点的指针
    */
    void AddElement(CXmlElement *oneElement);

    /*!
    * \brief 删除指定节点
    *
    * \param idx 要删除节点的索引
    */
    void RemoveElement(int idx);

    /*!
    * \brief 获取指定节点
    *
    * \param idx 节点索引
    */
    CXmlElement *GetElement(int idx);

    //
    //
    //
    /*!
    * \brief 执行写入操作
    */
    void Write();

    /*!
    * \brief 获得xml文档的字符串表示
    */
    std::string ToString() const;

  protected:
    // Concrete outputer, maybe disk file, memory,etc
    tstring m_fileName;
    std::ofstream m_stream;

    // The contents of one xml document
    std::string m_encoding;
    std::string m_styleSheet;
    CXmlElement *m_rootElement;
  };
}

#pragma warning( pop )
#endif
