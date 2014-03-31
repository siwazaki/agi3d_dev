//
//  BaseController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "AppDelegete.h"

using namespace agi3d;

std::unique_ptr<AppDelegete> AppDelegete::m_instance;
std::once_flag AppDelegete::m_onceFlag;

AppDelegete& AppDelegete::instance()
{
  std::call_once(m_onceFlag,
                 [] {
                   m_instance.reset(new AppDelegete);
                 });
  return *m_instance.get();
}


AppDelegete::AppDelegete()
{
  //ビューの作成
  _frame = std::shared_ptr<Frame>(new Frame(wxT("Active Graph Interface 3D")));
  
  _menuBar = std::shared_ptr<MenuBar>(new MenuBar());
  _frame->SetMenuBar(_menuBar.get());
  
  _base = std::shared_ptr<wxSplitterWindow>(new wxSplitterWindow(_frame.get(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE));
  _base->SetSashGravity(0.80);
  _base->SetMinimumPaneSize(100);
  
  int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16};
  _graphicPanel = std::shared_ptr<GraphicPanel>(new GraphicPanel(_base.get(), args));
  _controlPanel = std::shared_ptr<ControlPanel>(new ControlPanel(_base.get()));
  
  _base->SplitVertically(_graphicPanel.get(), _controlPanel.get());

}

AppDelegete::~AppDelegete()
{
  
}

void AppDelegete::initControllers()
{
  //コントローラの作成
  _userDefaultController = std::shared_ptr<UserDefaultController>(new UserDefaultController());
  _configurationController = std::shared_ptr<ConfigurationController>(new ConfigurationController());
  _graphicController = std::shared_ptr<GraphicController>(new GraphicController());
}

void AppDelegete::run()
{
  _frame->Show(true);
}

const std::shared_ptr<MenuBar>& AppDelegete::getMenuBar(){
  return _menuBar;
}

const std::shared_ptr<Frame>& AppDelegete::getFrame()
{
  return _frame;
}

const std::shared_ptr<wxSplitterWindow>& AppDelegete::getBase()
{
  return _base;
}

const std::shared_ptr<GraphicPanel>& AppDelegete::getGraphicPanel()
{
  return _graphicPanel;
}

const std::shared_ptr<ControlPanel>& AppDelegete::getControlPanel()
{
  return _controlPanel;
}



