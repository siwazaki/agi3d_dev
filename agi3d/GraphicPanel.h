#ifndef _glpane_
#define _glpane_

#include <memory>
#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "Graph.h"

namespace agi3d {
  
  class GraphicPanel : public wxGLCanvas {
    wxGLContext* m_context;
    
  public:
    GraphicPanel(wxWindow* parent, int* args);
    virtual ~GraphicPanel();
    
    void init(const std::shared_ptr<Graph>& graph);
    
    int getWidth();
    int getHeight();
    
    GLuint setUpVBA(float radius, int slices, int stacks);
    
    // events
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftDown(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseRightDown(wxMouseEvent& event);
    void mouseRightReleased(wxMouseEvent& event);
    void mouseScroll(wxMouseEvent& event);
    
    void resized(wxSizeEvent& evt);
    void RenderScene(wxPaintEvent& evt);
    void OnIdle(wxIdleEvent& evt);
    
    void resize();
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
    
  private:
    std::shared_ptr<Graph> _graph;
    
    DECLARE_EVENT_TABLE()
  };
}
#endif