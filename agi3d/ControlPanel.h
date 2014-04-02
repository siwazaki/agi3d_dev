#ifndef _subpanel_
#define _subpanel_

#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"


namespace agi3d {
  class ControlPanel : public wxPanel {
  public:
    ControlPanel(wxWindow* parent);
    
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
    
    DECLARE_EVENT_TABLE()
  };
}

#endif