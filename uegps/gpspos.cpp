/*
* The UrbanEye PND solution is a collection of packages based on native C++ classes
* that implement the fundamental algorithms & operations required to satisfy one kind
* of mobile or portable devices with competitive qualities being a leading one.
*
* Copyright (C) 2008 Xiujie Sheng
*
* This library isn't a free software, so you cann't take the sourcecode without author's permit.
* Of course, there is NO ANY WARRANTY that this library will harm your computer or ongoing business 
* if you illegally got it. 
*
* NOTE: 
* AUTHOR WOULD ALWAYS RESERVE LEGAL RIGHT TO PROTECT WHAT HE HAVE DONE ACCORDING TO
* LOCAL OR INTERNATIONAL COPYRIGHT OR RELATIVE LAW(S).
*
* For more information, contact:
*
* China University of Geoseciences(Wuhan)
* Graduate School
*
* Xiujie Sheng
* smartpoint@sohu.com
*/
#include "uebase\dbgmacro.h"
using namespace UeBase;

// Refer to UeGps
#include "gpspos.h"
#include "infopro.h"
#include "mapmatch.h"
using namespace UeGps;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
CGpsPos::CGpsPos(CPosInfo *parent, CInfoPro *infoPro) : CPosInfo(parent, infoPro)
{
  // Source type of one position
  m_type = PT_Gps;

  // Time bias
#ifdef _WINDOWS
  TIME_ZONE_INFORMATION tz;
  ::GetTimeZoneInformation(&tz);
  m_pos.m_timeBias = tz.Bias;
#else
  assert(false);
#endif

  // Processed str
  ::memset(m_packet, 0, sizeof(m_packet));
  m_packetLen = 0;
  m_packetPos = 0;
}

/**
*
*/
CGpsPos::~CGpsPos()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/**
*
*/
bool CGpsPos::Process(char *content, int length)
{
  //
  char *buf = content;
  while(length--)
  {
    // Meeting the begining of one sentence
    if(*buf == '$')
    {
      // Means getting one sentence
      if(m_packetPos)
      {
        *m_packetPos = '\0';
        Parse(m_packet);
      }

      // Continue to find from begining
      m_packetPos = m_packet;
      m_packetLen = 1;
      *m_packetPos++ = *buf++;

      continue;
    }
    else if(m_packetPos) // Search along the left part
    {
      if(*buf == '\r' || *buf == '\n')
      {
        *m_packetPos = '\0';
        Parse(m_packet);

        // For next sentences
        m_packetPos = 0;
        buf++;
        m_packetLen = 0;
        continue;
      }

      *m_packetPos++ = *buf++;
      m_packetLen++;
      if(m_packetLen >= MAXPACKETLENGTH)
      {
        assert(false);
        m_packetPos = 0;
        m_packetLen = 0;
      }

      continue;
    }

    // Skip those partial sentences 
    buf++;
  }

  //m_packetPos = 0;
  //m_packetLen = 0;
  return  true;
}

/**
* Note: Forward calling since there maybe exist different types of parse algorithms
*/
inline bool CGpsPos::Parse(const char *str)
{
  assert(m_infoPro);

  bool rt = m_infoPro->Parse(str, &m_pos);
  //assert(rt);

  return rt;
}

/**
*
*/
inline bool CGpsPos::IsLive()
{
  return m_pos.m_liveCount > 0;
}

/**
*
*/
inline bool CGpsPos::IsValid()
{
  // TODO:
  //
  // 1)
  if(m_pos.m_lon < 0. && m_pos.m_lat < 0.)
  {
    return false;
  }

  // 2)
  short mode = IMapMatch::GetMapMatch()->GetMatchMode();
  if(mode == MM_Walk)
  {
    if(m_pos.m_HDOP > WALKHDOPLIMITATION /*|| m_pos.m_HDOP == 0*/)
    {
      return false;
    }
  }
  else if(mode == MM_Car)
  {
    if(m_pos.m_HDOP > CARHDOPLIMITATION /*|| m_pos.m_HDOP == 0*/)
    {
      return false;
    }
  }
  else
  {
    assert(false);
  }

  return true;
}

/**
*
*/
inline int CGpsPos::SndCount()
{
  return m_pos.m_sndCount;
}

/**
*
*/
inline void CGpsPos::SndCount(int count)
{
  m_pos.m_sndCount = count;
}

/**
*
*/
inline void CGpsPos::SndAdd()
{
  m_pos.m_sndCount++;
}

/**
*
*/
inline void CGpsPos::SndDecrease()
{
  m_pos.m_sndCount--;
  if(m_pos.m_sndCount < 0)
  {
    m_pos.m_sndCount = 0;
  }
}

