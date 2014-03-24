#include "Main.h"
#include "MainFrame.h"
// Omajinai for Mac OS
#include <ApplicationServices/ApplicationServices.h>

// called OnInit in "Main" class
IMPLEMENT_APP(Main)

bool Main::OnInit(){
	// MainFrame: public wxFrame
	// Omajinai for Mac OS
	ProcessSerialNumber PSN;
	GetCurrentProcess(&PSN);
	TransformProcessType(&PSN,kProcessTransformToForegroundApplication);

	MainFrame *mFrame = new MainFrame(wxT("Active Graph Interface 3D"));
	mFrame->Show(true);
	return true;
}