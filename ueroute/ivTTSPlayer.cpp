// ivTTSPlayer.cpp: implementation of the ivCTTSPlayer class.
//
//////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include "ivTTSPlayer.h"

enum { CmdSynthText, CmdSynthStart, CmdCancel,
	CmdPause, CmdResume, CmdState, CmdGetParam, CmdSetParam };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void ivCTTSPlayer::Create(ivStr szResPath[], ivSize nResCount, ivTThreadPriority enPriority)
{
	Destroy();

	m_pHeap = new ivByte[ivTTS_HEAP_SIZE];
	m_pResPackDesc = new ivTResPackDescExt[m_nResCount = nResCount];
	ZeroMemory(m_pResPackDesc, sizeof(*m_pResPackDesc) * nResCount);

	for (ivSize i = 0; i < nResCount; ++ i)
	{
		m_pResPackDesc[i].pCBParam = _tfopen((LPCTSTR)szResPath[i], __T("rb"));
		m_pResPackDesc[i].pfnRead = ReadResCB;
		m_pResPackDesc[i].pfnMap = NULL;
		m_pResPackDesc[i].nSize = 0;

		if (m_pResPackDesc[i].pCBParam)
		{
			m_pResPackDesc[i].pCacheBlockIndex = new ivUInt8[ivTTS_CACHE_COUNT + ivTTS_CACHE_EXT];
			m_pResPackDesc[i].pCacheBuffer = new ivUInt8[ivTTS_CACHE_COUNT + ivTTS_CACHE_EXT][ivTTS_CACHE_SIZE];
			m_pResPackDesc[i].nCacheBlockSize = ivTTS_CACHE_SIZE;
			m_pResPackDesc[i].nCacheBlockCount = ivTTS_CACHE_COUNT;
			m_pResPackDesc[i].nCacheBlockExt = ivTTS_CACHE_EXT;
		}
	}

#if defined(DEBUG) || defined(_DEBUG)
	m_pThread = NewThread(ivThreadPriority_Idle);
#else
	m_pThread = NewThread(enPriority);
#endif
	m_pThread->Start();
}

void ivCTTSPlayer::Destroy()
{
	if (m_pThread)
	{
		delete m_pThread;

		for (DWORD i = 0; i < m_nResCount; ++ i)
		{
			if (m_pResPackDesc[i].pCBParam)
			{
				fclose((FILE*)m_pResPackDesc[i].pCBParam);
				delete[] m_pResPackDesc[i].pCacheBlockIndex;
				delete[] m_pResPackDesc[i].pCacheBuffer;
			}
		}

		delete[] m_pResPackDesc;
		delete[] m_pHeap;
	}
}

// process general messages for any state
ivTTSErrID ivCTTSPlayer::DoGeneralMessage(ivPUInt32 lpMsg)
{
	switch ( *lpMsg )
	{
	case CmdGetParam:
		return ivTTS_GetParam(m_hTTS, lpMsg[1], lpMsg + 2);
		
	case CmdSetParam:
		return ivTTS_SetParam(m_hTTS, lpMsg[1], lpMsg[2]);
	}
	
	return ivTTS_ERR_STATE_REFUSE;
}

// TTS thread procedure
int ivCTTSPlayer::ThreadProc()
{
	ivTTS_Create(&m_hTTS, m_pHeap, ivTTS_HEAP_SIZE, this, m_pResPackDesc, m_nResCount, NULL);
	ivTTS_SetParam(m_hTTS, ivTTS_PARAM_PARAMCH_CALLBACK, (ivUInt32)ParamChCB);
	ivTTS_SetParam(m_hTTS, ivTTS_PARAM_OUTPUT_CALLBACK, (ivUInt32)OutputCB);
#if defined(UNICODE) || defined(_UNICODE)
	ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_UNICODE);
#else
	ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_GBK);
