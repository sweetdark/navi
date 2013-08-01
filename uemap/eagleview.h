/**
* Copyright (c) 2012-2013 Guangdong Ritu Information System Co.,Ltd.
* author : JunWei Qiu
*/ 

#ifndef _UEMAP_EAGLEVIEW_H_
#define _UEMAP_EAGLEVIEW_H_

#include "uemap.h"
#include "viewstate.h"

namespace UeMap 
{
  class CViewImpl;
  class UEMAP_CLASS CEagleView : public CViewState
  {
  public:
    CEagleView(bool isLand, CViewImpl *navView);

    virtual ~CEagleView();

    virtual void OnDraw(short style);

    virtual void SetScrLayout(const ScreenLayout &layout);

  };
}
#endif //_UEMAP_EAGLEVIEW_H_