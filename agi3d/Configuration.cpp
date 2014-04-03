//
//  Configuration.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "Configuration.h"

using namespace agi3d;

Configuration::Configuration() : _nodethreshold_t(10000.0f), _nodethreshold_b(-1.0f), _edgethreshold_t(10000.0f), _edgethreshold_b(-1.0f)
{
  
}

Configuration::~Configuration()
{
  
}

const std::string& Configuration::graphName()
{
  return _graphName;
}