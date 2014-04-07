//
//  Observer.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/03.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__Observer__
#define __agi3d__Observer__

#include <iostream>
#include "Enums.h"
namespace agi3d {
  class Observable;
  
  class Observer {
    
  public:
    Observer(){}
    virtual ~Observer(){};
    virtual void update(const Observable& observable, E_ObserveType observeType) = 0;
    
  };
  
}

#endif /* defined(__agi3d__Observer__) */