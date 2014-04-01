//
//  ConfigurationController.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__ConfigurationController__
#define __agi3d__ConfigurationController__

#include <memory>
#include "Configuration.h"
#include "ControlPanel.h"

namespace agi3d {
  
  class AppDelegete;  
  class ConfigurationController
  {
    friend class AppDelegete;
    
  public:
    virtual ~ConfigurationController();
    ConfigurationController();
    
    void changeGraphName(const std::string& graphName);

  private:
    std::shared_ptr<Configuration> _current;
    std::shared_ptr<ControlPanel> _controlPanel;
    
    
  };
}

#endif /* defined(__agi3d__ConfigurationController__) */
