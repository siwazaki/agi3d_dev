#ifndef __agi3d_frame_h__
#define __agi3d_frame_h__

#include <memory>
#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/panel.h"

#include "GraphicPanel.h"
#include "ControlPanel.h"

namespace agi3d {
  /**
   *
   * メインのフレームを表すクラス。
   */
  class Frame : public wxFrame {
  public:
    Frame(const wxString& title);
    GraphicPanel *left;
    ControlPanel *right;    
    //@TODO: should be removed
    void initload();
    ControlPanel * GetSubPanel();
    GraphicPanel * GetDrawPanel();
    
  private:
    void OnQuit(wxCommandEvent& event);
  };
}

#endif