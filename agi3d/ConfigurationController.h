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
#include "ControlPanel.h"

namespace agi3d {
  
  class AppDelegete;  
  class ConfigurationController
  {
    friend class AppDelegete;
    
  public:
    ConfigurationController(const std::shared_ptr<Configuration>& configuration, const std::shared_ptr<Graph>& graph);
    virtual ~ConfigurationController();
    //@TODO remane
    void Init();
  private:
    std::shared_ptr<Configuration> _configuration;
    std::shared_ptr<Graph> _graph;
    ControlPanel *_controlPanel;
    //@TODO move to configuration
    float fps;
    
    void SetFPS(float);
    
    void handleButton1Event(wxCommandEvent& event);
    void handleButton2Event(wxCommandEvent& event);
    
    void handleListEvent(wxCommandEvent& event);
    
        void NortifyUpdateNodeSize(wxCommandEvent& event);
    void NortifyUpdateEdgeThickness(wxCommandEvent& event);
    void NortifyUpdateDelta(wxCommandEvent& event);
    void NortifyUpdateScale(wxCommandEvent& event);
    void NortifyUpdateDimension(wxCommandEvent& event);
    
    void UpdateEdgeValueThreshold_t(wxCommandEvent& event);
    void UpdateEdgeValueThreshold_b(wxCommandEvent& event);
    
    void UpdateNodeValueThreshold_t(wxCommandEvent& event);
    void UpdateNodeValueThreshold_b(wxCommandEvent& event);
    
    void OnToggleEdge(wxCommandEvent& event);
    void OnToggleNodeSize(wxCommandEvent& event);
    
    void SelectNodeAttr(wxCommandEvent& event);
    void SelectEdgeAttr(wxCommandEvent& event);
    
    void setTarget(int id);
    
  };
}

#endif /* defined(__agi3d__ConfigurationController__) */
