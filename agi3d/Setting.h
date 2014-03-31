//
//  Setting.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__Setting__
#define __agi3d__Setting__

#include <iostream>

namespace agi3d {  
  class Setting
  {
  public:
    Setting();
    virtual ~Setting();
    double getFps();
    void setFps(double fps);
  private:
    double _fps;
  };
}

#endif /* defined(__agi3d__Setting__) */
