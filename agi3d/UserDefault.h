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
  
  /**
   * MenuBarで変更可能なデータを保持するモデル。
   * @TODO: Jsonにシリアライズ
   */
  class UserDefault
  {
  public:
    UserDefault();
    virtual ~UserDefault();
  private:
    E_Layout _layout;
    E_Rotation _rotation;
    int _nodeSize;
    int _edgeThickness;
    int _scale;
    int _delta;
    int _dimention;
    bool _drawNode;
    bool _drawEdge;


  };
}

#endif /* defined(__agi3d__Setting__) */
