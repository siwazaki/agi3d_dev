#ifndef __agi3d__Observable__
#define __agi3d__Observable__

#include <vector>
#include "Enums.h"
#include "Observer.h"

namespace agi3d {
  
  class Observable {
    
  public:    
    Observable(){};
    virtual ~Observable(){}
    virtual void notify(E_ObserveType observeType);
    virtual void addObserver(Observer* observer);
    
  private:
    std::vector<Observer*> _observers;
    
  };
  
  inline void Observable::addObserver(Observer* observer) {
    _observers.push_back(observer);
  }
  
  inline void Observable::notify(E_ObserveType observeType) {
    typedef std::vector<Observer*>::iterator vi;
    for(vi itr = _observers.begin(), end = _observers.end(); itr != end; ++itr){
      (*itr)->update(*this, observeType);
    }
  }
  
}

#endif /* defined(__agi3d__Observable__) */