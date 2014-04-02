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

#include "Graph.h"

namespace agi3d {
  
  typedef cml::vector3f vector3;
  typedef cml::vector4f vector4;
  typedef cml::matrix44f_c matrix;
  typedef GLfloat Position[3];
  typedef GLuint Face[3];
  
  class GraphicPanel : public wxGLCanvas {
    wxGLContext* m_context;
    
  public:
    GraphicPanel(wxWindow* parent, int* args);
    virtual ~GraphicPanel();
    
    void init(const std::shared_ptr<Graph>& graph);
    
    void moveTo(int x, int y);
    void refresh();
    void resize();
    int pick(int x, int y);
    void releaseLeft();
    void releaseRight();
    void downRight(int x, int y);
    void moveEye(int delta);
    void renderScene();
    
    
    GLuint setUpVBA(float radius, int slices, int stacks);
    
    void Render(float x, float y, float z);
    int ProcessSelection(int, int);
    void SetupPanel();
    
    void ResetLayout();
    void changeColor(int);
    
    void UpdateEye(float);
    void UpdateSize(float);
    void UpdateThickness(float);
    
    void ChangeLayoutMode(int);
    
    void ResetIsDrawingNodes();
    void ResetIsDrawingEdges();
    
    float UpdateNodeThreshold_b(float, int);
    float UpdateNodeThreshold_t(float, int);
    
    void UpdateEdgeThreshold_b(float, int);
    void UpdateEdgeThreshold_t(float, int);
    
    void ModifyDelta(float);
    void ScaleLayout(float);
    void ChangeDimension(float);
    
    void SavePixelData();
    
    void DrawEdge();
    void NodeModeChange();
    void SetXRotation(bool);
    void SetYRotation(bool);
    
    void relayout3D();
    void relayout2D();
    
    void CalculateWorldCo(int x, int y, float depth, double &wx, double &wy, double &wz);
    
  private:
    std::shared_ptr<Graph> _graph;
    
    //Variables
    float nodethreshold_b = -1.0f;
    float nodethreshold_t = 100000000000;
    float edgethreshold_b = -1.0f;
    float edgethreshold_t = 100000000000;
    bool * isdrawingNodes;
    bool * isdrawingEdges;
    
    bool * edgeAttribute;
    bool * isNeighbor;
    
    //Layout Mode
    int LayoutMode = 3;
    
    //Node Attributes
    float * pos_x, * pos_y, * pos_z;
    vector3 * colors;
    float radius = 0.015f;
    float size_rate = 1.0f;
    
    //Line Attribute
    float default_linewidth = 1.0f;
    float linewidth = 1.0f;
    
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
    bool DRAW_EDGES = true;
    bool NODE_MODE = true;
    bool AUTO_X_ROTATION = false;
    bool AUTO_Y_ROTATION = false;
    
    //camera
    float default_v = 10.0f;
    float v = 10.0f;
    vector3 eye;
    vector3 target;
    vector3 up;
    vector3 _right;
    float phi = 0, theta = 0;
    float wheel_pos = 50.0;
    
    //Perspective
    float angle = 55.0f, near = 0.1f, far = 1000.0f;
    
    //Mouse Adaption
    bool isPicked = false, isDrag = false, isRightPressed = false;
    int mouse_pos_x = 0;
    int mouse_pos_y = 0;
    int id = -1;
    int highlited_id = -1;
    float pre_x, pre_y, pre_z;
    
    //Buffer
    GLuint vertexBuf, indexBuf;
    GLuint points;
    
    int imgnum = 1;
    
    //Timer for FPS
    wxStopWatch * sw;
    
    //View Matrix?
    GLdouble mvMatrix[16];
    
    //for FPS
    int GLframe = 0;
    long GLtimenow = 0;
    float fps = 0.0f;
    
    int getWidth();
    int getHeight();
    
  };
}
#endif