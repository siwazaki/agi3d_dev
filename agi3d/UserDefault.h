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
#include "Observable.h"

namespace agi3d {
  
  class UserDefaultController;
  /**
   * MenuBarで変更可能なデータを保持するモデル。
   * UserDefaultController以外はプロパティ変更不可。
   * @TODO: Jsonにシリアライズ
   */
  class UserDefault : public Observable
  {
    
    friend class UserDefaultController;
    
  public:
    UserDefault();
    virtual ~UserDefault();
    E_Layout layout() const;
    E_Rotation rotation() const;
    int nodeSize() const;
    int edgeThickness() const;
    int scale()const;
    int delta() const;
    int dimention() const ;
    bool isDrawNode() const;
    bool isDrawEdge() const;    
    void changeNodeSize(float nodeSize);
    void changeEdgeThickness(float thickness);
    
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
  
  inline E_Layout UserDefault::layout() const {
    return _layout;
  }
  
  inline E_Rotation UserDefault::rotation() const {
    return _rotation;
  }
  
  inline int UserDefault::nodeSize() const {
    return _nodeSize;
  }
  
  inline int UserDefault::edgeThickness() const {
    return _edgeThickness;
  }
  
  inline int UserDefault::scale() const {
    return _scale;
  }
  
  inline int UserDefault::delta() const {
    return _delta;
  }
  
  inline int UserDefault::dimention() const {
    return _dimention;
  }
  
  inline bool UserDefault::isDrawNode() const {
    return _isDrawNode;
  }
  
  inline bool UserDefault::isDrawEdge() const {
    return _isDrawEdge;
  }
  
}

#endif /* defined(__agi3d__Setting__) */