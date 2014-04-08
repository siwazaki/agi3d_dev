#ifndef __agi3d__MenuBar__
#define __agi3d__MenuBar__

#include <memory>

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/menu.h>

#include "UserDefault.h"

namespace agi3d {
  
  class MenuBar : public wxMenuBar
  {
    
  public:
    MenuBar();
    virtual ~MenuBar(){}
    void renderModel(const std::shared_ptr<UserDefault>& userDefault);
    
  private:
    wxMenuItem *quit;
    wxMenu *file;
    wxMenu *edit;
    wxMenu *layoutMenu;
    wxMenu *rotationMenu;
    
  };
}

#endif /* defined(__agi3d__MenuBar__) */