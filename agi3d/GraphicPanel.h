#ifndef _glpane_
#define _glpane_

#include <memory>
#include <cml/cml.h>
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "wx/stopwatch.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/lexical_cast.hpp>

#include "UserDefault.h"
#include "Configuration.h"
#include "Graph.h"
#include "Observer.h"

namespace agi3d {
  
  typedef cml::vector3f vector3;
  typedef cml::vector4f vector4;
  typedef cml::matrix44f_c matrix;
  typedef GLfloat Position[3];
  typedef GLuint Face[3];
    
  class GraphicPanel : public wxGLCanvas, public Observer {
    

    
  public:
    wxGLContext* m_context;
    GraphicPanel(wxWindow* parent, int* args);
    virtual ~GraphicPanel();
    
    virtual void update(const Observable& observable, E_ObserveType observeType);
    
    void init(const std::shared_ptr<Graph>& graph,
              const std::shared_ptr<UserDefault>& userDefault,
              const std::shared_ptr<Configuration>& configuration);
    
    void moveTo(int x, int y);
    void refresh();
    void resize();
    int pick(int x, int y);
    void releaseLeft();
    void releaseRight();
    void downRight(int x, int y);
    void renderScene();
    
    //TODO: refactor this
    void setupPanel();
    
    void savePixelData();
    
    //TODO:下のAPIは整理が必要
    //モデルの内容を書き換えて、Refresh呼ぶだけなので、
    //Observerパターンでよい。
    //そんで、前処理はGraphに持っていく
    void moveEye(int delta);
    
  private:
    /**
     * @TODO: remove this
     */
    void changeEye(float);

    int getWidth();
    int getHeight();
    void changeAutoRotation();
    void moveTo2D(int x, int y);
    void moveTo3D(int x, int y);
    void render(float x, float y, float z);
    void changeColor();
    void scaleLayout(float);
    GLuint setUpVBA(float radius, int slices, int stacks);
    int processSelection(int, int);
    void resetLayout();
    void relayout();
    void relayout3D();
    void relayout2D();
    void calculateWorldCo(int x, int y, float depth, double &wx, double &wy, double &wz);
    
  private:
    std::shared_ptr<Graph> _graph;
    std::shared_ptr<Configuration> _configuration;
    std::shared_ptr<UserDefault> _userDefault;
    int glframe = 0;
    long gltimenow = 0;
    wxStopWatch *sw;
 
    //Node Attributes
    float * pos_x, * pos_y, * pos_z;
    vector3 * colors;
    float radius = 0.015f;
    
    //Light Attribute
    float light_z = 200.0f;
    
    //Color Attribute
    vector3 blue;
    vector3 deepblue;
    vector3 green;
    vector3 red;
    vector3 purple;
    float brightness = 1.0f;
    
    //window
    int width;
    int height;
    
    //Option Flags
    bool LOAD_FLAG = false;
    bool AUTO_X_ROTATION = false;
    bool AUTO_Y_ROTATION = false;
    
    //camera
    float default_v = 10.0f;
    float v = 10.0f;
    vector3 eye;
    vector3 target;
    vector3 up;
    vector3 _right;
    float phi = 0;
    float theta = 0;
    float wheel_pos = 50.0;
    
    //Perspective
    float angle = 55.0f;
    float near = 0.1f;
    float far = 1000.0f;
    
    //Mouse Adaption
    bool isPicked = false, isDrag = false, isRightPressed = false;
    int mouse_pos_x = 0;
    int mouse_pos_y = 0;
    int id = -1;
    int highlited_id = -1;
    float pre_x;
    float pre_y;
    float pre_z;
    
    //Buffer
    GLuint vertexBuf, indexBuf;
    GLuint points;
    
    int imgnum = 1;
    
    //View Matrix?
    GLdouble mvMatrix[16];
  };
}
#endif