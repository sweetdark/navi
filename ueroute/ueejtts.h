#ifndef _UEROUTE_UEEJTTS_H
#define _UEROUTE_UEEJTTS_H

// Refer to its parent
#ifndef _UEROUTE_UETTS_H
#include "uetts.h"
#endif

namespace UeRoute
{
  class UEROUTE_CLASS CUeEjTTS : public CUeTTS
  {
  public:
    CUeEjTTS();

    virtual ~CUeEjTTS();
  public:
    virtual unsigned int Prepare(bool isReady = true);

    virtual unsigned int TextOut(char *str);

    virtual unsigned int SetRole(unsigned int role);
  };
}

#endif