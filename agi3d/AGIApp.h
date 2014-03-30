#include <wx/wx.h>

/**
  @mainpage
   アプリケーションの説明を書く
 */

/**
 * 本アプリケーションの入り口。<br>
 * OnInit()で、アプリケーションの初期値の設定、<br>
 * 初期化、及びAGI3DFrameを作成し表示する。<br>
 */
class AGIApp : public wxApp {
public:
  virtual bool OnInit();
};