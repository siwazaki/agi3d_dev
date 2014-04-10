#include "AppearanceWindow.h"

using namespace std;

AppearanceWindow::AppearanceWindow(wxWindow * parent, const wxString& title)
: wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(300, 300)) {

  wxPanel *panel = new wxPanel(this, wxID_ANY);
  wxPanel *childpanel = new wxPanel(panel, wxID_ANY);

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);

  wxButton * btnOK = new wxButton(panel, 300, wxT("Close"));

  wxStaticText * nodeSizeText = new wxStaticText(childpanel, wxID_ANY, wxT("Node Size"), wxPoint(20, 20));
  nodeSizeSlider = new wxSlider(childpanel, 40, 100, 20, 250, wxPoint(20, 40), wxSize(120, -1));

  wxStaticText * edgeThicknessText = new wxStaticText(childpanel, wxID_ANY, wxT("Edge Thikness"), wxPoint(160, 20));
  edgeThicknessSlider = new wxSlider(childpanel, 50, 50, 20, 200, wxPoint(160, 40), wxSize(120, -1));

  wxStaticText * CheckBoxes = new wxStaticText(childpanel, wxID_ANY, wxT("CheckBoxes About Attributes"), wxPoint(20, 80));
  edgeCheckBox = new wxCheckBox(childpanel, 200, wxT("Show Edge"), wxPoint(20, 100));
  edgeCheckBox->SetValue(true);
  nodeSizeCheckBox = new wxCheckBox(childpanel, 201, wxT("Node Size"), wxPoint(150, 100));
  nodeSizeCheckBox->SetValue(false);

  wxStaticText * DeltaText = new wxStaticText(childpanel, wxID_ANY, wxT("Parameter Delta"), wxPoint(20, 140));
  DeltaSlider = new wxSlider(childpanel, 60, 50, 0, 400, wxPoint(20, 160), wxSize(120, -1));

  wxStaticText * ScaleText = new wxStaticText(childpanel, wxID_ANY, wxT("Parameter Sclae"), wxPoint(160, 140));
  ScaleSlider = new wxSlider(childpanel, 61, 10, 10, 160, wxPoint(160, 160), wxSize(120, -1));

  wxStaticText * DimensionText = new wxStaticText(childpanel, wxID_ANY, wxT("Parameter Dimension"), wxPoint(20, 190));
  DimensionSlider = new wxSlider(childpanel, 62, 100, 1, 1000, wxPoint(20, 210), wxSize(120, -1));

  hbox1->Add(childpanel);
  hbox2->Add(btnOK);

  vbox->Add(hbox1, 1, wxEXPAND);
  vbox->Add(hbox2, 0, wxALIGN_CENTER | wxCENTER | wxBOTTOM, 10);

  panel->SetSizer(vbox);

  Connect(300, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AppearanceWindow::CloseWindow));

  Connect(40, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(AppearanceWindow::UpdateNodeSize));
  Connect(50, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(AppearanceWindow::UpdateEdgeThickness));
  Connect(60, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(AppearanceWindow::UpdateDelta));
  Connect(61, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(AppearanceWindow::UpdateScale));
  Connect(62, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(AppearanceWindow::UpdateDimension));

  Connect(200, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AppearanceWindow::OnToggleShowEdge));
  Connect(201, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(AppearanceWindow::OnToggleNodeSize));

  Center();
}

void AppearanceWindow::showModel() {
 //FIXME: userdefault, configurationの値に合わせて設定する。
  nodeSizeSlider->SetValue(100);
  edgeThicknessSlider->SetValue(50);
  edgeCheckBox->SetValue(true);
  nodeSizeCheckBox->SetValue(true);
  ScaleSlider->SetValue(20);
  DeltaSlider->SetValue(50);
  DimensionSlider->SetValue(1000);
}

void AppearanceWindow::CloseWindow(wxCommandEvent& event) {
  this->Show(false);
}

void AppearanceWindow::UpdateNodeSize(wxScrollEvent& event) {
  int pos = nodeSizeSlider->GetValue();
  event.SetInt(pos);
  event.Skip();
}

void AppearanceWindow::UpdateEdgeThickness(wxScrollEvent& event) {
  int pos = edgeThicknessSlider->GetValue();
  event.SetInt(pos);
  event.Skip();
}

void AppearanceWindow::UpdateDelta(wxScrollEvent& event) {
  int pos = DeltaSlider->GetValue();
  event.SetInt(pos);
  event.Skip();
}

void AppearanceWindow::UpdateScale(wxScrollEvent& event) {
  int pos = ScaleSlider->GetValue();
  event.SetInt(pos);
  event.Skip();
}

void AppearanceWindow::UpdateDimension(wxScrollEvent& event) {
  int pos = DimensionSlider->GetValue();
  event.SetInt(pos);
  event.Skip();
}

void AppearanceWindow::OnToggleShowEdge(wxCommandEvent& event) {
  event.Skip();
}

void AppearanceWindow::OnToggleNodeSize(wxCommandEvent& event) {
  event.Skip();
}