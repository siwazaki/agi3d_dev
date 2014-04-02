//
//  GraphicController.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__GraphicController__
#define __agi3d__GraphicController__

#include <memory>
#include "GraphicPanel.h"
#include "Graph.h"

namespace agi3d {
  
  class GraphicController
  {
  public:
    GraphicController(const std::shared_ptr<Graph>& graph);
    virtual ~GraphicController();
        
  private:
    GraphicPanel *_graphicPanel;
    std::shared_ptr<Graph> _graph;
    
  };
  
}

#endif /* defined(__agi3d__GraphicController__) */
