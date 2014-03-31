#ifndef __agi3d_frame_h__
#define __agi3d_frame_h__

#include <memory>
#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/panel.h"

#include "SettingController.h"
#include "GraphicPanel.h"
#include "ControlPanel.h"
#include "AppearanceWindow.h"
#include "MenuBar.h"

namespace agi3d {
  /**
   *
   * メインのフレームを表すクラス。次のコンポーネントを持つ
   * - MenuBar
   * - SplitterWindow
   */
  class Frame : public wxFrame {
  public:
    Frame(const wxString& title);
    void addController(const std::shared_ptr<agi3d::SettingController> &settingCongroller);
    
    wxSplitterWindow *base;
    GraphicPanel *left;
    ControlPanel *right;
    
    wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *demoMenu;
    wxMenu *demoMenu2;
    wxMenu *edit;
    wxMenu *layoutMenu;
    wxMenu *rotationMenu;
    wxMenuItem *quit;
    AppearanceWindow * appw;
    
    void initload();
    ControlPanel * GetSubPanel();
    GraphicPanel * GetDrawPanel();
    void ResetMenuParams();
    void OnQuit(wxCommandEvent& event);
    void CaptureImage(wxCommandEvent& event);
    void Reset(wxCommandEvent& event);
    void ChangeLayoutModeTo2D(wxCommandEvent& event);
    void ChangeLayoutModeTo3D(wxCommandEvent& event);
    void SetAutoXRotation(wxCommandEvent& event);
    void SetAutoYRotation(wxCommandEvent& event);
    void StopAutoRotation(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OpenAppearanceWindow(wxCommandEvent& event);
  };
}

#endif