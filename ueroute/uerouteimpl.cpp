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
// Refer to UeRoute package
#include "uerouteimpl.h"
#include "routebasic.h"
#include "routedecorator.h"
#include "blockeddecorator.h"
using namespace UeRoute;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
*/
CUeRouteImpl::CUeRouteImpl() : m_parent(0), m_guider(0)
{
}

/**
*
*/
CUeRouteImpl::~CUeRouteImpl()
{
  // Try to let other collegues release memory currently not being be used
  decr_itr first = m_decorators.begin();
  decr_itr end = m_decorators.end();
  for(; first != end; first++)
  {
    delete (*first).second;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
unsigned int CUeRouteImpl::AddDecorator(short type, CRouteDecorator *oneDecorator)
{
  //
  if(oneDecorator && m_decorators.find(type) == m_decorators.end())
  {
    m_decorators.insert(DecoratorMap::value_type(type, oneDecorator));
    return PEC_Success;
  }

  //
  return PEC_InvalidDecorator;
}

/**
*
**/
unsigned int CUeRouteImpl::RemoveDecorator(short type)
{
  //
  decr_itr oneItr = m_decorators.find(type);
  if(oneItr != m_decorators.end())
  {
    delete (*oneItr).second;
    m_decorators.erase(oneItr);

    //
    return PEC_Success;
  }

  //
  return PEC_InvalidDecorator;
}

/**
*
**/
inline unsigned int CUeRouteImpl::SetBlock(CGeoPoint<long> &onePos, bool isRemoved)
{
  //
  decr_itr oneItr = m_decorators.find(DT_Blocked);
  if(oneItr == m_decorators.end())
  {
    return PEC_NotHaveBlockDecorator;
  }
  CBlockedDecorator *decorator = dynamic_cast<CBlockedDecorator *>(oneItr->second);
  assert(decorator);

  //
  if(decorator)
  {
    return decorator->SetBlock(onePos, isRemoved);
  }

  return PEC_NotHaveBlockDecorator;
}

/**
*
**/
unsigned int CUeRouteImpl::SetBlock(CGeoRect<long> &oneExtent, bool isRemoved)
{
  //
  decr_itr oneItr = m_decorators.find(DT_Blocked);
  if(oneItr == m_decorators.end())
  {
    return PEC_NotHaveBlockDecorator;
  }
  CBlockedDecorator *decorator = dynamic_cast<CBlockedDecorator *>(oneItr->second);
  assert(decorator);

  //
  if(decorator)
  {
    return decorator->SetBlock(oneExtent, isRemoved);
  }

  return PEC_NotHaveBlockDecorator;
}

/**
*
**/
unsigned int CUeRouteImpl::SetBlock(CMemVector &ids, bool isRemoved)
{
  //
  decr_itr oneItr = m_decorators.find(DT_Blocked);
  if(oneItr == m_decorators.end())
  {
    return PEC_NotHaveBlockDecorator;
  }
  CBlockedDecorator *decorator = dynamic_cast<CBlockedDecorator *>(oneItr->second);
  assert(decorator);

  //
  if(decorator)
  {
    return decorator->SetBlock(ids, isRemoved);
  }

  return PEC_NotHaveBlockDecorator;
}

/**
*
**/
unsigned int CUeRouteImpl::SetBlock(bool isRemoved)
{
  //
  decr_itr oneItr = m_decorators.find(DT_Blocked);
  if(oneItr == m_decorators.end())
  {
    return PEC_NotHaveBlockDecorator;
  }
  CBlockedDecorator *decorator = dynamic_cast<CBlockedDecorator *>(oneItr->second);
  assert(decorator);

  //
  //
  if(decorator)
  {
    return decorator->SetBlock(isRemoved);
  }

  return PEC_NotHaveBlockDecorator;
}