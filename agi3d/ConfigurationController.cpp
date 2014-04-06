//
//  ConfigurationController.cpp
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

#include "ConfigurationController.h"
#include "AppDelegete.h"

using namespace std;
using namespace agi3d;

//TODO: Use Builder or Factory
ConfigurationController::ConfigurationController(const std::shared_ptr<Configuration>& configuration
                                                 , const std::shared_ptr<UserDefault>& userDefault
                                                 , const std::shared_ptr<Graph>& graph) {
  _configuration = configuration;
  _userDefault = userDefault;
  _graph = graph;
  
  _controlPanel = AppDelegete::instance().getControlPanel();
  
  //@TODO
  //labelMap.clear();
  
  auto notifyUpdateNodeSizeHandler(bind(&ConfigurationController::notifyUpdateNodeSize, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateNodeSizeHandler, 40);
  
  auto notifyUpdateEdgeThicknessHandler(bind(&ConfigurationController::notifyUpdateEdgeThickness, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateEdgeThicknessHandler, 50);
  
  auto notifyUpdateDeltaHandler(bind(&ConfigurationController::notifyUpdateDelta, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateDeltaHandler, 60);
  
  auto notifyUpdateScaleHandler(bind(&ConfigurationController::notifyUpdateScale, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateScaleHandler, 61);
  
  auto notifyUpdateDimensionHandler(bind(&ConfigurationController::notifyUpdateDimension, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, notifyUpdateDimensionHandler, 62);
  
  auto updateNodeValueThresholdHandler(bind(&ConfigurationController::updateNodeValueThreshold, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateNodeValueThresholdHandler, 70);
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateNodeValueThresholdHandler, 71);

  auto updateEdgeValueThresholdHandler(bind(&ConfigurationController::updateEdgeValueThreshold, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateEdgeValueThresholdHandler, 80);
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateEdgeValueThresholdHandler, 81);
  
  auto selectNodeAttrHandler(bind(&ConfigurationController::selectNodeAttr, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHOICE_SELECTED, selectNodeAttrHandler, 120);
  
  auto selectEdgeAttrHandler(bind(&ConfigurationController::selectEdgeAttr, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHOICE_SELECTED, selectEdgeAttrHandler, 121);
  
  auto onToggleEdgeHandler(bind(&ConfigurationController::onToggleEdge, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, onToggleEdgeHandler, 200);
  
  auto onToggleNodeSizeHandler(bind(&ConfigurationController::onToggleNodeSize, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, onToggleNodeSizeHandler, 201);
  
  auto handleListHandler(bind(&ConfigurationController::handleListEvent, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_LISTBOX, handleListHandler, 555);
  
}

ConfigurationController::~ConfigurationController() {
  
}

void ConfigurationController::setFPS(float _f) {
  fps = _f;
  wxString str = wxString::Format(wxT("   FPS : %f"), fps);
  
  _controlPanel->m_FPS->SetLabel(str);
}

void ConfigurationController::init() {
  _controlPanel->DeltaSlider->SetValue(50);
  
  _controlPanel->nodeThresholdSlider_b->SetValue(0);
  _controlPanel->nodeThresholdSlider_t->SetValue(100);
  
  _controlPanel->edgeThresholdSlider_b->SetValue(0);
  _controlPanel->edgeThresholdSlider_t->SetValue(100);
  
  _controlPanel->nodeAttrsChoice->SetSelection(0);
  _configuration->nodeThresholdAttrID = 0;
  _controlPanel->edgeAttrsChoice->SetSelection(0);
  _configuration->edgeThresholdAttrID = 0;
  
  _controlPanel->target->SetLabel(wxString());
  _controlPanel->listbox->Clear();
  
  _configuration->_labelMap.clear();
  
  auto dp = AppDelegete::instance().getGraphicPanel();
  
  dp->UpdateSize(1.0f);
  dp->UpdateThickness(1.0f);
  
  dp->SetXRotation(false);
  dp->SetYRotation(false);
  
  //@TODO 一時的な実装
  wxString _graphlabel = wxString::Format(wxT("   FileName \t %s"), _configuration->graphName());
  //@TODO
  //wxString _nodeSize = wxString::Format(wxT("   #Node \t %i"), N);
  //wxString _edgeSize = wxString::Format(wxT("   #Edge   \t %i"), M);
  
  _controlPanel->fileNameLabel->SetLabel(_graphlabel);
  //@TODO
  //  _controlPanel->nodeSizeLabel->SetLabel(nodeSize);
  //  _controlPanel->edgeSizeLabel->SetLabel(edgeSize);
}

void ConfigurationController::handleListEvent(wxCommandEvent& event) {
  int m = event.GetInt();
  auto dp = AppDelegete::instance().getGraphicPanel();
  //TODO:
  //dp->changeColor(labelMap[m]);
}

void ConfigurationController::notifyUpdateNodeSize(wxCommandEvent& event) {
  //TODO:
  float rate = (float) (event.GetInt()) / 100.0f;
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->UpdateSize(rate);
}

void ConfigurationController::notifyUpdateEdgeThickness(wxCommandEvent& event) {
  //TODO:
  float rate = (float) (event.GetInt()) / 50.0f;
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->UpdateThickness(rate);
}

void ConfigurationController::notifyUpdateDelta(wxCommandEvent&) {
  //TODO:
  float rate = (float) (_controlPanel->DeltaSlider->GetValue()) / 100.0f;
  _graph->changeProjectionFactor(rate, _userDefault->layout());
  
}

void ConfigurationController::notifyUpdateScale(wxCommandEvent& event) {
  //TODO:
  float rate = (float) (event.GetInt()) / 20.0f;
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->ScaleLayout(rate);
}

void ConfigurationController::notifyUpdateDimension(wxCommandEvent& event) {
  //TODO:
  //  float rate = (float) (event.GetInt()) / 1000.0f;
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //  dp->ChangeDimension(rate);
}

void ConfigurationController::updateNodeValueThreshold(wxCommandEvent& event) {
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

void ConfigurationController::updateEdgeValueThreshold(wxCommandEvent&) {
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

void ConfigurationController::onToggleEdge(wxCommandEvent&) {
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->DrawEdge();
}

void ConfigurationController::onToggleNodeSize(wxCommandEvent&) {
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->NodeModeChange();
}

void ConfigurationController::selectNodeAttr(wxCommandEvent&) {
  //TODO:
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //  wxString label = nodeAttrsChoice->GetStringSelection();
  //  int n = nodeAttrsChoice->GetSelection();
  //  nodeThresholdSlider_b->SetValue(0);
  //  nodeThresholdSlider_t->SetValue(100);
  //  node_slider_b_pos = 0;
  //  node_slider_t_pos = 100;
  //  nodeThresholdAttrID = n;
  //  string strs = string(label.mb_str());
  //
  //  //一時的な実装
  //  auto configurationController = AppDelegete::instance().getConfigurationController();
  //  loadNodeAttrData(n, configurationController->getGraphName());
  //  dp->ResetIsDrawingNodes();
}

void ConfigurationController::selectEdgeAttr(wxCommandEvent&) {
  //TODO:
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //  wxString label = edgeAttrsChoice->GetStringSelection();
  //  int n = edgeAttrsChoice->GetSelection();
  //  edgeThresholdSlider_b->SetValue(0);
  //  edgeThresholdSlider_t->SetValue(100);
  //  edge_slider_b_pos = 0;
  //  edge_slider_t_pos = 100;
  //  edgeThresholdAttrID = n;
  //  string strs = string(label.mb_str());
  //
  //  //一時的な実装
  //  auto configurationController = AppDelegete::instance().getConfigurationController();
  //  loadEdgeAttrData(n, configurationController->getGraphName());
  //  dp->ResetIsDrawingEdges();
}


