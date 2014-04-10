#ifndef __agi3d__MenuBar__
#define __agi3d__MenuBar__

#include <memory>

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/menu.h>

#include "UserDefault.h"
#include "Observer.h"

namespace agi3d {
  
  class MenuBar : public wxMenuBar, public Observer
  {
    
  public:
    MenuBar();
    virtual ~MenuBar(){}
    virtual void update(const Observable& observable, E_ObserveType observeType);
    void init(const std::shared_ptr<UserDefault> userDefault);
    void showModel();
    
  private:
    std::shared_ptr<UserDefault> _userDefault;
    wxMenuItem *quit;
    wxMenu *file;
    wxMenu *edit;
    wxMenu *layoutMenu;
    wxMenu *rotationMenu;
    void changeMenu();
    
  };
}

#endif /* defined(__agi3d__MenuBar__) */