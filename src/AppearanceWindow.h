#include <wx/wx.h>
#include "wx/slider.h"

class AppearanceWindow : public wxFrame {
public:
  AppearanceWindow(wxWindow * parent, const wxString& title);

  wxSlider * nodeSizeSlider;
  wxSlider * edgeThicknessSlider;
  wxSlider * DeltaSlider;
  wxSlider * ScaleSlider;
  wxSlider * DimensionSlider;

  wxCheckBox * edgeCheckBox;
  wxCheckBox * nodeSizeCheckBox;

  void Init();

  void CloseWindow(wxCommandEvent & event);

  void UpdateNodeSize(wxScrollEvent& event);
  void UpdateEdgeThickness(wxScrollEvent& event);

  void UpdateDelta(wxScrollEvent& event);
  void UpdateScale(wxScrollEvent& event);
  void UpdateDimension(wxScrollEvent& event);

  void OnToggleShowEdge(wxCommandEvent& event);
  void OnToggleNodeSize(wxCommandEvent& event);
};