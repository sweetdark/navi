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
/*
* Note: this class mainly refer to XmlElement encapsulated by CPPUNIT framework
*/
#ifndef _UEXML_XMLELEMENT_H
#define _UEXML_XMLELEMENT_H

// Refer to EXPORT & IMPORT macroes
#ifndef _UEXML_BASE_H
#include "uexml.h"
#endif

// Refer to std::string
#ifndef _PORTABILITY_DEF_H
#include "portability.h"
#endif

// Erase compile warnings
#pragma warning(push)
#pragma warning(disable: 4251)  // X needs to have dll-interface to be used by clients of class Z

/// UeXml namespace
namespace UeXml
{
  /*! \class CXmlElement
  *
  * \brief XML文档的节点定义
  * XML Element has:
  * - a name, specified on construction,
  * - a content, specified on construction (may be empty),
  * - zero or more attributes, added with addAttribute(),
  * - zero or more child elements, added with addElement().
  */
  class UEXML_CLASS CXmlElement
  {
  public:
    //
    // Constructors and deconstructor
    //
    /*!
    * \brief Constructs an element with the specified name and string content.
    *
    * \param elementName Name of the element. Must not be empty. 
    * \param content Content of the element.
    */
    CXmlElement(std::string elementName, std::string content = "");

    /*!
    * \brief Constructs an element with the specified name and numeric content.
    *
    * \param elementName Name of the element. Must not be empty.
    * \param numericContent numericContent Content of the element.
    */
    CXmlElement(std::string elementName, int numericContent);

  private:
    // Doesn't permit other objects to declare this object as stack variable
    friend class CXmlDocument;

    /**
    * Destructs the element and its child elements.
    */
    virtual ~CXmlElement();

  public:
    //
    // Basic methods
    //
    /*!
    * \brief Returns the name of the element.
    *
    * \return string 节点名称
    */
    std::string Name() const;

    /*!
    * \brief Returns the content of the element.
    *
    * \return string 节点内容
    */
    std::string Content() const;

    /*!
    * \brief Sets the name of the element.
    *
    * \param name 节点名称
    */
    void SetName(const std::string &name);

    /*!
    * \brief Sets the content of the element.
    *
    * \param content New content for the element.
    */
    void SetContent(const std::string &content);

    /*!
    * \brief overload void setContent( const std::string &content )
    */
    void SetContent(int numericContent);

    /*!
    * \brief Adds an attribute with the specified string value.
    *
    * \param attributeName Name of the attribute. Must not be an empty.
    * \param value Value of the attribute.
    */
    void AddAttribute(std::string attributeName, std::string value);

    /*!
    * \brief Adds an attribute with the specified numeric value.
    *
    * \param attributeName Name of the attribute. Must not be empty.
    * \param numericValue Numeric value of the attribute.
    */
    void AddAttribute(std::string attributeName, int numericValue);

    //
    // Methods for element composition
    //
    /*!
    * \brief Adds a child element to the element.
    *
    * \param element Child element to add. Must not be NULL.
    */
    void AddElement(CXmlElement *element, unsigned short position = 0);

    /*!
    * \brief Remove a child element.
    *
    * \param idx Child element to remove.
    */
    void RemoveElement(int idx);

    /*!
    * \brief Returns the number of child elements.
    */
    int ElementCount() const;

    /*!
    * \brief Returns the child element at the specified index.
    *
    * \param index Zero based index of the element to return. 
    */
    CXmlElement *ElementAt(int index) const;

    /*!
    * \brief Returns the first child element with the specified name.
    *
    * \param First child element found which is named by name.
    */
    CXmlElement *ElementFor(const std::string &name) const;

    /*!
    * \brief Returns a XML string that represents the element.
    *
    * \param String of spaces representing the amount of 'indent'.
    * \return XML string that represents the element, its attributes and its child elements.
    */
    std::string ToString(const std::string &indent = "") const;

  private:
    /**
    *
    */
    std::string AttributesAsString() const;

    /**
    *
    */
    std::string Escape(std::string value) const;

  private:
    // New types or traits
    typedef std::pair<std::string,std::string> Attribute;
    typedef std::deque<Attribute> Attributes;
    typedef std::deque<CXmlElement *> Elements;

    // Element information
    std::string m_name;
    std::string m_content;
    Attributes m_attributes;

    // Compositees
    Elements m_elements;
  };
}

#pragma warning( pop )
#endif
