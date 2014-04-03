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
ConfigurationController::ConfigurationController(const std::shared_ptr<Configuration>& configuration, const std::shared_ptr<UserDefault>& userDefault, const std::shared_ptr<Graph>& graph) {
  _configuration = configuration;
  _userDefault = userDefault;
  _graph = graph;
  
  _controlPanel = AppDelegete::instance().getControlPanel();
  
  //@TODO
  //labelMap.clear();
  
  auto nortifyUpdateNodeSizeHandler(bind(&ConfigurationController::NortifyUpdateNodeSize, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, nortifyUpdateNodeSizeHandler, 40);
  
  auto nortifyUpdateEdgeThicknessHandler(bind(&ConfigurationController::NortifyUpdateEdgeThickness, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, nortifyUpdateEdgeThicknessHandler, 50);
  
  auto nortifyUpdateDeltaHandler(bind(&ConfigurationController::NortifyUpdateDelta, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, nortifyUpdateDeltaHandler, 60);
  
  auto nortifyUpdateScaleHandler(bind(&ConfigurationController::NortifyUpdateScale, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, nortifyUpdateScaleHandler, 61);
  
  auto nortifyUpdateDimensionHandler(bind(&ConfigurationController::NortifyUpdateDimension, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, nortifyUpdateDimensionHandler, 62);
  
  auto updateNodeValueThreshold_b_Handler(bind(&ConfigurationController::UpdateNodeValueThreshold_b, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateNodeValueThreshold_b_Handler, 70);
  
  auto updateNodeValueThreshold_t_Handler(bind(&ConfigurationController::UpdateNodeValueThreshold_t, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateNodeValueThreshold_t_Handler, 71);
  
  auto updateEdgeValueThreshold_b_Handler(bind(&ConfigurationController::UpdateEdgeValueThreshold_b, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateEdgeValueThreshold_b_Handler, 80);
  
  auto updateEdgeValueThreshold_t_Handler(bind(&ConfigurationController::UpdateEdgeValueThreshold_t, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_SLIDER_UPDATED, updateEdgeValueThreshold_t_Handler, 81);
  
  auto selectNodeAttrHandler(bind(&ConfigurationController::SelectNodeAttr, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHOICE_SELECTED, selectNodeAttrHandler, 120);
  
  auto selectEdgeAttrHandler(bind(&ConfigurationController::SelectEdgeAttr, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHOICE_SELECTED, selectEdgeAttrHandler, 121);
  
  auto onToggleEdgeHandler(bind(&ConfigurationController::OnToggleEdge, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, onToggleEdgeHandler, 200);
  
  auto onToggleNodeSizeHandler(bind(&ConfigurationController::OnToggleNodeSize, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, onToggleNodeSizeHandler, 201);
  
  auto handleListHandler(bind(&ConfigurationController::handleListEvent, this, placeholders::_1 ));
  this->_controlPanel->Bind(wxEVT_LISTBOX, handleListHandler, 555);
  
  
}

ConfigurationController::~ConfigurationController() {
  
}

void ConfigurationController::SetFPS(float _f) {
  fps = _f;
  wxString str = wxString::Format(wxT("   FPS : %f"), fps);
  
  _controlPanel->m_FPS->SetLabel(str);
}

void ConfigurationController::Init() {
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

void ConfigurationController::setTarget(int id) {
  //@TODO
  //  target->Clear();
  //  listbox->Clear();
  //  labelMap.clear();
  //  if (id != -1) {
  //    target->Append(wxString(labels[id]));
  //    _id = id;
  //    int key = 0;
  //    for (int i = 0; i < neighbor[id].size(); i++) {
  //      int _nei = neighbor[id][i];
  //      if (nodevalues[_nei] >= _nodethreshold_b && nodevalues[_nei] <= _nodethreshold_t) {
  //        listbox->Append(wxString(labels[_nei]));
  //        labelMap.insert(map<int, int>::value_type(key, i));
  //        key++;
  //      }
  //    }
  //    listbox->SetFirstItem(0);
  //  } else {
  //    _id = id;
  //  }
}

void ConfigurationController::handleListEvent(wxCommandEvent& event) {
  int m = event.GetInt();
  auto dp = AppDelegete::instance().getGraphicPanel();
  //@TODO
  //dp->changeColor(labelMap[m]);
}

void ConfigurationController::NortifyUpdateNodeSize(wxCommandEvent& event) {
  std::cout << "NortifyUpdateNodeSize" << std::endl;
  
  float rate = (float) (event.GetInt()) / 100.0f;
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->UpdateSize(rate);
}

void ConfigurationController::NortifyUpdateEdgeThickness(wxCommandEvent& event) {
  float rate = (float) (event.GetInt()) / 50.0f;
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->UpdateThickness(rate);
}

void ConfigurationController::NortifyUpdateDelta(wxCommandEvent& event) {
  float rate = (float) (_controlPanel->DeltaSlider->GetValue()) / 100.0f;
  _graph->changeProjectionFactor(rate, _userDefault->layout());
  
}
void ConfigurationController::NortifyUpdateScale(wxCommandEvent& event) {
  float rate = (float) (event.GetInt()) / 20.0f;
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->ScaleLayout(rate);
}

void ConfigurationController::NortifyUpdateDimension(wxCommandEvent& event) {
  //  float rate = (float) (event.GetInt()) / 1000.0f;
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //  dp->ChangeDimension(rate);
}

void ConfigurationController::UpdateNodeValueThreshold_b(wxCommandEvent& event) {
  
  auto node_slider_b_pos = _controlPanel->nodeThresholdSlider_b->GetValue();
  
  auto node_slider_t_pos = _controlPanel->nodeThresholdSlider_t->GetValue();
  
    if (node_slider_b_pos <= node_slider_t_pos) {
      float z = (float) (node_slider_b_pos) / 100.0f;
      float nodethreshold_b = _graph->calcNodeThreshold(z);
      
      
//      dp->UpdateNodeThreshold_b(z, nodeThresholdAttrID);
    } else {
//      nodeThresholdSlider_b->SetValue(node_slider_t_pos - 1);
  //    float nodethreshold_b = _nodethreshold_t * 0.99;
    }
 

  
  
  //@TODO
  //
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //
}

void ConfigurationController::UpdateNodeValueThreshold_t(wxCommandEvent& event) {
  //  @TODO
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //  node_slider_t_pos = nodeThresholdSlider_t->GetValue();
  //  if (node_slider_b_pos <= node_slider_t_pos) {
  //    float z = (float) (node_slider_t_pos) / 100.0f;
  //    _nodethreshold_t = dp->UpdateNodeThreshold_t(z, nodeThresholdAttrID);
  //  } else {
  //    nodeThresholdSlider_t->SetValue(node_slider_b_pos + 1);
  //    _nodethreshold_t = _nodethreshold_b * 1.01;
  //  }
}

void ConfigurationController::UpdateEdgeValueThreshold_b(wxCommandEvent& event) {
  //@TODO
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //
  //  edge_slider_b_pos = edgeThresholdSlider_b->GetValue();
  //  if (edge_slider_b_pos < edge_slider_t_pos) {
  //    float z = (float) (edge_slider_b_pos) / 100.0f;
  //    dp->UpdateEdgeThreshold_b(z, edgeThresholdAttrID);
  //  } else {
  //    edgeThresholdSlider_b->SetValue(edge_slider_t_pos - 1);
  //  }
}

void ConfigurationController::UpdateEdgeValueThreshold_t(wxCommandEvent& event) {
  //@TODO
  //  auto dp = AppDelegete::instance().getGraphicPanel();
  //  edge_slider_t_pos = edgeThresholdSlider_t->GetValue();
  //  if (edge_slider_b_pos < edge_slider_t_pos) {
  //    float z = (float) (edge_slider_t_pos) / 100.0f;
  //    dp->UpdateEdgeThreshold_t(z, edgeThresholdAttrID);
  //  } else {
  //    edgeThresholdSlider_t->SetValue(edge_slider_b_pos + 1);
  //  }
}

void ConfigurationController::OnToggleEdge(wxCommandEvent& event) {
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->DrawEdge();
}

void ConfigurationController::OnToggleNodeSize(wxCommandEvent& event) {
  auto dp = AppDelegete::instance().getGraphicPanel();
  dp->NodeModeChange();
}

void ConfigurationController::SelectNodeAttr(wxCommandEvent& event) {
  //  @TODO
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

void ConfigurationController::SelectEdgeAttr(wxCommandEvent& event) {
  //  @TODO
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


