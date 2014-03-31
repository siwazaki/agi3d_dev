#include <memory>
#include <wx/wx.h>
#include "SettingController.h"
/**
 @mainpage
 アプリケーションの説明を書く
 */

namespace agi3d {
  /**
   * 本アプリケーションの入り口。<br>
   * OnInit()で、アプリケーションの初期値の設定、<br>
   * 初期化、及び3DFrameを作成し表示する。<br>
   */
  class App : public wxApp {
  public:
    virtual bool OnInit();
  private:
    std::shared_ptr<SettingController> _settingController;
    
  };
}