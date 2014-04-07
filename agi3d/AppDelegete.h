//
//  BaseController.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__AppDelegete__
#define __agi3d__AppDelegete__

#include <mutex>
#include <memory>
#include "Frame.h"
#include "GraphicPanel.h"
#include "ControlPanel.h"
#include "UserDefaultController.h"
#include "ConfigurationController.h"
#include "GraphicController.h"
#include "AppearanceWindow.h"

namespace agi3d {
  
  class AppDelegete {
    
  public:
    static AppDelegete& instance();
    virtual ~AppDelegete();
    void initMVC();
    void run();
    
    Frame* getFrame() const;
    wxSplitterWindow* getBase() const;
    MenuBar* getMenuBar() const;
    ControlPanel* getControlPanel() const;
    GraphicPanel* getGraphicPanel() const;
    AppearanceWindow* getAppearanceWindow() const;
    
    const std::shared_ptr<ConfigurationController>& getConfigurationController();
    const std::shared_ptr<GraphicController>& getGraphicController();
    
    const std::shared_ptr<Graph>& getGraph();
    const std::shared_ptr<UserDefault>& getUserDefault();
    const std::shared_ptr<Configuration>& getConfiguration();
    
    
  private:
    Frame *_frame;
    wxSplitterWindow *_base;
    MenuBar *_menuBar;
    GraphicPanel *_graphicPanel;
    ControlPanel *_controlPanel;
    AppearanceWindow *_appearanceWindow;
    
    std::shared_ptr<UserDefaultController> _userDefaultController;
    std::shared_ptr<ConfigurationController> _configurationController;
    std::shared_ptr<GraphicController> _graphicController;
    
    std::shared_ptr<Graph> _graph;
    std::shared_ptr<UserDefault> _userDefault;
    std::shared_ptr<Configuration> _configuration;
  
    
  private:
    static std::unique_ptr<AppDelegete> m_instance;
    static std::once_flag m_onceFlag;
    AppDelegete(void);
    AppDelegete(const AppDelegete& src);
    AppDelegete& operator=(const AppDelegete& rhs);
  };
  
}

#endif /* defined(__agi3d__AppDelegete__) */