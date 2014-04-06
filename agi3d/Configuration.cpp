//
//  Configuration.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include "Configuration.h"

using namespace agi3d;

Configuration::Configuration() :
_label(),
_neiborLabels(),
_nodethreshold_t(10000.0f),
_nodethreshold_b(-1.0f),
_edgethreshold_t(10000.0f),
_edgethreshold_b(-1.0f)
{
  
}

Configuration::~Configuration() {
  
}

void Configuration::changeTarget(const std::string& label, const std::vector<std::string>& neiborLabels) {
  this->_label = label;
  this->_neiborLabels.clear();
  typedef std::vector<std::string>::const_iterator vi;
  for(vi itr = neiborLabels.begin(), end = neiborLabels.end(); itr != end; ++itr){
    _neiborLabels.push_back(*itr);
  }
  this->notify(E_ObserveType::RefreshOnly);  
}