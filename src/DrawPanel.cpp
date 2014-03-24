#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <string>
#include <cml/cml.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/glcanvas.h"
#include "DrawPanel.h"
#include "SubPanel.h"
#include <png.h>
#include "wx/stopwatch.h"
#include "MainFrame.h"

typedef cml::vector3f vector3;
typedef cml::vector4f vector4;
typedef cml::matrix44f_c matrix;
using namespace std;

//import functions from calcLayout.cpp
int getNew3DLayout(int, float, float, float, float, float, float);
void resetLayout3D();
void UpdateProjection3D(float);
void UpdateScale3D(float);
void UpdateDimension3D(float);

int getNew2DLayout(int, float, float, float, float);
void resetLayout2D();
void UpdateProjection2D(float);
void UpdateScale2D(float);
void UpdateDimension2D(float);

double getETime();

string IntToString(int);

//Params about Graph
extern int N;
extern int M;
extern vector<int> *neighbor;
extern vector< pair<int, int> >  edges;
extern vector<int> *edgelist;
extern string graphName;

extern float * nodevalues;
extern float * edgevalues;
extern float nodevalue_max;
extern float nodevalue_min;
extern float edgevalue_max;
extern float edgevalue_min;

extern float * Layout3D;
extern float * Layout2D;

//Variables
static float nodethreshold_b = -1.0f;
static float nodethreshold_t = 100000000000;
static float edgethreshold_b = -1.0f;
static float edgethreshold_t = 100000000000;

static bool * isdrawingNodes;
static bool * isdrawingEdges;

static bool * edgeAttribute;
static bool * isNeighbor;

//Layout Mode
static int LayoutMode = 3;

//Node Attributes
static float * pos_x, * pos_y, * pos_z;
static vector3 * colors;
static float radius = 0.015f;
static float size_rate = 1.0f;

//Line Attribute
float default_linewidth = 1.0f;
static float linewidth = 1.0f;

//Light Attribute
static float light_z = 200.0f;

//Color Attribute
vector3 blue(0.0f, 0.75f, 1.0f);
vector3 deepblue(0.0f, 0.40f, 0.8f);
vector3 green(0.0f, 1.0f, 0.0f);
vector3 red(1.0f,0.0f,0.0f);
vector3 purple(1.0f,0.0f,1.0f);
static float brightness = 1.0f;

//window
static int width = 1000; static int height = 870;

//Option Flags
static bool LOAD_FLAG = false;
static bool DRAW_EDGES = true;
static bool NODE_MODE = true;
static bool AUTO_X_ROTATION = false;
static bool AUTO_Y_ROTATION = false;

//camera
static float default_v = 10.0f;
static float v = 10.0f;
static vector3 eye(0, 0, v);
static vector3 target(0, 0, 0);
static vector3 up(0, 1, 0);
static vector3 _right(1, 0, 0);
static float phi = 0, theta = 0;
static float wheel_pos = 50.0;

//Perspective
static float angle = 55.0f, near = 0.1f, far = 1000.0f;

//Mouse Adaption
static bool isPicked = false, isDrag = false, isRightPressed = false;
static int mouse_pos_x = 0; static int mouse_pos_y = 0;
static int id = -1;
static int highlited_id = -1;
static float pre_x, pre_y, pre_z;

//Buffer
#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))
static GLuint vertexBuf, indexBuf;
static GLuint points;
typedef GLfloat Position[3];
typedef GLuint Face[3];

static int imgnum = 1;

//Timer for FPS
static wxStopWatch * sw;

//SubPanel
static SubPanel * sbp;

//View Matrix?
static GLdouble mvMatrix[16];

//for FPS
static int GLframe = 0;
static long GLtimenow = 0;
static float fps = 0.0f;

//wx Macros
BEGIN_EVENT_TABLE(DrawPanel, wxGLCanvas)
EVT_MOTION(DrawPanel::mouseMoved)
EVT_LEFT_DOWN(DrawPanel::mouseLeftDown)
EVT_LEFT_UP(DrawPanel::mouseLeftReleased)
EVT_RIGHT_DOWN(DrawPanel::mouseRightDown)
EVT_RIGHT_UP(DrawPanel::mouseRightReleased)
EVT_SIZE(DrawPanel::resized)
EVT_PAINT(DrawPanel::RenderScene)
EVT_IDLE(DrawPanel::OnIdle)
EVT_MOUSEWHEEL(DrawPanel::mouseScroll)
END_EVENT_TABLE()

DrawPanel::~DrawPanel(){
    delete m_context;
}

