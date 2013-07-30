// ivThread.cpp: implementation of the ivCThread class.
//
//////////////////////////////////////////////////////////////////////

#include "ivThread.h"


ivCThread::ivCThread(ivCThreadImpl* pThreadImpl, ivTThreadPriority enPriority)
{
	m_hMsgEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_hMsgFull = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hRetEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);
	m_hRetFull = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_bExiting = false;
	m_hThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ivCThreadImpl::ThreadProcS,
		pThreadImpl, CREATE_SUSPENDED, &m_dwThreadID);

	switch ( enPriority )
	{
	case ivThreadPriority_Idle:
		SetThreadPriority(m_hThread, THREAD_PRIORITY_IDLE);
		break;

	case ivThreadPriority_Normal:
		SetThreadPriority(m_hThread, THREAD_PRIORITY_NORMAL);
		break;

	case ivThreadPriority_RealTime:
		SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		break;
	}
}

ivCThread::~ivCThread()
{
	m_bExiting = true;
	m_lpMsg = NULL;
	SetEvent(m_hMsgFull);

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);

	CloseHandle(m_hRetFull);
	CloseHandle(m_hRetEmpty);

	CloseHandle(m_hMsgFull);
	CloseHandle(m_hMsgEmpty);
}

void ivCThread::Start()
{
	ResumeThread(m_hThread);
}

int ivCThread::SendMessage(void* lpMsg)
{
	WaitForSingleObject(m_hMsgEmpty, INFINITE);
	m_lpMsg = lpMsg;
	SetEvent(m_hMsgFull);

	WaitForSingleObject(m_hRetFull, INFINITE);
	int nRet = m_nRet;
	SetEvent(m_hRetEmpty);

	return nRet;
}

bool ivCThread::TestMessage()
{
	if ( m_bExiting )
		return true;

	if ( WAIT_OBJECT_0 != WaitForSingleObject(m_hMsgFull, 0) )
		return false;

	SetEvent(m_hMsgFull);
	return true;
}

void* ivCThread::GetMessage()
{
	if ( m_bExiting )
		return NULL;

	while ( WAIT_TIMEOUT == WaitForSingleObject(m_hMsgFull, 10) )
	{
		MSG msg;
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void* lpMsg = (void*)m_lpMsg;
	SetEvent(m_hMsgEmpty);

	return lpMsg;
}

void ivCThread::SetResult(int nRet)
{
	WaitForSingleObject(m_hRetEmpty, INFINITE);
	m_nRet = nRet;
	SetEvent(m_hRetFull);
}

DWORD WINAPI ivCThreadImpl::ThreadProcS(ivCThreadImpl* lpThreadImpl)
{
	return (DWORD)lpThreadImpl->ThreadProc();
}
