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
#include <map>

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
    
    float fps = 0.0f;
    float tmp = 0.0f;
    
    int nodeThresholdAttrID = 0;
    int edgeThresholdAttrID = 0;
    

    int pickid = -1;
    
  
    
    float _nodethreshold_t = 10000000;
    float _nodethreshold_b = -1;
    std::map<int, int> labelMap;
    int node_slider_b_pos = 0;
    int node_slider_t_pos = 100;
    int edge_slider_b_pos = 0;
    int edge_slider_t_pos = 100;
    int _id = -1;
  };
}

#endif /* defined(__agi3d__Configuration__) */