#endif

	// process messages for stopped state
	ivPUInt32 lpMsg;
	while ( (lpMsg = (ivPUInt32)m_pThread->GetMessage()) )
	{
		m_bStopMsg = true;

		ivTTSErrID tErr;
		if ( CmdSynthText == *lpMsg || CmdSynthStart == *lpMsg )
		{
			if ( CmdSynthText == *lpMsg )
			{
				tErr = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_MODE, ivTTS_INPUT_FIXED_BUFFER);
				if ( ivTTS_ERR_OK == tErr )
				{
					tErr = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_TEXT_BUFFER, lpMsg[1]);
					if ( ivTTS_ERR_OK == tErr )
						tErr = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_TEXT_SIZE, lpMsg[2]);
				}
			}
			else
				tErr = ivTTS_SetParam(m_hTTS, ivTTS_PARAM_INPUT_MODE, ivTTS_INPUT_CALLBACK);

			if ( ivTTS_ERR_OK == tErr )
			{
				tErr = OnParamChange(ivTTS_PARAM_STATE, ivTTS_STATE_PLAY);

				if ( ivTTS_ERR_OK == tErr )
				{
					m_pThread->SetResult(ivTTS_ERR_OK);

					m_bStopMsg = false;

					CWavePlayer::Reset();
					tErr = ivTTS_Run(m_hTTS);

					if ( ivTTS_ERR_OK == tErr )
					{
						while ( !m_bStopMsg && !CWavePlayer::Wait(1) )
							tErr = DoMessage();
					}
					else
					{
						CWavePlayer::Reset();
					}

					OnParamChange(ivTTS_PARAM_STATE, ivTTS_STATE_STOP);
				}
			}
		}
		else if ( CmdState == *lpMsg )
		{
			lpMsg[1] = ivTTS_STATE_STOP;
			tErr = ivTTS_ERR_OK;
		}
		else
		{
			tErr = DoGeneralMessage(lpMsg);
		}

		if ( m_bStopMsg )
			m_pThread->SetResult((int)tErr);
	}

	ivTTS_Destroy(m_hTTS);
	return 0;
}

// process messages for playing state
ivTTSErrID ivCTTSPlayer::DoMessage()
{
	if ( !m_pThread->TestMessage() )
		return ivTTS_ERR_OK;

	ivPUInt32 lpMsg = (ivPUInt32)m_pThread->GetMessage();
	if ( !lpMsg )
		return ivTTS_ERR_FAILED;

	ivTTSErrID tErr;
	switch ( *lpMsg )
	{
	case CmdPause:
		// switch into pausing state
		CWavePlayer::Pause();
		tErr = OnParamChange(ivTTS_PARAM_STATE, ivTTS_STATE_PAUSE);
		if ( tErr != ivTTS_ERR_OK )
		{
			CWavePlayer::Reset();
			m_bStopMsg = true;
			return tErr;
		}

		// return OK for Pause call
		m_pThread->SetResult(tErr);

		// process messages for pausing state
		for ( ; ; )
		{
			lpMsg = (ivPUInt32)m_pThread->GetMessage();
			if ( !lpMsg )
				return ivTTS_ERR_FAILED;

			if ( CmdResume == *lpMsg )
				break;

			if ( CmdCancel == *lpMsg )
			{
				CWavePlayer::Reset();
				tErr = ivTTS_Exit(m_hTTS);
				if ( ivTTS_ERR_OK == tErr )
				{
					m_bStopMsg = true;
					return tErr;
				}
			}
			else if ( CmdState == *lpMsg )
			{
				lpMsg[1] = ivTTS_STATE_PAUSE;
				tErr = ivTTS_ERR_OK;
			}
			else
				tErr = DoGeneralMessage(lpMsg);

			m_pThread->SetResult((int)tErr);
		}

		// switch back into playing state
		tErr = OnParamChange(ivTTS_PARAM_STATE, ivTTS_STATE_PLAY);
		if ( tErr != ivTTS_ERR_OK )
		{
			CWavePlayer::Reset();
			m_bStopMsg = true;
			return tErr;
		}
		CWavePlayer::Resume();
		break;

	case CmdCancel:
		CWavePlayer::Reset();
		tErr = ivTTS_Exit(m_hTTS);
		if ( ivTTS_ERR_OK == tErr )
		{
			m_bStopMsg = true;
			return tErr;
		}
		break;

	case CmdState:
		lpMsg[1] = ivTTS_STATE_PLAY;
		tErr = ivTTS_ERR_OK;
		break;

	default:
		tErr = DoGeneralMessage(lpMsg);
	}

	m_pThread->SetResult((int)tErr);
	return ivTTS_ERR_OK;
}

/* read resource callback */
ivBool ivCall ivCTTSPlayer::ReadResCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivPointer		pBuffer,		/* [out] read resource buffer */
	ivResAddress	iPos,			/* [in] read start position */
	ivResSize		nSize )			/* [in] read size */
{
	FILE* pFile = (FILE*)pParameter;
	fseek(pFile, iPos, SEEK_SET);
	fread(pBuffer, nSize, 1, pFile);

	return true;
}