GLuint setUpVBA(float radius, int slices, int stacks){
    glGenBuffers(1, &vertexBuf);
    glGenBuffers(1, &indexBuf);
    GLuint vertices = (slices + 1) * (stacks + 1);
    GLuint faces = slices * stacks * 2;

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof (Position) * vertices * 2, NULL, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (Face) * faces, NULL, GL_STATIC_DRAW);

    Position *position = (Position *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    Face *face = (Face *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int j = 0; j <= stacks; ++j) {
        float ph = 3.141593f * (float)j / (float)stacks;
        float y = radius*cosf(ph);
        float r = radius*sinf(ph);
        for (int i = 0; i <= slices; ++i) {
            float th = 2.0f * 3.141593f * (float)i / (float)slices;
            float x = r * cosf(th), z = r * sinf(th);
            (*position)[0] = x; (*position)[1] = y; (*position)[2] = z;
            ++position;
            (*position)[0] = x; (*position)[1] = y; (*position)[2] = z;
            ++position;
        }
    }

    for (int j = 0; j < stacks; ++j) {
        for (int i = 0; i < slices; ++i) {
            int count = (slices + 1) * j + i;
            (*face)[0] = count; (*face)[1] = count + 1; (*face)[2] = count + slices + 2;
            ++face;
            (*face)[0] = count; (*face)[1] = count + slices + 2; (*face)[2] = count + slices + 1;
            ++face;
        }
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return faces * 3;
}

void relayout3D(){
    for(int i = 0; i < N; i++){
        pos_x[i] = Layout3D[i+N*0];
        pos_y[i] = Layout3D[i+N*1];
        pos_z[i] = Layout3D[i+N*2];
    }
}

void relayout2D(){
    for(int i = 0; i < N; i++){
        pos_x[i] = Layout2D[i+N*0];
        pos_y[i] = Layout2D[i+N*1];
        pos_z[i] = 0;
    }
}

void DrawPanel::SetupPanel(){
    //Free Memory
    {
        delete[] pos_x; delete[] pos_y; delete[] pos_z;
        delete[] isNeighbor; delete[] isdrawingNodes;
        delete[] edgeAttribute; delete[] isdrawingEdges;
    }

    linewidth = default_linewidth;

    DRAW_EDGES = true;
    NODE_MODE = true;
    AUTO_X_ROTATION = false;
    AUTO_Y_ROTATION = false;

    id = -1;

    pos_x = new float[N];
    pos_y = new float[N];
    pos_z = new float[N];
    colors = new vector3[N];

    isNeighbor = new bool[N];
    isdrawingNodes = new bool[N];

    edgeAttribute = new bool[M];
    isdrawingEdges = new bool[M];

    for(int i = 0; i < M; i++){
        edgeAttribute[i] = true;
        isdrawingEdges[i] = true;
    }

    float end = 0;

    if(LayoutMode == 3){
        for(int i = 0; i < N; i++){
            pos_x[i] = Layout3D[i+N*0];
            pos_y[i] = Layout3D[i+N*1];
            pos_z[i] = Layout3D[i+N*2];
            colors[i] = blue;
            isNeighbor[i] = false;
            isdrawingNodes[i] = true;
            end = max(end, sqrt(pos_x[i]*pos_x[i]+pos_y[i]*pos_y[i]+pos_z[i]*pos_z[i]));
        }
        angle = 55;
    }
    else if(LayoutMode == 2){
        for(int i = 0; i < N; i++){
            pos_x[i] = Layout2D[i+N*0];
            pos_y[i] = Layout2D[i+N*1];
            pos_z[i] = 0;
            colors[i] = blue;
            isNeighbor[i] = false;
            isdrawingNodes[i] = true;
            end = max(end, sqrt(pos_x[i]*pos_x[i]+pos_y[i]*pos_y[i]+pos_z[i]*pos_z[i]));
        }
        angle = 45;
    }

    LOAD_FLAG = true;

    //Params
    default_v = end*1.05f / tan((double)(3.141593f*angle/360.0));
    v = default_v;
    wheel_pos = 50.0;

    eye.set(0, 0, v);
    target.set(0, 0, 0);
    up.set(0, 1, 0);
    _right.set(1,0,0);

    imgnum = 1;

    Refresh();
}

void DrawPanel::ResetLayout(){
    linewidth = default_linewidth;

    DRAW_EDGES = true;
    NODE_MODE = true;
    AUTO_X_ROTATION = false;
    AUTO_Y_ROTATION = false;

    id = -1;
    size_rate = 1.0f;

    phi = 0;
    theta = 0;

    isPicked = false, isDrag = false;
    mouse_pos_x = 0; mouse_pos_y = 0;

    nodethreshold_b = -1;
    edgethreshold_b = -1;

    nodethreshold_t = 100000000000;
    edgethreshold_t = 100000000000;

    float end = 0;

    if(LayoutMode == 3){
        resetLayout3D();
        for(int i = 0; i < N; i++){
            pos_x[i] = Layout3D[i+N*0];
            pos_y[i] = Layout3D[i+N*1];
            pos_z[i] = Layout3D[i+N*2];
            colors[i] = blue;
            isNeighbor[i] = false;
            isdrawingNodes[i] = true;
            end = max(end, sqrt(pos_x[i]*pos_x[i]+pos_y[i]*pos_y[i]+pos_z[i]*pos_z[i]));
        }
        radius = 0.03f;
        angle = 55;
    }
    else if(LayoutMode == 2){
        resetLayout2D();
        for(int i = 0; i < N; i++){
            pos_x[i] = Layout2D[i+N*0];
            pos_y[i] = Layout2D[i+N*1];
            pos_z[i] = 0;
            colors[i] = blue;
            isNeighbor[i] = false;
            isdrawingNodes[i] = true;
            end = max(end, sqrt(pos_x[i]*pos_x[i]+pos_y[i]*pos_y[i]+pos_z[i]*pos_z[i]));
        }
        radius = 0.01f;
        angle = 45;
    }

    for(int i = 0; i < M; i++){
        isdrawingEdges[i] = true;
        edgeAttribute[i] = true;
    }

    LOAD_FLAG = true;

    //Params
    default_v = end*1.05f / tan((double)(3.141593f*angle/360.0));
    v = default_v;
    wheel_pos = 50.0;

    eye.set(0, 0, v);
    target.set(0, 0, 0);
    up.set(0, 1, 0);
    _right.set(1,0,0);

    resize();

    Refresh();
}

DrawPanel::DrawPanel(wxWindow* parent, int* args) :
wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxSize(width,height), wxFULL_REPAINT_ON_RESIZE){

    m_context = new wxGLContext(this);

    sw = new wxStopWatch();

    wxWindow * p =  this->GetParent();
    MainFrame * mf = (MainFrame *)(p->GetParent());
    sbp = (SubPanel *)mf->GetSubPanel();

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    for(int i = 0; i < N; i++){
        colors[i] = blue;
    }

    wxGLCanvas::SetCurrent(*m_context);

    //camera setting
    eye.set(0,0,v);
    target.zero();
    up.set(0.0f,1.0f,0.0f);
    _right.set(1,0,0);

    glutInitDisplayMode(GLUT_MULTISAMPLE|GLUT_DOUBLE|GLUT_RGB);

    glLineWidth((GLfloat)linewidth);

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

void DrawPanel::resized(wxSizeEvent& evt){
    int topleft_x = 0 , topleft_y = 0, bottomrigth_x = getWidth(), bottomrigth_y = getHeight();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //white Background
    glClearDepth(1.0f);

    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat fAspect = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
    gluPerspective(angle, fAspect, near, far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat light_specular[] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat light_ambient[] = {0.2f,0.2f,0.2f,1.0f};
    GLfloat light_diffuse[] = {0.7f,0.7f,0.7f,1.0f};
    GLfloat light_position[] = {0.0f, 0.0f, light_z, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    width = bottomrigth_x - topleft_x; height = bottomrigth_y - topleft_y;
    Refresh();
}

void DrawPanel::resize(){
    int topleft_x = 0 , topleft_y = 0, bottomrigth_x = getWidth(), bottomrigth_y = getHeight();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //white i
    glClearDepth(1.0f);

    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat fAspect = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
    gluPerspective(angle, fAspect, near, far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat light_specular[] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat light_ambient[] = {0.2f,0.2f,0.2f,1.0f};
    GLfloat light_diffuse[] = {0.7f,0.7f,0.7f,1.0f};
    GLfloat light_position[] = {0.0f, 0.0f, light_z, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0,GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    width = bottomrigth_x - topleft_x; height = bottomrigth_y - topleft_y;

    Refresh();
}

int DrawPanel::getWidth(){
    return GetSize().x;
}

int DrawPanel::getHeight(){
    return GetSize().y;
}

void DrawPanel::RenderScene(wxPaintEvent& evt){
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    if(LayoutMode==3){
        glEnable(GL_LIGHTING);
    }
    else if(LayoutMode==2){
        glDisable(GL_LIGHTING);
    }

    //ModelView
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //Set View
    {
        matrix xRot, yRot, zRot, view;
        if(LayoutMode == 3){
            if(AUTO_X_ROTATION){
                theta += 0.01f;
                if(theta > 3.141593f*2) theta = 0;
            }
            if(AUTO_Y_ROTATION){
                phi += 0.01f;
                if(phi > 3.141593f*2) phi = 0;
            }

            cml::matrix_rotation_world_x(xRot,-theta);
            cml::matrix_rotation_world_y(yRot,phi);
            up.set(0.0f,1.0f,0.0f);
            vector4 _up(up[0],up[1],up[2],1);
            _up = yRot*xRot*_up;
            up.set(_up[0],_up[1],_up[2]);
            up.normalize();

            float camera_y = v * (float)sin(theta);
            float camera_xz = v * (float)cos(theta);
            float camera_x =  camera_xz * (float)sin(phi);
            float camera_z =  camera_xz * (float)cos(phi);
            eye.set(camera_x, camera_y, camera_z);

            cml::matrix_look_at_RH(view, eye, target, up);
            glLoadMatrixf(view.data());

            if(id != -1 && brightness >= 0.5f){
                brightness -= 0.02f;
            }

            if(id == -1 && brightness <= 1.0f){
                brightness += 0.02f;
            }

        }
        if(LayoutMode == 2){
            cml::matrix_rotation_world_z(zRot,0.0f);
            up.set(0.0f,1.0f,0.0f);
            vector4 _up(up[0],up[1],up[2],1);
            _up = zRot*_up;
            up.set(_up[0],_up[1],_up[2]);
            up.normalize();

            eye.set(0, 0, v);

            cml::matrix_look_at_RH(view, eye, target, up);
            glLoadMatrixf(view.data());
            brightness = 1.0;
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

        GLsizei stride = sizeof(Position)*2;
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
        glVertexPointer(3, GL_FLOAT, stride, NULL);
        glNormalPointer(GL_FLOAT, stride, BUFFER_OFFSET(sizeof(Position)));

        for (int i = 0; i < N; i++) {
            glPushMatrix();
            if( isdrawingNodes[i] ){
                if(isNeighbor[i]){
                    glColor4f((GLfloat)colors[i][0], (GLfloat)colors[i][1], (GLfloat)colors[i][2], 1.0f);
                }
                else{
                    glColor4f((GLfloat)colors[i][0], (GLfloat)colors[i][1], (GLfloat)colors[i][2], brightness);
                }

                glTranslatef((GLfloat)pos_x[i], (GLfloat)pos_y[i], (GLfloat)pos_z[i]);

                if(NODE_MODE){
                    float cb = (float)3.0*pow((double)nodevalues[i]/nodevalue_max, 1.0/3.0);
                    glScalef((GLfloat)cb*size_rate, (GLfloat)cb*size_rate, (GLfloat)cb*size_rate);
                }
                else{
                    glScalef((GLfloat)size_rate, (GLfloat)size_rate, (GLfloat)size_rate);
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
        if(DRAW_EDGES){
            if(id != -1){
                for(int i = 0; i < M; i ++){
                    int from = edges[i].first, to = edges[i].second;
                    if(isdrawingEdges[i] && isdrawingNodes[from] && isdrawingNodes[to]){
                        if(edgeAttribute[i]){
                            glColor4f(0.5f, 0.5f, 0.5f, brightness*0.7f);
                            glLineWidth((GLfloat)linewidth*edgevalues[i]/edgevalue_max);
                        }
                        else{
                            glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
                            glLineWidth((GLfloat)linewidth*2*edgevalues[i]/edgevalue_max);
                        }
                        glBegin(GL_LINES);
                        glVertex3f(pos_x[from], pos_y[from], pos_z[from]);
                        glVertex3f(pos_x[to], pos_y[to], pos_z[to]);
                        glEnd();
                    }
                }
            }
            else{
                glColor4f(0.5f, 0.5f, 0.5f, brightness*0.7f);
                for(int i = 0; i < M; i ++){
                    int from = edges[i].first, to = edges[i].second;
                    if(isdrawingEdges[i] && isdrawingNodes[from] && isdrawingNodes[to]){
                        glLineWidth((GLfloat)linewidth*edgevalues[i]/edgevalue_max);
                        glBegin(GL_LINES);
                        glVertex3f(pos_x[from], pos_y[from], pos_z[from]);
                        glVertex3f(pos_x[to], pos_y[to], pos_z[to]);
                        glEnd();
                    }
                }
            }

        }
        else{
            if(id != -1){
                glColor4f(0.1f, 0.1f, 0.1f, 0.7f);
                for(int i = 0; i < edgelist[id].size(); i++){
                    int from = edges[edgelist[id][i]].first, to = edges[edgelist[id][i]].second;
                    glLineWidth((GLfloat)linewidth*2*edgevalues[i]/edgevalue_max);
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

    GLframe++;
    GLtimenow = sw->Time();
    if ( (GLtimenow  > 1000) && LOAD_FLAG ){
        float _fps = GLframe * 1000.0 / GLtimenow;
        sbp->SetFPS(_fps);
        sw->Start(0);
        GLframe = 0;
    }

    SwapBuffers();

    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);

    glDisable(GL_DEPTH_TEST);
}

void DrawPanel::Render(float x, float y, float z){
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
        if(LayoutMode == 3){
            if(AUTO_X_ROTATION){
                theta += 0.01f;
                if(theta > 3.141593f*2) theta = 0;
            }
            if(AUTO_Y_ROTATION){
                phi += 0.01f;
                if(phi > 3.141593f*2) phi = 0;
            }
            cml::matrix_rotation_world_x(xRot,-theta);
            cml::matrix_rotation_world_y(yRot,phi);
            up.set(0.0f,1.0f,0.0f);
            vector4 _up(up[0],up[1],up[2],1);
            _up = yRot*xRot*_up;
            up.set(_up[0],_up[1],_up[2]);
            up.normalize();

            float camera_y = v * (float)sin(theta);
            float camera_xz = v * (float)cos(theta);
            float camera_x =  camera_xz * (float)sin(phi);
            float camera_z =  camera_xz * (float)cos(phi);
            eye.set(camera_x, camera_y, camera_z);

            cml::matrix_look_at_RH(view, eye, target, up);
            glLoadMatrixf(view.data());
        }
        else if(LayoutMode == 2){
            cml::matrix_rotation_world_z(zRot,0.0f);
            up.set(0.0f,1.0f,0.0f);
            vector4 _up(up[0],up[1],up[2],1);
            _up = zRot*_up;
            up.set(_up[0],_up[1],_up[2]);
            up.normalize();

            eye.set(0, 0, v);

            cml::matrix_look_at_RH(view, eye, target, up);
            glLoadMatrixf(view.data());
        }
    }
    // Initialize the names stack
    glInitNames();
    glPushName(N);

    //Draw Nodes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    GLsizei stride = sizeof(Position)*2;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
    glVertexPointer(3, GL_FLOAT, stride, NULL);
    glNormalPointer(GL_FLOAT, stride, BUFFER_OFFSET(sizeof(Position)));

    for (int i = 0; i < N; i++) {
        glPushMatrix();
        float dist = (x - pos_x[i])*(x - pos_x[i]) + (y - pos_y[i])*(y - pos_y[i]) + (z - pos_z[i])*(z - pos_z[i]);
        if( isdrawingNodes[i] && (dist < 0.1)){
            glTranslatef((GLfloat)pos_x[i], (GLfloat)pos_y[i], (GLfloat)pos_z[i]);
            if(NODE_MODE){
                float cb = (float)3.0*pow((double)nodevalues[i]/nodevalue_max, 1.0/3.0);
                glScalef((GLfloat)cb, (GLfloat)cb, (GLfloat)cb);
            }
            else
                glScalef((GLfloat)size_rate, (GLfloat)size_rate, (GLfloat)size_rate);
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

void DrawPanel::changeColor(int m){
    if(id != -1){
        colors[neighbor[id][m]] = red;
        if(highlited_id != -1){
            colors[neighbor[id][highlited_id]] = deepblue;
        }
        highlited_id = m;
    }
}

void DrawPanel::UpdateEye(float _v){
    v = default_v*_v;
    eye.set(eye[0]*v, eye[1]*v, eye[2]*v);
    Refresh();
}

void DrawPanel::UpdateSize(float _s){
    size_rate = _s;
    Refresh();
}

void DrawPanel::UpdateThickness(float t){
    linewidth = t;
    Refresh();
}

void DrawPanel::ChangeLayoutMode(int mode){
    LayoutMode = mode;
    ResetLayout();
}

void DrawPanel::ResetIsDrawingNodes(){
    nodethreshold_b = nodevalue_min;
    nodethreshold_t = nodevalue_max;
    for(int i = 0; i < N; i++){
        isdrawingNodes[i] = true;
    }
}

void DrawPanel::ResetIsDrawingEdges(){
    edgethreshold_b = edgevalue_min;
    edgethreshold_t = edgevalue_max;
    for(int i = 0; i < M; i++){
       isdrawingEdges[i] = true;
   }
}

float DrawPanel::UpdateNodeThreshold_b(float t, int attrID){
    nodethreshold_b = (1.0-t*t)*(nodevalue_min) + (t*t)*(nodevalue_max);
    for(int i = 0; i < N; i++){
        isdrawingNodes[i] = ((nodevalues[i] >= nodethreshold_b) && (nodevalues[i] <= nodethreshold_t));
    }
    for(int i = 0; i < N; i++){
        if(isdrawingNodes[i]){
            bool f = false;
            for(int j = 0; j < neighbor[i].size(); j++){
                f |= isdrawingNodes[neighbor[i][j]];
                if(f) break;
            }
            if(!f) isdrawingNodes[i] = false;
        }
    }
    Refresh();
    return nodethreshold_b;
}

float DrawPanel::UpdateNodeThreshold_t(float t, int attrID){
    nodethreshold_t = (1.0-t*t)*(nodevalue_min) + (t*t)*(nodevalue_max);
    for(int i = 0; i < N; i++){
        isdrawingNodes[i] = ((nodevalues[i] >= nodethreshold_b) && (nodevalues[i] <= nodethreshold_t));
    }
    for(int i = 0; i < N; i++){
        if(isdrawingNodes[i]){
            bool f = false;
            for(int j = 0; j < neighbor[i].size(); j++){
                f |= isdrawingNodes[neighbor[i][j]];
                if(f) break;
            }
            if(!f) isdrawingNodes[i] = false;
        }
    }
    Refresh();
    return nodethreshold_t;
}

void DrawPanel::UpdateEdgeThreshold_b(float t, int attrID){
    edgethreshold_b = (1-t*t)*(edgevalue_min) + t*t*(edgevalue_max);
    for(int i = 0; i < M; i++){
        isdrawingEdges[i] = ((edgevalues[i] >= edgethreshold_b) && (edgevalues[i] <= edgethreshold_t));
    }
    Refresh();
}

void DrawPanel::UpdateEdgeThreshold_t(float t, int attrID){
    edgethreshold_t = (1-t*t)*(edgevalue_min) + t*t*(edgevalue_max);
    for(int i = 0; i < M; i++){
        isdrawingEdges[i] = ((edgevalues[i] >= edgethreshold_b) && (edgevalues[i] <= edgethreshold_t));
    }
    Refresh();
}

void DrawPanel::ScaleLayout(float f){
    if(LayoutMode==3){
        UpdateScale3D(f);
        relayout3D();
    }
    else if(LayoutMode==2){
        UpdateScale2D(f);
        relayout2D();
    }
    Refresh();
}

void DrawPanel::ModifyDelta(float f){
    if(LayoutMode==3){
        UpdateProjection3D(f);
        relayout3D();
    }
    else if(LayoutMode==2){
        UpdateProjection2D(f);
        relayout2D();
    }
    Refresh();
}

void DrawPanel::ChangeDimension(float f){
    if(LayoutMode == 3){
        UpdateDimension3D(f);
        relayout3D();
    }
    else if(LayoutMode == 2){
        UpdateDimension2D(f);
        relayout2D();
    }
}

void DrawPanel::DrawEdge(){
    DRAW_EDGES = !DRAW_EDGES;
    Refresh();
}

void DrawPanel::NodeModeChange(){
    NODE_MODE = !NODE_MODE;
    Refresh();
}

void DrawPanel::SetXRotation(bool value){
    AUTO_X_ROTATION = value;
    Refresh();
}

void DrawPanel::SetYRotation(bool value){
    AUTO_Y_ROTATION = value;
    Refresh();
}

void DrawPanel::SavePixelData(){
    string name = graphName + "img" + IntToString(imgnum) + ".png";
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

    raw1D = (png_bytep)malloc(height * png_get_rowbytes(pp, ip));
    raw2D = (png_bytepp)malloc(height * sizeof(png_bytep));

    for (int i = 0; i < height; i++){
        raw2D[i] = &raw1D[i*png_get_rowbytes(pp, ip)];
    }

    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)raw1D);
    for (int i = 0; i < height/ 2; i++){
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

float getDepth(int x, int y){
    float z;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glReadPixels(x,viewport[3]-y,1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    return z;
}

void CalculateWorldCo(int x, int y, float depth, double &wx, double &wy,double &wz){
    GLdouble pjMatrix[16];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, pjMatrix);
    gluUnProject((double)x,(double)viewport[3]-y,depth, mvMatrix,pjMatrix,viewport,&wx,&wy,&wz);
}

#define BUFFER_LENGTH 64
int DrawPanel::ProcessSelection(int xPos, int yPos){
    GLfloat fAspect;
    static GLuint selectBuff[BUFFER_LENGTH];
    GLint hits, viewport[4];

    float depth = getDepth(xPos, yPos);
    double _x, _y, _z;
    CalculateWorldCo(xPos, yPos, depth, _x, _y, _z);

    glSelectBuffer(BUFFER_LENGTH, selectBuff);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glRenderMode(GL_SELECT);
    glLoadIdentity();
    gluPickMatrix(xPos, viewport[3] - yPos + viewport[1], 1,1, viewport);
    fAspect = (float)viewport[2] / (float)viewport[3];
    gluPerspective(angle, fAspect, near, far);

    Render((float)_x, (float)_y, (float)_z);

    hits = glRenderMode(GL_RENDER);

    int res = -1, tmp = -1;

    if(hits >= 1){
        for(int i = 3; i < BUFFER_LENGTH; i+= 3){
            tmp = selectBuff[i];
            if(tmp >= 0 && tmp < N){
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

void DrawPanel::OnIdle(wxIdleEvent&){
    Refresh();
}

void DrawPanel::mouseMoved(wxMouseEvent& event) {
    if(LOAD_FLAG){
        int x = event.GetX(), y = event.GetY();
        int dx = x - mouse_pos_x;
        int dy = y - mouse_pos_y;
        if(LayoutMode == 3){
            if(isRightPressed && isDrag){
                vector3 vec = target - eye;
                _right = cml::cross(up,vec);
                _right.normalize();
                target += (dx*_right + dy*up)*0.008 ;
            }
            else if((isDrag && !isPicked)) {
                theta -= 0.005*dy;
                phi += 0.005*dx;
            }
            else if(isDrag && isPicked){
                vector3 pos(pos_x[id],pos_y[id],pos_z[id]);
                vector3 vec = target - eye;
                vector3 eye2pos = pos - eye;
                float dot = cml::dot(vec,eye2pos);
                _right = cml::cross(up,vec);
                _right.normalize();
                float dist = dot/vec.length();

                //Proper Rate is ???
                float rate_x = (float)-dx * dist / ((float)width*0.90);
                float rate_y =  (float)-dy * dist / ((float)height*0.90);

                vector3 delta = rate_x * _right + rate_y * up;

                float new_x = pos_x[id] + delta[0];
                float new_y = pos_y[id] + delta[1];
                float new_z = pos_z[id] + delta[2];

                mouse_pos_x = x; mouse_pos_y = y;
                int a = getNew3DLayout(id, pre_x, pre_y, pre_z, new_x, new_y, new_z);

                if(a == 1){
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
        }
        else if(LayoutMode == 2){
            if(isRightPressed && isDrag){
                vector3 vec = target - eye;
                _right = cml::cross(up,vec);
                _right.normalize();
                target += (dx*_right + dy*up)*0.008 ;
            }
            else if(isDrag && isPicked){
                vector3 pos(pos_x[id],pos_y[id],pos_z[id]);
                vector3 vec = target - eye;
                vector3 eye2pos = pos - eye;
                float dot = cml::dot(vec,eye2pos);
                _right = cml::cross(up,vec);
                _right.normalize();
                float dist = dot/vec.length();

                //Proper Rate is ???
                float rate_x = (float)-dx * dist / ((float)width*0.90);
                float rate_y =  (float)-dy * dist / ((float)height*0.90);

                vector3 delta = rate_x * _right + rate_y * up;

                float new_x = pos_x[id] + delta[0];
                float new_y = pos_y[id] + delta[1];

                int a = getNew2DLayout(id, pre_x, pre_y, new_x, new_y);

                if(a == 1){
                    relayout2D();
                    pre_x = pos_x[id];
                    pre_y = pos_y[id];
                    pos_x[id] = new_x;
                    pos_y[id] = new_y;
                    pos_z[id] = radius;
                    for(int i = 0; i < neighbor[id].size(); i++){
                        pos_z[neighbor[id][i]] = radius;
                    }
                }
            }
            Refresh();
        }
        mouse_pos_x = x; mouse_pos_y = y;
    }
}

void DrawPanel::mouseLeftDown(wxMouseEvent& event) {
    if(LOAD_FLAG){
        int x = event.GetX(), y = event.GetY();
        {
            int p_id = ProcessSelection(x, y);

            if(p_id >= 0){
                //case :: some node is beeing picked
                if(id >= 0){
                    colors[id] = blue;
                    for(int i = 0; i < neighbor[id].size(); i++){
                        int adj = neighbor[id][i];
                        isdrawingNodes[adj] = ((nodevalues[adj] >= nodethreshold_b) && (nodevalues[adj] <= nodethreshold_t));
                        colors[adj] = blue;
                    }
                    for(int i = 0; i < edgelist[id].size(); i++){
                        int l = edgelist[id][i];
                        isdrawingEdges[l] = ((edgevalues[l] >= edgethreshold_b) && (edgevalues[l] <= edgethreshold_t));
                    }
                }

                id = p_id;
                pre_x = pos_x[id];
                pre_y = pos_y[id];
                pre_z = pos_z[id];

                for(int i = 0; i < N; i++){
                    isNeighbor[i] = false;
                }

                colors[id] = purple;
                isNeighbor[id] = true;
                isdrawingNodes[id] = true;

                for(int i = 0; i < neighbor[id].size(); i++){
                    int adj = neighbor[id][i];
                    colors[adj] = deepblue;
                    isNeighbor[adj] = true;
                    isdrawingNodes[adj] = ((nodevalues[adj] >= nodethreshold_b) && (nodevalues[adj] <= nodethreshold_t));
                }

                isPicked = true;

                for(int i = 0; i < M; i++){
                    edgeAttribute[i] = true;
                }

                for(int i = 0; i < edgelist[id].size(); i++){
                    int l = edgelist[id][i];
                    edgeAttribute[l] = false;
                    isdrawingEdges[l] = ((edgevalues[l] >= edgethreshold_b) && (edgevalues[l] <= edgethreshold_t));
                }

                sbp->setTarget(id);
            }

            else{
                //case :: some node is beeing picked
                if(id >= 0){
                    for(int i = 0; i < neighbor[id].size(); i++){
                        int adj = neighbor[id][i];
                        isdrawingNodes[adj] = ((nodevalues[adj] >= nodethreshold_b) && (nodevalues[adj] <= nodethreshold_t));
                    }
                    for(int i = 0; i < edgelist[id].size(); i++){
                        int l = edgelist[id][i];
                        isdrawingEdges[l] =  ((edgevalues[l] >= edgethreshold_b) && (edgevalues[l] <= edgethreshold_t));
                    }
                    for(int i = 0; i < N; i++){
                        colors[i] = blue;
                        isdrawingNodes[i] = ((nodevalues[i] >= nodethreshold_b) && (nodevalues[i] <= nodethreshold_t));
                    }

                    for(int i = 0; i < N; i++){
                        if(isdrawingNodes[i]){
                            bool f = false;
                            for(int j = 0; j < neighbor[i].size(); j++){
                                f |= isdrawingNodes[neighbor[i][j]];
                                if(f) break;
                            }
                            if(!f) isdrawingNodes[i] = false;
                        }
                    }

                    id = -1;
                    sbp->setTarget(id);
                }

                isPicked = false;
                highlited_id = -1;
            }

            if(!isDrag) isDrag = true;

            mouse_pos_x = x;
            mouse_pos_y = y;
        }
    }
}

void DrawPanel::mouseLeftReleased(wxMouseEvent& event) {
    if(LOAD_FLAG){
        isDrag = false;
        Refresh();
    }
}

void DrawPanel::mouseRightDown(wxMouseEvent& event) {
    if(LOAD_FLAG){
        int x = event.GetX(), y = event.GetY();

        if(!isDrag) isDrag = true;
        if(!isRightPressed) isRightPressed = true;

        mouse_pos_x = x;
        mouse_pos_y = y;
    }
}

void DrawPanel::mouseRightReleased(wxMouseEvent& event) {
    if(LOAD_FLAG){
        isDrag = false;
        isRightPressed = false;
        Refresh();
    }
}

void DrawPanel::mouseScroll(wxMouseEvent& event){
    int delta = event.GetWheelRotation();
    if(delta != 0){
        wheel_pos -= (float)delta/4.0;
        if(wheel_pos > 200) wheel_pos = min(wheel_pos ,200.0f);
        wheel_pos = max(wheel_pos ,5.0f);
        float z = wheel_pos / 50.0;
        v = default_v*z;
        eye.set(eye[0]*v, eye[1]*v, eye[2]*v);
        Refresh();
    }
}