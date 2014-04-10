//
//  ControlPanelController.cpp
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#include <functional>
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"

#include "ControlPanelController.h"
#include "AppDelegete.h"

using namespace std;
using namespace agi3d;

ControlPanelController::ControlPanelController() {  
  
}

ControlPanelController::~ControlPanelController() {
  
}

void ControlPanelController::initEventHandlers()
{
  auto notifyUpdateNodeSizeHandler(bind(&ControlPanelController::notifyUpdateNodeSize, this, placeholders::_1));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateNodeSizeHandler, 40);
  
  auto notifyUpdateEdgeThicknessHandler(bind(&ControlPanelController::notifyUpdateEdgeThickness, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateEdgeThicknessHandler, 50);
  
  auto notifyUpdateDeltaHandler(bind(&ControlPanelController::notifyUpdateDelta, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateDeltaHandler, 60);
  
  auto notifyUpdateScaleHandler(bind(&ControlPanelController::notifyUpdateScale, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateScaleHandler, 61);
  
  auto notifyUpdateDimensionHandler(bind(&ControlPanelController::notifyUpdateDimension, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateDimensionHandler, 62);
  
  auto updateNodeValueThresholdHandler(bind(&ControlPanelController::updateNodeValueThreshold, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateNodeValueThresholdHandler, 70);
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateNodeValueThresholdHandler, 71);
  
  auto updateEdgeValueThresholdHandler(bind(&ControlPanelController::updateEdgeValueThreshold, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateEdgeValueThresholdHandler, 80);
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateEdgeValueThresholdHandler, 81);
  
  auto selectNodeAttrHandler(bind(&ControlPanelController::selectNodeAttr, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHOICE_SELECTED, selectNodeAttrHandler, 120);
  
  auto selectEdgeAttrHandler(bind(&ControlPanelController::selectEdgeAttr, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHOICE_SELECTED, selectEdgeAttrHandler, 121);
  
  auto onToggleEdgeHandler(bind(&ControlPanelController::onToggleEdge, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, onToggleEdgeHandler, 200);
  
  auto onToggleNodeSizeHandler(bind(&ControlPanelController::onToggleNodeSize, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, onToggleNodeSizeHandler, 201);
  
  auto hilightHandler(bind(&ControlPanelController::hilightNode, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_LISTBOX, hilightHandler, 555);

}

void ControlPanelController::hilightNode(wxCommandEvent& event) {
  int m = event.GetInt();
  int centerNodeId = _configuration->getNodeIdinLabels(m);
  _configuration->changeColor(centerNodeId);
}

void ControlPanelController::notifyUpdateNodeSize(wxCommandEvent& event) {
  float rate = (float) (event.GetInt()) / 100.0f;
  _userDefault->changeNodeSize(rate);
}

void ControlPanelController::notifyUpdateEdgeThickness(wxCommandEvent& event) {
  float rate = (float) (event.GetInt()) / 50.0f;
  _userDefault->changeEdgeThickness(rate);
}

void ControlPanelController::notifyUpdateDelta(wxCommandEvent&) {
  float rate = (float) (_controlPanel->DeltaSlider->GetValue()) / 100.0f;
  _graph->changeProjectionFactor(rate, _userDefault->layout());
}

void ControlPanelController::notifyUpdateScale(wxCommandEvent& event) {
  float rate = (float) (event.GetInt()) / 20.0f;
  _graph->changeScaleLayout(rate, _userDefault->layout());
}

void ControlPanelController::notifyUpdateDimension(wxCommandEvent& event) {
  float rate = (float) (event.GetInt()) / 1000.0f;
  _graph->changeDimension(rate, _userDefault->layout());
}

void ControlPanelController::updateNodeValueThreshold(wxCommandEvent&) {
  auto node_slider_b_pos = _controlPanel->nodeThresholdSlider_b->GetValue();
  auto node_slider_t_pos = _controlPanel->nodeThresholdSlider_t->GetValue();
  if (node_slider_b_pos <= node_slider_t_pos) {
    float bz = (float) (node_slider_b_pos) / 100.0f;
    float tz = (float) (node_slider_t_pos) / 100.0f;
    float nodethreshold_b = _graph->calcNodeThreshold(bz);
    float nodethreshold_t = _graph->calcNodeThreshold(tz);
    _configuration->_nodethreshold_b = nodethreshold_b;
    _configuration->_nodethreshold_t = nodethreshold_t;
    _graph->changeNodeThreshold(nodethreshold_b, nodethreshold_t);
  } else {
    _controlPanel->nodeThresholdSlider_b->SetValue(node_slider_t_pos - 1);
    float nodethreshold_b = _configuration->_nodethreshold_t * 0.99;
    _controlPanel->nodeThresholdSlider_t->SetValue(node_slider_b_pos + 1);
    float nodethreshold_t = nodethreshold_b * 1.01;
    _configuration->_nodethreshold_b = nodethreshold_b;
    _configuration->_nodethreshold_t = nodethreshold_t;
  }
}

void ControlPanelController::updateEdgeValueThreshold(wxCommandEvent&) {
  auto edge_slider_b_pos = _controlPanel->edgeThresholdSlider_b->GetValue();
  auto edge_slider_t_pos = _controlPanel->edgeThresholdSlider_t->GetValue();
  if (edge_slider_b_pos < edge_slider_t_pos) {
    float bz = (float) (edge_slider_b_pos) / 100.0f;
    float tz = (float) (edge_slider_t_pos) / 100.0f;
    float edgethreshold_b = _graph->calcEdgeThreshold(bz);
    float edgethreshold_t = _graph->calcEdgeThreshold(tz);
    _configuration->_edgethreshold_b = edgethreshold_b;
    _configuration->_edgethreshold_t = edgethreshold_t;
    _graph->changeEdgeThreshold(edgethreshold_b, edgethreshold_t);
  } else {
    _controlPanel->edgeThresholdSlider_b->SetValue(edge_slider_t_pos - 1);
    _controlPanel->edgeThresholdSlider_t->SetValue(edge_slider_b_pos + 1);
  }
}

void ControlPanelController::onToggleEdge(wxCommandEvent&) {
  bool isDraw = !_userDefault->isDrawEdge();
  _userDefault->changeIsDrawEdge(isDraw);
}

void ControlPanelController::onToggleNodeSize(wxCommandEvent&) {
  bool isDraw = !_userDefault->isDrawNode();
  _userDefault->changeIsDrawNode(isDraw);
}

void ControlPanelController::selectNodeAttr(wxCommandEvent&) {
  wxString label = _controlPanel->nodeAttrsChoice->GetStringSelection();
  int n = _controlPanel->nodeAttrsChoice->GetSelection();
  _controlPanel->nodeThresholdSlider_b->SetValue(0);
  _controlPanel->nodeThresholdSlider_t->SetValue(100);
  _graph->loadNodeAttrData(n);
  _configuration->changeNodeThreshholdAtrr(n);
}

void ControlPanelController::selectEdgeAttr(wxCommandEvent&) {
  wxString label = _controlPanel->edgeAttrsChoice->GetStringSelection();
  int n = _controlPanel->edgeAttrsChoice->GetSelection();
  _controlPanel->edgeThresholdSlider_b->SetValue(0);
  _controlPanel->edgeThresholdSlider_t->SetValue(100);
  _graph->loadEdgeAttrData(n);
  _configuration->changeEdgeThreshholdAttr(n);
}