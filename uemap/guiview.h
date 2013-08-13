/**
* Copyright (c) 2012-2013 Guangdong Ritu Information System Co.,Ltd.
* author: Junwei Qiu
*/ 
#ifndef _UEMAP_GUIVIEW_H
#define _UEMAP_GUIVIEW_H
#include "uemap.h"
#include "viewstate.h"

namespace UeMap
{
  class CViewImpl;
  class CGuiView : public CViewState
  {
  public:
    CGuiView(bool isLand, CViewImpl *navView);

    virtual ~CGuiView();

    virtual void OnDraw(short style);
    /**
    *
    **/
    virtual void DrawProgress(short step);

  };
}
#endif