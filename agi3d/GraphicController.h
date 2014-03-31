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

namespace agi3d {
  
  class GraphicController
  {
  public:
    GraphicController();
    virtual ~GraphicController();
    
  private:
    std::shared_ptr<GraphicPanel> _graphicPanel;
        
  };
  
}

#endif /* defined(__agi3d__GraphicController__) */
