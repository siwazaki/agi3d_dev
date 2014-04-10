#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string>
#include <png.h>
#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/glcanvas.h"

#include "GraphicPanel.h"
#include "ControlPanel.h"
#include "Frame.h"
#include "AppDelegete.h"

using namespace std;
using namespace agi3d;

#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))

float getDepth(int x, int y) {
  float z;
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
  return z;
}

GraphicPanel::GraphicPanel(wxWindow* parent, int* args) :
wxGLCanvas(parent,
           wxID_ANY,
           args,
           wxDefaultPosition,
           wxSize(1000, 870),
           wxFULL_REPAINT_ON_RESIZE),
　　　　　 　glframe(0),
           gltimenow(0.0),
           width(1000),
           height(870) {
  
  sw = new wxStopWatch();
  m_context = new wxGLContext(this);
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
  
  //  for (int i = 0; i < N; i++) {
  //    colors[i] = blue;
  //  }
  //
  wxGLCanvas::SetCurrent(*m_context);
  
  //camera setting
  eye.set(0, 0, v);
  target.zero();
  up.set(0.0f, 1.0f, 0.0f);
  _right.set(1, 0, 0);
  
  glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_RGB);
  
  glLineWidth((GLfloat) 1.0f);
  
  glEnable(GL_NORMALIZE);
  
  //culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  //Back Ground
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  //Buffer
  points = setUpVBA(radius, 20, 20);
  
  glShadeModel(GL_SMOOTH);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
}

GraphicPanel::~GraphicPanel() {
  delete m_context;
}

void GraphicPanel::init(const std::shared_ptr<Graph>& graph,
                        const std::shared_ptr<UserDefault>& userDefault,
                        const std::shared_ptr<Configuration>& configuration) {
  _graph = graph;
  _userDefault = userDefault;
  _configuration = configuration;
  blue = vector3(0.0f,0.75f,1.0f);
  deepblue = vector3(0.0f, 0.40f, 0.8f);
  green = vector3(0.0f, 1.0f, 0.0f);
  red = vector3(1.0f, 0.0f, 0.0f);
  purple = vector3(1.0f, 0.0f, 1.0f);
  eye = vector3(0, 0, v);
  target = vector3(0, 0, 0);
  up=vector3(0, 1, 0);
  _right = vector3(1, 0, 0);
}

void GraphicPanel::update(const agi3d::Observable &, E_ObserveType observeType) {
  if(!LOAD_FLAG) { return; }
  switch (observeType) {
    case E_ObserveType::NeedRelayout:
    {
      relayout();
      Refresh();
      break;
    }
    case E_ObserveType::RefreshOnly:
    {
      Refresh();
      break;
    }
    case E_ObserveType::ColorOnly:
    {
      this->changeColor();
      break;
    }
    case E_ObserveType::ChangeAutoRotation:
    {
      this->changeAutoRotation();
      Refresh();
      break;
    }
    default:
      break;
  }
}

void GraphicPanel::changeAutoRotation() {
  switch(_userDefault->rotation())
  {
    case E_Rotation::X:
    {
      AUTO_X_ROTATION = true;
      AUTO_Y_ROTATION = false;;
      break;
    }
      
    case E_Rotation::Y:
    {
      AUTO_X_ROTATION = false;
      AUTO_Y_ROTATION = true;
      break;
    }
      
    case E_Rotation::XY:
    {
      AUTO_X_ROTATION = true;
      AUTO_Y_ROTATION = true;
      break;
    }
      
    case E_Rotation::None:
    {
      AUTO_X_ROTATION = false;
      AUTO_Y_ROTATION = false;
      break;
    }
      
  }
}

void GraphicPanel::refresh() {
  Refresh();
}

//:TODO rename
void GraphicPanel::releaseLeft() {
  if(!LOAD_FLAG) { return; }
  isDrag = false;
  Refresh();
}

//:TODO rename
void GraphicPanel::releaseRight() {
  if(!LOAD_FLAG) { return; }
  isDrag = false;
  isRightPressed = false;
  Refresh();
}

//TODO: rename
void GraphicPanel::downRight(int x, int y) {
  if(!LOAD_FLAG) { return; }
  if (!isDrag) isDrag = true;
  if (!isRightPressed) isRightPressed = true;
  
  mouse_pos_x = x;
  mouse_pos_y = y;
}

