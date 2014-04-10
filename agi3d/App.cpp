#include <sstream>

#include "App.h"
#include "AppDelegete.h"
#include <ApplicationServices/ApplicationServices.h>
using namespace std;
using namespace agi3d;
IMPLEMENT_APP(App)

bool App::OnInit() {
  ProcessSerialNumber PSN;
  GetCurrentProcess(&PSN);
  TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
  AppDelegete &delegete = AppDelegete::instance();
  delegete.initMVC();
  delegete.run();
  return true;
}