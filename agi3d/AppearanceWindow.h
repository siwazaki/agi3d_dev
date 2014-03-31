#ifndef __agi3d_appearance_window_h__
#define	__agi3d_appearance_window_h__

#include <wx/wx.h>
#include "wx/slider.h"
/**
  Hogeクラスの詳細説明です
    - リスト表記です1
    - リスト表記です2
    - リスト表記です3

    サンプルコードなどの記述
    @code
    Hoge hoge;
    hoge.Func();
    @endcode  
 */
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

#endif