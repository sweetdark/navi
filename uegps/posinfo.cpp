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
#include "uebase\dbgmacro.h"
using namespace UeBase;

#include "posinfo.h"
#include "infopro.h"
using namespace UeGps;

//
std::vector<PosBasic> CPosInfo::m_positions;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
CPosInfo::CPosInfo(CPosInfo *parent, CInfoPro *infoPro) : m_parent(parent), m_infoPro(infoPro), m_type(PT_Unknown),
m_altAvg(-10000.), m_speedAvg(-10000.)
{
  m_positions.reserve(100);
}

/**
*
*/
CPosInfo::~CPosInfo()
{
  //
  m_parent = 0;

  //
  if(m_infoPro)
  {
    delete m_infoPro;
    m_infoPro = 0;
  }

  //
  m_positions.clear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
bool CPosInfo::AddInfo()
{
  // TODO: ...
  assert(false);
  return false;
}

/**
*
*/
bool CPosInfo::RemoveInfo()
{
  // TODO: ...
  assert(false);
  return false;
}

