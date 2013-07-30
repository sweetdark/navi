// ivTTSPlayer.h: interface for the ivCTTSPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IV_TTS_PLAYER_H__BE81E2A4_23AF_4176_AD6C_071CDF5F5694__INCLUDED_)
#define AFX_IV_TTS_PLAYER_H__BE81E2A4_23AF_4176_AD6C_071CDF5F5694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

// TODO:
// Below clumsy include references is caused by different tts version from iFly productor
//...
#ifdef _WIN32_WCE
#include "third\arm\ivTTS.h"
#else
#include "third\x86\ivTTS.h"
#endif

//
#include "ivThread.h"
#include "WavePlayer.h"

/* constant for TTS heap size */
#define ivTTS_HEAP_SIZE		128000

/* constant for cache allocation */
#define ivTTS_CACHE_SIZE	512
#define ivTTS_CACHE_COUNT	1024
#define ivTTS_CACHE_EXT		8

/* constant for process state parameter */
#define ivTTS_PARAM_STATE	 0x000000FF

/* constants for values of parameter ivTTS_PARAM_STATE */
#define ivTTS_STATE_STOP	0	/* stopped */
#define ivTTS_STATE_PLAY	1	/* playing */
#define ivTTS_STATE_PAUSE	2	/* pausing */

class ivCTTSPlayer : ivCThreadImpl, CWavePlayer
{
	ivPByte m_pHeap;
	ivPResPackDescExt m_pResPackDesc;
	DWORD m_nResCount;
	ivHTTS m_hTTS;
	ivCThread* m_pThread;
	bool m_bStopMsg;

	ivTTSErrID DoGeneralMessage(ivPUInt32 lpMsg);
	int ThreadProc();
	ivTTSErrID DoMessage();

	static ivBool ivCall ReadResCB(ivPointer, ivPointer, ivResAddress, ivResSize);

	static ivTTSErrID ivCall ParamChCB(ivPointer, ivUInt32, ivUInt32);
	static ivTTSErrID ivCall ProgressCB(ivPointer, ivUInt32, ivUInt32);
	static ivTTSErrID ivCall InputCB(ivPointer, ivPointer, ivSize ivPtr);
	static ivTTSErrID ivCall OutputCB(ivPointer, ivUInt16, ivCPointer, ivSize);
	static ivTTSErrID ivCall WatchCB(ivPointer, ivUInt16, ivCPointer, ivSize, ivCPointer, ivSize);

public:
	ivCTTSPlayer() { m_pHeap = NULL; m_pThread = NULL; }
	virtual ~ivCTTSPlayer() { Destroy(); }

	void Create(ivStr szResPath, ivTThreadPriority enPriority = ivThreadPriority_RealTime) { Create(&szResPath, 1, enPriority); }
	void Create(ivStr szResPath[], ivSize nResCount, ivTThreadPriority enPriority = ivThreadPriority_RealTime);
	void Destroy();

protected:
	/* parameter change callback */
	virtual ivTTSErrID OnParamChange(
		ivUInt32	nParamID,		/* [in] parameter ID */
		ivUInt32	nParamValue );	/* [in] parameter value */

protected:
	/* progress callback */
	virtual ivTTSErrID OnProgress(
		ivUInt32	iProcPos,		/* [in] current processing position */
		ivUInt32	nProcLen );		/* [in] current processing length */

private:
	/* input callback */
	virtual ivTTSErrID OnInput(
		ivPointer	pText,			/* [out] input text buffer */
		ivSize&		nSize );		/* [in/out] input buffer/text size */

protected:
	/* output callback */
	virtual ivTTSErrID OnOutput(
		ivUInt16	nCode,			/* [in] output data code */
		ivCPointer	pcData,			/* [in] output data buffer */
		ivSize		nSize );		/* [in] output data size */

protected:
	/* watch callback */
	virtual ivTTSErrID OnWatch(
		ivUInt16	nSylType,		/* [in] syllable type */
		ivCPointer	pcSylText,		/* [in] syllable text buffer */
		ivSize		nTextLen,		/* [in] syllable text length */
		ivCPointer	pcSylPhone,		/* [in] syllable phoneme buffer */
		ivSize		nPhoneLen );	/* [in] syllable phoneme length */

public:
	ivTTSErrID SynthText(ivStr szText, ivSize nLength = -1);
	ivTTSErrID SynthStart();
	ivTTSErrID Cancel();

	ivTTSErrID Pause();
	ivTTSErrID Resume();

	ivUInt32 GetParam(ivUInt32 nParamID);
	ivTTSErrID GetParam(ivUInt32 nParamID, ivUInt32& nParamValue);
	ivTTSErrID SetParam(ivUInt32 nParamID, ivUInt32 nParamValue);

	inline ivUInt16 GetState() { return (ivUInt16)GetParam(ivTTS_PARAM_STATE); }
};

#endif // !defined(AFX_IV_TTS_PLAYER_H__BE81E2A4_23AF_4176_AD6C_071CDF5F5694__INCLUDED_)
