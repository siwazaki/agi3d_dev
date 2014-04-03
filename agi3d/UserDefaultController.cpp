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

using namespace agi3d;
using namespace std;

UserDefaultController:: UserDefaultController(const std::shared_ptr<UserDefault>& userDefault, const std::shared_ptr<Graph>& graph)
{  
  this->_userDefault = userDefault;
  this->_graph = graph;
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
  _userDefault->_rotation = (E_Rotation)(_userDefault->_rotation ^ E_Rotation::X);
  std::cout << _userDefault->_rotation << std::endl;
  _menuBar->renderModel(_userDefault);


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
  _userDefault->_rotation = (E_Rotation)(_userDefault->_rotation ^ E_Rotation::Y);
  _menuBar->renderModel(_userDefault);
  
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
  _userDefault->_rotation = E_Rotation::None;
  _menuBar->renderModel(_userDefault);

  //ResetMenuParams();
}


void UserDefaultController::OnOpen(wxCommandEvent& event) {
  wxFileDialog * openFileDialog = new wxFileDialog(_menuBar);
  if (openFileDialog->ShowModal() == wxID_OK) {
     wxString filePath = openFileDialog->GetPath();
    _graph->loadData(std::string(filePath.mb_str()));
    
    auto configurationController = AppDelegete::instance().getConfigurationController();
    //configurationController->changeGraphName(graphName);
    
    //should be refactored
    auto graphicPanel = AppDelegete::instance().getGraphicPanel();
    auto appearanceWindow = AppDelegete::instance().getAppearanceWindow();
    graphicPanel->setupPanel();
    configurationController->Init();
    appearanceWindow->Init();
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