/* parameter change callback */
ivTTSErrID ivCall ivCTTSPlayer::ParamChCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivUInt32		nParamID,		/* [in] parameter id */
	ivUInt32		nParamValue )	/* [in] parameter value */
{
	ivCTTSPlayer* pThis = (ivCTTSPlayer*)pParameter;
	ivTTSErrID tErr = pThis->DoMessage();
	if ( tErr != ivTTS_ERR_OK ) return tErr;
	return pThis->OnParamChange(nParamID, nParamValue);
}

/* progress callback */
ivTTSErrID ivCall ivCTTSPlayer::ProgressCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivUInt32		iProcBegin,		/* [in] current processing position */
	ivUInt32		nProcLen )		/* [in] current processing length */
{
	ivCTTSPlayer* pThis = (ivCTTSPlayer*)pParameter;
	ivTTSErrID tErr = pThis->DoMessage();
	if ( tErr != ivTTS_ERR_OK ) return tErr;
	return pThis->OnProgress(iProcBegin, nProcLen);
}

/* input callback */
ivTTSErrID ivCall ivCTTSPlayer::InputCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivPointer		pText,			/* [out] input text buffer */
	ivSize ivPtr	pnSize )		/* [out] input text size */
{
	ivCTTSPlayer* pThis = (ivCTTSPlayer*)pParameter;
	ivTTSErrID tErr = pThis->DoMessage();
	if ( tErr != ivTTS_ERR_OK ) return tErr;
	return pThis->OnInput(pText, *pnSize);
}

/* output callback */
ivTTSErrID ivCall ivCTTSPlayer::OutputCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivUInt16		nCode,			/* [in] output data code */
	ivCPointer		pcData,			/* [in] output data buffer */
	ivSize			nSize )			/* [in] output data size */
{
	ivCTTSPlayer* pThis = (ivCTTSPlayer*)pParameter;
	ivTTSErrID tErr = pThis->DoMessage();
	if ( tErr != ivTTS_ERR_OK || pThis->m_bStopMsg ) return tErr;
	return pThis->OnOutput(nCode, pcData, nSize);
}

/* watch callback */
ivTTSErrID ivCall ivCTTSPlayer::WatchCB(
	ivPointer		pParameter,		/* [in] user callback parameter */
	ivUInt16		nSylType,		/* [in] syllable type */
	ivCPointer		pcSylText,		/* [in] syllable text buffer */
	ivSize			nTextLen,		/* [in] syllable text length */
	ivCPointer		pcSylPhone,		/* [in] syllable phoneme buffer */
	ivSize			nPhoneLen )		/* [in] syllable phoneme length */
{
	ivCTTSPlayer* pThis = (ivCTTSPlayer*)pParameter;
	ivTTSErrID tErr = pThis->DoMessage();
	if ( tErr != ivTTS_ERR_OK ) return tErr;
	return pThis->OnWatch(nSylType, pcSylText, nTextLen, pcSylPhone, nPhoneLen);
}

/* parameter change callback */
ivTTSErrID ivCTTSPlayer::OnParamChange(
	ivUInt32		nParamID,		/* [in] parameter id */
	ivUInt32		nParamValue )	/* [in] parameter value */
{
	if ( ivTTS_PARAM_STATE == nParamID && ivTTS_STATE_STOP == nParamValue )
		ivTTS_Exit(m_hTTS);

	return ivTTS_ERR_OK;
}

/* progress callback */
ivTTSErrID ivCTTSPlayer::OnProgress(
	ivUInt32		iProcPos,		/* [in] current processing position */
	ivUInt32		nProcLen )		/* [in] current processing length */
{
	return ivTTS_ERR_OK;
}

/* input callback */
ivTTSErrID ivCTTSPlayer::OnInput(
	ivPointer		pText,			/* [out] input text buffer */
	ivSize&			nSize )			/* [out] input text size */
{
	return ivTTS_ERR_END_OF_INPUT;
}

/* output callback */
ivTTSErrID ivCTTSPlayer::OnOutput(
	ivUInt16		nCode,			/* [in] output data code */
	ivCPointer		pcData,			/* [in] output data buffer */
	ivSize			nSize )			/* [in] output data size */
{
	CWavePlayer::Append(nCode, pcData, nSize);
	return ivTTS_ERR_OK;
}

