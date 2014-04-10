#ifndef __agi3d__Setting__
#define __agi3d__Setting__

#include <iostream>
#include "Enums.h"
#include "Observable.h"

namespace agi3d {
  
  class MenuBarController;
  /**
   * MenuBarで変更可能なデータを保持するモデル。
   * MenuBarController以外はプロパティ変更不可。
   */
  class UserDefault : public Observable
  {
    
    friend class MenuBarController;
    
  public:
    UserDefault();
    virtual ~UserDefault();
    E_Layout layout() const;
    E_Rotation rotation() const;
    int nodeSize() const;
    int edgeThickness() const;
    float scale() const;
    float delta() const;
    int dimention() const ;
    bool isDrawNode() const;
    bool isDrawEdge() const;    
    void changeNodeSize(float nodeSize);
    void changeEdgeThickness(float thickness);
    void changeIsDrawEdge(bool isDraw);
    void changeIsDrawNode(bool isDraw);
    void changeLayout(E_Layout layout);
    void changeRotation(E_Rotation rotation);
    
  private:
    E_Layout _layout;
    E_Rotation _rotation;
    int _nodeSize;
    int _edgeThickness; 
    bool _isDrawNode;
    bool _isDrawEdge;
    
  };
  
  inline E_Layout UserDefault::layout() const {
    return _layout;
  }
  
  inline E_Rotation UserDefault::rotation() const {
    return _rotation;
  }
  
  inline int UserDefault::nodeSize() const {
    return _nodeSize;
  }
  
  inline int UserDefault::edgeThickness() const {
    return _edgeThickness;
  }
  
  inline bool UserDefault::isDrawNode() const {
    return _isDrawNode;
  }
  
  inline bool UserDefault::isDrawEdge() const {
    return _isDrawEdge;
  }
  
}

#endif /* defined(__agi3d__Setting__) */