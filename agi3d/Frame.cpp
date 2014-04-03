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