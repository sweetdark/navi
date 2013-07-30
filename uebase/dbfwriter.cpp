#include "dbfwriter.h"
#include "pathbasic.h"
#include "filebasic.h"
using namespace UeBase;

///////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
CDbfWriter::CDbfWriter(const tstring &file) : m_fileName(file), m_dataHandle(0),
m_pathBasic(CPathBasic::Get()), m_fileBasic(CFileBasic::Get())
{
}

/**
*
**/
CDbfWriter::~CDbfWriter()
{
}

///////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CDbfWriter::Open()
{
  assert(!m_dataHandle);
  if(!m_dataHandle)
  {
    tstring fileName = m_fileName;
    fileName += _T(".dbf");

    m_dataHandle = m_fileBasic.OpenFile(fileName, CFileBasic::UE_FILE_WRITE);
    assert(m_fileBasic.IsValidHandle(m_dataHandle));
    if(!m_fileBasic.IsValidHandle(m_dataHandle))
    {
      return false;
    }

    // Prepare basic information
    m_description.m_records = 0;
    m_description.m_fields = 0;
    m_description.m_recordLength = 1;
    m_description.m_headerLength = 33;
    m_description.m_curRecord = -1;
  }

  return true;
}

/**
*
**/
void CDbfWriter::Close()
{
  // The last recrod
  FlushRecord();

  // Writer header lastly
  // ...
  WriteHeader();

  //
  if(m_dataHandle)
  {
    m_fileBasic.FlushFile(m_dataHandle);
    m_fileBasic.CloseFile(m_dataHandle);
  }
}

////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
int CDbfWriter::AddFieldInfo(const char *fldName, short fldType, int width, int decimals)
{
  // Do some checking to ensure we can add records to this file
  if(m_description.m_records > 0)
  {
    return -1;
  }

  if(fldType != DFT_Double && decimals != 0)
  {
    return -1;
  }

  if(width < 1)
  {
    return -1;
  }

  // SfRealloc all the arrays larger to hold the additional field information. 
  m_description.m_fields++;
  m_description.m_offsets = (!m_description.m_offsets) ? (int *)::malloc(sizeof(int) * m_description.m_fields) :
    (int *)::realloc(m_description.m_offsets, sizeof(int) * m_description.m_fields);
  m_description.m_sizes = (!m_description.m_sizes) ? (int *)::malloc(sizeof(int) * m_description.m_fields) :
    (int *)::realloc(m_description.m_sizes, sizeof(int) * m_description.m_fields);
  m_description.m_decimals = (!m_description.m_decimals) ? (int *)::malloc(sizeof(int) * m_description.m_fields) :
    (int *)::realloc(m_description.m_decimals, sizeof(int) * m_description.m_fields);
  m_description.m_types = (!m_description.m_types) ? (unsigned char *)::malloc(sizeof(unsigned char) * m_description.m_fields) :
    (unsigned char *)::realloc(m_description.m_types, sizeof(unsigned char) * m_description.m_fields);

  // Assign the new field information fields
  m_description.m_offsets[m_description.m_fields-1] = m_description.m_recordLength;
  m_description.m_recordLength += width;
  m_description.m_sizes[m_description.m_fields-1] = width;
  m_description.m_decimals[m_description.m_fields-1] = decimals;

  if(fldType == DFT_Logical)
  {
    m_description.m_types[m_description.m_fields-1] = 'L';
  }
  else if( fldType == DFT_String)
  {
    m_description.m_types[m_description.m_fields-1] = 'C';
  }
  else
  {
    m_description.m_types[m_description.m_fields-1] = 'N';
  }

  // Extend the required header information
  m_description.m_headerLength += 32;
  m_description.m_header = (!m_description.m_header) ? (unsigned char *)::malloc(m_description.m_fields*32) : 
    (unsigned char *)::realloc(m_description.m_header, m_description.m_fields*32);

  unsigned char *cursor = m_description.m_header + 32 * (m_description.m_fields-1);
  int i = 0;
  for(; i < 32; i++)
  {
    cursor[i] = '\0';
  }

  if((int)::strlen(fldName) < 10)
  {
    ::strncpy((char *)cursor, fldName, strlen(fldName));
  }
  else
  {
    ::strncpy((char *)cursor, fldName, 10);
  }
  cursor[11] = m_description.m_types[m_description.m_fields-1];

  if(fldType == DFT_String)
  {
    cursor[16] = width % 256;
    cursor[17] = width / 256;
  }
  else
  {
    cursor[16] = width;
    cursor[17] = decimals;
  }

  // Make the current record buffer appropriately larger
  m_description.m_content = (!m_description.m_content) ? (unsigned char *)::malloc(m_description.m_recordLength) : 
    (unsigned char *)::realloc(m_description.m_content, m_description.m_recordLength);

  return m_description.m_fields /*- 1*/;
}

