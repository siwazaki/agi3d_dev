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
#include "MenuBarController.h"
#include "MenuBar.h"

using namespace agi3d;
using namespace std;

MenuBarController::MenuBarController() {
}

MenuBarController::~MenuBarController() {
  
}

void MenuBarController::initEventHandlers() {
  //イベントハンドラの登録
  auto resetHandler(bind(&MenuBarController::reset, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, resetHandler, 10);
  
  auto changeLayoutModeTo3DHandler(bind(&MenuBarController::changeLayoutModeTo3D, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, changeLayoutModeTo3DHandler, 11);
  
  auto changeLayoutModeTo2DHandler(bind(&MenuBarController::changeLayoutModeTo2D, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, changeLayoutModeTo2DHandler, 12);
  
  auto xRotationHandler(bind(&MenuBarController::setAutoXRotation, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, xRotationHandler, 13);
  
  auto yRotationHandler(bind(&MenuBarController::setAutoYRotation, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, yRotationHandler, 14);
  
  auto stopAutoRotationHandler( bind(&MenuBarController::stopAutoRotation, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, stopAutoRotationHandler, 15);
  
  auto openApperanceWindowHandler(bind(&MenuBarController::openAppearanceWindow, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, openApperanceWindowHandler, 16);
  
  auto openFileHandler(bind(&MenuBarController::onOpen, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, openFileHandler, wxID_OPEN);
  
  auto captureImageHandler(bind(&MenuBarController::captureImage, this, placeholders::_1));
  this->_menuBar->Bind(wxEVT_COMMAND_MENU_SELECTED, captureImageHandler, 17);

}

void MenuBarController::setAutoXRotation(wxCommandEvent&) {
  E_Rotation rotation = (E_Rotation)(_userDefault->_rotation ^ E_Rotation::X);
  _userDefault->changeRotation(rotation);
}

void MenuBarController::setAutoYRotation(wxCommandEvent&) {
  E_Rotation rotation = (E_Rotation)(_userDefault->_rotation ^ E_Rotation::Y);
  _userDefault->changeRotation(rotation);
}

void MenuBarController::stopAutoRotation(wxCommandEvent&) {
  _userDefault->changeRotation(E_Rotation::None);
}

void MenuBarController::onOpen(wxCommandEvent&) {
  wxFileDialog * openFileDialog = new wxFileDialog(_menuBar);
  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString filePath = openFileDialog->GetPath();
    AppDelegete::instance().changeGraph(std::string(filePath.mb_str()));
  }
}

void MenuBarController::reset(wxCommandEvent&) {
  _appearanceWindow->showModel();
}

void MenuBarController::changeLayoutModeTo2D(wxCommandEvent&) {
  _userDefault->changeLayout(E_Layout::D2);
  _appearanceWindow->showModel();
  _controlPanel->showModel();
}

void MenuBarController::changeLayoutModeTo3D(wxCommandEvent&) {
  _userDefault->changeLayout(E_Layout::D3);
  _appearanceWindow->showModel();
  _controlPanel->showModel();
}


void MenuBarController::captureImage(wxCommandEvent&) {
  _graphicPanel->savePixelData();
}

void MenuBarController::openAppearanceWindow(wxCommandEvent&) {
  _appearanceWindow->Show(true);
  _appearanceWindow->Raise();
}