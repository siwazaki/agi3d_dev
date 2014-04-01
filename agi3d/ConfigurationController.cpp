//
//  ConfigurationController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "ConfigurationController.h"
#include "AppDelegete.h"

using namespace agi3d;

ConfigurationController::ConfigurationController() {
  _current = std::shared_ptr<Configuration>(new Configuration());
  _controlPanel = AppDelegete::instance().getControlPanel();
                                       
}

ConfigurationController::~ConfigurationController() {
  
}

void ConfigurationController::changeGraphName(const std::string &graphName)
{
  _current->_graphName = graphName;
}


