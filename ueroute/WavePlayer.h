// WavePlayer.h: interface for the CWavePlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CWAVEPLAYER_H__2B987EE0_790D_4E71_AF63_FE169D1FFB87__INCLUDED_)
#define AFX_CWAVEPLAYER_H__2B987EE0_790D_4E71_AF63_FE169D1FFB87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Win32Speaker.h"

class CWavePlayer : CWin32Speaker
{
	unsigned short m_wPrevCode;

public:
	CWavePlayer();
	virtual ~CWavePlayer();

	void Append(unsigned short wCode, void const* lpData, unsigned long nSize);

	void Pause();
	void Resume();

	bool Wait(int nMsTimeout = -1);
	void Reset();
};

#endif // !defined(AFX_CWAVEPLAYER_H__2B987EE0_790D_4E71_AF63_FE169D1FFB87__INCLUDED_)
