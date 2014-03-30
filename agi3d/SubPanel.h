#ifndef _subpanel_
#define _subpanel_

#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"

// MainFrame extends wxFrame

class SubPanel : public wxPanel {
public:
  SubPanel(wxWindow* parent);

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

  void SetFPS(float);

  void handleButton1Event(wxCommandEvent& event);
  void handleButton2Event(wxCommandEvent& event);

  void handleListEvent(wxCommandEvent& event);

  void NortifyUpdateNodeSize(wxScrollEvent& event);
  void NortifyUpdateEdgeThickness(wxScrollEvent& event);
  void NortifyUpdateDelta(wxScrollEvent& event);
  void NortifyUpdateScale(wxScrollEvent& event);
  void NortifyUpdateDimension(wxScrollEvent& event);

  void UpdateEdgeValueThreshold_t(wxScrollEvent& event);
  void UpdateEdgeValueThreshold_b(wxScrollEvent& event);

  void UpdateNodeValueThreshold_t(wxScrollEvent& event);
  void UpdateNodeValueThreshold_b(wxScrollEvent& event);

  void OnToggleEdge(wxCommandEvent& event);
  void OnToggleNodeSize(wxCommandEvent& event);

  void SelectNodeAttr(wxCommandEvent& event);
  void SelectEdgeAttr(wxCommandEvent& event);

  void setTarget(int id);

  void Init();

  DECLARE_EVENT_TABLE()
};

#endif