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
#include "xmlwriter.h"
#include "xmlelement.h"
#include "xmldocument.h"
using namespace UeXml;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
///
/**
*
**/
CXmlWriter::CXmlWriter(const tstring &fileName, std::string &encoding) : m_xml(new CXmlDocument(fileName, encoding))
{
}

/**
*
**/
CXmlWriter::~CXmlWriter()
{
  delete m_xml;
  m_xml = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
void CXmlWriter::SetStream(const tstring &fileName)
{
  m_xml->SetStream(fileName);
}

/**
*
**/
void CXmlWriter::SetEndcoding(std::string &encoding)
{
  assert(m_xml);
  m_xml->SetEncoding(encoding);
}

/**
* Sets the XSL style sheet used.
*
* \param styleSheet: Name of the style sheet used. If empty, then no style sheet is used (default).
*/
void CXmlWriter::SetStyleSheet(const std::string &styleSheet)
{
  assert(m_xml);
  m_xml->SetStyleSheet(styleSheet);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
void CXmlWriter::SetRootElement(CXmlElement *rootElement)
{
  assert(m_xml);
  m_xml->SetRootElement(rootElement);
}

/**
*
**/
void CXmlWriter::AddElement(CXmlElement *oneElement)
{
  assert(m_xml);
  m_xml->AddElement(oneElement);
}

/**
*
**/
void CXmlWriter::RemoveElement(int idx)
{
  assert(m_xml);
  m_xml->RemoveElement(idx);
}

/**
*
**/
CXmlElement *CXmlWriter::GetElement(int idx)
{
  assert(m_xml);
  return m_xml->GetElement(idx);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
void CXmlWriter::Write()
{
  assert(m_xml);
  m_xml->Write();
}

