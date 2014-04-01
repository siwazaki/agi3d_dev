#include "Frame.h"

#include <wx/event.h>
#include <fstream>
#include <iostream>

using namespace agi3d;

Frame::Frame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 870)) {

  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Frame::OnQuit));
  this->Centre();
}

void Frame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
  Close(true);
}

void Frame::initload() {
//  wxFileDialog * openFileDialog = new wxFileDialog(this);
//  if (openFileDialog->ShowModal() == wxID_OK) {
//    wxString filePath = openFileDialog->GetPath();
//    std::string fname = string(filePath.mb_str());
//    int path_i = fname.find_last_of("/");
//    int ext_i = fname.find_last_of(".");
//    std::string f_ext = fname.substr(ext_i + 1);
//    std::string filename = fname.substr(path_i + 1);
//    
//    if (f_ext == "txt") {
//      if (filename.find("DisMat") != string::npos) {
//        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
//        loadMatrixData_t(filePath.mb_str());
//      } else if (filename.find("Data.") != string::npos) {
//        graphName = fname.substr(path_i + 1, fname.size() - path_i - 9);
//        loadLayoutData_t(filePath.mb_str());
//      } else if (filename.find("DataAll") != string::npos) {
//        graphName = fname.substr(path_i + 1, fname.size() - path_i - 12);
//        loadLayoutData_t(filePath.mb_str());
//      } else {
//        cerr << "This file is not available" << endl;
//        exit(1);
//      }
//    } else if (f_ext == "bin") {
//      cout << "bin" << endl;
//      if (filename.find("DisMat") != string::npos) {
//        cout << "DisMat" << endl;
//        graphName = fname.substr(path_i + 1, fname.size() - path_i - 11);
//        loadMatrixData_b(filePath.mb_str());
//      } else if (filename.find("Data.") != string::npos) {
//        graphName = fname.substr(path_i + 1, fname.size() - path_i - 9);
//        loadLayoutData_b(filePath.mb_str());
//      } else if (filename.find("DataAll") != string::npos) {
//        graphName = fname.substr(path_i + 1, fname.size() - path_i - 12);
//        loadLayoutData_b(filePath.mb_str());
//      } else {
//        cerr << "This file is not available" << endl;
//        exit(1);
//      }
//    }
//    cout << "graphName:" << graphName << endl;
//    left->SetupPanel();
//    right->Init();
//    appw->Init();
//  }
}