//FIXME: Graphモデルのプロパティを書き換えている部分を、Graphに委譲する
int GraphicPanel::pick(int x, int y) {
  if(!LOAD_FLAG) { return -1; }
  int N = _graph->getN();
  int M = _graph->getM();
  float *nodevalues = _graph->getNodeValues();
  float *edgevalues = _graph->getEdgeValues();
  const std::vector<int>*  edgelist = _graph->getEdgeList();
  auto neighbor = _graph->getNeighbor();
  
  float nodethreshold_b = _configuration->getNodeThreshHold_b();
  float nodethreshold_t = _configuration->getNodeThreshHold_t();
  
  float edgethreshold_b = _configuration->getEdgeThreshHold_b();
  float edgethreshold_t = _configuration->getEdgeThreshHold_t();
  
  bool* isdrawingNodes = _graph->getIsDrawingNodes();
  bool* isdrawingEdges = _graph->getIsDrawingEdges();
  bool* isNeighbor = _graph->getIsNeighbor();
  bool* edgeAttribute = _graph->getEdgeAttribute();
  
  
  {
    int p_id = processSelection(x, y);
    
    if (p_id >= 0) {
      //case :: some node is beeing picked
      if (id >= 0) {
        colors[id] = blue;
        for (size_t i = 0; i < neighbor[id].size(); i++) {
          int adj = neighbor[id][i];
          isdrawingNodes[adj] = ((nodevalues[adj] >= nodethreshold_b) && (nodevalues[adj] <= nodethreshold_t));
          colors[adj] = blue;
        }
        for (size_t i = 0; i < edgelist[id].size(); i++) {
          int l = edgelist[id][i];
          isdrawingEdges[l] = ((edgevalues[l] >= edgethreshold_b) && (edgevalues[l] <= edgethreshold_t));
        }
      }
      
      id = p_id;
      pre_x = pos_x[id];
      pre_y = pos_y[id];
      pre_z = pos_z[id];
      
      for (int i = 0; i < N; i++) {
        isNeighbor[i] = false;
      }
      
      colors[id] = purple;
      isNeighbor[id] = true;
      isdrawingNodes[id] = true;
      
      for (size_t i = 0; i < neighbor[id].size(); i++) {
        int adj = neighbor[id][i];
        colors[adj] = deepblue;
        isNeighbor[adj] = true;
        isdrawingNodes[adj] = ((nodevalues[adj] >= nodethreshold_b) && (nodevalues[adj] <= nodethreshold_t));
      }
      
      isPicked = true;
      
      for (int i = 0; i < M; i++) {
        edgeAttribute[i] = true;
      }
      
      for (size_t i = 0; i < edgelist[id].size(); i++) {
        int l = edgelist[id][i];
        edgeAttribute[l] = false;
        isdrawingEdges[l] = ((edgevalues[l] >= edgethreshold_b) && (edgevalues[l] <= edgethreshold_t));
      }
    }
    else {
      //case :: some node is beeing picked
      if (id >= 0) {
        for (size_t i = 0; i < neighbor[id].size(); i++) {
          int adj = neighbor[id][i];
          isdrawingNodes[adj] = ((nodevalues[adj] >= nodethreshold_b) && (nodevalues[adj] <= nodethreshold_t));
        }
        for (size_t i = 0; i < edgelist[id].size(); i++) {
          int l = edgelist[id][i];
          isdrawingEdges[l] = ((edgevalues[l] >= edgethreshold_b) && (edgevalues[l] <= edgethreshold_t));
        }
        for (int i = 0; i < N; i++) {
          colors[i] = blue;
          isdrawingNodes[i] = ((nodevalues[i] >= nodethreshold_b) && (nodevalues[i] <= nodethreshold_t));
        }
        
        for (int i = 0; i < N; i++) {
          if (isdrawingNodes[i]) {
            bool f = false;
            for (size_t j = 0; j < neighbor[i].size(); j++) {
              f |= isdrawingNodes[neighbor[i][j]];
              if (f) break;
            }
            if (!f) isdrawingNodes[i] = false;
          }
        }
        
        id = -1;
      }
      
      isPicked = false;
      highlited_id = -1;
    }
    
    if (!isDrag) isDrag = true;
    
    mouse_pos_x = x;
    mouse_pos_y = y;
    return id;
    
  }
  
}

