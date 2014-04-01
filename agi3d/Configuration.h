//
//  Configuration.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__Configuration__
#define __agi3d__Configuration__

#include <string>


namespace agi3d {

  class ConfigurationController;
  /**
   * ControlPanelで変更可能なデータを保持するモデル。
   * @TODO: Jsonにシリアライズ
   */
  class Configuration
  {
    friend class ConfigurationController;
    
    const std::string& graphName();
  private:
    std::string _graphName;
  };
}

#endif /* defined(__agi3d__Configuration__) */
