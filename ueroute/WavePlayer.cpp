// WavePlayer.cpp: implementation of the CWavePlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "WavePlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWavePlayer::CWavePlayer()
{
	m_wPrevCode = NULL;
}

CWavePlayer::~CWavePlayer()
{
}

void CWavePlayer::Append(unsigned short wCode, void const* lpData, unsigned long nSize)
{
	if ( wCode )
	{
		if ( wCode != m_wPrevCode )
		{
			WAVEFORMATEX wfx;

			switch ( wCode & 0xFF00 )
			{
			case 0x0100:
				wfx.wBitsPerSample = 8;
				break;

			case 0x0200:
				wfx.wBitsPerSample = 16;
				break;

			default:
				return;
			}

			wfx.wFormatTag = WAVE_FORMAT_PCM;
			wfx.nChannels = 1;
			wfx.nSamplesPerSec = (wCode & 0x00FF) * 1000;
			wfx.nBlockAlign = PCM_BLOCK_ALIGN(wfx.wBitsPerSample);
			wfx.nAvgBytesPerSec = PCM_BYTES_PER_SEC(wfx.nChannels, wfx.nSamplesPerSec, wfx.nBlockAlign);
			wfx.cbSize = 0;

			CWin32Speaker::SetWaveFormat(&wfx);
			m_wPrevCode = wCode;
		}

		CWin32Speaker::Append(lpData, nSize);
	}
}

void CWavePlayer::Pause()
{
	CWin32Speaker::Pause();
}

void CWavePlayer::Resume()
{
	CWin32Speaker::Resume();
}

bool CWavePlayer::Wait(int nMsTimeout)
{
	return !!CWin32Speaker::Wait(nMsTimeout);
}

void CWavePlayer::Reset()
{
	CWin32Speaker::Reset();
}
