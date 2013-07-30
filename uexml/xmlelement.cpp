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
#include "xmlelement.h"
using namespace UeXml;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
static std::string Int2Str(int value)
{
  std::ostringstream stream;
  stream << value;

  return stream.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor implementation
// 
/**
*
*/
CXmlElement::CXmlElement(std::string elementName, std::string content) : m_name(elementName), m_content(content)
{
}

/**
*
*/
CXmlElement::CXmlElement(std::string elementName, int numericContent) : m_name(elementName)
{
  SetContent(numericContent);
}

/**
*
*/
CXmlElement::~CXmlElement()
{
  Elements::iterator itNode = m_elements.begin();
  while(itNode != m_elements.end())
  {
    CXmlElement *element = *itNode++;
    delete element;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
/**
*
*/
std::string CXmlElement::Name() const
{
  return m_name;
}

/**
*
*/
std::string CXmlElement::Content() const
{
  return m_content;
}

/**
*
*/
void CXmlElement::SetName(const std::string &name)
{
  m_name = name;
}

/**
*
*/
void CXmlElement::SetContent(const std::string &content)
{
  m_content = content;
}

/**
*
*/
void CXmlElement::SetContent(int numericContent)
{
  m_content = Int2Str(numericContent);
}

/**
*
*/
void CXmlElement::AddAttribute(std::string attributeName, std::string value)
{
  m_attributes.push_back(Attribute(attributeName, value));
}

/**
*
*/
void CXmlElement::AddAttribute(std::string attributeName, int numericValue)
{
  AddAttribute(attributeName, Int2Str(numericValue));
}

/**
*
*/
void CXmlElement::AddElement(CXmlElement *node, unsigned short position)
{
  if(!position)
  {
    m_elements.push_back(node);
  }
  else
  {
    m_elements.push_front(node);
  }
}

/**
*
*/
void CXmlElement::RemoveElement(int idx)
{
  m_elements.erase(m_elements.begin() + idx);
}

/**
*
*/
int CXmlElement::ElementCount() const
{
  return static_cast<int>(m_elements.size());
}

/**
*
*/
CXmlElement *CXmlElement::ElementAt(int index) const
{
  if(index < 0  ||  index >= ElementCount())
  {
    throw std::invalid_argument( "XmlElement::elementAt(), out of range index" );
  }

  return m_elements[index];
}

/**
*
*/
CXmlElement *CXmlElement::ElementFor(const std::string &name) const
{
  Elements::const_iterator itElement = m_elements.begin();
  for(; itElement != m_elements.end(); ++itElement )
  {
    if((*itElement)->Name() == name)
    {
      return *itElement;
    }
  }

  throw std::invalid_argument("XmlElement::elementFor(), not matching child element found");
  return 0;  // make some compilers happy.
}

/**
*
*/
std::string CXmlElement::ToString(const std::string &indent) const
{
  //
  std::string element(indent);
  element += "<";
  element += m_name;
  if(!m_attributes.empty())
  {
    element += " ";
    element += AttributesAsString();
  }
  element += ">";

  if(!m_elements.empty())
  {
    element += "\n";

    std::string subNodeIndent(indent + "  ");
    Elements::const_iterator itNode = m_elements.begin();
    while (itNode != m_elements.end())
    {
      const CXmlElement *node = *itNode++;
      element += node->ToString(subNodeIndent);
    }

    element += indent;
  }

  if(!m_content.empty())
  {
    element += Escape(m_content);
    if(!m_elements.empty())
    {
      element += "\n";
      element += indent;
    }
  }

  element += "</";
  element += m_name;
  element += ">\n";

  return element;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private methods
/**
* The recursive way to generate all name,content of elements
*/
std::string CXmlElement::AttributesAsString() const
{
  std::string attributes;
  Attributes::const_iterator itAttribute = m_attributes.begin();
  while(itAttribute != m_attributes.end())
  {
    if(!attributes.empty())
    {
      attributes += " ";
    }

    const Attribute &attribute = *itAttribute++;
    attributes += attribute.first;
    attributes += "=\"";
    attributes += Escape(attribute.second);
    attributes += "\"";
  }

  return attributes;
}

/**
*
*/
std::string CXmlElement::Escape(std::string value) const
{
  std::string escaped;
  unsigned int index =0;
  for(; index < value.length(); ++index)
  {
    char c = value[index];
    switch (c)    // escape all predefined XML entity (safe?)
    {
    case '<': 
      escaped += "&lt;";
      break;
    case '>': 
      escaped += "&gt;";
      break;
    case '&': 
      escaped += "&amp;";
      break;
    case '\'': 
      escaped += "&apos;";
      break;
    case '"': 
      escaped += "&quot;";
      break;
    default:
      escaped += c;
    }
  }

  return escaped;
}
