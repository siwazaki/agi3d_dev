//
//  Setting.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "Setting.h"

using namespace agi3d;

Setting::Setting():_fps(0.0){}

Setting::~Setting(){}

double Setting::getFps() {
  return _fps;
}

void Setting::setFps(double fps)
{
  this->_fps = fps;
}

