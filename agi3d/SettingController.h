//
//  SettingController.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__SettingController__
#define __agi3d__SettingController__

#include <memory>
#include "wx/wx.h"
#include "wx/event.h"

#include "Setting.h"
#include "ControlPanel.h"
#include "GraphicPanel.h"

namespace agi3d {
  
  class SettingController
  {
  public:
    SettingController(std::shared_ptr<GraphicPanel> graphicPanel, std::shared_ptr<ControlPanel> controlPanel);
    virtual ~SettingController();
    void nortifyUpdateNodeSize(wxScrollEvent& event);
    void SetAutoXRotation(wxCommandEvent& event);
  private:
    std::shared_ptr<Setting> _current;
    std::shared_ptr<ControlPanel> _controlPanel;
    std::shared_ptr<GraphicPanel> _graphicPanel;
    
    
  };
}

#endif /* defined(__agi3d__SettingController__) */
