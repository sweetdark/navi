// ivThread.h: interface for the CIsThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IV_THREAD_H__1EC2C16C_76A3_4553_B833_22E60862F81A__INCLUDED_)
#define AFX_IV_THREAD_H__1EC2C16C_76A3_4553_B833_22E60862F81A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class ivCThread;
class ivCThreadImpl;

enum ivTThreadPriority { ivThreadPriority_Idle, ivThreadPriority_Normal, ivThreadPriority_RealTime };

class ivCThread
{
	friend class ivCThreadImpl;

	DWORD m_dwThreadID;
	HANDLE m_hThread;

	void* m_lpMsg;
	int m_nRet;
	bool m_bExiting;

	HANDLE m_hMsgEmpty, m_hMsgFull;
	HANDLE m_hRetEmpty, m_hRetFull;

protected:
	ivCThread(ivCThreadImpl* pThreadImpl, ivTThreadPriority enPriority = ivThreadPriority_Normal);

public:
	~ivCThread();

	// start
	void Start();

	// used out of this thread
	int SendMessage(void* lpMsg);

	// used in this thread
	bool TestMessage();
	void* GetMessage();
	void SetResult(int nRet);
};

class ivCThreadImpl
{
	friend class ivCThread;

	static DWORD WINAPI ThreadProcS(ivCThreadImpl* lpThreadImpl);
	virtual int ThreadProc() = 0;

public:
	inline ivCThread* NewThread(ivTThreadPriority enPriority = ivThreadPriority_Normal)
	{
		return new ivCThread(this, enPriority);
	}
};

#endif // !defined(AFX_IV_THREAD_H__1EC2C16C_76A3_4553_B833_22E60862F81A__INCLUDED_)
