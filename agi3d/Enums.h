//
//  Enums.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/03/31.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__Enums__
#define __agi3d__Enums__

namespace agi3d {
  
  typedef enum {
    D2,
    D3
  } E_Layout;
  
  typedef enum {
    None = 0,
    X = 1,
    Y = 2,
    XY = 3,
  } E_Rotation;
  
  typedef enum {
    RefreshOnly = 0,
    NeedReLayout = 2,
    
  } E_ObserveType;
}

#endif /* defined(__agi3d__Enums__) */
