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
#include "xmldocument.h"
#include "xmlelement.h"
using namespace UeXml;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and deconstructor implementation
// 
/**
*
*/
CXmlDocument::CXmlDocument(const tstring &fileName, const std::string &encoding, const std::string &styleSheet)
: m_rootElement(new CXmlElement("DummyRoot")), m_styleSheet(styleSheet)
{
  SetStream(fileName);
  SetEncoding(encoding);
}

/**
*
*/
CXmlDocument::~CXmlDocument()
{
  m_stream.close();
  delete m_rootElement;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//
/**
*
*/
std::string CXmlDocument::Encoding() const
{
  return m_encoding;
}

/**
* The encoding way to crroectly explain the characters included in this XML file
*/
void CXmlDocument::SetEncoding(const std::string &encoding)
{
  m_encoding = encoding.empty() ? std::string("ISO-8859-1") : encoding;
}

/**
*
*/
std::string CXmlDocument::StyleSheet() const
{
  return m_styleSheet;
}

/**
*
*/
void CXmlDocument::SetStyleSheet(const std::string &styleSheet)
{
  m_styleSheet = styleSheet;
}

/**
*
**/
void CXmlDocument::SetStream(const tstring &fileName)
{
  // Close existed file
  if(m_stream.is_open())
  {
    m_stream.close();
  }

  m_fileName = fileName;
  m_stream.open(fileName.c_str());
  assert(m_stream.is_open());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* One and only one root element in one XML file
*/
void CXmlDocument::SetRootElement(const CXmlElement *rootElement)
{
  if (rootElement == m_rootElement)
  {
    return;
  }

  delete m_rootElement;
  m_rootElement = const_cast<CXmlElement *>(rootElement);
}

/**
*
*/
CXmlElement &CXmlDocument::RootElement() const
{
  return *m_rootElement;
}

/**
*
**/
void CXmlDocument::AddElement(CXmlElement *oneElement)
{
  assert(m_rootElement);
  m_rootElement->AddElement(oneElement);
}

/**
*
**/
void CXmlDocument::RemoveElement(int idx)
{
  assert(m_rootElement);
  m_rootElement->RemoveElement(idx);
}

/**
*
**/
CXmlElement *CXmlDocument::GetElement(int idx)
{
  return m_rootElement->ElementAt(idx);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
std::string CXmlDocument::ToString() const
{
  std::string asString = "<?xml version=\"1.0\" "
    "encoding='" + m_encoding + "' standalone='yes' ?>\n";

  if (!m_styleSheet.empty())
  {
    asString += "<?xml-stylesheet type=\"text/xsl\" href=\"" + m_styleSheet + "\"?>\n";
  }

  asString += m_rootElement->ToString();
  return asString;
}

/**
*
**/
void CXmlDocument::Write()
{
  // Close existed file
  if(m_stream.is_open())
  {
    m_stream.close();
  }

  m_stream.open(m_fileName.c_str());
  m_stream << ToString().c_str();
  m_stream.close();
}