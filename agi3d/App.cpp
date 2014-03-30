#include "Config.h"
#include "App.h"
#include "Frame.h"
// For OSX
#include <ApplicationServices/ApplicationServices.h>

// called OnInit in "Main" class

IMPLEMENT_APP(App)

bool App::OnInit() {
  // MainFrame: public wxFrame
  // Omajinai for Mac OS
  ProcessSerialNumber PSN;
  GetCurrentProcess(&PSN);
  TransformProcessType(&PSN, kProcessTransformToForegroundApplication);

  Frame *mFrame = new Frame(wxT("Active Graph Interface 3D"));
  mFrame->Show(true);
    return true;
}