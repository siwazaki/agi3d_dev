#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/panel.h"

#include "GraphicPanel.h"
#include "ControlPanel.h"
#include "AppearanceWindow.h"
#include "MenuBar.h"

/**
 *  
 * メインのフレームを表すクラス。次のコンポーネントを持つ
 * - MenuBar 
 */
class Frame : public wxFrame {
public:
  Frame(const wxString& title);

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