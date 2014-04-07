//
//  SettingController.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__UserDefaultController__
#define __agi3d__UserDefaultController__

#include <memory>
#include "wx/wx.h"
#include "wx/event.h"

#include "Graph.h"
#include "UserDefault.h"
#include "MenuBar.h"
#include "AppearanceWindow.h"

namespace agi3d {
  class AppDelegete;
  
  class UserDefaultController
  {
    friend class AppDelegete;
  public:
    UserDefaultController(const std::shared_ptr<UserDefault>& userDefault, const std::shared_ptr<Graph>& graph);
    virtual ~UserDefaultController();

  private:
    std::shared_ptr<UserDefault> _userDefault;
    std::shared_ptr<Graph> _graph;
    MenuBar* _menuBar;
    AppearanceWindow* _appearanceWindow;

    void notifyUpdateNodeSize(wxScrollEvent& event);
    void setAutoXRotation(wxCommandEvent& event);
    void setAutoYRotation(wxCommandEvent& event);
    void stopAutoRotation(wxCommandEvent& event);
    void resetMenuParams();
    void captureImage(wxCommandEvent& event);
    void reset(wxCommandEvent& event);
    void changeLayoutModeTo2D(wxCommandEvent& event);
    void changeLayoutModeTo3D(wxCommandEvent& event);
    void onOpen(wxCommandEvent& event);
    void openAppearanceWindow(wxCommandEvent& event);

  };
}

#endif /* defined(__agi3d__SettingController__) */