/**
*
*/
inline bool CGpsPos::GetPos(PosBasic &pos, bool isCounted)
{
  //
  if(isCounted)
  {
    //
    if(m_pos.m_receiveCount)
    {
      //
      m_pos.m_receiveCount--;
      if(m_pos.m_liveCount > 0)
      {
        //
        m_pos.m_liveCount--;

        //
        //GetStatistic();
      }
    }
    else
    {
      m_pos.m_liveCount = 0;
    }
  }

  // Get the quality score description following by HDOP description
  if(m_pos.m_HDOP > 0 && m_pos.m_HDOP < 6)
  {
    m_pos.m_score = 0;
  }
  else if(m_pos.m_HDOP >= 6 && m_pos.m_HDOP < 15)
  {
    m_pos.m_score = 5;
  }
  else 
  {
    m_pos.m_score = 10;
  }

  //
  pos = m_pos;
  return (m_pos.m_qualityIndicator) ? m_pos.m_liveCount >= 0 : false;
}

/**
*
*/
inline bool CGpsPos::GetPos(GpsBasic &pos, bool isCounted)
{
  //
  if(isCounted)
  {
    //
    if(m_pos.m_receiveCount)
    {
      //
      m_pos.m_receiveCount--;

      //
      if(m_pos.m_liveCount)
      {
        m_pos.m_liveCount--;
      }
    }
    else
    {
      m_pos.m_liveCount = 0;
    }
  }

  //
  pos = m_pos;
  return m_pos.m_liveCount >= 0;
}

/**
* Note:
* Below part commented out is the code for letting GPS signals sensitive to higher and lower ways' switch
**/
inline void CGpsPos::GetStatistic()
{
  // Only store the positions after strict filter
  if(m_pos.m_speed > 5. && m_pos.m_qualityIndicator && m_pos.m_HDOP < 6 && m_pos.m_VDOP < 6)
  {
    //
    if(m_positions.size() >= MAXCACHESIZE)
    {
      //
      double speedSum = 0.;
      int i = 0;
      int count = static_cast<int>(m_positions.size());
      for(; i < count; i++)
      {
        speedSum += m_positions[i].m_speed;
      }

      //
      m_speedAvg = speedSum/MAXCACHESIZE;
      m_positions.clear();

      ////
      //double altSum = 0.;
      //double speedSum = 0.;
      //int i = 0;
      //int count = m_positions.size();
      //for(; i < count; i++)
      //{
      //	altSum += m_positions[i].m_alt;
      //	speedSum += m_positions[i].m_speed;
      //}

      //// Consider the first time
      //double altAvg = altSum/MAXCACHESIZE;
      //double speedAvg = speedSum/MAXCACHESIZE;
      //if(m_altAvg == -10000.)
      //{
      //	m_altAvg = altAvg;
      //}
      //if(m_speedAvg == -10000.)
      //{
      //	m_speedAvg = speedAvg;
      //}
      //int altDiff = static_cast<int>(altAvg - m_altAvg);
      //int speedDiff = static_cast<int>(speedAvg - m_speedAvg);

      ////
      //unsigned int upDown = m_pos.m_change & 0x00FF;
      //if(altDiff > MINALTDIFFERENCE)
      //{
      //	// If it is initial status or down status
      //	if(upDown == 0xFF || upDown == 0x80)
      //	{
      //		upDown = PC_Lift;
      //	}
      //	else
      //	{
      //		//upDown <<= 1;
      //		//if(upDown >= 0x08)
      //		{
      //			upDown = 0x08;
      //			m_altAvg = altAvg;
      //		}
      //	}
      //}
      //else if(altDiff < -MINALTDIFFERENCE)
      //{
      //	// If it is initial status or lift status
      //	if(upDown == 0xFF || upDown == 0x08)
      //	{
      //		upDown = PC_Down;
      //	}
      //	else
      //	{
      //		//upDown <<= 1;
      //		//if(upDown >= 0x80)
      //		{
      //			upDown = 0x80;
      //			m_altAvg = altAvg;
      //		}
      //	}
      //}
      //else
      //{
      //	upDown = upDown & PC_Same;
      //}

      ////
      //unsigned int fastSlow = m_pos.m_change & 0xFF00;
      //if(speedDiff > MINSPEEDDIFFERENCE)
      //{
      //	// if it is initial or slow status
      //	if(fastSlow == 0xFF00 || fastSlow == 0x8000)
      //	{
      //		fastSlow = PC_Fast;
      //	}
      //	else
      //	{
      //		//fastSlow <<=1;
      //		//if(fastSlow >= 0x0800)
      //		{
      //			fastSlow = 0x0800;
      //			m_speedAvg = speedAvg;
      //		}
      //	}
      //}
      //else if(speedDiff < -MINSPEEDDIFFERENCE)
      //{
      //	// if it is initial or fast status
      //	if(fastSlow == 0xFF00 || fastSlow == 0x0800)
      //	{
      //		fastSlow = PC_Slow;
      //	}
      //	else
      //	{
      //		//fastSlow <<=1;
      //		//if(fastSlow >= 0x8000)
      //		{
      //			fastSlow = 0x8000;
      //			m_speedAvg = speedAvg;
      //		}
      //	}
      //}
      //else
      //{
      //	fastSlow = fastSlow & PC_Same;
      //}

      ////
      //m_pos.m_change = fastSlow | upDown;
      //m_positions.clear();
    }

    //
    m_positions.push_back(m_pos);
  }
}

void UeGps::CGpsPos::ClearPos()
{
  ::memset(&m_pos, 0, sizeof(m_pos));
}
