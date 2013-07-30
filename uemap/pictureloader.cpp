#include "pictureloader.h"
#include "navpicture.h"
#include "view.h"
using namespace NavView;

//
#include "navbase\directorytool.h"
using namespace NavBase;

//
#include "tracer.h"

// Static variable initialization
CPictureLoader *CPictureLoader::m_poiLoader = 0;
CPictureLoader *CPictureLoader::m_dirLoader = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**
*
**/
CPictureLoader::CPictureLoader() : m_saveDate(0), m_saveTime(0), m_picNum(0), m_pictures(0), m_strOffset(0),
	m_strSize(0), m_strings(0), m_loaderHandle(INVALID_HANDLE_VALUE), m_loadedCount(0), m_isCached(false)
{
}

/**
*
**/
CPictureLoader::~CPictureLoader()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
CPictureLoader *CPictureLoader::GetPictureLoader(short type, bool isCached)
{
	// Exceptions
	assert(type > PC_Text && type <= PC_User);
	if(type <= PC_Text || type > PC_User)
	{
		return 0;
	}

    // Location
    TCHAR path[301] = {0, };
    CDirectoryTool::GetModulePath(path, 300);
    tstring dataPath = path;

	// Specify one picture loader for this picture category
	CPictureLoader *oneLoader = 0;
	switch(type)
	{
	case PC_POI:
		{
			if(!m_poiLoader)
			{
				if(!m_poiLoader)
				{
					m_poiLoader = new CPictureLoader;
					tstring onePicture = dataPath + _T("\\data\\image\\poi.uib");
					if(!m_poiLoader->Open(onePicture))
					{
						delete m_poiLoader;
						m_poiLoader = 0;
					}
					else
					{
						m_poiLoader->IsCached(isCached);
					}

					// Register it as one kind of memory consumer
					INavView::GetNavView()->GetCollector()->RegisterConsumer(MC_PictureLoader, m_poiLoader);
				}
			}

			oneLoader = m_poiLoader;
		}
		break;
	case PC_Guidance:
		{
			if(!m_dirLoader)
			{
				if(!m_dirLoader)
				{
					m_dirLoader = new CPictureLoader;
					tstring onePicture = dataPath + _T("\\data\\image\\dir.uib");
					if(!m_dirLoader->Open(onePicture))
					{
						delete m_dirLoader;
						m_dirLoader = 0;
					}
					else
					{
						m_dirLoader->IsCached(isCached);
					}

					// Register it as one kind of memory consumer
					INavView::GetNavView()->GetCollector()->RegisterConsumer(MC_PictureLoader, m_dirLoader);
				}
			}

			oneLoader = m_dirLoader;
		}
		break;
	default:
		{
			assert(false);
		}
	}

	return oneLoader;
}

