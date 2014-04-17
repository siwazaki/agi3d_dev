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