void GraphicPanel::moveTo2D(int x, int y) {
  auto neighbor = _graph->getNeighbor();
  int dx = x - mouse_pos_x;
  int dy = y - mouse_pos_y;
  if (isRightPressed && isDrag) {
    vector3 vec = target - eye;
    _right = cml::cross(up, vec);
    _right.normalize();
    target += (dx * _right + dy * up)*0.008;
  } else if (isDrag && isPicked) {
    vector3 pos(pos_x[id], pos_y[id], pos_z[id]);
    vector3 vec = target - eye;
    vector3 eye2pos = pos - eye;
    float dot = cml::dot(vec, eye2pos);
    _right = cml::cross(up, vec);
    _right.normalize();
    float dist = dot / vec.length();
    
    //Proper Rate is ???
    float rate_x = (float) -dx * dist / ((float) width * 0.90);
    float rate_y = (float) -dy * dist / ((float) height * 0.90);
    
    vector3 delta = rate_x * _right + rate_y * up;
    
    float new_x = pos_x[id] + delta[0];
    float new_y = pos_y[id] + delta[1];
    
    int a = _graph->getNew2DLayout(id, pre_x, pre_y, new_x, new_y);
    
    if (a == 1) {
      relayout2D();
      pre_x = pos_x[id];
      pre_y = pos_y[id];
      pos_x[id] = new_x;
      pos_y[id] = new_y;
      pos_z[id] = radius;
      for (size_t i = 0; i < neighbor[id].size(); i++) {
        pos_z[neighbor[id][i]] = radius;
      }
    }
  }
  Refresh();
  mouse_pos_x = x;
  mouse_pos_y = y;
}

void GraphicPanel::moveTo3D(int x, int y) {
  int dx = x - mouse_pos_x;
  int dy = y - mouse_pos_y;
  if (isRightPressed && isDrag) {
    vector3 vec = target - eye;
    _right = cml::cross(up, vec);
    _right.normalize();
    target += (dx * _right + dy * up)*0.008;
  } else if ((isDrag && !isPicked)) {
    theta -= 0.005 * dy;
    phi += 0.005 * dx;
  } else if (isDrag && isPicked) {
    vector3 pos(pos_x[id], pos_y[id], pos_z[id]);
    vector3 vec = target - eye;
    vector3 eye2pos = pos - eye;
    float dot = cml::dot(vec, eye2pos);
    _right = cml::cross(up, vec);
    _right.normalize();
    float dist = dot / vec.length();
    
    //Proper Rate is ???
    float rate_x = (float) -dx * dist / ((float) width * 0.90);
    float rate_y = (float) -dy * dist / ((float) height * 0.90);
    
    vector3 delta = rate_x * _right + rate_y * up;
    
    float new_x = pos_x[id] + delta[0];
    float new_y = pos_y[id] + delta[1];
    float new_z = pos_z[id] + delta[2];
    
    mouse_pos_x = x;
    mouse_pos_y = y;
    int a = _graph->getNew3DLayout(id, pre_x, pre_y, pre_z, new_x, new_y, new_z);
    
    if (a == 1) {
      relayout3D();
      pre_x = pos_x[id];
      pre_y = pos_y[id];
      pre_z = pos_z[id];
      pos_x[id] = new_x;
      pos_y[id] = new_y;
      pos_z[id] = new_z;
    }
  }
  Refresh();
  mouse_pos_x = x;
  mouse_pos_y = y;
  
}

void GraphicPanel::moveTo(int x, int y) {
  if(!LOAD_FLAG) { return; }
  switch (_userDefault->layout()) {
    case E_Layout::D2:
      moveTo2D(x, y);
      break;
    case E_Layout::D3:
      moveTo3D(x,y);
    default:
      break;
  }
}

void GraphicPanel::resize() {
  int topleft_x = 0, topleft_y = 0, bottomrigth_x = getWidth(), bottomrigth_y = getHeight();
  
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //white i
  glClearDepth(1.0f);
  
  glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  GLfloat fAspect = (float) (bottomrigth_x - topleft_x) / (float) (bottomrigth_y - topleft_y);
  gluPerspective(angle, fAspect, near, far);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat light_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat light_position[] = {0.0f, 0.0f, light_z, 1.0f};
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  width = bottomrigth_x - topleft_x;
  height = bottomrigth_y - topleft_y;
  
  Refresh();
}

