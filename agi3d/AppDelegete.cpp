#include "AppDelegete.h"

using namespace agi3d;
using namespace std;

std::unique_ptr<AppDelegete> AppDelegete::m_instance;
std::once_flag AppDelegete::m_onceFlag;

AppDelegete& AppDelegete::instance()
{
  std::call_once(m_onceFlag,
                 [] {
                   m_instance.reset(new AppDelegete);
                 });
  return *m_instance.get();
}


AppDelegete::AppDelegete()
{
  //ビューの作成
  _frame = new Frame(wxT("Active Graph Interface 3D"));
  
  _menuBar = new MenuBar();
  _frame->SetMenuBar(_menuBar);
  
  _base = new wxSplitterWindow(_frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
  _base->SetSashGravity(0.80);
  _base->SetMinimumPaneSize(100);
  
  int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16};
  _graphicPanel = new  GraphicPanel(_base, args);
  _controlPanel = new ControlPanel(_base);
  _base->SplitVertically(_graphicPanel, _controlPanel);
  _appearanceWindow  = new AppearanceWindow(_controlPanel, wxT("Appearance"));
}

AppDelegete::~AppDelegete()
{
  
}

void AppDelegete::initMVC()
{
  //モデルの生成
  _graph = std::shared_ptr<Graph>(new Graph);
  _userDefault = std::shared_ptr<UserDefault>(new UserDefault);
  _configuration = std::shared_ptr<Configuration>(new Configuration);
  
  //コントローラの作成
  _userDefaultController = std::shared_ptr<UserDefaultController>(new UserDefaultController(_userDefault, _graph));
                                                                  
  _configurationController = std::shared_ptr<ConfigurationController>(new ConfigurationController(_configuration, _userDefault, _graph));
                                                                  
  _graphicController = std::shared_ptr<GraphicController>(new GraphicController(_graph, _configuration));

  
  //ビューにモデルをセット
  _graphicPanel->init(_graph, _userDefault, _configuration);
  _controlPanel->init(_graph, _userDefault, _configuration);
  _menuBar->init(_userDefault);
  
  //モデルにオブザーバーをセット
  _graph->addObserver(_graphicPanel);
  _configuration->addObserver(_controlPanel);
  _configuration->addObserver(_graphicPanel);
  _userDefault->addObserver(_graphicPanel);
  _userDefault->addObserver(_menuBar);
  _userDefault->addObserver(_controlPanel);
  
  //OpenGLの情報を出力
  outputOpenGLInfo();
  
}

void AppDelegete::run()
{
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//  glfwCreateWindow(640, 480, "Hello World", NULL, NULL );
  AppDelegete &delegete = AppDelegete::instance();

 _frame->Show(true);
}

MenuBar* AppDelegete::getMenuBar() const {
  return _menuBar;
}

Frame* AppDelegete::getFrame() const
{
  return _frame;
}

wxSplitterWindow* AppDelegete::getBase() const
{
  return _base;
}

GraphicPanel* AppDelegete::getGraphicPanel() const
{
  return _graphicPanel;
}

ControlPanel* AppDelegete::getControlPanel() const
{
  return _controlPanel;
}

AppearanceWindow* AppDelegete::getAppearanceWindow() const
{
  return _appearanceWindow;
}

const std::shared_ptr<ConfigurationController>& AppDelegete::getConfigurationController(){
  return _configurationController;
}

const std::shared_ptr<GraphicController>& AppDelegete::getGraphicController()
{
  return _graphicController;
}

const std::shared_ptr<Graph>& AppDelegete::getGraph()
{
  return _graph;
}

const std::shared_ptr<UserDefault>& AppDelegete::getUserDefault()
{
  return _userDefault;
}

const std::shared_ptr<Configuration>& AppDelegete::getConfiguration()
{
  return _configuration;
}


void AppDelegete::outputOpenGLInfo()
{
  cout << "Vendor :"<<glGetString(GL_VENDOR)<<'\n';
  cout << "GPU : "<<glGetString(GL_RENDERER) <<'\n';
  cout << "OpenGL ver. " <<glGetString(GL_VERSION)<<'\n';
  cout << "【拡張機能一覧】"<<endl;
  cout << glGetString(GL_EXTENSIONS) <<endl;
}