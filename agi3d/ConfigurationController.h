//
//  ConfigurationController.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__ConfigurationController__
#define __agi3d__ConfigurationController__

#include <memory>
#include "wx/wx.h"
#include "wx/event.h"

#include "Graph.h"
#include "Configuration.h"
#include "UserDefault.h"
#include "ControlPanel.h"

namespace agi3d {
  
  class AppDelegete;  
  class ConfigurationController
  {
    friend class AppDelegete;
    
  public:
    ConfigurationController(const std::shared_ptr<Configuration>& configuration, const std::shared_ptr<UserDefault>& userDefault, const std::shared_ptr<Graph>& graph);
    virtual ~ConfigurationController();
    //TODO: remane
    void init();
  private:
    std::shared_ptr<Configuration> _configuration;
    std::shared_ptr<UserDefault> _userDefault;
    std::shared_ptr<Graph> _graph;
    ControlPanel *_controlPanel;
    
    //TODO: move to configuration
    float fps;
    void setFPS(float);
    
    void handleListEvent(wxCommandEvent& event);
    
    void notifyUpdateNodeSize(wxCommandEvent& event);
    void notifyUpdateEdgeThickness(wxCommandEvent& event);
    void notifyUpdateDelta(wxCommandEvent& event);
    void notifyUpdateScale(wxCommandEvent& event);
    void notifyUpdateDimension(wxCommandEvent& event);
    void updateNodeValueThreshold(wxCommandEvent& event);
    void updateEdgeValueThreshold(wxCommandEvent& event);
    void onToggleEdge(wxCommandEvent& event);
    void onToggleNodeSize(wxCommandEvent& event);
    void selectNodeAttr(wxCommandEvent& event);
    void selectEdgeAttr(wxCommandEvent& event);
    
    void setTarget(int id);
    
  };
}

#endif /* defined(__agi3d__ConfigurationController__) */
