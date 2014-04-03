//
//  Setting.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__Setting__
#define __agi3d__Setting__

#include <iostream>
#include "Enums.h"

namespace agi3d {
  
  class UserDefaultController;
  /**
   * MenuBarで変更可能なデータを保持するモデル。
   * UserDefaultController以外はプロパティ変更不可。
   * @TODO: Jsonにシリアライズ
   */
  class UserDefault
  {
    
    friend class UserDefaultController;
    
  public:
    UserDefault();
    virtual ~UserDefault();
    E_Layout layout();
    E_Rotation rotation();
    int nodeSize();
    int edgeThickness();
    int scale();
    int delta();
    int dimention();
    bool isDrawNode();
    bool isDrawEdge();
    
  private:
    E_Layout _layout;
    E_Rotation _rotation;
    int _nodeSize;
    int _edgeThickness;
    int _scale;
    int _delta;
    int _dimention;
    bool _isDrawNode;
    bool _isDrawEdge;
    
  };
}

#endif /* defined(__agi3d__Setting__) */
