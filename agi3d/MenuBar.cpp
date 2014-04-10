#include <exception>

#include <wx/wx.h>
#include <wx/event.h>

#include "Enums.h"
#include "MenuBar.h"

using namespace agi3d;

MenuBar::MenuBar() {
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
  
  edit->AppendSubMenu(rotationMenu, wxT("Auto Rotation"));
  edit->AppendSeparator();
  
  edit->Append(17, wxT("Capture Image"));
  
  quit = new wxMenuItem(file, wxID_EXIT, wxT("&Quit\tCtrl+W"));
  file->Append(quit);
  
  this->Append(file, wxT("&File"));
  this->Append(edit, wxT("&Edit"));
}

void MenuBar::showModel()
{
  this->changeMenu();
}

void MenuBar::init(const std::shared_ptr<UserDefault> userDefault) {
  _userDefault = userDefault;
}

void MenuBar::changeMenu() {
  switch(_userDefault->rotation()) {
    case E_Rotation::X: {
      rotationMenu->SetLabel(13, wxT(" X Rotation  ✓"));
      rotationMenu->SetLabel(14, wxT(" Y Rotation  "));
      break;
    }
      
    case E_Rotation::Y: {
      rotationMenu->SetLabel(13, wxT(" X Rotation  "));
      rotationMenu->SetLabel(14, wxT(" Y Rotation  ✓"));
      break;
    }
      
    case E_Rotation::XY: {
      rotationMenu->SetLabel(13, wxT(" X Rotation  ✓"));
      rotationMenu->SetLabel(14, wxT(" Y Rotation  ✓"));
      break;
    }
    case E_Rotation::None: {
      rotationMenu->SetLabel(13, wxT(" X Rotation  "));
      rotationMenu->SetLabel(14, wxT(" Y Rotation  "));
      break;
    }      
    default:
      throw std::invalid_argument("unknown rotaion type found");
  }
  
  switch(_userDefault->layout())
  {
    case E_Layout::D2: {
      layoutMenu->SetLabel(11, wxT(" 3D"));
      layoutMenu->SetLabel(12, wxT(" 2D  ✓"));
      break;
    }
    case E_Layout::D3: {
      layoutMenu->SetLabel(11, wxT(" 3D  ✓"));
      layoutMenu->SetLabel(12, wxT(" 2D"));
      break;
    }
    default:
      throw std::invalid_argument("unknown layout type found");
  }
}

void MenuBar::update(const Observable&, E_ObserveType)
{
  this->changeMenu();
}
