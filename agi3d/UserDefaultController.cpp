//
//  SettingController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//
#include <sstream>
#include <exception>

#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "AppDelegete.h"
#include "UserDefaultController.h"
#include "MenuBar.h"


//import from calclayout.cpp
void loadMatrixData_t(const char *);
void loadLayoutData_t(const char *);
void loadMatrixData_b(const char *);
void loadLayoutData_b(const char *);

using namespace agi3d;
using namespace std;

UserDefaultController::UserDefaultController()
{
  
  this->_current = std::shared_ptr<UserDefault>(new UserDefault());
  this->_menuBar = AppDelegete::instance().getMenuBar();
  this->_appearanceWindow = AppDelegete::instance().getAppearanceWindow();
  
  //イベントハンドラの登録
  auto xRotationHandler(boost::bind(&UserDefaultController::SetAutoXRotation, this, _1 ));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, xRotationHandler, 13);
  
  auto yRotationHandler( boost::bind(&UserDefaultController::SetAutoYRotation, this, _1 ));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, yRotationHandler, 14);
  
  auto stopAutoRotationHandler( boost::bind(&UserDefaultController::StopAutoRotation, this, _1 ));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, stopAutoRotationHandler, 15);
  
  auto openApperanceWindowHandler( boost::bind(&UserDefaultController::OpenAppearanceWindow, this, _1 ));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, openApperanceWindowHandler, 16);
  
  auto openFileHandler( boost::bind(&UserDefaultController::OnOpen, this, _1 ));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, openFileHandler, wxID_OPEN);
  
  
/*  Connect(10, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(UserDefaultController::Reset));
  Connect(11, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(UserDefaultController::ChangeLayoutModeTo3D));
  Connect(12, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(UserDefaultController::ChangeLayoutModeTo2D));
  Connect(17, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(UserDefaultController::CaptureImage));
*/
  
}

UserDefaultController::~UserDefaultController()
{
  
}



void UserDefaultController::SetAutoXRotation(wxCommandEvent& event) {
  _current->_rotation = (E_Rotation)(_current->_rotation ^ E_Rotation::X);
  std::cout << _current->_rotation << std::endl;
  _menuBar->renderModel(_current);


  //  if (x_rotation) {
  //    left->SetXRotation(false);
  //    x_rotation = false;
  //    rotationMenu->SetLabel(13, wxT(" X Rotation  "));
  //  } else {
  //    left->SetXRotation(true);
  //    x_rotation = true;
  //    rotationMenu->SetLabel(13, wxT(" X Rotation  ✓"));
  //  }
}

void UserDefaultController::SetAutoYRotation(wxCommandEvent& event) {
  _current->_rotation = (E_Rotation)(_current->_rotation ^ E_Rotation::Y);
  _menuBar->renderModel(_current);
  
  //  if (y_rotation) {
  //    left->SetYRotation(false);
  //    y_rotation = false;
  //    rotationMenu->SetLabel(14, wxT(" Y Rotation  "));
  //  } else {
  //    left->SetYRotation(true);
  //    y_rotation = true;
  //    rotationMenu->SetLabel(14, wxT(" Y Rotation  ✓"));
  //  }
}

void UserDefaultController::StopAutoRotation(wxCommandEvent& event) {
  _current->_rotation = E_Rotation::None;
  _menuBar->renderModel(_current);

  //ResetMenuParams();
}


void UserDefaultController::OnOpen(wxCommandEvent& event) {
  wxFileDialog * openFileDialog = new wxFileDialog(_menuBar.get());
  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString filePath = openFileDialog->GetPath();
    string fname = string(filePath.mb_str());
    int path_i = fname.find_last_of("/");
    int ext_i = fname.find_last_of(".");
    string f_ext = fname.substr(ext_i + 1);
    string filename = fname.substr(path_i + 1);
    
    std::string graphName = "unknown";
    
    if (f_ext == "txt") {
      if (filename.find("DisMat") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
        //loadMatrixData_t(filePath.mb_str());
      } else if (filename.find("Data.") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 9);
        //loadLayoutData_t(filePath.mb_str());
      } else if (filename.find("DataAll") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 12);
        //loadLayoutData_t(filePath.mb_str());
      } else {
        cerr << "This file is not available" << endl;
      }
    } else if (f_ext == "bin") {
      if (filename.find("DisMat") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
        //loadMatrixData_b(filePath.mb_str());
      } else if (filename.find("Data.") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 15);
        //loadLayoutData_b(filePath.mb_str());
      } else if (filename.find("DataAll") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 18);
        //loadLayoutData_b(filePath.mb_str());
      } else {
        cerr << "This file is not available" << endl;
      }
    }
    
    auto configurationController = AppDelegete::instance().getConfigurationController();
    configurationController->changeGraphName(graphName);
    cout << "graphName:" << graphName << endl;
//    _graphicPanel->SetupPanel();
    //right->Init();
    //appw->Init();
  }
}

void UserDefaultController::ResetMenuParams() {
//  x_rotation = false;
//  left->SetXRotation(false);
//  rotationMenu->SetLabel(13, wxT(" X Rotation"));
//  y_rotation = false;
//  left->SetYRotation(false);
//  rotationMenu->SetLabel(14, wxT(" Y Rotation"));
}

void UserDefaultController::Reset(wxCommandEvent& event) {
//  left->ResetLayout();
//  right->Init();
//  appw->Init();
//  ResetMenuParams();
}

void UserDefaultController::ChangeLayoutModeTo2D(wxCommandEvent& event) {
//  left->ChangeLayoutMode(2);
//  right->Init();
//  appw->Init();
//  layoutMenu->SetLabel(11, wxT(" 3D"));
//  layoutMenu->SetLabel(12, wxT(" 2D  ✓"));
//  ResetMenuParams();
}

void UserDefaultController::ChangeLayoutModeTo3D(wxCommandEvent& event) {
//  left->ChangeLayoutMode(3);
//  right->Init();
//  appw->Init();
//  layoutMenu->SetLabel(11, wxT(" 3D  ✓"));
//  layoutMenu->SetLabel(12, wxT(" 2D"));
//  ResetMenuParams();
}


void UserDefaultController::CaptureImage(wxCommandEvent& event) {
  //left->SavePixelData();
}

void UserDefaultController::OpenAppearanceWindow(wxCommandEvent& event) {
  //_appearanceWindow->Show(true);
  //_appearanceWindow->Raise();
}