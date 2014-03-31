//
//  MenuBar.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/30.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__MenuBar__
#define __agi3d__MenuBar__

#include <memory>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/menu.h>




namespace agi3d {
  
  class UserDefaultController;
  
  class MenuBar : public wxMenuBar
  {
    
  public:
    MenuBar();
    virtual ~MenuBar(){}
  private:
    wxMenuItem *quit;
    wxMenu *file;
    wxMenu *edit;
    wxMenu *layoutMenu;
    wxMenu *rotationMenu;
    
  };
  
/*  template<typename T>
  void MenuBar::addHandler(wxEventType eventType, boost::function< void ( T& )> handler, int wxId)
  {
      this->Bind(eventType, handler, wxId);
  }
 */

  
}

#endif /* defined(__agi3d__MenuBar__) */
