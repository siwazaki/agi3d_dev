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

namespace agi3d {
  
  class AppDelegete {
    
  public:
    static AppDelegete& instance();
    virtual ~AppDelegete();
    void initControllers();
    void run();
    
    const std::shared_ptr<Frame>& getFrame();
    const std::shared_ptr<wxSplitterWindow>& getBase();
    const std::shared_ptr<MenuBar>& getMenuBar();
    const std::shared_ptr<ControlPanel>& getControlPanel();
    const std::shared_ptr<GraphicPanel>& getGraphicPanel();
    
    
  private:
    std::shared_ptr<Frame> _frame;
    std::shared_ptr<wxSplitterWindow> _base;
    std::shared_ptr<MenuBar> _menuBar;
    std::shared_ptr<GraphicPanel> _graphicPanel;
    std::shared_ptr<ControlPanel> _controlPanel;
    
    std::shared_ptr<UserDefaultController> _userDefaultController;
    std::shared_ptr<ConfigurationController> _configurationController;
    std::shared_ptr<GraphicController> _graphicController;
    
  private:
    static std::unique_ptr<AppDelegete> m_instance;
    static std::once_flag m_onceFlag;
    AppDelegete(void);
    AppDelegete(const AppDelegete& src);
    AppDelegete& operator=(const AppDelegete& rhs);
  };
  
}

#endif /* defined(__agi3d__AppDelegete__) */
