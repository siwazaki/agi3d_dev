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
  
  class ControlPanelController
  {
    friend class AppDelegete;
    
  public:
    ControlPanelController();
    virtual ~ControlPanelController();
    void initEventHandlers();

  private:
    std::shared_ptr<Configuration> _configuration;
    std::shared_ptr<UserDefault> _userDefault;
    std::shared_ptr<Graph> _graph;
    ControlPanel *_controlPanel;    
    
    void hilightNode(wxCommandEvent& event);
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
    
  };
}

#endif /* defined(__agi3d__ConfigurationController__) */