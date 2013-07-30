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

// Refer to platform definitions
#ifndef _MAKE_OPTIONS_H
#include "makeopts.h"
#endif

// Refer to EXPORT & IMPORT macro defintions
#ifndef _UEDECRYPT_BASE_H
#include "uedecrypt.h"
#endif

// Refer to gps basic defintions
#ifndef _UEGPS_GPSBASIC_H
#include "uegps\gpsbasic.h"
#endif
using namespace UeGps;

// Note:
// In Survey bureau, directly replace below c code
#include "safemodule.c"

/**
* The entry point of NavBase DLL
*/
int __stdcall DllMain(void *hModule, unsigned long ul_reason_for_call, void *lpReserved)
{
    //switch (ul_reason_for_call)
    //{
    //case DLL_PROCESS_ATTACH:
    //case DLL_THREAD_ATTACH:
    //case DLL_THREAD_DETACH:
    //case DLL_PROCESS_DETACH:
    //	break;
    //}

    return 1;
}

/**
*
*/
UEDECRYPT_API unsigned int PrepareCHNCoords(const PosTime &time, CoordEncrypted &coords)
{
    // Exceptions
    //if(time.m_year < 1981 || time.m_month < 1 || time.m_month > 12)
    //{
    //    return 0;
    //}

    // The number of leap years passed
    int leapYears = 0;
    int i = 1981;
    for(; i < time.m_year; i++)
    {
        if((i % 4) == 0 && (i % 4) != 100)
        {
            leapYears++;
        }
    }

	// Feb days in a leap year
	int febDays = 0; 
    if((time.m_year % 4) == 0 && (time.m_year % 4) != 100)
    {
        febDays = 29;
    }
	else
    {
        febDays = 28;
    }

	// days for this year
    int days[] = {0, 31,29,31,30,31,30,31,31,30,31,30,31};
    int thisYearDays = 0;

    i = 1;
    for(; i < time.m_month; i++)
    {
        if(i == 2)
        {
            thisYearDays += febDays;
            continue;
        }

        thisYearDays += days[i];
    }
    thisYearDays += time.m_day;

    // days for passed years
    int elpasedDays = (360 + 366 * leapYears + 365 * (time.m_year - 1980 - 1 - leapYears) + thisYearDays) % 7;
    coords.m_time = (((elpasedDays * 24 + time.m_hour ) * 60 + time.m_min) * 60 + time.m_sec) * 1000;
    coords.m_weeks = (360 + 366 * leapYears + 365 * (time.m_year - 1980 - 1 - leapYears) + thisYearDays) / 7;

    //
    return 1;
}

/**
* Factory interface which is the indirect way for serving different outside requirments 
* which may need to support xml, text etc outputter respectively
*/
UEDECRYPT_API unsigned int DecryptCHNCoords(int flag, const PosTime &time, CoordEncrypted &coords)
{
    //
    if(/*!time.m_year &&*/ PrepareCHNCoords(time, coords))
    {
        // TODO:
        // Call the function encapsulated by data vendor in safemodule.c
        coords.m_wgsLon = coords.m_wgsLon * 1024 * 3600;
        coords.m_wgsLat = coords.m_wgsLat * 1024 * 3600;
        if(coords.m_wgsAlt < 1)
        {
            coords.m_wgsAlt = 1;
        }

        //
        unsigned int rt = wgtochina_lb(flag, coords.m_wgsLon, coords.m_wgsLat, coords.m_wgsAlt, coords.m_weeks, coords.m_time, &coords.m_chnX, &coords.m_chnY);

        //
        coords.m_chnX = coords.m_chnX * 100000. / (1024 * 3600);
        coords.m_chnY = coords.m_chnY * 100000. / (1024 * 3600);
    }

    return 0;
}
