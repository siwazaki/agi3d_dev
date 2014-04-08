//
//  SettingController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//
#include <sstream>
#include <exception>
#include <functional>

#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"

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
  auto resetHandler(bind(&UserDefaultController::reset, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, resetHandler, 10);
  
  auto changeLayoutModeTo3DHandler(bind(&UserDefaultController::changeLayoutModeTo3D, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, changeLayoutModeTo3DHandler, 11);

  auto changeLayoutModeTo2DHandler(bind(&UserDefaultController::changeLayoutModeTo2D, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, changeLayoutModeTo2DHandler, 12);
  
  auto xRotationHandler(bind(&UserDefaultController::setAutoXRotation, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, xRotationHandler, 13);
  
  auto yRotationHandler(bind(&UserDefaultController::setAutoYRotation, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, yRotationHandler, 14);
  
  auto stopAutoRotationHandler( bind(&UserDefaultController::stopAutoRotation, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, stopAutoRotationHandler, 15);
  
  auto openApperanceWindowHandler(bind(&UserDefaultController::openAppearanceWindow, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, openApperanceWindowHandler, 16);
  
  auto openFileHandler(bind(&UserDefaultController::onOpen, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, openFileHandler, wxID_OPEN);
  
  auto captureImageHandler(bind(&UserDefaultController::captureImage, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, captureImageHandler, 17);
  
}

UserDefaultController::~UserDefaultController()
{
  
}

void UserDefaultController::setAutoXRotation(wxCommandEvent&) {
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

void UserDefaultController::setAutoYRotation(wxCommandEvent&) {
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

void UserDefaultController::stopAutoRotation(wxCommandEvent&) {
  _userDefault->_rotation = E_Rotation::None;
  _menuBar->renderModel(_userDefault);
  //ResetMenuParams();
}


void UserDefaultController::onOpen(wxCommandEvent&) {
  wxFileDialog * openFileDialog = new wxFileDialog(_menuBar);
  if (openFileDialog->ShowModal() == wxID_OK) {
     wxString filePath = openFileDialog->GetPath();
    _graph->loadData(std::string(filePath.mb_str()));
      
    //TODO:should be refactored
    auto graphicPanel = AppDelegete::instance().getGraphicPanel();
    auto appearanceWindow = AppDelegete::instance().getAppearanceWindow();
    graphicPanel->setupPanel();

    appearanceWindow->Init();
  }
}

void UserDefaultController::resetMenuParams() {
//  x_rotation = false;
//  left->SetXRotation(false);
//  rotationMenu->SetLabel(13, wxT(" X Rotation"));
//  y_rotation = false;
//  left->SetYRotation(false);
//  rotationMenu->SetLabel(14, wxT(" Y Rotation"));
}

void UserDefaultController::reset(wxCommandEvent&) {
//  left->ResetLayout();
//  right->Init();
//  appw->Init();
//  ResetMenuParams();
}

void UserDefaultController::changeLayoutModeTo2D(wxCommandEvent&) {
//  left->ChangeLayoutMode(2);
//  right->Init();
//  appw->Init();
//  layoutMenu->SetLabel(11, wxT(" 3D"));
//  layoutMenu->SetLabel(12, wxT(" 2D  ✓"));
//  ResetMenuParams();
}

void UserDefaultController::changeLayoutModeTo3D(wxCommandEvent&) {
//  left->ChangeLayoutMode(3);
//  right->Init();
//  appw->Init();
//  layoutMenu->SetLabel(11, wxT(" 3D  ✓"));
//  layoutMenu->SetLabel(12, wxT(" 2D"));
//  ResetMenuParams();
}


void UserDefaultController::captureImage(wxCommandEvent&) {
  //left->SavePixelData();
}

void UserDefaultController::openAppearanceWindow(wxCommandEvent&) {
  _appearanceWindow->Show(true);
  _appearanceWindow->Raise();
}