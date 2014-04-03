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

#include "Observable.h"

namespace agi3d {

  class ConfigurationController;
  /**
   * ControlPanelで変更可能なデータを保持するモデル。
   * @TODO: Jsonにシリアライズ
   */
  class Configuration : public Observable
  {
    
    friend class ConfigurationController;
    
  public:
    Configuration();
    virtual ~Configuration();
    const std::string& graphName() const;
    int getNodeThreshHold_t() const;
    int getNodeThreshHold_b() const;
    int getEdgeThreshHold_t() const;
    int getEdgeThreshHold_b() const;
    
  private:
    std::string _graphName;
    std::map<int, int> _labelMap;
    float fps = 0.0f;
    float tmp = 0.0f;    
    int nodeThresholdAttrID = 0;
    int edgeThresholdAttrID = 0;
    int pickid = -1;
    int _id = -1;
    
    float _nodethreshold_t;
    float _nodethreshold_b;
    float _edgethreshold_t;
    float _edgethreshold_b;
    
  };
  
  inline const std::string& Configuration::graphName() const
  {
    return _graphName;
  }
  
  inline int Configuration::getNodeThreshHold_t() const
  {
    return _nodethreshold_t;
  }
  
  inline int Configuration::getNodeThreshHold_b() const
  {
    return _nodethreshold_b;
  }
  
  inline int Configuration::getEdgeThreshHold_t() const
  {
    return _edgethreshold_t;
  }
  
  inline int Configuration::getEdgeThreshHold_b() const
  {
    return _edgethreshold_b;
  }

}

#endif /* defined(__agi3d__Configuration__) */
