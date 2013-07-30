#ifndef _NAVVIEW_PICTURELOADER_H
#define _NAVVIEW_PICTURELOADER_H

//
#ifndef _PORTING_DEF_H
#include "port.h"
#endif

//
#ifndef _NAVVIEW_BASE_H
#include "navview.h"
#endif

//
#ifndef _PORTING_DEF_H
#include "port.h"
#endif

//
#ifndef _NAVVIEW_DEF_H
#include "viewdef.h"
#endif

//
#ifndef _NAVBASE_MEMFUNCTOR_H
#include "navbase\memfunctor.h"
#endif
using namespace NavBase;

//
#pragma warning( push )
#pragma warning( disable: 4251 )  // Deprecated function calling

// Declare namespace
namespace NavView
{
	// Forward classes
	class CNavPicture;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// This class is as basic IO supporting different picture format IO operations and manage those picture object 
	/// in memory
	///
	class NAVVIEW_CLASS CPictureLoader : public CMemConsumer
	{
		// General definitions
		const static int MAXREFCOUNT = 10; // Means how many CNavPicture object are referencing to the same memory
		struct PictureDescription
		{
			// File positions differenciating different parts
			long m_strOffset;
			long m_picOffset;

			// Picture or image description
			short m_imgType;
			long m_picSize;
			NavPictureBasic m_picInfo;

			// References for cache mechansim
			short m_refCount;
			CNavPicture *m_refObjects[MAXREFCOUNT];

			/**
			*
			**/
			PictureDescription() : m_strOffset(0), m_picOffset(0), m_imgType(0), m_picSize(0), m_refCount(0)
			{
				int i = 0;
				for(; i < MAXREFCOUNT; i++)
				{
					m_refObjects[i] = 0;
				}
			}
		};

	public:
		//
		//
		//
		/**
		*
		**/
		CPictureLoader();

		/**
		*
		**/
		virtual ~CPictureLoader(void);

	public:
		//
		// Methods for open and close one kind of picture loader
		//
		/**
		*
		**/
		bool Open(const tstring &oneFile, bool isLazy = true);

		/**
		*
		**/
		void Close();

		/**
		*
		**/
		bool IsReady();

		//
		// Methods for string part
		//
		/**
		*
		**/
		char *GetString(int idx);

		/**
		*
		**/
		int FindIdx(char *str);

		//
		// Methods for picture part
		//
		/**
		*
		**/
		int GetPictureSize(int idx);

		/**
		*
		**/
		int GetPictureType(int idx);

		/**
		*
		**/
		bool LoadPicture(int idx, CNavPicture *onePicture);
	
		/**
		*
		**/
		void FreePicture(int idx, CNavPicture *onePicture);

		//
		// Methods for simple cache operations
		//
		/**
		*
		**/
		void IsCached(bool isCached);

		/**
		*
		**/
		bool IsCached();

		/**
		*
		**/
		void RemoveCache(int maxCount);

		//
		// Different kinds of UIB loader which has its own picture description or specification and as a singleton 
		// shared by different CNavPicture representing one access mechansim 
		//
		/**
		*
		**/
		static CPictureLoader *GetPictureLoader(short type, bool isCached = true);

		/**
		*
		**/
		static void ReleaseLoaders();

		//
		// Overrided function as one kind of memory consumer
		//
		/**
		*
		**/
		virtual bool ForCollegues()
		{
			// Note:
			// Currently it seems no sense to release memory for other collegues since it should take few
			// in the meaning time it also need to be reopened as there maybe some pictures being rendered
			return false;
		}

	private:
		//
		//
		//
		/**
		*
		**/
		int LoadPicture(int idx, char *buf);

		/**
		*
		**/
		void ReleasePicture(PictureDescription &onePic);

		/**
		* For lazy access
		**/
		bool Open();

	private:
		// Load identifications
		long m_saveDate;	// 20060313 means 2006/03/13
		long m_saveTime;	// 112244 means 11: 33 : 44

		// Memory blockes for image part
		int m_picNum;
		PictureDescription *m_pictures;

		// Memory blocks for string part
		long m_strOffset;
		long m_strSize;
		char *m_strings;

		// Loader itself
		tstring m_loaderFile;
		void *m_loaderHandle;
		int m_loadedCount;
		bool m_isCached;

		// Access to POI pictures
		static CPictureLoader *m_poiLoader;
		// Access to guidance or direction loader
		static CPictureLoader *m_dirLoader;
	};
}

#endif