//
//  MenuBar.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/30.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include <fstream>
#include <iostream>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <wx/wx.h>
#include <wx/event.h>

#include "MenuBar.h"
#include "UserDefaultController.h"
#include "Config.h"


using namespace agi3d;

MenuBar::MenuBar()
{
  file = new wxMenu();
  file->Append(wxID_OPEN, wxT("Open"));
  file->AppendSeparator();
  
  edit = new wxMenu();
  edit->Append(10, wxT("Reset"));
  edit->AppendSeparator();
  
  edit->Append(16, wxT("Appearance"));
  edit->AppendSeparator();
  
  layoutMenu = new wxMenu();
  layoutMenu->Append(11, wxT(" 3D  ✓"));
  layoutMenu->Append(12, wxT(" 2D"));
  
  edit->AppendSubMenu(layoutMenu, wxT("Layout Mode"));
  edit->AppendSeparator();
  
  rotationMenu = new wxMenu();
  rotationMenu->Append(13, wxT(" X Rotation  "));
  rotationMenu->Append(14, wxT(" Y Rotation  "));
  rotationMenu->Append(15, wxT(" Stop "));
  
  Config& config = Config::instance();
  config.rotateX(false);
  config.rotateY(false);
  
  edit->AppendSubMenu(rotationMenu, wxT("Auto Rotation"));
  edit->AppendSeparator();
  
  edit->Append(17, wxT("Capture Image"));
  
  quit = new wxMenuItem(file, wxID_EXIT, wxT("&Quit\tCtrl+W"));
  file->Append(quit);
  
  this->Append(file, wxT("&File"));
  this->Append(edit, wxT("&Edit"));

}