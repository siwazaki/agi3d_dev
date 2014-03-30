#include "Frame.h"
#include <fstream>
#include <iostream>
using namespace std;

//import from calclayout.cpp
void loadMatrixData_t(const char *);
void loadLayoutData_t(const char *);
void loadMatrixData_b(const char *);
void loadLayoutData_b(const char *);

extern string graphName;

static bool x_rotation;
static bool y_rotation;

Frame::Frame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 870)) {

  base = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
  base->SetSashGravity(0.80);
  base->SetMinimumPaneSize(100);

  //Panels
  int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16};
  left = new GraphicPanel(base, args);
  right = new ControlPanel(base);

  base->SplitVertically(left, right);

  //MenuBar
  menubar = new wxMenuBar();
  file = new wxMenu();
  file->Append(wxID_OPEN, wxT("Open"));
  file->AppendSeparator();

  edit = new wxMenu();
  edit->Append(10, wxT("Reset"));
  edit->AppendSeparator();

  edit->Append(16, wxT("Appearance"));
  edit->AppendSeparator();

  layoutMenu = new wxMenu();
  layoutMenu->Append(11, wxT(" 3D  ✓"));
  layoutMenu->Append(12, wxT(" 2D"));

  edit->AppendSubMenu(layoutMenu, wxT("Layout Mode"));
  edit->AppendSeparator();

  rotationMenu = new wxMenu();
  rotationMenu->Append(13, wxT(" X Rotation  "));
  rotationMenu->Append(14, wxT(" Y Rotation  "));
  rotationMenu->Append(15, wxT(" Stop "));
  x_rotation = false;
  y_rotation = false;

  edit->AppendSubMenu(rotationMenu, wxT("Auto Rotation"));
  edit->AppendSeparator();

  edit->Append(17, wxT("Capture Image"));

  quit = new wxMenuItem(file, wxID_EXIT, wxT("&Quit\tCtrl+W"));
  file->Append(quit);

  menubar->Append(file, wxT("&File"));
  menubar->Append(edit, wxT("&Edit"));
  SetMenuBar(menubar);

  appw = new AppearanceWindow(right, wxT("Appearance"));

  //Connect Event
  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnQuit));
  Connect(10, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::Reset));
  Connect(11, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::ChangeLayoutModeTo3D));
  Connect(12, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::ChangeLayoutModeTo2D));
  Connect(13, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::SetAutoXRotation));
  Connect(14, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::SetAutoYRotation));
  Connect(15, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::StopAutoRotation));
  Connect(16, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OpenAppearanceWindow));
  Connect(17, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::CaptureImage));

  Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnOpen));

  this->Centre();
}

ControlPanel * Frame::GetSubPanel() {
  return right;
}

GraphicPanel * Frame::GetDrawPanel() {
  return left;
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
  Close(true);
}

void Frame::initload() {
  wxFileDialog * openFileDialog = new wxFileDialog(this);
  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString filePath = openFileDialog->GetPath();
    string fname = string(filePath.mb_str());
    int path_i = fname.find_last_of("/");
    int ext_i = fname.find_last_of(".");
    string f_ext = fname.substr(ext_i + 1);
    string filename = fname.substr(path_i + 1);

    if (f_ext == "txt") {
      if (filename.find("DisMat") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
        loadMatrixData_t(filePath.mb_str());
      } else if (filename.find("Data.") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 9);
        loadLayoutData_t(filePath.mb_str());
      } else if (filename.find("DataAll") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 12);
        loadLayoutData_t(filePath.mb_str());
      } else {
        cerr << "This file is not available" << endl;
        exit(1);
      }
    } else if (f_ext == "bin") {
      cout << "bin" << endl;
      if (filename.find("DisMat") != string::npos) {
        cout << "DisMat" << endl;
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
        loadMatrixData_b(filePath.mb_str());
      } else if (filename.find("Data.") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 9);
        loadLayoutData_b(filePath.mb_str());
      } else if (filename.find("DataAll") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 12);
        loadLayoutData_b(filePath.mb_str());
      } else {
        cerr << "This file is not available" << endl;
        exit(1);
      }
    }
    cout << "graphName:" << graphName << endl;
    left->SetupPanel();
    right->Init();
    appw->Init();
  }
}

