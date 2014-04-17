#ifndef __agi3d__GraphicController__
#define __agi3d__GraphicController__

#include <memory>
#include "GraphicPanel.h"
#include "Graph.h"
#include "Configuration.h"

namespace agi3d {
  class AppDelegete;
  
  class GraphicController
  {
    friend class AppDelegete;
  public:
    GraphicController();
    virtual ~GraphicController();
    void initEventHandlers();
        
  private:
    GraphicPanel *_graphicPanel;
    std::shared_ptr<Graph> _graph;
    std::shared_ptr<Configuration> _configuration;

    void mouseMoved(wxMouseEvent& event);
    void mouseLeftDown(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseRightDown(wxMouseEvent& event);
    void mouseRightReleased(wxMouseEvent& event);
    void mouseScroll(wxMouseEvent& event);
    
    void resized(wxSizeEvent& evt);
    void renderScene(wxPaintEvent& evt);
    void onIdle(wxEvent& evt);
    
  };
  
}

#endif /* defined(__agi3d__GraphicController__) */