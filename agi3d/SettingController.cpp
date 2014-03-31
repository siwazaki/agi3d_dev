//
//  SettingController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include <sstream>
#include "wx/panel.h"

#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"
#include "SettingController.h"


using namespace agi3d;

SettingController::SettingController(std::shared_ptr<GraphicPanel> graphicPanel, std::shared_ptr<ControlPanel> controlPanel)
{
  this->_graphicPanel = graphicPanel;
  this->_controlPanel = controlPanel;
  this->_current = std::shared_ptr<Setting>(new Setting()); 
}

SettingController::~SettingController()
{
  
}

void SettingController::nortifyUpdateNodeSize(wxScrollEvent& event)
{
  std::cout << "aaaa" << std::endl;
}

void SettingController::SetAutoXRotation(wxCommandEvent &event)
{
  std::cout << "SetAutoXRotation" << std::endl;
}