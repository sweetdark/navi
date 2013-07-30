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
// Refer to UeBase package
#include "dbgmacro.h"
#include "uemapsio.h"
#include "pathconfig.h"
using namespace UeBase;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
*/

CUeMapsIO::CUeMapsIO() : m_fileBasic(CFileBasic::Get()), m_pathBasic(CPathBasic::Get())
{
  // Location of uemaps.ztt file
  /*TCHAR modulePath[CPathBasic::MAXPATHLENGTH] = {0, };
  m_pathBasic.GetModulePath(modulePath, CPathBasic::MAXPATHLENGTH);*/
  m_fileName = CPathConfig::GetResolutionPath(CPathConfig::RPK_MapstylePath);
  /*m_pathBasic.GetPathSeperator(m_fileName);
  m_fileName += _T("maps");
  m_pathBasic.GetPathSeperator(m_fileName);*/
  m_fileName += _T("uemaps.ztt");
}

/**
*
*/
CUeMapsIO::~CUeMapsIO()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
void CUeMapsIO::GetHeader(MapsHeader &mapsHeader)
{
  if(m_pathBasic.IsFileExist(m_fileName))
  {
    void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_READ);
    assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));

    if(m_fileBasic.IsValidHandle(fileHandle))
    {
      int count = 1;
      m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);

      void *buf = (void *)&mapsHeader;
      m_fileBasic.ReadFile(fileHandle, (void **)&buf, sizeof(MapsHeader), count);
    }

    m_fileBasic.CloseFile(fileHandle);
  }
}

/**
*
**/
void CUeMapsIO::UpdateHeader(MapsHeader &mapsHeader)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));

  int count = 1;
  m_fileBasic.SeekFile(fileHandle, 0, CFileBasic::UE_SEEK_BEGIN);
  void *ptr = &mapsHeader;
  m_fileBasic.WriteFile(fileHandle, ptr, sizeof(MapsHeader), count);
  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeMapsIO::GetProperties(short type, void **properties, int count, bool isDay)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  assert(*properties);

  if(m_fileBasic.IsValidHandle(fileHandle))
  {
    int offset = 0;
    int size = 0;
    GetOffsetSize(type, offset, size, isDay);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.ReadFile(fileHandle, properties, size, count);
  }

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeMapsIO::UpdateProperties(short type, void *properties, int count, bool isDay)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  assert(properties);

  if(m_fileBasic.IsValidHandle(fileHandle))
  {
    int offset = 0;
    int size = 0;
    GetOffsetSize(type, offset, size, isDay);
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    m_fileBasic.WriteFile(fileHandle, properties, size, count);
  }

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
**/
void CUeMapsIO::UpdateProperties(void *properties, int offset, int size)
{
  void *fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_ALL);
  if(!m_fileBasic.IsValidHandle(fileHandle))
  {
    fileHandle = m_fileBasic.OpenFile(m_fileName, CFileBasic::UE_FILE_WRITE);
  }
  assert(fileHandle && m_fileBasic.IsValidHandle(fileHandle));
  assert(properties);

  if(m_fileBasic.IsValidHandle(fileHandle))
  {
    m_fileBasic.SeekFile(fileHandle, offset, CFileBasic::UE_SEEK_BEGIN);
    int count = 1;
    m_fileBasic.WriteFile(fileHandle, properties, size, count);
  }

  m_fileBasic.CloseFile(fileHandle);
}

/**
*
*/
inline void CUeMapsIO::GetOffsetSize(short type, int &offset, int &size, bool isDay)
{
  MapsHeader header;
  GetHeader(header);
  offset = 0;
  size = 0;

  switch(type)
  {
  case PT_Font:
    {
      size = sizeof(MapsFont);
      offset = sizeof(MapsHeader);
      if(!isDay)
      {
        offset += (sizeof(MapsFont) * header.m_fonts);
        offset += (sizeof(MapsPoint) * header.m_points);
        offset += (sizeof(MapsLine) * header.m_lines);
        offset += (sizeof(MapsPoly) * header.m_polys);
        offset += (sizeof(MapsText) * header.m_texts);
      }
    }
    break;
  case PT_Point:
    {
      size = sizeof(MapsPoint);
      offset = sizeof(MapsHeader);
      offset += header.m_fonts * sizeof(MapsFont);
      if(!isDay)
      {
        offset += (sizeof(MapsPoint) * header.m_points);
        offset += (sizeof(MapsLine) * header.m_lines);
        offset += (sizeof(MapsPoly) * header.m_polys);
        offset += (sizeof(MapsText) * header.m_texts);
        offset += (sizeof(MapsFont) * header.m_fonts);
      }
    }
    break;
  case PT_Line:
    {
      size = sizeof(MapsLine);
      offset = sizeof(MapsHeader);
      offset += header.m_fonts * sizeof(MapsFont);
      offset += header.m_points * sizeof(MapsPoint);
      if(!isDay)
      {
        offset += (sizeof(MapsLine) * header.m_lines);
        offset += (sizeof(MapsPoly) * header.m_polys);
        offset += (sizeof(MapsText) * header.m_texts);
        offset += (sizeof(MapsFont) * header.m_fonts);
        offset += (sizeof(MapsPoint) * header.m_points);
      }
    }
    break;
  case PT_Poly:
    {
      size = sizeof(MapsPoly);
      offset = sizeof(MapsHeader);
      offset += header.m_fonts * sizeof(MapsFont);
      offset += header.m_points * sizeof(MapsPoint);
      offset += header.m_lines * sizeof(MapsLine);
      if(!isDay)
      {
        offset += (sizeof(MapsPoly) * header.m_polys);
        offset += (sizeof(MapsText) * header.m_texts);
        offset += (sizeof(MapsFont) * header.m_fonts);
        offset += (sizeof(MapsPoint) * header.m_points);
        offset += (sizeof(MapsLine) * header.m_lines);
      }
    }
    break;
  case PT_Text:
    {
      size = sizeof(MapsText);
      offset = sizeof(MapsHeader);
      offset += (header.m_fonts * sizeof(MapsFont));
      offset += (header.m_points * sizeof(MapsPoint));
      offset += (header.m_lines * sizeof(MapsLine));
      offset += (header.m_polys * sizeof(MapsPoly));
      if(!isDay)
      {
        offset += (sizeof(MapsText) * header.m_texts);
        offset += (sizeof(MapsFont) * header.m_fonts);
        offset += (sizeof(MapsPoint) * header.m_points);
        offset += (sizeof(MapsLine) * header.m_lines);
        offset += (sizeof(MapsPoly) * header.m_polys);
      }
    }
    break;
  case PT_Layer:
    {
      size = sizeof(MapsLayer);
      offset = sizeof(MapsHeader);
      offset += (header.m_fonts * sizeof(MapsFont));
      offset += (header.m_points * sizeof(MapsPoint));
      offset += (header.m_lines * sizeof(MapsLine));
      offset += (header.m_polys * sizeof(MapsPoly));
      offset += (header.m_texts * sizeof(MapsText));
      offset += (sizeof(MapsText) * header.m_texts);
      offset += (sizeof(MapsFont) * header.m_fonts);
      offset += (sizeof(MapsPoint) * header.m_points);
      offset += (sizeof(MapsLine) * header.m_lines);
      offset += (sizeof(MapsPoly) * header.m_polys);
      offset += (sizeof(MapsText) * header.m_texts);
    }
    break;
  default:
    {
      assert(false);
    }
    break;
  }
  assert(size);
}

