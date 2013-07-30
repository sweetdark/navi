#ifndef _UEXML_XMLREADEREX_H
#define _UEXML_XMLREADEREX_H

#include "xmlnode.h"
#include "CXMLReaderImpl.h"

using namespace irr::io;

/// 提供针对XML文档的一系列操作接口
namespace UeXml
{
	/*! \class CXmlReaderEx
	*
	* \brief XML解析器，用于从xml文件中获取节点数据
	*/
  template<class char_type>
  class CXmlReaderEx
	{
	public:
		/*!
		* \brief 默认构造函数
		*/
    CXmlReaderEx() : m_xml_reader(NULL), m_xmlroot(NULL)
		{
      
		}
		/*!
		* \brief 构造函数
		*
		* \param xmlFile xml文件名
		*/
		CXmlReaderEx(const char *xmlFile) : m_xml_reader(NULL), m_xmlroot(NULL)
    {
      OpenXML(xmlFile);
      ParseXML();
    }

		/*!
		* \brief 析构函数
		*/
    ~CXmlReaderEx() 
    {
      Release();
    }
	public:
    /*
		* \brief 打开要读取的xml文件
		*
		* \param xmlFile xml文件名
		*
		* \return 打开成功则返回true，否则返回false。
		*/
		bool OpenXML(const char *xmlFile)
    {
      Release();
      m_xmlroot = new CXmlNode<char_type>(EXN_NONE, NULL);
      std::string node_name = "XMLDOC";
      m_xmlroot->SetName(node_name.c_str());
      assert(m_xmlroot);
      m_xml_reader = new irr::io::CXMLReaderImpl<char_type, IXMLBase>(new CFileReadCallBack(xmlFile));       
      assert(m_xml_reader);
      return m_xml_reader != NULL;
    }

		/*!
		* \brief 判断xml文件是否打开
		*
		* \return 如果xml已经打开则返回true，否则返回false。
		*/
		bool IsOpened()
		{
			return m_xml_reader != NULL;
		}

		/*!
		* \brief 判断是否存在指定的节点
		*
		* \param nodeName 节点名称
		*
		* \return 如果存在则返回true，否则返回false。
		*/
		bool IsNodeExist(const std::string &nodeName)
    {
      return false;
    }

		/*!
		* \brief 关闭打开的文件，释放资源。
		*/
		void Release()
		{
			if(m_xml_reader)
			{
				delete m_xml_reader;
			}
      m_xml_reader = NULL;
      if (m_xmlroot)
      {
        delete m_xmlroot;
      }			
      m_xmlroot = NULL;
		}
    /*
    *解析XML文件
    */
    void ParseXML() 
    {
      CXmlNode<char_type>* current_node = m_xmlroot;
      // parse the file until end reached
      m_xml_reader->reset();
      while(m_xml_reader && m_xml_reader->read())
      {
        EXML_NODE nodetype = m_xml_reader->getNodeType();
        const char_type *nodename = m_xml_reader->getNodeName();
        switch(nodetype)
        {
        case EXN_ELEMENT:
          {            
            const char_type* node_name_ = reinterpret_cast<const char_type*>(m_xml_reader->getNodeName());
            CXmlNode<char_type>* new_node = current_node->AddChild(node_name_, EXN_ELEMENT);

            int attribute_count = m_xml_reader->getAttributeCount();
             for (int i = 0; i < attribute_count; ++i)
             {  
                const char_type* attribute_name = m_xml_reader->getAttributeName(i);
                const char_type* attribute_value = m_xml_reader->getAttributeValue(i);
                new_node->AddAttribute(attribute_name, attribute_value);
             }
            //给当前节点添加属性
            //如果是节点
            m_xml_reader->read();
            new_node->SetValue(m_xml_reader->getNodeData());
            //当前节点指针指向新添加的子节点
            current_node = new_node;
            break;
          }
        case EXN_COMMENT:
          {
            //如果是注释
            current_node->AddChild(m_xml_reader->getNodeName(), EXN_COMMENT);
            break;
          }
        case EXN_ELEMENT_END:
          {
            //如果是节点结束，则将当前节点指针指向父节点
            current_node = current_node->GetParent();
            break;
          }
        }
      }
    }

    // 根据节点的名称读取节点
    CXmlNode<char_type>* GetNode(const std::string &nodeName)
    {
       return m_xmlroot->GetNode(nodeName);
    }

    // 根据节点名称读取当前节点的所有子节点
    std::list<CXmlNode<char_type>*>* GetNodes(const std::string &nodeName)
    {
      CXmlNode<char_type>* xmlnode = GetNode(nodeName);
      if (xmlnode && xmlnode->HasChild())
      {
        return xmlnode->GetChildren();  
      }
      return NULL;
    }

		/*!
		* \brief 获取节点内容
		*
		* \param nodeName 节点名称
		* \param content 用于返回节点内容的字符串
		*/
		const char_type* GetContent(const std::string &nodeName, std::string &content) const
    {
      CXmlNode<char_type>* find_node = m_xmlroot->GetNode(nodeName);
      if (find_node)
      {
        return find_node->GetNodeValue();
      }
    }

		/*!
		* \brief 获取节点属性
		*
		* \param nodeName 节点名称
		* \param nodeField 属性字段
		* \param value 返回的属性值
		*/
		const char_type* GetAttribute(const std::string &nodeName, const std::string &nodeField) const
    {
      CXmlNode<char_type>* xmlNode = m_xmlroot->GetNode(nodeName);
      if (xmlNode)
      {
        return xmlNode->GetAttributeByName(nodeField);
      }
      return NULL;
    }
	private:
		// Delegate to XML reader
		IIrrXMLReader<char_type, IXMLBase>* m_xml_reader;
    // XML文件根节点
    CXmlNode<char_type>* m_xmlroot;
	};
}

#endif