/* watch callback */
ivTTSErrID ivCTTSPlayer::OnWatch(
	ivUInt16		nSylType,		/* [in] syllable type */
	ivCPointer		pcSylText,		/* [in] syllable text buffer */
	ivSize			nTextLen,		/* [in] syllable text length */
	ivCPointer		pcSylPhone,		/* [in] syllable phoneme buffer */
	ivSize			nPhoneLen )		/* [in] syllable phoneme length */
{
	return ivTTS_ERR_OK;
}

ivTTSErrID ivCTTSPlayer::SynthText(ivStr szText, ivSize nLength)
{
	if ( -1 == nLength )
	{
		nLength = 0;
		while ( szText[nLength] )
			++ nLength;
	}

	ivUInt32 lpMsg[3] = { CmdSynthText, (ivUInt32)szText, (ivUInt32)(nLength * sizeof(*szText)) };
	return m_pThread->SendMessage(lpMsg);
}

ivTTSErrID ivCTTSPlayer::SynthStart()
{
	ivUInt32 nMsg = CmdSynthStart;
	return m_pThread->SendMessage(&nMsg);
}

ivTTSErrID ivCTTSPlayer::Cancel()
{
	ivUInt32 nMsg = CmdCancel;
	return m_pThread->SendMessage(&nMsg);
}

ivTTSErrID ivCTTSPlayer::Pause()
{
	ivUInt32 nMsg = CmdPause;
	return m_pThread->SendMessage(&nMsg);
}

ivTTSErrID ivCTTSPlayer::Resume()
{
	ivUInt32 nMsg = CmdResume;
	return m_pThread->SendMessage(&nMsg);
}

ivUInt32 ivCTTSPlayer::GetParam(ivUInt32 nParamID)
{
	ivUInt32 nParamValue;

	if ( ivTTS_ERR_OK == GetParam(nParamID, nParamValue) )
		return nParamValue;

	return ivNull;
}

ivTTSErrID ivCTTSPlayer::GetParam(ivUInt32 nParamID, ivUInt32& nParamValue)
{
	ivUInt32 lpMsg[3];
	ivTTSErrID tErr;

	switch ( nParamID )
	{
	case ivTTS_PARAM_PARAMCH_CALLBACK:
	case ivTTS_PARAM_INPUT_CALLBACK:
	case ivTTS_PARAM_PROGRESS_CALLBACK:
	case ivTTS_PARAM_OUTPUT_CALLBACK:
	// TODO:
    // Below comment out is caused by different TTS version from iFly product
//#ifdef _WIN32_WCE
//	case ivTTS_PARAM_WATCH_CALLBACK:
//#endif
	case ivTTS_PARAM_INPUT_TEXT_BUFFER:
	case ivTTS_PARAM_INPUT_TEXT_SIZE:
		tErr = ivTTS_ERR_INVALID_PARAM_ID;

	case ivTTS_PARAM_STATE:
		lpMsg[0] = CmdState;
		tErr = m_pThread->SendMessage(lpMsg);
		if ( ivTTS_ERR_OK == tErr )
			nParamValue = lpMsg[1];
		break;

	default:
		lpMsg[0] = CmdGetParam;
		lpMsg[1] = nParamID;
		tErr = m_pThread->SendMessage(lpMsg);
		if ( ivTTS_ERR_OK == tErr )
			nParamValue = lpMsg[2];
	}

	return tErr;
}

ivTTSErrID ivCTTSPlayer::SetParam(ivUInt32 nParamID, ivUInt32 nParamValue)
{
	switch ( nParamID )
	{
	case ivTTS_PARAM_PARAMCH_CALLBACK:
	case ivTTS_PARAM_INPUT_CALLBACK:
	case ivTTS_PARAM_PROGRESS_CALLBACK:
	case ivTTS_PARAM_OUTPUT_CALLBACK:
	// TODO:
    // Below comment out is caused by different TTS version from iFly product
//#ifdef _WIN32_WCE
//	case ivTTS_PARAM_WATCH_CALLBACK:
//#endif
	case ivTTS_PARAM_INPUT_TEXT_BUFFER:
	case ivTTS_PARAM_INPUT_TEXT_SIZE:
	case ivTTS_PARAM_STATE:
		return ivTTS_ERR_INVALID_PARAM_ID;
	}

	ivUInt32 lpMsg[3] = { CmdSetParam, nParamID, nParamValue };
	return m_pThread->SendMessage(lpMsg);
}
