#include "AGIApp.h"
#include "AGIFrame.h"
// For OSX
#include <ApplicationServices/ApplicationServices.h>

// called OnInit in "Main" class

IMPLEMENT_APP(AGIApp)

bool AGIApp::OnInit() {
  // MainFrame: public wxFrame
  // Omajinai for Mac OS
  ProcessSerialNumber PSN;
  GetCurrentProcess(&PSN);
  TransformProcessType(&PSN, kProcessTransformToForegroundApplication);

  AGIFrame *mFrame = new AGIFrame(wxT("Active Graph Interface 3D"));
  mFrame->Show(true);
  return true;
}