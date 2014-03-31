#include "App.h"
#include "AppDelegete.h"

// For OSX
#include <ApplicationServices/ApplicationServices.h>

using namespace agi3d;

IMPLEMENT_APP(App)

bool App::OnInit() {
  // MainFrame: public wxFrame
  // Omajinai for Mac OS
  ProcessSerialNumber PSN;
  GetCurrentProcess(&PSN);
  TransformProcessType(&PSN, kProcessTransformToForegroundApplication);  
  AppDelegete &delegete = AppDelegete::instance();
  delegete.initControllers();
  delegete.run();
  return true;
}