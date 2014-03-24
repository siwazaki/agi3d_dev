#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/panel.h"
#include <wx/menu.h>
#include "DrawPanel.h"
#include "SubPanel.h"
#include "AppearanceWindow.h"

// MainFrame extends wxFrame
class MainFrame : public wxFrame{
public:
	MainFrame(const wxString& title);

	wxSplitterWindow *base;
	DrawPanel *left;
	SubPanel *right;

	wxMenuBar *menubar;
	wxMenu *file;
	wxMenu *demoMenu;
	wxMenu *demoMenu2;
	wxMenu *edit;
	wxMenu *layoutMenu;
	wxMenu *rotationMenu;
	wxMenuItem *quit;

	AppearanceWindow * appw;

	void initload();
	SubPanel * GetSubPanel();
	DrawPanel * GetDrawPanel();
	void ResetMenuParams();
	void OnQuit(wxCommandEvent& event);
	void CaptureImage(wxCommandEvent& event);
	void Reset(wxCommandEvent& event);
	void ChangeLayoutModeTo2D(wxCommandEvent& event);
	void ChangeLayoutModeTo3D(wxCommandEvent& event);
	void SetAutoXRotation(wxCommandEvent& event);
	void SetAutoYRotation(wxCommandEvent& event);
	void StopAutoRotation(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OpenAppearanceWindow(wxCommandEvent& event);
};