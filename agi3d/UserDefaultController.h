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

#include "UserDefault.h"
#include "ControlPanel.h"
#include "GraphicPanel.h"
#include "MenuBar.h"
#include "AppearanceWindow.h"

namespace agi3d {
  
  class UserDefaultController
  {
  public:
    UserDefaultController();
    virtual ~UserDefaultController();

  private:
    std::shared_ptr<UserDefault> _current;
    std::shared_ptr<MenuBar> _menuBar;

    void nortifyUpdateNodeSize(wxScrollEvent& event);
    void SetAutoXRotation(wxCommandEvent& event);
    void SetAutoYRotation(wxCommandEvent& event);
    void StopAutoRotation(wxCommandEvent& event);
    void ResetMenuParams();
    void CaptureImage(wxCommandEvent& event);
    void Reset(wxCommandEvent& event);
    void ChangeLayoutModeTo2D(wxCommandEvent& event);
    void ChangeLayoutModeTo3D(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OpenAppearanceWindow(wxCommandEvent& event);

  };
}

#endif /* defined(__agi3d__SettingController__) */