/**
*
**/
bool CDbfWriter::WriteField(int recIdx, int fldIdx, int fldType, void *data)
{
  // Is this a valid record
  bool isSuccess = true;
  if(recIdx < 0 || recIdx > m_description.m_records)
  {
    return isSuccess = false;
  }

  // Is this a brand new record? 
  if(recIdx == m_description.m_records)
  {
    if(!FlushRecord())
    {
      return false;
    }
    m_description.m_records++;

    int i = 0;
    for(; i < m_description.m_recordLength; i++)
    {
      m_description.m_content[i] = ' ';
    }
    m_description.m_curRecord = recIdx;
  }

  // Is this an existing record, but different than the last one we accessed
  int recordOffset = 0;
  if(m_description.m_curRecord != recIdx)
  {
    if(!FlushRecord())
    {
      return false;
    }
    recordOffset = m_description.m_recordLength * recIdx + m_description.m_headerLength;

    m_fileBasic.SeekFile(m_dataHandle, recordOffset);
    int count = 1;
    bool rt = m_fileBasic.ReadFile(m_dataHandle, (void **)&m_description.m_content, m_description.m_recordLength, count);
    assert(rt);
    if(!rt)
    {
      return isSuccess = false;
    }
    m_description.m_curRecord = recIdx;
  }

  char fields[400] = {0, };
  char formats[20] = {0, };
  char *cursor = (char *)m_description.m_content;

  // Translate NULL value to valid DBF file representation Contributed by Jim Matthews
  if(!data)
  {
    switch(m_description.m_types[fldIdx])
    {
    case 'N':
    case 'F':
      {
        // NULL numeric fields have value "****************"
        ::memset( (char *) (cursor+m_description.m_offsets[fldIdx]), '*', m_description.m_sizes[fldIdx]);
      }
      break;
    case 'D':
      {
        //  NULL date fields have value "00000000"
        ::memset( (char *) (cursor+m_description.m_offsets[fldIdx]), '0', m_description.m_sizes[fldIdx] );
      }
      break;
    case 'L':
      {
        // NULL boolean fields have value "?"
        ::memset( (char *) (cursor+m_description.m_offsets[fldIdx]), '?', 
          m_description.m_sizes[fldIdx] );
      }
      break;
    default:
      {
        //empty string fields are considered NULL
        ::memset( (char *) (cursor+m_description.m_offsets[fldIdx]), '\0', 
          m_description.m_sizes[fldIdx] );
      }
      break;
    }

    return isSuccess = true;
  }

  // Assign all the record fields
  switch(m_description.m_types[fldIdx])
  {
  case 'D':
  case 'N':
  case 'F':
    {
      int	width = m_description.m_sizes[fldIdx];
      if(sizeof(fields)-2 < width)
      {
        width = sizeof(fields)-2;
      }

      if(m_description.m_decimals[fldIdx] == 0 )
      {
        ::sprintf(formats, "%%%dd", width);
        ::sprintf(fields, formats, (int) *((double *)data));
        if((int)::strlen(fields) > m_description.m_sizes[fldIdx])
        {
          fields[m_description.m_sizes[fldIdx]] = '\0';
          isSuccess = false;
        }

        ::strncpy((char *)(cursor+m_description.m_offsets[fldIdx]), fields, ::strlen(fields));
      }
      else
      {
        ::sprintf( formats, "%%%d.%df", width, m_description.m_decimals[fldIdx]);
        ::sprintf(fields, formats, *((double *) data));
        if((int)::strlen(fields) > m_description.m_sizes[fldIdx])
        {
          fields[m_description.m_sizes[fldIdx]] = '\0';
          isSuccess = false;
        }
        ::strncpy((char *)(cursor+m_description.m_offsets[fldIdx]), fields, strlen(fields));
      }
    }
    break;
  case 'L':
    {
      if(m_description.m_sizes[fldIdx] >= 1  && (*(char*)data == 'F' || *(char*)data == 'T'))
      {
        *(cursor+m_description.m_offsets[fldIdx]) = *(char*)data;
      }
    }
    break;
  default:
    {
      int j = 0;
      if((int)::strlen((char *) data) > m_description.m_sizes[fldIdx])
      {
        j = m_description.m_sizes[fldIdx];
        isSuccess = false;
      }
      else
      {
        ::memset(cursor+m_description.m_offsets[fldIdx], ' ', m_description.m_sizes[fldIdx] );
        j = static_cast<int>(::strlen((char *)data));
      }

      ::strncpy((char *) (cursor+m_description.m_offsets[fldIdx]), (char *) data, j);
    }
    break;
  }

  return isSuccess;
}

////////////////////////////////////////////////////////////////////////////////////
//
//
/**
*
**/
bool CDbfWriter::FlushRecord()
{
  int recordOffset = 0;
  if(m_description.m_curRecord > -1)
  {
    recordOffset = m_description.m_recordLength * m_description.m_curRecord + m_description.m_headerLength;
    m_fileBasic.SeekFile(m_dataHandle, recordOffset);

    return m_fileBasic.WriteFile(m_dataHandle, m_description.m_content, m_description.m_recordLength, 1);
  }

  return true;
}

/**
*
**/
void CDbfWriter::WriteHeader()
{
  // Initialize the file header information
  char header[FIXEDHEADERSIZE];
  ::memset(header, 0x00, FIXEDHEADERSIZE);

  // memo field? - just copying
  header[0] = 0x03;

  header[1] = 8;	// YY
  header[2] = 8;	// MM
  header[3] = 16;	// DD

  header[4] = m_description.m_records % 256;
  header[5] = (m_description.m_records/256) % 256;
  header[6] = (m_description.m_records/(256*256)) % 256;
  header[7] = (m_description.m_records/(256*256*256)) % 256;

  // date updated on close, record count preset at zero
  header[8] = m_description.m_headerLength % 256;
  header[9] = m_description.m_headerLength / 256;
  header[10] = m_description.m_recordLength % 256;
  header[11] = m_description.m_recordLength / 256;

  // Write the initial 32 byte file header, and all the field descriptions
  m_fileBasic.SeekFile(m_dataHandle, 0);
  int count = 1;
  m_fileBasic.WriteFile(m_dataHandle, header, FIXEDHEADERSIZE, count);
  count = m_description.m_fields;
  m_fileBasic.WriteFile(m_dataHandle, m_description.m_header, FIXEDHEADERSIZE, count);

  // Write out the newline character if there is room for it
  if(m_description.m_headerLength > 32*m_description.m_fields + 32)
  {
    char newline = 0x0d;
    count = 1;
    m_fileBasic.WriteFile(m_dataHandle, &newline, 1, 1);
  }
}