#ifndef _UEXML_XMLNODE_H_
#define _UEXML_XMLNODE_H_

#include <assert.h>
#include <string>
#include <list>
#include <istream>
#include "irrXML.h"
#include "irrString.h"
#include "irrArray.h"

using irr::io::EXML_NODE;
using irr::core::array;

namespace UeXml
{
  //XML基类，供扩展使用
  template<class char_type>
  class CXmlBase
  {
  public:
    CXmlBase() {}
    ~CXmlBase() {}
  protected:
    irr::core::string<char_type> m_node_name;
    irr::core::string<char_type> m_node_value;
  };

  template<class char_type>
  class CXmlNode : public CXmlBase<char_type>
  {
  public:
    CXmlNode(irr::io::EXML_NODE node_type, CXmlNode<char_type>* node_parent) : m_node_type(node_type), m_parent(node_parent) {}
    ~CXmlNode() 
    {
      ClearChild();
      ClearAttribute();
      // 注意父节点是外部传入的指针，这里不能释放，而是由外部负责释放
      m_parent = NULL;
    }
  public:
    struct SAttribute
    {
      irr::core::string<char_type> m_name;
      irr::core::string<char_type> m_value;
    };

    // 设置当前节点的名称
    void SetName(irr::core::string<char_type>& node_name)
    {
      m_node_name = node_name;
    }

    void SetName(const char_type* node_name)
    {
      m_node_name = node_name;
    }

    // 设置当前节点的值
    void SetValue(irr::core::string<char_type>& node_value)
    {
      m_node_value = node_value;
    }

    void SetValue(const char_type* node_value)
    {
      m_node_value = node_value;
    }

    // 添加当前节点的子节点
    CXmlNode<char_type>* AddChild(const char_type* node_name = NULL, EXML_NODE node_type = EXN_ELEMENT)
    {
      if (node_type != EXN_COMMENT && node_type != EXN_ELEMENT)
      {
        return NULL;
      }
      CXmlNode<char_type>* child = new CXmlNode<char_type>(node_type, this);
      if (node_name != NULL)
      {
        child->SetName(node_name);
      }
      m_children.push_back(child);
      return child;
    }

    // 添加当前节点的子节点
    CXmlNode<char_type>* AddChild(irr::core::string<char_type>& node_name = NULL, EXML_NODE node_type = EXN_ELEMENT)
    {
      if (node_type != EXN_COMMENT && node_type != EXN_ELEMENT)
      {
        return NULL;
      }
      CXmlNode<char_type>* child = new CXmlNode<char_type>(node_type, this);
      if (node_name != NULL)
      {
        child->SetName(node_name);
      }
      m_children.push_back(child);
      return child;
    }

    // 添加当前节点的属性值
    void AddAttribute(const char_type* name, const char_type* value)
    {
      SAttribute attribute;
      attribute.m_name = name;
      attribute.m_value = value;
      m_attribute_list.push_back(attribute);
    }

    // 添加当前节点的属性值
    void AddAttribute(irr::core::string<char_type> name, irr::core::string<char_type> value)
    {
      SAttribute attribute;
      attribute.m_name = name;
      attribute.m_value = value;
      m_attribute_list.push_back(attribute);
    }

    // 删除当前节点的所有子节点
    void ClearChild()
    {
      for (std::list<CXmlNode<char_type>*>::iterator iter = m_children.begin();
           iter != m_children.end();
           ++iter)
      {
        CXmlNode<char_type>* child = *iter;
        assert(child != NULL);
        delete child;
      }
      m_children.clear();
    }

    // 清除当前节点的属性列表
    inline void ClearAttribute()
    {
      m_attribute_list.clear();
    }

    inline bool HasParent()
    {
      return NULL != m_parent;
    }

    // 获取当前节点的父节点
    inline CXmlNode<char_type>* GetParent()
    {
      return m_parent;
    }

    //读取当前节点的子节点(注意这里返回的类型为常量指针，并且该函数是个常量成员函数)
    inline std::list<CXmlNode<char_type>*>* GetChildren()
    {
       return &m_children;
    }

    //根据节点名称从指定节点及子节点中查找
    CXmlNode<char_type>* GetNode(const std::string& nodename)
    {
      std::list<CXmlNode<char_type>*>::iterator iter = m_children.begin();
      for (; iter != m_children.end(); ++iter)
      {
        CXmlNode<char_type>* child_node = *iter;
        const char* child_name_ = reinterpret_cast<const char_type*>(child_node->GetNodeName());
        if (strcmp(child_name_, nodename.c_str()) == 0)
        {
          //如果在子节点中找到则返回找到节点
          return child_node;
        }
        else
        {
          if (child_node->HasChild())
          {
            //如果当前节点找不到,则在其子节点中继续递归查找
            CXmlNode<char_type>* find_node = FindNode(child_node, nodename);
            if (find_node != NULL)
            {
              //如果在子节点的子节点找到则返回找到节点
              return find_node;
            }
          }
        }
      }
      return NULL;
    }

    //读取节点的名称
    inline const char_type* GetNodeName() const
    {
      return m_node_name.c_str();
    }

    inline const char_type* GetNodeValue() const
    {
      return m_node_value.c_str();
    }

    // 获取节点类型
    inline EXML_NODE GetNodeType() const
    {
      return m_node_type;
    }

    // 获取子节点的个数
    inline size_t GetChildCount() const
    {
      return m_children.size();
    }

    // 当前节点是否有子节点
    inline bool HasChild() const
    {
      return !m_children.empty();
    }

    // 获取属性值的字段数
    inline int GetAttributeCount() const
    {
      return m_attribute_list.size();
    }

    // 根据属性名称读取属性值
    const char_type* GetAttributeByName(const std::string& name) const
    {
      if (name.empty()) 
      {
        return NULL;
      }

      irr::core::string<char_type> name_ = name.c_str();

      for (int i=0; i<(int)m_attribute_list.size(); ++i)
      {
        if (m_attribute_list[i].m_name == name_)
        {
          return m_attribute_list[i].m_value.c_str();
        }
      }
      return NULL;
    }

    // 根据属性名称读取属性值
    const char_type* GetAttributeByName(const char_type* name) const
    {
      if (!name) 
      {
        return NULL;
      }

      irr::core::string<char_type> name_ = name;

      for (int i=0; i<(int)m_attribute_list.size(); ++i)
      {
        if (m_attribute_list[i].m_name == name_)
        {
          return m_attribute_list[i].m_value.c_str();
        }
      }
      return NULL;
    }
  private:
    //从指定节点及子节点的子节点查找指定名称的节点
    CXmlNode<char_type>* FindNode(CXmlNode<char_type>* xmlnode, const std::string& nodename)
    {
      if (!xmlnode->HasChild())
      {
        return NULL;
      }
      return xmlnode->GetNode(nodename);
    }
  private:
    // 当前节点类型
    EXML_NODE m_node_type;
    // 当期节点的父节点，在构造函数时给父节点赋值
    CXmlNode<char_type>* m_parent;
    // 当前节点的子节点
    std::list<CXmlNode<char_type>*> m_children;
    // 当前节点属性列表
    array<SAttribute> m_attribute_list;
  };
}
#endif