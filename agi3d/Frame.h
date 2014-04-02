#ifndef __agi3d_frame_h__
#define __agi3d_frame_h__

#include <memory>
#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/panel.h"

namespace agi3d {
  /**
   *
   * メインのフレームを表すクラス。
   */
  class Frame : public wxFrame {
  public:
    Frame(const wxString& title);
    //@TODO: should be removed
    void initload();
    
  private:
    void OnQuit(wxCommandEvent& event);
  };
}

#endif