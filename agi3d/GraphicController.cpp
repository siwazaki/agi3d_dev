//
//  GraphicController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "GraphicController.h"
#include "AppDelegete.h"

using namespace agi3d;

GraphicController::GraphicController(const std::shared_ptr<Graph>& graph)
{
  _graph = graph;
  _graphicPanel = AppDelegete::instance().getGraphicPanel();
}

GraphicController::~GraphicController()
{
  
}