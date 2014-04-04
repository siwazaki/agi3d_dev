#include <iostream>
#include <string>
#include <vector>
#include <cml/cml.h>
#include <map>

#include "ControlPanel.h"
#include "AppDelegete.h"

using namespace std;
using namespace agi3d;

//wx Macros


using namespace agi3d;

ControlPanel::ControlPanel(wxWindow* parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(280, 870)) {
  
  wxPanel *myPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(280, 870));

  wxBoxSizer *mybox = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *b1 = new wxBoxSizer(wxHORIZONTAL);

  m_FPS = new wxStaticText(this, -1, wxT("   FPS"), wxDefaultPosition, wxSize(100, 28), wxALIGN_LEFT);
  fileNameLabel = new wxStaticText(this, -1, wxT("   filename"), wxDefaultPosition, wxSize(100, 25), wxALIGN_LEFT);
  nodeSizeLabel = new wxStaticText(this, -1, wxT("   #Node "), wxDefaultPosition, wxSize(100, 25), wxALIGN_LEFT);
  edgeSizeLabel = new wxStaticText(this, -1, wxT("   #Edge  "), wxDefaultPosition, wxSize(100, 25), wxALIGN_LEFT);

  mybox->Add(m_FPS, 1, wxEXPAND, 5);
  mybox->Add(fileNameLabel, 1, wxEXPAND, 0);
  mybox->Add(nodeSizeLabel, 1, wxEXPAND, 0);
  mybox->Add(edgeSizeLabel, 1, wxEXPAND, 0);

  mybox->Add(myPanel, 1, wxEXPAND | wxALL, 5);
  this->SetSizer(mybox);

  wxStaticText * DeltaText = new wxStaticText(myPanel, wxID_ANY, wxT("Projection Factor"), wxPoint(10, 0));
  DeltaSlider = new wxSlider(myPanel, 60, 50, 0, 400, wxPoint(10, 20), wxSize(245, -1));

  wxStaticText * nodeThresholdText = new wxStaticText(myPanel, wxID_ANY, wxT("Node Filtering"), wxPoint(10, 50));

  wxArrayString nodeAttrs;
  nodeAttrs.Add("No Feature");
  nodeAttrs.Add("Degree Centrality");
  nodeAttrs.Add("Closness Centrality");
  nodeAttrs.Add("Betweeness Centrality");
  nodeAttrs.Add("Clustering Coefficient");
  nodeAttrs.Add("Eigenvector Centrality");
  nodeAttrs.Add("PageRank");
  nodeAttrsChoice = new wxChoice(myPanel, 120, wxPoint(7, 70), wxSize(250, -1), nodeAttrs);

  nodeThresholdSlider_b = new wxSlider(myPanel, 70, 0, 0, 100, wxPoint(10, 95), wxSize(247, -1));
  nodeThresholdSlider_t = new wxSlider(myPanel, 71, 100, 0, 100, wxPoint(10, 112), wxSize(247, -1));
 
  wxStaticText * edgeThresholdText = new wxStaticText(myPanel, wxID_ANY, wxT("Edge Filtering"), wxPoint(10, 140));

  wxArrayString edgeAttrs;
  edgeAttrs.Add("No Feature");
  edgeAttrs.Add("Simpson");
  edgeAttrs.Add("Extended Simpson");
  edgeAttrs.Add("Betweeness");
  edgeAttrs.Add("Weight");
  edgeAttrsChoice = new wxChoice(myPanel, 121, wxPoint(7, 165), wxSize(250, -1), edgeAttrs);

  edgeThresholdSlider_b = new wxSlider(myPanel, 80, 0, 0, 100, wxPoint(10, 190), wxSize(247, -1));
  edgeThresholdSlider_t = new wxSlider(myPanel, 81, 100, 0, 100, wxPoint(10, 207), wxSize(247, -1));
  target = new wxListBox(myPanel, 554, wxPoint(5, 240), wxSize(260, 25));

  listbox = new wxListBox(myPanel, 555, wxPoint(5, 270), wxSize(260, 460));
 
}

ControlPanel::~ControlPanel() {
  
}

void ControlPanel::update(const agi3d::Observable &observable, E_ObserveType observeType)
{
  switch (observeType) {
    default:
      break;
  }
}

void ControlPanel::init(const std::shared_ptr<Graph>& graph,
                        const std::shared_ptr<UserDefault>& userDefault,
                        const std::shared_ptr<Configuration>& configuration) {
  _graph = graph;
  _userDefault = userDefault;
  _configuration = configuration;
  
}