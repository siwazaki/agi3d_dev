//
//  Setting.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "UserDefault.h"

using namespace agi3d;

UserDefault::UserDefault() : _layout(E_Layout::D2), _rotation(E_Rotation::None), _nodeSize(1.0f), _edgeThickness(1.0f), _scale(20), _delta(50), _dimention(1000) ,_isDrawNode(true), _isDrawEdge(true){
/*  dp->changeSize(1.0f);
  dp->changeThickness(1.0f);
  
  dp->setXRotation(false);
  dp->setYRotation(false);
 */


}

UserDefault::~UserDefault() {}

E_Layout UserDefault::layout() {
  return _layout;
}

E_Rotation UserDefault::rotation() {
  return _rotation;
}

int UserDefault::nodeSize() {
  return _nodeSize;
}

int UserDefault::edgeThickness() {
  return _edgeThickness;
}

int UserDefault::scale() {
  return _scale;
}

int UserDefault::delta() {
  return _delta;
}

int UserDefault::dimention() {
  return _dimention;
}

bool UserDefault::isDrawNode() {
  return _isDrawNode;
}

bool UserDefault::isDrawEdge() {
  return _isDrawEdge;
}

void UserDefault::changeNodeSize(float nodeSize)
{
  _nodeSize = nodeSize;
  this->notify(E_ObserveType::RefreshOnly);
}
