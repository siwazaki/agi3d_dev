#ifndef _subpanel_
#define _subpanel_

#include <memory>

#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"

#include "Observer.h"
#include "Graph.h"
#include "Configuration.h"
#include "UserDefault.h"

namespace agi3d {
  
  class ConfigurationController;
  
  class ControlPanel : public wxPanel, public Observer {
    
    friend class ConfigurationController;
    
  public:
    ControlPanel(wxWindow* parent);
    virtual ~ControlPanel();
    
    virtual void update(const Observable& observable, E_ObserveType observeType);
    
    void init(const std::shared_ptr<Graph>& graph,
              const std::shared_ptr<UserDefault>& userDefault,
              const std::shared_ptr<Configuration>& configuration);
    
  private:
    std::shared_ptr<Graph> _graph;
    std::shared_ptr<Configuration> _configuration;
    std::shared_ptr<UserDefault> _userDefault;
    
    wxSlider * cameraPositionSlider;
    wxSlider * DeltaSlider;
    
    wxSlider * nodeThresholdSlider_t;
    wxSlider * nodeThresholdSlider_b;
    wxSlider * edgeThresholdSlider_t;
    wxSlider * edgeThresholdSlider_b;
    
    wxCheckBox * cb_xRotation;
    wxCheckBox * cb_yRotation;
    
    wxChoice * nodeAttrsChoice;
    wxChoice * edgeAttrsChoice;
    
    wxListBox * target;
    wxListBox * listbox;
    wxStaticText * m_FPS;
    wxStaticText * fileNameLabel;
    wxStaticText * nodeSizeLabel;
    wxStaticText * edgeSizeLabel;
    
    void refresh();
    
  };
  
}

#endif