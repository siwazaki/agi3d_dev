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
    RefreshOnly,
    NeedRelayout,
    ColorOnly,
    ChangeAutoRotation,
    ConfigurationOnly,
    ConfigurationFPSOnly,
  } E_ObserveType;
}

#endif /* defined(__agi3d__Enums__) */