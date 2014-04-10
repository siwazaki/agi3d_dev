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
  E_Rotation rotation = (E_Rotation)(_userDefault->_rotation ^ E_Rotation::X);
  _userDefault->changeRotation(rotation);
}

void UserDefaultController::setAutoYRotation(wxCommandEvent&) {
  E_Rotation rotation = (E_Rotation)(_userDefault->_rotation ^ E_Rotation::Y);
  _userDefault->changeRotation(rotation);
}

void UserDefaultController::stopAutoRotation(wxCommandEvent&) {
  _userDefault->changeRotation(E_Rotation::None);
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

void UserDefaultController::reset(wxCommandEvent&) {
  auto graphicPanel = AppDelegete::instance().getGraphicPanel();
  auto appearanceWindow = AppDelegete::instance().getAppearanceWindow();
  appearanceWindow->Init();
}

void UserDefaultController::changeLayoutModeTo2D(wxCommandEvent&) {
  _userDefault->changeLayout(E_Layout::D2);
//TODO:
//  right->Init();
//  appw->Init();
}

void UserDefaultController::changeLayoutModeTo3D(wxCommandEvent&) {
    _userDefault->changeLayout(E_Layout::D3);
//TODO:
//  right->Init();
//  appw->Init();
}


void UserDefaultController::captureImage(wxCommandEvent&) {
//TODO:  
  //left->SavePixelData();
}

void UserDefaultController::openAppearanceWindow(wxCommandEvent&) {
  _appearanceWindow->Show(true);
  _appearanceWindow->Raise();
}