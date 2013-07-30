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
#include "tokenstring.h"
#include "dbgmacro.h"
using namespace UeBase;

// Refer to functions mentioned by "tchar.h"
#include "portability.h"

///////////////////////////////////////////////////////////////////////////////////
// Constructors & deconstructor
/**
*
*/
CTokenString::CTokenString() : m_strResults(0), m_strNum(0), m_columnWidth(0), m_columnNum(0)
{
  memset(m_strToken,0x00,sizeof(char) *(m_maxTokenSize+1));
  memset(m_strBuilder,0x00,sizeof(char) *(m_maxTokenSize+1));
  memset(m_strDivider,0x00,sizeof(char) *(m_maxTokenSize+1));
  SetDefault();
}

/**
*
*/
CTokenString::CTokenString(const char* token, const char* builder, const char* divider) : m_isDefault(false), 
m_strResults(0), m_strNum(0), m_columnWidth(0), m_columnNum(0)
{
  memset(m_strToken,0x00,sizeof(char) *(m_maxTokenSize+1));
  memset(m_strBuilder,0x00,sizeof(char) *(m_maxTokenSize+1));
  memset(m_strDivider,0x00,sizeof(char) *(m_maxTokenSize+1));

  SetToken(token, builder, divider);
}

/**
*
*/
CTokenString::~CTokenString()
{
  RemoveResult();
  if(m_columnWidth)
  {
    ::free(m_columnWidth);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////
// Methods specify pragma rules
//
/**
*
*/
inline void CTokenString::SetDefault()
{
  m_isDefault = true;
  SetToken(",", "\"\"''()", "=;#");
}

/**
*
*/
inline void CTokenString::SetToken(const char *token, const char *builder, const char *divider)
{
  assert(token && ::strlen(token) < m_maxTokenSize);
  if(m_columnWidth)	
  {
    ::free(m_columnWidth);
  }
  m_columnNum = 0;
  m_columnWidth = 0;

  ::strncpy(m_strToken, token, ::strlen(token));
  m_strToken[::strlen(token)] = 0;

  if(builder)
  {
    assert(::strlen(builder) < m_maxTokenSize);
    ::strncpy(m_strBuilder, builder, ::strlen(builder));
    m_strBuilder[::strlen(builder)] = 0;
  }

  if(divider)
  {
    assert(::strlen(divider) < m_maxTokenSize);
    ::strncpy(m_strDivider, divider, ::strlen(divider));
    m_strDivider[::strlen(divider)] = 0;
  }
}

/**
*
*/
void CTokenString::SetFixedWidth (int num, int *widths)
{
  m_isDefault = false;
  if(m_columnWidth)
  {
    ::free(m_columnWidth);
  }
  m_columnNum = 0;
  m_columnWidth = 0;

  if(num)
  {
    m_columnNum = num;
    m_columnWidth = reinterpret_cast<int *>(::malloc(sizeof(int) * num));
    ::memcpy(m_columnWidth, widths, sizeof(int) * num);
  }
}

//////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
bool CTokenString::AddResult(const char *res, int max)
{
  if(m_strNum < max)
  {
    m_strResults[m_strNum] = ::_strdup(res);
    if(m_strResults[m_strNum] == 0)	
    {
      return false;
    }

    m_strNum ++;
    return true;
  }

  return false;
}

/**
*
*/
int CTokenString::Trim(const char *str, int max)
{
  if(m_isDefault) 
  {
    return TrimByDefault(str, max);
  }
  else if(m_columnNum)
  {
    return TrimByFixed(str);
  }
  else				
  {
    return TrimByNormal(str, max);
  }
}

////////////////////////////////////////////////////////////////////////////////////
//
/**
*
*/
bool CTokenString::IsSame(const char *key, bool isCase)
{
  if(m_strNum == 0)
  {
    return false;
  }

  char cpAll[1024+1];
  cpAll[0] = 0;

  ::strcpy (cpAll, GetAt (0));
  for (int ii=1; ii<m_strNum; ii++)
  {
    ::strcat(cpAll, " ");
    ::strcat(cpAll, GetAt (ii));
  }

  char cpIn[1024+1];
  strcpy (cpIn, key);

  if (!isCase)
  {
    ::_strupr(cpAll); 
    ::_strupr(cpIn);
  }

  if(!::strcmp(cpAll, cpIn))
  {
    return true;
  }

  return false;
}

/**
*
*/
bool CTokenString::IsSame(int idx, const char *oneKey, bool isCase, int num)
{
  char *key = GetAt(idx);
  if(!key || !key[0])	
  {
    return false;
  }

  char cpKeyR[1024+1];
  char cpKeyI[1024+1];

  cpKeyR[0] = 0;
  strcpy(cpKeyR, key);

  for (int ii=1; ii<num && ii<m_strNum; ii++)
  {
    key = GetAt (idx+ii);
    if (key)
    {
      strcat(cpKeyR, " ");
      strcat(cpKeyR, key);
    }
  }

  ::strncpy(cpKeyI, oneKey, 128);	
  cpKeyI[128] = 0;

  if(!isCase)
  {
    ::_strupr(cpKeyR); 
    ::_strupr (cpKeyI);
  }

  if(!::strcmp(cpKeyR, cpKeyI))
  {
    return true;
  }

  return false;
}

/**
*
*/
int CTokenString::Count()
{
  return m_strNum;
}

/**
*
*/
int CTokenString::GetCount()
{
  return m_strNum;
}

/**
*
*/
int CTokenString::Find (int start, const char* str, bool isCase) const
{
  int ii = start;

  if(ii < 0)		
  {
    ii = 0;
  }

  for (; ii<m_strNum; ii++)
  {
    if (isCase)	
    {
      if(!::strcmp(m_strResults[ii], str))
        return ii;
    }
    else		
    {
      if (!::_stricmp (m_strResults[ii], str))	
        return ii;
    }
  }
  return -1;
}

/**
*
*/
char* CTokenString::GetAt(int idx) const
{
  if (idx >= 0 && idx < m_strNum)	
  {
    return m_strResults[idx];
  }
  return 0;
}

/**
*
*/
char* CTokenString::operator[](int idx) const
{
  return GetAt(idx);
}

/**
*
*/
void CTokenString::RemoveAfter(int idx)
{
  if(idx >= 0 && m_strResults)
  {
    for (int ii=idx; ii<m_strNum; ii++)
    {
      ::free(m_strResults[ii]);
    }
    m_strNum = idx;
  }

  if(m_strNum == 0)
  {
    RemoveResult();
  }
}

/**
*
*/
void CTokenString::RemoveResult()
{
  if (m_strResults)
  {
    for (int ii=0; ii<m_strNum; ii++)
    {
      if(m_strResults[ii])
      {
        ::free(m_strResults[ii]);
      }
    }

    ::free(m_strResults);
    m_strResults = NULL;
    m_strNum = 0;
  }
}

/**
*
*/
inline bool CTokenString::FindToken(char token)
{
  char *cursor = m_strToken;
  while(*cursor)
  {
    if(*cursor == token)
    {
      return true;
    }

    cursor++;
  };

  return  false;
}

/**
*
*/
inline bool CTokenString::FindDivider(char divider)
{
  char *cursor = m_strDivider ;
  while(*cursor)
  {
    if(*cursor == divider)
    {
      return true;
    }

    cursor++;
  };

  return false;
}

/**
*
*/
inline char CTokenString::FindBuilder(char builder, bool &mode)
{
  mode = 0;
  char *cursor = m_strBuilder;
  while(*cursor)
  {
    if(*cursor == builder)
    {
      char *tmp = cursor + 1;
      if(*cursor == *tmp)
      {
        mode = 1;
      }

      return *tmp;
    }

    cursor++;
  };

  return  0;
}

/**
*
*/
inline void CTokenString::RemoveEndSpace(char *str)
{
  int ll = static_cast<int>(::strlen(str));
  for(ll--; ll > -1; ll--)
  {
    if(str[ll] != ' ')
    {
      return;
    }

    str[ll] = 0;
  }
}

/**
*
*/
int CTokenString::TrimByNormal(const char *str, int max)
{
  RemoveResult();
  if(str[0] == 0 || str[0] == '\n' || str[0] == '\r')	
  {
    return 0;
  }

  int len = static_cast<int>(::strlen(str));
  const char *in = str;
  if(!in)
  {
    return -1;
  }

  char *tmp = reinterpret_cast<char *>(::malloc(len+5));
  if(!tmp)
  {
    return -1;
  }
  tmp[0] = 0;

  m_strResults = reinterpret_cast<char**>(::malloc(sizeof(char*) * (max+2)));
  if(!m_strResults)	
  {
    ::free(tmp); 
    return -1;
  }
  ::memset(m_strResults, 0, sizeof(char*) * (max+2));

  enum
  {
    mNone=0, 
    mFind
  }mode = mNone;

  char findCh = 0;
  bool findChMode = 0;
  bool isLastAdd = 0;
  bool isFind = 0;

  int pos = 0;
  int to = 0;
  for(; in[pos]; pos++)
  {
    if(mode == mFind) 
    {
      if(in[pos] == findCh)
      {
        if(findChMode && (in[pos+1] == findCh))
        {
          tmp[to++] = in[pos];
          tmp[to] = 0;
          pos ++;
          isLastAdd = 0;	
        }
        else
        {
          tmp[to] = 0;
          to = 0;
          if(!AddResult(tmp, max))	
          {
            break;
          }

          mode = mNone;
          tmp[to] = 0;
          isLastAdd = 1;	
        }
      }
      else
      {
        tmp[to++] = in[pos];
        tmp[to]   = 0;
        isLastAdd = 0;	
      }
    }
    else
    {
      isFind = FindToken(in[pos]);
      if(isFind)
      {
        bool old = isLastAdd;
        isLastAdd = 0;
        if (old)
        {
          continue;
        }

        RemoveEndSpace(tmp);
        if(!AddResult(tmp, max))	
        {
          break;
        }

        to = 0;
        tmp[to] = 0;

        continue;
      }

      isFind = FindDivider(in[pos]);
      if(isFind)
      {
        bool old = isLastAdd;
        isLastAdd = 0;
        if (!old)
        {
          to = 0;
          RemoveEndSpace(tmp);
          if(!AddResult(tmp, max)) 
          {
            break;
          }
        }

        tmp[0] = in[pos];
        tmp[1] = 0;
        if (!AddResult(tmp, max))	
        {
          break;
        }
        to = 0;
        tmp[to] = 0;

        continue;
      }

      isFind = 0;
      if((in[pos] == '\n') || (in[pos] == '\r'))
      {
        break;
      }

      if((in[pos] == ' ') || (in[pos] == '\t'))
      {
        if(m_isDefault)
        {
          to = 0;
          RemoveEndSpace(tmp);
          if(!isLastAdd && tmp[0])
          {
            if(!AddResult(tmp, max))
            {
              break;
            }

            isLastAdd = 1;
          }
          tmp[to] = 0;
          continue;
        }


        if(!to)	
        {
          continue;
        }

        tmp[to] = ' ';
        to++;
        tmp[to] = 0;
        isLastAdd = 0;

        continue;
      }

      isLastAdd = 0;
      if(!to)
      {
        findCh = FindBuilder(in[pos], findChMode);
        if(findCh)
        {
          mode = mFind;
          continue;
        }
      }

      tmp[to] = in[pos];
      to ++;
      tmp[to] = 0;
      isLastAdd = 0;
    }
  }

  RemoveEndSpace(tmp);
  if (!isLastAdd || tmp[0])
  {
    AddResult (tmp, max);
  }
  ::free(tmp);

  return m_strNum;
}

/**
*
*/
int CTokenString::TrimByDefault(const char *str, int max)
{
  RemoveResult();
  if(str[0] == 0 || str[0] == '\n' || str[0] == '\r')	
  {
    return 0;
  }

  int len = static_cast<int>(::strlen(str));
  const char *in = str;
  if(!in)
  {
    return -1;
  }

  // Used to record different string parts divided by tokens
  char *tmp = (char*)(::malloc(len+5));
  if(!tmp)
  {
    return -1;
  }
  tmp[0] = 0;

  // Results analyzed
  m_strResults = (char**)(::malloc(sizeof(char*) * (max+2)));
  if(!m_strResults)
  {
    ::free(tmp); 
    return -1;
  }
  ::memset(m_strResults, 0, sizeof(char*) * (max+2));

  // Whether find duplicate characters as builder token
  enum	
  {
    mNone=0, 
    mFind	
  }mode = mNone;

  char findCh = 0;
  bool findChMode	= 0; // 0: find only, 1: dup->single
  bool isLastAdd = 1;
  bool isFind = 0;

  int pos = 0;
  int to = 0;
  for(; in[pos]; pos++)
  {
    if(mode == mFind) // find builder
    {
      if(in[pos] == findCh)
      {
        if(findChMode && (in[pos+1] == findCh))
        {
          tmp[to++] = in[pos];
          tmp[to] = 0;
          pos++;
          isLastAdd = 0;	
        }
        else
        {
          tmp[to] = 0;
          to = 0;
          if(!AddResult(tmp, max))
          {
            break;
          }

          mode = mNone;
          tmp[to] = 0;
          isLastAdd = 1;	
        }
      }
      else
      {
        // Move forward ...
        tmp[to++] = in[pos];
        tmp[to] = 0;
        isLastAdd = 0;	
      }
    }
    else
    {
      // Firstly always to find strin part divided by builder token
      findCh = FindBuilder(in[pos], findChMode);
      if(findCh)
      {
        RemoveEndSpace(tmp);
        to = 0;
        isLastAdd = 1;
        if(tmp[0])
        {
          if(!AddResult(tmp, max))
          {
            break;
          }
        }

        tmp[0] = 0;
        mode = mFind;
        continue;
      }

      isFind = FindToken(in[pos]);
      if(isFind)
      {
        bool old = isLastAdd;
        isLastAdd = 0;
        if(old)
        {
          continue;
        }

        RemoveEndSpace(tmp);
        if(!AddResult(tmp, max))
        {
          break;
        }

        to = 0;
        tmp[to] = 0;
        continue;
      }

      isFind = FindDivider(in[pos]);
      if(isFind)
      {
        bool old = isLastAdd;
        isLastAdd = 0;
        if(!old)
        {
          to = 0;
          RemoveEndSpace (tmp);
          if (!AddResult(tmp, max))
          {
            break;
          }
        }

        tmp[0] = in[pos];
        tmp[1] = 0;
        if(!AddResult(tmp, max))
        {
          break;
        }
        to = 0;
        tmp[to] = 0;
        continue;
      }

      isFind = 0;
      if((in[pos] == '\n') || (in[pos] == '\r'))
      {
        break;
      }

      // It also take below two token by default
      if((in[pos] == ' ') || (in[pos] == '\t'))
      {
        if(m_isDefault)
        {
          to = 0;
          RemoveEndSpace (tmp);
          if(!isLastAdd && tmp[0])
          {
            if(!AddResult (tmp, max))	
            {
              break;
            }
            isLastAdd = 1;
          }
          tmp[to] = 0;
          continue;
        }


        if(!to)
        {
          continue;
        }

        tmp[to] = ' ';
        to++;
        tmp[to] = 0;
        isLastAdd = 0;
        continue;
      }

      // Move forward ...
      tmp[to] = in[pos];
      to++;
      tmp[to] = 0;
      isLastAdd = 0;
    }
  }

  RemoveEndSpace (tmp);
  if(!isLastAdd || tmp[0])
  {
    AddResult(tmp, max);
  }

  ::free(tmp);
  return m_strNum;
}

/**
*
*/
int CTokenString::TrimByFixed(const char *str)
{
  RemoveResult();
  if(str[0] == 0 || str[0] == '\n' || str[0] == '\r')
  {
    return 0;
  }

  int len = static_cast<int>(::strlen (str));
  for(; len>0; len--)
  {
    if((str[len-1] == '\n') || (str[len-1] == '\r'))
    {
    }
    else	
    {
      break;
    }
  }

  const char*	in = str;
  if (!in)	
  {
    return -1;
  }

  char *tmp = (char*)malloc (len+5);
  if(!tmp)
  {
    return -1;
  }
  tmp[0] = 0;

  m_strResults = (char**) malloc(sizeof(char*) * (m_columnNum+2));
  if(!m_strResults)
  {
    ::free(tmp); 
    return -1;
  }
  ::memset(m_strResults, 0, sizeof(char*) * (m_columnNum+2));

  bool isSkip = 0;
  int col_no = 0;
  int col_width = 0;

  int pos = 0;
  int to = 0;
  for(col_no = 0; ((col_no < m_columnNum) && (pos < len)); col_no++)
  {
    col_width = m_columnWidth[col_no];
    if (col_width < 0)
    {
      col_width = - col_width;
      isSkip = 1;
    }

    tmp[0] = 0;
    to = 0;

    for(int ii=0; ((ii<col_width) && (pos < len)); ii++, pos++)
    {
      if (!isSkip)
      {
        tmp[to] = in[pos];
        to ++;
        tmp[to] = 0;
      }
    }
    if (to)
    {
      if (!AddResult (tmp, m_columnNum))	break;
    }
  }

  ::free(tmp);
  return m_strNum;
}