void GraphicPanel::moveEye(int delta) {
  wheel_pos -= (float) delta / 4.0;
  if (wheel_pos > 200) wheel_pos = min(wheel_pos, 200.0f);
  wheel_pos = max(wheel_pos, 5.0f);
  float z = wheel_pos / 50.0;
  v = default_v*z;
  eye.set(eye[0] * v, eye[1] * v, eye[2] * v);
  Refresh();
}

void GraphicPanel::renderScene() {
  if(!LOAD_FLAG) return;
  float size_rate = _userDefault->nodeSize();
  int N = _graph->getN();
  int M = _graph->getM();
  float nodevalue_max = _graph->getMaxNodeValue();
  float edgevalue_max = _graph->getMaxEdgeValue();
  float *nodevalues = _graph->getNodeValues();
  float *edgevalues = _graph->getEdgeValues();
  const std::vector< std::pair<int, int> >& edges = _graph->getEdges();
  const std::vector<int>*  edgelist = _graph->getEdgeList();
  bool* isdrawingNodes = _graph->getIsDrawingNodes();
  bool* isdrawingEdges = _graph->getIsDrawingEdges();
  bool* isNeighbor = _graph->getIsNeighbor();
  bool* edgeAttribute = _graph->getEdgeAttribute();
  float linewidth = _userDefault->edgeThickness();
  //for FPS
  wxGLCanvas::SetCurrent(*m_context);
  wxPaintDC(this);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  switch(_userDefault->layout()) {
    case E_Layout::D3 :
    {
      glEnable(GL_LIGHTING);
      break;
    }
    case E_Layout::D2 :
    {
      glDisable(GL_LIGHTING);
      break;
    default:
      glEnable(GL_LIGHTING);
      
    }
  }
  
  //ModelView
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  //Set View
  {
    matrix xRot, yRot, zRot, view;
    switch (_userDefault->layout()) {
      case E_Layout::D3: {
        if (AUTO_X_ROTATION) {
          theta += 0.01f;
          if (theta > 3.141593f * 2) theta = 0;
        }
        if (AUTO_Y_ROTATION) {
          phi += 0.01f;
          if (phi > 3.141593f * 2) phi = 0;
        }
        
        cml::matrix_rotation_world_x(xRot, -theta);
        cml::matrix_rotation_world_y(yRot, phi);
        up.set(0.0f, 1.0f, 0.0f);
        vector4 _up(up[0], up[1], up[2], 1);
        _up = yRot * xRot*_up;
        up.set(_up[0], _up[1], _up[2]);
        up.normalize();
        
        float camera_y = v * (float) sin(theta);
        float camera_xz = v * (float) cos(theta);
        float camera_x = camera_xz * (float) sin(phi);
        float camera_z = camera_xz * (float) cos(phi);
        eye.set(camera_x, camera_y, camera_z);
        
        cml::matrix_look_at_RH(view, eye, target, up);
        glLoadMatrixf(view.data());
        
        if (id != -1 && brightness >= 0.5f) {
          brightness -= 0.02f;
        }
        
        if (id == -1 && brightness <= 1.0f) {
          brightness += 0.02f;
        }
        
        break;
      }
      case E_Layout::D2: {
        cml::matrix_rotation_world_z(zRot, 0.0f);
        up.set(0.0f, 1.0f, 0.0f);
        vector4 _up(up[0], up[1], up[2], 1);
        _up = zRot*_up;
        up.set(_up[0], _up[1], _up[2]);
        up.normalize();
        
        eye.set(0, 0, v);
        
        cml::matrix_look_at_RH(view, eye, target, up);
        glLoadMatrixf(view.data());
        brightness = 1.0;
        break;
      }
      default:
        break;
    }
  }
  
  glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
  
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_MULTISAMPLE);
  
  // Initialize the names stack
  glInitNames();
  glPushName(N);
  
  //Draw Nodes
  {
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    GLsizei stride = sizeof (Position)*2;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
    glVertexPointer(3, GL_FLOAT, stride, NULL);
    glNormalPointer(GL_FLOAT, stride, BUFFER_OFFSET(sizeof (Position)));
    
    for (int i = 0; i < N; i++) {
      glPushMatrix();
      if (isdrawingNodes[i]) {
        if (isNeighbor[i]) {
          glColor4f((GLfloat) colors[i][0], (GLfloat) colors[i][1], (GLfloat) colors[i][2], 1.0f);
        } else {
          glColor4f((GLfloat) colors[i][0], (GLfloat) colors[i][1], (GLfloat) colors[i][2], brightness);
        }
        
        glTranslatef((GLfloat) pos_x[i], (GLfloat) pos_y[i], (GLfloat) pos_z[i]);
        
        if (_userDefault->isDrawNode()) {
          float cb = (float) 3.0 * pow((double) nodevalues[i] / nodevalue_max, 1.0 / 3.0);
          glScalef((GLfloat) cb*size_rate, (GLfloat) cb*size_rate, (GLfloat) cb * size_rate);
        } else {
          glScalef((GLfloat) size_rate, (GLfloat) size_rate, (GLfloat) size_rate);
        }
        
        glLoadName(i);
        //Using GPU Vertex
        glDrawElements(GL_TRIANGLES, points, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        //Not Using GPU Vertex
        //glutSolidSphere(radius,15,15);
      }
      glPopMatrix();
    }
  }
  
  glDisable(GL_LIGHTING);
  
  //Draw Edges
  {
    if (_userDefault->isDrawEdge()) {
      if (id != -1) {
        for (int i = 0; i < M; i++) {
          int from = edges[i].first, to = edges[i].second;
          if (isdrawingEdges[i] && isdrawingNodes[from] && isdrawingNodes[to]) {
            if (edgeAttribute[i]) {
              glColor4f(0.5f, 0.5f, 0.5f, brightness * 0.7f);
              glLineWidth((GLfloat) linewidth * edgevalues[i] / edgevalue_max);
            } else {
              glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
              glLineWidth((GLfloat) linewidth * 2 * edgevalues[i] / edgevalue_max);
            }
            glBegin(GL_LINES);
            glVertex3f(pos_x[from], pos_y[from], pos_z[from]);
            glVertex3f(pos_x[to], pos_y[to], pos_z[to]);
            glEnd();
          }
        }
      } else {
        glColor4f(0.5f, 0.5f, 0.5f, brightness * 0.7f);
        for (int i = 0; i < M; i++) {
          int from = edges[i].first, to = edges[i].second;
          if (isdrawingEdges[i] && isdrawingNodes[from] && isdrawingNodes[to]) {
            glLineWidth((GLfloat) linewidth * edgevalues[i] / edgevalue_max);
            glBegin(GL_LINES);
            glVertex3f(pos_x[from], pos_y[from], pos_z[from]);
            glVertex3f(pos_x[to], pos_y[to], pos_z[to]);
            glEnd();
          }
        }
      }
      
    } else {
      if (id != -1) {
        glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
        for (size_t i = 0; i < edgelist[id].size(); i++) {
          int from = edges[edgelist[id][i]].first, to = edges[edgelist[id][i]].second;
          glLineWidth((GLfloat) linewidth * 2 * edgevalues[i] / edgevalue_max);
          glBegin(GL_LINES);
          glVertex3f(pos_x[from], pos_y[from], pos_z[from]);
          glVertex3f(pos_x[to], pos_y[to], pos_z[to]);
          glEnd();
        }
      }
    }
  }
  glPopMatrix();
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  
  glframe++;
  gltimenow = sw->Time();
  if ((gltimenow > 1000) && LOAD_FLAG) {
    float fps = glframe * 1000.0 / gltimenow;
    _configuration->changeFPS(fps);
    sw->Start(0);
    glframe = 0;
  }
  
  SwapBuffers();
  
  glDisable(GL_POINT_SMOOTH);
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_POLYGON_SMOOTH);
  
  glDisable(GL_DEPTH_TEST);
}

