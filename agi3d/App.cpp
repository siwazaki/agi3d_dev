#include "Config.h"
#include "App.h"
#include "Frame.h"
// For OSX
#include <ApplicationServices/ApplicationServices.h>

using namespace agi3d;

// called OnInit in "Main" class
IMPLEMENT_APP(App)

bool App::OnInit() {
  // MainFrame: public wxFrame
  // Omajinai for Mac OS
  ProcessSerialNumber PSN;
  GetCurrentProcess(&PSN);
  TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
  _settingController = std::shared_ptr<SettingController>(new SettingController(NULL,NULL));
  Frame *mFrame = new Frame(wxT("Active Graph Interface 3D"));
  mFrame->addController(_settingController);
  mFrame->Show(true);
    return true;
}