void Frame::OnOpen(wxCommandEvent& event) {
  wxFileDialog * openFileDialog = new wxFileDialog(this);
  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString filePath = openFileDialog->GetPath();
    string fname = string(filePath.mb_str());
    int path_i = fname.find_last_of("/");
    int ext_i = fname.find_last_of(".");
    string f_ext = fname.substr(ext_i + 1);
    string filename = fname.substr(path_i + 1);

    if (f_ext == "txt") {
      if (filename.find("DisMat") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
        loadMatrixData_t(filePath.mb_str());
      } else if (filename.find("Data.") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 9);
        loadLayoutData_t(filePath.mb_str());
      } else if (filename.find("DataAll") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 12);
        loadLayoutData_t(filePath.mb_str());
      } else {
        cerr << "This file is not available" << endl;
        exit(1);
      }
    } else if (f_ext == "bin") {
      if (filename.find("DisMat") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
        loadMatrixData_b(filePath.mb_str());
      } else if (filename.find("Data.") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 15);
        loadLayoutData_b(filePath.mb_str());
      } else if (filename.find("DataAll") != string::npos) {
        graphName = fname.substr(path_i + 1, fname.size() - path_i - 18);
        loadLayoutData_b(filePath.mb_str());
      } else {
        cerr << "This file is not available" << endl;
        exit(1);
      }
    }
    cout << "graphName:" << graphName << endl;
    left->SetupPanel();
    right->Init();
    appw->Init();
  }
}

void Frame::ResetMenuParams() {
  x_rotation = false;
  left->SetXRotation(false);
  rotationMenu->SetLabel(13, wxT(" X Rotation"));
  y_rotation = false;
  left->SetYRotation(false);
  rotationMenu->SetLabel(14, wxT(" Y Rotation"));
}

void Frame::Reset(wxCommandEvent& event) {
  left->ResetLayout();
  right->Init();
  appw->Init();
  ResetMenuParams();
}

void Frame::ChangeLayoutModeTo2D(wxCommandEvent& event) {
  left->ChangeLayoutMode(2);
  right->Init();
  appw->Init();
  layoutMenu->SetLabel(11, wxT(" 3D"));
  layoutMenu->SetLabel(12, wxT(" 2D  ✓"));
  ResetMenuParams();
}

void Frame::ChangeLayoutModeTo3D(wxCommandEvent& event) {
  left->ChangeLayoutMode(3);
  right->Init();
  appw->Init();
  layoutMenu->SetLabel(11, wxT(" 3D  ✓"));
  layoutMenu->SetLabel(12, wxT(" 2D"));
  ResetMenuParams();
}

void Frame::SetAutoXRotation(wxCommandEvent& event) {
  if (x_rotation) {
    left->SetXRotation(false);
    x_rotation = false;
    rotationMenu->SetLabel(13, wxT(" X Rotation  "));
  } else {
    left->SetXRotation(true);
    x_rotation = true;
    rotationMenu->SetLabel(13, wxT(" X Rotation  ✓"));
  }
}

void Frame::SetAutoYRotation(wxCommandEvent& event) {
  if (y_rotation) {
    left->SetYRotation(false);
    y_rotation = false;
    rotationMenu->SetLabel(14, wxT(" Y Rotation  "));
  } else {
    left->SetYRotation(true);
    y_rotation = true;
    rotationMenu->SetLabel(14, wxT(" Y Rotation  ✓"));
  }
}

void Frame::StopAutoRotation(wxCommandEvent& event) {
  ResetMenuParams();
}

void Frame::CaptureImage(wxCommandEvent& event) {
  left->SavePixelData();
}

void Frame::OpenAppearanceWindow(wxCommandEvent& event) {
  appw->Show(true);
  appw->Raise();
}