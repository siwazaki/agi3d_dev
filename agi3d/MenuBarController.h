#ifndef __agi3d__UserDefaultController__
#define __agi3d__UserDefaultController__

#include <memory>
#include "wx/wx.h"
#include "wx/event.h"

#include "Graph.h"
#include "UserDefault.h"

#include "GraphicPanel.h"
#include "MenuBar.h"
#include "AppearanceWindow.h"

namespace agi3d {
  class AppDelegete;
  
  class MenuBarController
  {
    friend class AppDelegete;
  public:
    MenuBarController();
    virtual ~MenuBarController();
    void initEventHandlers();    

  private:
    std::shared_ptr<UserDefault> _userDefault;
    std::shared_ptr<Graph> _graph;
    MenuBar* _menuBar;
    AppearanceWindow* _appearanceWindow;
    GraphicPanel* _graphicPanel;
    ControlPanel* _controlPanel;
    
    void notifyUpdateNodeSize(wxScrollEvent& event);
    void setAutoXRotation(wxCommandEvent& event);
    void setAutoYRotation(wxCommandEvent& event);
    void stopAutoRotation(wxCommandEvent& event);
    void captureImage(wxCommandEvent& event);
    void reset(wxCommandEvent& event);
    void changeLayoutModeTo2D(wxCommandEvent& event);
    void changeLayoutModeTo3D(wxCommandEvent& event);
    void onOpen(wxCommandEvent& event);
    void openAppearanceWindow(wxCommandEvent& event);

  };
}

#endif /* defined(__agi3d__SettingController__) */