GLuint GraphicPanel::setUpVBA(float radius, int slices, int stacks) {
  glGenBuffers(1, &vertexBuf);
  glGenBuffers(1, &indexBuf);
  GLuint vertices = (slices + 1) * (stacks + 1);
  GLuint faces = slices * stacks * 2;
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof (Position) * vertices * 2, NULL, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (Face) * faces, NULL, GL_STATIC_DRAW);
  
  Position *position = (Position *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  Face *face = (Face *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
  
  for (int j = 0; j <= stacks; ++j) {
    float ph = 3.141593f * (float) j / (float) stacks;
    float y = radius * cosf(ph);
    float r = radius * sinf(ph);
    for (int i = 0; i <= slices; ++i) {
      float th = 2.0f * 3.141593f * (float) i / (float) slices;
      float x = r * cosf(th), z = r * sinf(th);
      (*position)[0] = x;
      (*position)[1] = y;
      (*position)[2] = z;
      ++position;
      (*position)[0] = x;
      (*position)[1] = y;
      (*position)[2] = z;
      ++position;
    }
  }
  
  for (int j = 0; j < stacks; ++j) {
    for (int i = 0; i < slices; ++i) {
      int count = (slices + 1) * j + i;
      (*face)[0] = count;
      (*face)[1] = count + 1;
      (*face)[2] = count + slices + 2;
      ++face;
      (*face)[0] = count;
      (*face)[1] = count + slices + 2;
      (*face)[2] = count + slices + 1;
      ++face;
    }
  }
  
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return faces * 3;
}

void GraphicPanel::relayout() {
  if(_userDefault->layout() == E_Layout::D2)
  {
    relayout2D();
  }else {
    relayout3D();
  }
}

void GraphicPanel::relayout3D() {
  int N = _graph->getN();
  for (int i = 0; i < N; i++) {
    pos_x[i] = _graph->posX3D(i);
    pos_y[i] = _graph->posY3D(i);
    pos_z[i] = _graph->posZ3D(i);
  }
}

void GraphicPanel::relayout2D() {
  int N = _graph->getN();
  for (int i = 0; i < N; i++) {
    pos_x[i] = _graph->posX2D(i);
    pos_y[i] = _graph->posY2D(i);
    pos_z[i] = 0;
  }
}

void GraphicPanel::setupPanel() {
  _graph->reset();
  //Free Memory
  {
    if(LOAD_FLAG) {
      delete[] pos_x;
      delete[] pos_y;
      delete[] pos_z;
    }
  }
  
  
  AUTO_X_ROTATION = false;
  AUTO_Y_ROTATION = false;
  
  id = -1;
  
  int N = _graph->getN();
  pos_x = new float[N];
  pos_y = new float[N];
  pos_z = new float[N];
  colors = new vector3[N];
  float end = 0;
  switch(_userDefault->layout()) {
    case E_Layout::D3:
    {
      for (int i = 0; i < N; i++) {
        pos_x[i] = _graph->posX3D(i);
        pos_y[i] = _graph->posY3D(i);
        pos_z[i] = _graph->posZ3D(i);
        colors[i] = blue;
        end = max(end, sqrt(pos_x[i] * pos_x[i] + pos_y[i] * pos_y[i] + pos_z[i] * pos_z[i]));
      }
      angle = 55;
      
      break;
    }
    case E_Layout::D2: {
      for (int i = 0; i < N; i++) {
        pos_x[i] = _graph->posX2D(i);
        pos_y[i] = _graph->posY2D(i);
        pos_z[i] = 0;
        colors[i] = blue;
        end = max(end, sqrt(pos_x[i] * pos_x[i] + pos_y[i] * pos_y[i] + pos_z[i] * pos_z[i]));
      }
      angle = 45;
      break;
    default:
      break;
    }
  }
  
  LOAD_FLAG = true;
  
  //Params
  default_v = end * 1.05f / tan((double) (3.141593f * angle / 360.0));
  v = default_v;
  wheel_pos = 50.0;
  
  eye.set(0, 0, v);
  target.set(0, 0, 0);
  up.set(0, 1, 0);
  _right.set(1, 0, 0);
  
  imgnum = 1;
  
  Refresh();
}

void GraphicPanel::resetLayout() {
  
  AUTO_X_ROTATION = false;
  AUTO_Y_ROTATION = false;
  
  id = -1;
  
  phi = 0;
  theta = 0;
  
  isPicked = false, isDrag = false;
  mouse_pos_x = 0;
  mouse_pos_y = 0;
  
  float end = 0;
  
  int N = _graph->getN();
  int M = _graph->getM();
  bool* isdrawingNodes = _graph->getIsDrawingNodes();
  bool* isdrawingEdges = _graph->getIsDrawingEdges();
  bool* isNeighbor = _graph->getIsNeighbor();
  bool* edgeAttribute = _graph->getEdgeAttribute();
  
  switch(_userDefault->layout()) {
    case E_Layout::D3: {
      _graph->resetLayout3D();
      for (int i = 0; i < N; i++) {
        pos_x[i] = _graph->posX3D(i);
        pos_y[i] = _graph->posY3D(i);
        pos_z[i] = _graph->posZ3D(i);
        colors[i] = blue;
        isNeighbor[i] = false;
        isdrawingNodes[i] = true;
        end = max(end, sqrt(pos_x[i] * pos_x[i] + pos_y[i] * pos_y[i] + pos_z[i] * pos_z[i]));
      }
      radius = 0.03f;
      angle = 55;
      break;
    }
    case E_Layout::D2: {
      _graph->resetLayout2D();
      for (int i = 0; i < N; i++) {
        pos_x[i] = _graph->posX2D(i);
        pos_y[i] = _graph->posY2D(i);
        pos_z[i] = 0;
        colors[i] = blue;
        isNeighbor[i] = false;
        isdrawingNodes[i] = true;
        end = max(end, sqrt(pos_x[i] * pos_x[i] + pos_y[i] * pos_y[i] + pos_z[i] * pos_z[i]));
      }
      radius = 0.01f;
      angle = 45;
      break;
    }
    default: {
      break;
    }
  }
  
  for (int i = 0; i < M; i++) {
    isdrawingEdges[i] = true;
    edgeAttribute[i] = true;
  }
  
  LOAD_FLAG = true;
  
  //Params
  default_v = end * 1.05f / tan((double) (3.141593f * angle / 360.0));
  v = default_v;
  wheel_pos = 50.0;
  
  eye.set(0, 0, v);
  target.set(0, 0, 0);
  up.set(0, 1, 0);
  _right.set(1, 0, 0);
  
  resize();
  
  Refresh();
}

int GraphicPanel::getWidth() {
  return GetSize().x;
}

int GraphicPanel::getHeight() {
  return GetSize().y;
}

void GraphicPanel::render(float x, float y, float z) {
  int N = _graph->getN();
  float nodevalue_max = _graph->getMaxNodeValue();
  float *nodevalues = _graph->getNodeValues();
  bool* isdrawingNodes = _graph->getIsDrawingNodes();
  float size_rate = _userDefault->nodeSize();
  wxGLCanvas::SetCurrent(*m_context);
  wxPaintDC(this);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  
  //ModelView
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  //Set View
  {
    matrix xRot, yRot, zRot, view;
    switch(_userDefault->layout()) {
        
      case E_Layout::D3: {
        if (AUTO_X_ROTATION) {
          theta += 0.01f;
          if (theta > 3.141593f * 2) theta = 0;
        }
        if (AUTO_Y_ROTATION) {
          phi += 0.01f;
          if (phi > 3.141593f * 2) phi = 0;
        }
        cml::matrix_rotation_world_x(xRot, -theta);
        cml::matrix_rotation_world_y(yRot, phi);
        up.set(0.0f, 1.0f, 0.0f);
        vector4 _up(up[0], up[1], up[2], 1);
        _up = yRot * xRot*_up;
        up.set(_up[0], _up[1], _up[2]);
        up.normalize();
        
        float camera_y = v * (float) sin(theta);
        float camera_xz = v * (float) cos(theta);
        float camera_x = camera_xz * (float) sin(phi);
        float camera_z = camera_xz * (float) cos(phi);
        eye.set(camera_x, camera_y, camera_z);
        
        cml::matrix_look_at_RH(view, eye, target, up);
        glLoadMatrixf(view.data());
        break;
      }
      case E_Layout::D2: {
        cml::matrix_rotation_world_z(zRot, 0.0f);
        up.set(0.0f, 1.0f, 0.0f);
        vector4 _up(up[0], up[1], up[2], 1);
        _up = zRot*_up;
        up.set(_up[0], _up[1], _up[2]);
        up.normalize();
        
        eye.set(0, 0, v);
        
        cml::matrix_look_at_RH(view, eye, target, up);
        glLoadMatrixf(view.data());
        break;
      }
    }
    // Initialize the names stack
    glInitNames();
    glPushName(N);
  }
  
  //Draw Nodes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  GLsizei stride = sizeof (Position)*2;
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
  glVertexPointer(3, GL_FLOAT, stride, NULL);
  glNormalPointer(GL_FLOAT, stride, BUFFER_OFFSET(sizeof (Position)));
  
  for (int i = 0; i < N; i++) {
    glPushMatrix();
    float dist = (x - pos_x[i])*(x - pos_x[i]) + (y - pos_y[i])*(y - pos_y[i]) + (z - pos_z[i])*(z - pos_z[i]);
    if (isdrawingNodes[i] && (dist < 0.1)) {
      glTranslatef((GLfloat) pos_x[i], (GLfloat) pos_y[i], (GLfloat) pos_z[i]);
      if (_userDefault->isDrawNode()) {
        float cb = (float) 3.0 * pow((double) nodevalues[i] / nodevalue_max, 1.0 / 3.0);
        glScalef((GLfloat) cb, (GLfloat) cb, (GLfloat) cb);
      } else
        glScalef((GLfloat) size_rate, (GLfloat) size_rate, (GLfloat) size_rate);
      glLoadName(i);
      glDrawElements(GL_TRIANGLES, points, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    }
    glPopMatrix();
  }
  glPopMatrix();
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisable(GL_DEPTH_TEST);
}

void GraphicPanel::changeColor() {
  int m = _configuration->getSelectedId();
  int highlited_id = _configuration->getPrevSelectedId();
  colors[m] = red;
  if (highlited_id != -1) {
    colors[highlited_id] = deepblue;
  }
}

void GraphicPanel::changeEye(float _v) {
  v = default_v*_v;
  eye.set(eye[0] * v, eye[1] * v, eye[2] * v);
  Refresh();
}

void GraphicPanel::savePixelData() {
  //@TODO 一時的な実装
  const std::string& graphName = _graph->getName();
  string name = graphName + "img" + boost::lexical_cast<std::string>(imgnum) + ".png";
  imgnum++;
  
  png_bytep raw1D;
  png_bytepp raw2D;
  
  FILE *fp = fopen(name.c_str(), "wb");
  png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop ip = png_create_info_struct(pp);
  
  png_init_io(pp, fp);
  png_set_IHDR(pp, ip, width, height,
               8,
               PNG_COLOR_TYPE_RGBA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  
  raw1D = (png_bytep) malloc(height * png_get_rowbytes(pp, ip));
  raw2D = (png_bytepp) malloc(height * sizeof (png_bytep));
  
  for (int i = 0; i < height; i++) {
    raw2D[i] = &raw1D[i * png_get_rowbytes(pp, ip)];
  }
  
  glReadBuffer(GL_FRONT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*) raw1D);
  for (int i = 0; i < height / 2; i++) {
    png_bytep swp = raw2D[i];
    raw2D[i] = raw2D[height - i - 1];
    raw2D[height - i - 1] = swp;
  }
  
  png_write_info(pp, ip);
  png_write_image(pp, raw2D);
  png_write_end(pp, ip);
  
  png_destroy_write_struct(&pp, &ip);
  fclose(fp);
}

void GraphicPanel::calculateWorldCo(int x, int y, float depth, double &wx, double &wy, double &wz) {
  GLdouble pjMatrix[16];
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);
  gluUnProject((double) x, (double) viewport[3] - y, depth, mvMatrix, pjMatrix, viewport, &wx, &wy, &wz);
}

#define BUFFER_LENGTH 64

int GraphicPanel::processSelection(int xPos, int yPos) {
  int N = _graph->getN();
  GLfloat fAspect;
  static GLuint selectBuff[BUFFER_LENGTH];
  GLint hits, viewport[4];
  
  float depth = getDepth(xPos, yPos);
  double _x, _y, _z;
  calculateWorldCo(xPos, yPos, depth, _x, _y, _z);
  
  glSelectBuffer(BUFFER_LENGTH, selectBuff);
  glGetIntegerv(GL_VIEWPORT, viewport);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  
  glRenderMode(GL_SELECT);
  glLoadIdentity();
  gluPickMatrix(xPos, viewport[3] - yPos + viewport[1], 1, 1, viewport);
  fAspect = (float) viewport[2] / (float) viewport[3];
  gluPerspective(angle, fAspect, near, far);
  
  render((float) _x, (float) _y, (float) _z);
  
  hits = glRenderMode(GL_RENDER);
  
  int res = -1, tmp = -1;
  
  if (hits >= 1) {
    for (int i = 3; i < BUFFER_LENGTH; i += 3) {
      tmp = selectBuff[i];
      if (tmp >= 0 && tmp < N) {
        res = tmp;
        break;
      }
    }
  }
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  
  return res;
}