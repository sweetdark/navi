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
#include "xmlreader.h"
using namespace UeXml;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
CXmlReader::CXmlReader(const char *xmlFile) : m_xml(0)
{
  Open(xmlFile);
}

/**
*
**/
CXmlReader::~CXmlReader()
{
  Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
* Note:
* For IRRXML opensource, it seems can't work correctly if not recover the P pointer which is used to analyze different
* parts of one XML file. So, currently add one function "reset()" in order to get correct thing
**/
void CXmlReader::GetContent(const std::string &nodeName, std::string &content)
{
  // parse the file until end reached
  const char *oneName = nodeName.c_str();
  m_xml->reset();

  while(m_xml && m_xml->read())
  {
    switch(m_xml->getNodeType())
    {
    case EXN_ELEMENT:
      {
        if (!::strcmp(oneName, m_xml->getNodeName()))
        {
          m_xml->read();
          content = m_xml->getNodeData();

          return;
        }
      }
      break;
    }
  }
}

/**
*
**/
void CXmlReader::GetAttribute(const std::string &nodeName, const std::string &nodeField, std::string &value)
{
  // parse the file until end reached
  const char *oneName = nodeName.c_str();
  m_xml->reset();

  while(m_xml && m_xml->read())
  {
    switch(m_xml->getNodeType())
    {
    case EXN_ELEMENT:
      {
        if (!::strcmp(oneName, m_xml->getNodeName()) /*&& m_xml->getAttributeCount()*/)
        {
          value = m_xml->getAttributeValueSafe(nodeField.c_str());
          return;
        }
      }
      break;
    }
  }
}

/**
*
**/
bool CXmlReader::IsExist(const std::string &nodeName)
{
  // parse the file until end reached
  const char *oneName = nodeName.c_str();
  m_xml->reset();
  while(m_xml && m_xml->read())
  {
    switch(m_xml->getNodeType())
    {
    case EXN_ELEMENT:
      {
        if(!::strcmp(oneName, m_xml->getNodeName()))
        {
          return true;
        }
      }
      break;
    }
  }

  return false;
}

/**
*
*/
bool CXmlReader::Open(const char *xmlFile)
{
  //
  Release();

  //
  m_xml = createIrrXMLReader(xmlFile);
  assert(m_xml);

  return m_xml != 0;
}