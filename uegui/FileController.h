#ifndef _UEGUI_FILECONTROLLER_H
#define _UEGUI_FILECONTROLLER_H

#ifndef _UEBASE_PATHBASIC_H
#include "uebase\pathbasic.h"
#endif
#ifndef _UEBASE_FILEBASIC_H
#include "uebase\filebasic.h"
#endif
// #ifndef _UEBASE_STRINGBASIC_H
// #include "uebase\stringbasic.h"
// #endif
#ifndef _UEBASE_FILEREADER_H
#include "uebase\customerfilereader.h"
#endif
using namespace UeBase;

#ifndef _UEGUI_DATASFORTRUCK_H
#include "datasfortruck.h"
#endif


namespace UeGui
{
	template <class T>
	class CFileController
	{
	public:
		static CFileController<GoodsSource>& getGoodsSourceCtrl()
		{
			static CFileController<GoodsSource> m_SingleGoodsSource(_T("goodssource.db"));
			return m_SingleGoodsSource;
		}
		static CFileController<GoodsSourceDetail>& getGoodsSourceDetailCtrl()
		{
			static CFileController<GoodsSourceDetail> m_SingleGSDetail(_T("sourcedetail.db"));
			return m_SingleGSDetail;
		}
		static CFileController<LogisticsTrackInfo>& getLogisticsTrackCtrl()
		{
			static CFileController<LogisticsTrackInfo> m_SingleLogist(_T("LogisticsTrack.db"));
			return m_SingleLogist;
		}

	public:
		~CFileController(void)
		{
			if (m_pFileReaderCtrl != 0)
			{
				delete m_pFileReaderCtrl;
				m_pFileReaderCtrl = 0;
			}
		}

		//初始化文件句柄
		bool InitFileReaderCtrl(void)
		{
			if (m_pFileReaderCtrl==0)
			{
        m_fileName = CPathConfig::GetCommonPath(CPathConfig::CPK_AttrsPath)+m_fileName;
				m_pFileReaderCtrl = new CCustomerFileReader(m_fileName,sizeof(T));
				//如果文件不存在则创建。
				if (!m_pathBasic.IsFileExist(theFile))
				{
					//这里因为该函数可以达到效果故使用，并非移除所有数据的意思
					m_pFileReaderCtrl->RemoveAllData(); 
				}
			}
			return (m_pFileReaderCtrl!=0);
		}

		//链接到文件
		bool ConnectToFile(void)
		{
			if ( !InitFileReaderCtrl() )
				return false;
			if (m_pFileReaderCtrl->ConnectFile())
				return true;
			return false;
		}

		//释放文件
		bool DisconnectFile(void)
		{
			if ( m_pFileReaderCtrl!=0 && m_pFileReaderCtrl->DisconnectFile() )
			{
				return true;
			}
			return false;
		}

		//获取记录的数目
		int GetCount(void)
		{
			if (m_pFileReaderCtrl!=0)
			{
				return m_pFileReaderCtrl->GetDataCount();
			}
			return -1;
		}
		//获取索引为idx的记录
		const T* GetOneItem (int idx)
		{
			if (m_pFileReaderCtrl!=0)
			{
				size_t readSize;
				return reinterpret_cast<const T *>(
					m_pFileReaderCtrl->GetBlockData(idx,readSize));
			}
			return 0;
		}
		//添加一条记录
		bool AddOneItem (const T &curItem)
		{
			if (m_pFileReaderCtrl!=0)
			{
				m_pFileReaderCtrl->AddBlockData(
					reinterpret_cast<const char *>(&curItem), CFileBasic::UE_SEEK_END
					);
				return true;
			}
			return false;
		}
		//删除索引为idx的记录
		bool RemoveOneItem(int idx)
		{
			if (m_pFileReaderCtrl!=0)
			{
				m_pFileReaderCtrl->RemoveBlockData(idx);
				return true;
			}
			return false;
		}
		//删除所有记录
// 		bool RemoveAllItem()
// 		{
// 			HANDLE handleWrite;
// 			// 覆盖原文件
// 			handleWrite=::CreateFile(m_fileName.c_str(),GENERIC_WRITE,0,
// 				NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
// 			if (INVALID_HANDLE_VALUE==handleWrite)
// 			{
// 				return false;
// 			}
// 			//更新文件
// 			int irealRWSize=0;
// 			int value=0;
// 			SetFilePointer(handleWrite,0,0,FILE_BEGIN);
// 			if ( !(WriteFile(handleWrite,&value,sizeof(int),(LPDWORD)&irealRWSize,NULL)) )
// 			{
// 				CloseHandle(handleWrite);
// 				return false;
// 			}
// 			//关闭文件
// 			CloseHandle(handleWrite);
// 			return true;
// 		}
		//更新索引为idx的记录
		bool UpdateOneItem(const T &curItem,int idx)
		{
			if (m_pFileReaderCtrl!=0)
			{
				m_pFileReaderCtrl->EditBlockData(idx,reinterpret_cast<const char *>(&curItem));
				return true;
			}
			return false;
		}


	private:
		CFileController(tstring fileName):m_pathBasic(CPathBasic::Get()), 
			m_fileBasic(CFileBasic::Get()),m_fileName(fileName),m_pFileReaderCtrl(0)
		{
			if ( !InitFileReaderCtrl() )
			{
				assert("文件初始化失败");
				return;
			}
		}
		//CFileController(void){};
		// 防止使用赋值时调用默认拷贝构造函数
		CFileController & operator = (const CFileController& var);
		CFileController(const CFileController& var);

		tstring m_fileName;	//文件名
		const CPathBasic &m_pathBasic;
		const CFileBasic &m_fileBasic;
		//const CStringBasic &m_stringBasic;
		CCustomerFileReader *m_pFileReaderCtrl;
	};
}
#endif