/**
*
**/
void CPictureLoader::ReleaseLoaders()
{
	//
	if(m_poiLoader)
	{
		delete m_poiLoader;
		m_poiLoader = 0;
	}

	//
	if(m_dirLoader)
	{
		delete m_dirLoader;
		m_dirLoader = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
bool CPictureLoader::Open(const tstring &oneFile, bool isLazy)
{
	// Exceptions
	if(oneFile.size() <= 0)
	{
		return false;
	}

	//
	if(m_loaderHandle != INVALID_HANDLE_VALUE)
	{
		if(m_loaderFile == oneFile)
		{
			return true;
		}
		else
		{
			Close();
		}
	}

	//
	MEMSTATISTICS
	m_loaderFile = oneFile;
	return (isLazy) ? true : Open();
}

/**
* TODO:
* Change those APIs from OS dependence to standard C or C++ methods
**/
bool CPictureLoader::Open()
{
	// Open specified loader
	m_loaderHandle = ::CreateFile(m_loaderFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_loaderHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	m_loadedCount = 0;

	// The size of HDR description is 8
	DWORD rt = 0;
	char hdr[8] = "";
	if (!::ReadFile(m_loaderHandle, hdr, 8, &rt, NULL) ||
		(8 != rt))
	{
		Close(); 
		return false;
	}

	// Date information
	if (!::ReadFile(m_loaderHandle, &m_saveDate, 4, &rt, NULL) ||
		(4 != rt))
	{
		Close(); 
		return false;
	}

	// Time information
	if (!::ReadFile(m_loaderHandle, &m_saveTime, 4, &rt, NULL) ||
		(4 != rt))
	{
		Close(); 
		return false;
	}

	// The total number of pictures contained by this loader
	if (!::ReadFile(m_loaderHandle, &m_picNum, 4, &rt, NULL) ||
		(4 != rt))
	{
		Close(); 
		return false;
	}

	// Offset to string part 
	if (!::ReadFile(m_loaderHandle, &m_strOffset, 4, &rt, NULL) ||
		(4 != rt))
	{
		Close(); 
		return false;
	}

	// The size of string part
	if (!::ReadFile(m_loaderHandle, &m_strSize, 4, &rt, NULL) ||
		(4 != rt))
	{
		Close(); 
		return false;
	}

	// Build simple cache
	m_pictures = reinterpret_cast<PictureDescription*>(::malloc(sizeof(PictureDescription) * m_picNum));
	if(!m_pictures)
	{
		Close();
		return false;
	}
	::memset(m_pictures, 0, sizeof(PictureDescription) * m_picNum);

	// No string part & has string part
	if(!m_strSize)
	{
		// Prepare enough memory for storing picture part's descriptions
		long *tmp = reinterpret_cast<long *>(::malloc((4+4+4) * m_picNum + 10));
		if(!tmp)
		{
			Close();
			return false;
		}
	
		// 5) offset + size: image type & offset to one picture & picture size
		if (!::ReadFile(m_loaderHandle, tmp, (4+4+4) * m_picNum, &rt, NULL) ||
			(((4+4+4)*m_picNum) != rt))
		{
			::free(tmp);
			Close(); 
			return false;
		}

		//
		int i = 0;
		for(; i < m_picNum; i++)
		{
			m_pictures[i].m_imgType = static_cast<short>(tmp[i*3]);
			m_pictures[i].m_picOffset = tmp[i*3+1];
			m_pictures[i].m_picSize = tmp[i*3+2];
		}
		::free(tmp);
		tmp = 0;
	}
	else
	{
		//
		long *tmp = reinterpret_cast<long *>(::malloc((4+4+4+4) * m_picNum + 10));
		if(!tmp)
		{
			Close();
			return false;
		}
	
		// 5) offset + size 8
		if (!::ReadFile(m_loaderHandle, tmp, (4+4+4+4) * m_picNum, &rt, NULL) ||
			(((4+4+4+4) * m_picNum) != rt))
		{
			::free(tmp);
			Close(); 
			return false;
		}

		//
		int i = 0;
		for(; i < m_picNum; i++)
		{
			m_pictures[i].m_strOffset = tmp[i*4];
			m_pictures[i].m_imgType = static_cast<short>(tmp[i*4+1]);
			m_pictures[i].m_picOffset = tmp[i*4+2];
			m_pictures[i].m_picSize = tmp[i*4+3];
		}
		::free(tmp);
		tmp = 0;

		//
		m_strings = reinterpret_cast<char *>(::malloc(m_strSize));
		if(!m_strings)
		{
			Close();
			return false;
		}

		//
		if(::SetFilePointer(m_loaderHandle, m_strOffset, NULL, FILE_BEGIN) !=
			(DWORD)m_strOffset)
		{
			Close();
			return false;
		}

		//
		if(!::ReadFile(m_loaderHandle, m_strings, m_strSize, &rt, NULL) ||
			((DWORD)(m_strSize) != rt))
		{
			Close(); 
			return false;
		}
	}

	return true;
}

/**
*
**/
void CPictureLoader::Close()
{
	// Exceptions
	if(m_loaderHandle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// 
	int i = 0;
	for(; i < m_picNum; i++)
	{
		//
		if(m_pictures[i].m_refCount)
		{
			// Dereference one picture object
			int j = 0;
			for(; j < MAXREFCOUNT; j++)
			{
				if(m_pictures[i].m_refObjects[j])
				{
					m_pictures[i].m_refObjects[j]->Reset();
				}
			}

			// Release or free memory for this picture object
			ReleasePicture(m_pictures[i]);
		}
	}

	//
	::CloseHandle(m_loaderHandle);
	m_loaderHandle = INVALID_HANDLE_VALUE;

	//
	if(m_pictures)
	{
		::free(m_pictures);
	}
	m_pictures = 0;
	m_picNum = 0;

	//
	if(m_strings)
	{
		::free(m_strings);
	}
	m_strings = 0;
}

/**
*
**/
inline bool CPictureLoader::IsReady()
{
	return m_loaderHandle != INVALID_HANDLE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
char *CPictureLoader::GetString(int idx)
{
	// Exceptions
	assert(m_strings && m_pictures && idx >= 0 && idx < m_picNum);
	if(!m_strings || !m_pictures || idx < 0 || idx >= m_picNum)
	{
		return 0;
	}

	//
	return m_strings + m_pictures[idx].m_strOffset;
}

/**
*
**/
int CPictureLoader::FindIdx(char *str)
{
	// Exceptions
	if(!m_strings)
	{
		return -1;
	}

	//
	int st = 0;
	int ed = m_picNum - 1;
	int mid = 0;
	while(st <= ed)
	{
		mid = (st + ed) >> 1;
		int cmp = ::strcmp(m_strings + m_pictures[mid].m_strOffset, str);
		if(!cmp)
		{
			return mid;
		}
		else if(cmp < 0)
		{
			st = mid + 1;
		}
		else
		{
			ed = mid - 1;
		}
	}

	// Not found yet
	return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
inline int CPictureLoader::GetPictureSize(int idx)
{
	assert(m_pictures && idx >=0 && idx < m_picNum);
	if(!m_pictures || idx < 0 || idx >= m_picNum)
	{
		return 0;
	}

	return m_pictures[idx].m_picSize;
}

/**
*
**/
inline int CPictureLoader::GetPictureType(int idx) 
{
	assert(m_pictures && idx >= 0 && idx < m_picNum);
	if(!m_pictures || idx < 0 || idx >= m_picNum)
	{
		return 0;
	}

	return m_pictures[idx].m_imgType;
}

/**
* 
**/
bool CPictureLoader::LoadPicture(int idx, CNavPicture *onePicture)
{
	// Memory information
	MEMSTATISTICS

	// Lazy access
	if(m_loaderHandle == INVALID_HANDLE_VALUE && !Open())
	{
		return false;
	}

	assert(m_pictures && idx >= 0 && idx < m_picNum);
	if(!m_pictures || idx < 0 || idx >= m_picNum)
	{
		return false;
	}

	// Whether already exist with one simple cache way
	assert(m_pictures && onePicture);
	if(m_pictures[idx].m_refCount)
	{
		int i = 0;
		for(; i < MAXREFCOUNT; i++)
		{
			if(!m_pictures[idx].m_refObjects[i])
			{
				m_pictures[idx].m_refObjects[i] = onePicture;
				onePicture->m_basic = m_pictures[idx].m_picInfo;
				break;
			}
		}
		m_pictures[idx].m_refCount++;
		return true;
	}

	//
	if(::SetFilePointer(m_loaderHandle, m_pictures[idx].m_picOffset, NULL, FILE_BEGIN) !=
		(DWORD)m_pictures[idx].m_picOffset)
	{
		return false;
	}

	//
	if(GetPictureType(idx) == 1)	// PNG file
	{
		unsigned int size = GetPictureSize(idx);
		char* buff = reinterpret_cast<char*>(::malloc(size));
		if(!buff)
		{
			return false;
		}

		DWORD rt;
		if (!::ReadFile(m_loaderHandle, buff, m_pictures[idx].m_picSize, &rt, NULL))	
		{
			::free(buff);
			return false;
		}

		if(rt != static_cast<DWORD>(m_pictures[idx].m_picSize))	
		{
			::free(buff);
			return false;
		}

		//
		m_pictures[idx].m_picInfo.m_formatType = PDF_PNG;
		bool b = CNavPicture::LoadPicture(buff, size, &m_pictures[idx].m_picInfo);
		::free(buff);

		//
		if(b)
		{
			int i = 0;
			for(; i < MAXREFCOUNT; i++)
			{
				if(!m_pictures[idx].m_refObjects[i])
				{
					m_pictures[idx].m_refObjects[i] = onePicture;
					onePicture->m_basic = m_pictures[idx].m_picInfo;
					break;
				}
			}

			m_loadedCount++;
			m_pictures[idx].m_refCount++;
			return true;
		}
	}
	else 
	{	// BMP file
        BITMAPFILEHEADER  bmf;
		DWORD rt;
		if (!::ReadFile(m_loaderHandle, &bmf, sizeof(bmf), &rt, NULL))
		{
			return 0;
		}

		if(rt != sizeof(bmf))
		{
			return 0;
		}

		//
		if(bmf.bfType == 0x4D42)
		{
			//
			int len = bmf.bfSize - sizeof(BITMAPFILEHEADER);
			char *buff = reinterpret_cast<char *>(::malloc(len));
			if (!buff)
			{
				return 0;
			}

			//
			if(!::ReadFile(m_loaderHandle, buff, len, &rt, NULL))
			{
				::free(buff);
				return false;
			}
			if(rt != static_cast<DWORD>(len))
			{
				::free(buff);
				return false;
			}

			//
			m_pictures[idx].m_picInfo.m_formatType = PDF_BMP;
			bool b = CNavPicture::LoadPicture(buff, len, &m_pictures[idx].m_picInfo);
			if (!b)
			{
				::free(buff);
				return false;
			}

			//
			int i = 0;
			for(; i < MAXREFCOUNT; i++)
			{
				if(!m_pictures[idx].m_refObjects[i])
				{
					m_pictures[idx].m_refObjects[i] = onePicture;
					onePicture->m_basic = m_pictures[idx].m_picInfo;
					break;
				}
			}

			//
			m_loadedCount++;
			m_pictures[idx].m_refCount++;
			return true;
		}
	}

	return false;
}

/**
*
**/
void CPictureLoader::FreePicture(int idx, CNavPicture *onePicture)
{
	//
	assert(m_pictures);
	MEMSTATISTICS

	//
	if(0 != m_pictures && m_pictures[idx].m_refCount > 0)
	{
		m_pictures[idx].m_refCount--;
		if(m_pictures[idx].m_refCount == 0)
		{
			if (!m_isCached)
			{
				ReleasePicture(m_pictures[idx]);
				m_loadedCount--;
			}
			::memset(m_pictures[idx].m_refObjects, 0, sizeof(m_pictures[idx].m_refObjects));
		}		
		else
		{
			int i = 0;
			for(; i < MAXREFCOUNT; i++)
			{
				if(m_pictures[idx].m_refObjects[i] == onePicture)
				{
					m_pictures[idx].m_refObjects[i] = 0;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
**/
void CPictureLoader::IsCached(bool isCached)
{
	m_isCached = isCached;
}

/**
*
**/
bool CPictureLoader::IsCached()
{
	return m_isCached;
}

/**
* Note:
* CNavPicture object currently is sharing the same memory as m_picInfo of m_pictures[idx]
**/
void CPictureLoader::RemoveCache(int maxCount)
{
	if(m_loadedCount > maxCount)
	{
		m_loadedCount = 0;
		int i = 0;
		for(; i < m_picNum; i++)
		{
			if(m_pictures[i].m_refCount)
			{
				int j = 0;
				for(; j < MAXREFCOUNT; j++)
				{
					if(m_pictures[i].m_refObjects[j])
					{
						m_pictures[i].m_refObjects[j]->Reset();
					}
				}

				//
				m_pictures[i].m_refCount = 0;
				ReleasePicture(m_pictures[i]);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Private functions
///
/**
* TODO:
* Apply standard C or C++ file operation APIs
**/
int CPictureLoader::LoadPicture(int idx, char *buf)
{
	assert(m_pictures && buf);
	if(m_loaderHandle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if(::SetFilePointer(m_loaderHandle, m_pictures[idx].m_picOffset, NULL, FILE_BEGIN) !=
		(DWORD)m_pictures[idx].m_picOffset)
	{
		return 0;
	}

	DWORD rt;
	if (!::ReadFile(m_loaderHandle, buf, m_pictures[idx].m_picSize, &rt, NULL))
	{
		return 0;
	}

	if(rt != (DWORD)m_pictures[idx].m_picSize)
	{
		return 0;
	}

	return rt;

}

/**
*
**/
inline void CPictureLoader::ReleasePicture(PictureDescription &onePic)
{
	// Release the common picture memory
	NavPictureBasic &basic = onePic.m_picInfo;
	if(basic.m_bmp)
	{
		::DeleteObject(basic.m_bmp);
		basic.m_bmp = 0;
	}

	//
	if(basic.m_bmpInfo)
	{
		::free(basic.m_bmpInfo);
		basic.m_bmpInfo = 0;
	}

	//
	if(basic.m_bmpBase)
	{
		::free(basic.m_bmpBase);
	}

	//
	if(basic.m_rows && reinterpret_cast<void *>(basic.m_rows) != reinterpret_cast<void *>(basic.m_bmpBase))
	{
		int i = 0;
		for(; i < basic.m_height; i++)
		{
			::free(basic.m_rows[i]);
			basic.m_rows[i] = 0;
		}
		::free(basic.m_rows);
	}
	basic.m_bmpBase = 0;
	basic.m_rows = 0;
}
