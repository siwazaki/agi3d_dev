//
//  MenuBar.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/30.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__MenuBar__
#define __agi3d__MenuBar__

#include <wx/menu.h>

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

#endif /* defined(__agi3d__MenuBar__) */
