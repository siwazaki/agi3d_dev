//
//  Graph.h
//  agi3d
//
//  Created by 岩崎 敏 on 2014/04/01.
//  Copyright (c) 2014年 com.nefrock. All rights reserved.
//

#ifndef __agi3d__Graph__
#define __agi3d__Graph__

#include <string>
#include <utility>
#include <vector>
#include <boost/numeric/ublas/vector.hpp>

#include "Enums.h"
#include "Observable.h"

namespace agi3d
{
  
  class GraphicController;
  typedef boost::numeric::ublas::vector<float> fvector;
  
  class Graph : public Observable
  {
    
    friend class GraphicController;
    
  public:
    Graph();
    virtual ~Graph();
    
    //gtters
    const std::string& getName() const;
    int getN() const;
    int getM() const;
    float* getNodeValues() const;
    float* getEdgeValues() const;
    float getMaxNodeValue() const;
    float getMinNodeValue() const;
    float getMaxEdgeValue() const;
    float getMinEdgeValue() const;
    int** getD() const;
    const std::vector<int>* getNeighbor() const;
    const std::vector< std::pair<int, int> >&  getEdges() const;
    const std::vector<std::string>& getLabels() const;
    const std::vector<int>* getEdgeList() const;
    bool * getIsDrawingNodes() const;
    bool * getIsDrawingEdges() const;
    bool * getEdgeAttribute() const ;
    bool * getIsNeighbor() const;
        
    //notify observers
    void changeProjectionFactor(float f, E_Layout layout);
    void changeNodeThreshold(float b, float t);
    void changeEdgeThreshold(float b, float t);
    
    //calcuation functions
    float calcNodeThreshold(float t) const;
    float calcEdgeThreshold(float t) const;

    //initialization
    bool isLoaded() const;
    //FIXME:この関数は消すこと
    void reset();
    bool loadData(const std::string& filePath);
    
    int getNew3DLayout(int, float, float, float, float, float, float);
    void resetLayout3D();
    void updateProjection3D(float);
    void updateScale3D(float);
    void updateDimension3D(float);
    
    int getNew2DLayout(int, float, float, float, float);
    void resetLayout2D();
    void updateProjection2D(float);
    void updateScale2D(float);
    void updateDimension2D(float);
    
    //データ取込
    void loadNodeAttrData(int n, const std::string& graphName);
    void loadEdgeAttrData(int n, const std::string& graphName);
    void loadLabelData(const std::string& graphName);
    void loadMatrixData_t(const char * data, const std::string& graphName);
    void loadMatrixData_b(const char * data, const std::string& graphName);
    void loadLayoutData_b(const char * data, const std::string& graphName);
    void loadLayoutData_t(const char * data, const std::string& graphName);
    
    void calcmdsLayout();
    void calc2DLayout();
    void calc3DLayout();
    
    double getETime();
    
    float posX2D(int i) const;
    float posY2D(int i) const;
    float posX3D(int i) const;
    float posY3D(int i) const;
    float posZ3D(int i) const;
    
  private:
    //graph properties
    bool _isLoaded;
    std::string _name;
    int N;
    int M;
    int ** D;
    std::vector<int> * neighbor;
    std::vector< std::pair<int, int> > edges;        
    std::vector<int> * edgelist;
    std::vector<std::string> labels;
    float * nodevalues;
    float * edgevalues;
    float nodevalue_max, nodevalue_min;
    float edgevalue_min, edgevalue_max;
    bool * isdrawingNodes;
    bool * isdrawingEdges;
    bool * edgeAttribute;
    bool * isNeighbor;
    
    //layout properties
    int maxDimension = 10000;
    int dim;
    float * lambdas;
    float * P;
    float * P_norms;
    float * Layout3D;
    float * E_3D;
    float * E_3D_init;
    float * Layout2D;
    float * E_2D;
    float * E_2D_init;
    float init3D[15];
    float init2D[6];
    float scale = 1.0f;
    float delta = 0.5;
    float memory_status = false;
    int strToInt(const std::string &str);
    std::string IntToString(int num);
    
    //private functions
    void setNodeEdgeValue();
    
  };
  
  inline bool Graph::isLoaded() const
  {
    return _isLoaded;
  }
  
  inline float Graph::posX2D(int i) const
  {
    return Layout2D[i + N * 0];
  }
  
  inline float Graph::posY2D(int i) const
  {
    return Layout2D[i + N * 1];
  }
  
  inline float Graph::posX3D(int i) const
  {
    return Layout3D[i + N * 0];
  }
  inline float Graph::posY3D(int i) const
  {
    return Layout3D[i + N * 1];
  }
  
  inline float Graph::posZ3D(int i) const
  {
    return Layout3D[i + N * 2];
  }
  
  inline int Graph::getN() const
  {
    return Graph::N;
  }
  
  inline int Graph::getM() const
  {
    return M;
  }
  
  inline float Graph::getMaxNodeValue() const
  {
    return nodevalue_max;
  }
  
  inline float Graph::getMinNodeValue() const
  {
    return nodevalue_min;
  }
  
  inline float Graph::getMaxEdgeValue() const
  {
    return edgevalue_max;
  }
  
  inline float Graph::getMinEdgeValue() const
  {
    return edgevalue_min;
  }
  
  inline float* Graph::getNodeValues() const
  {
    return nodevalues;
  }
  
  inline float* Graph::getEdgeValues() const
  {
    return edgevalues;
  }
  
  inline int ** Graph::getD() const
  {
    return D;
  }
  
  inline const std::vector<int>* Graph::getNeighbor() const
  {
    return neighbor;
  }
  
  inline const std::vector< std::pair<int, int> >&  Graph::getEdges() const
  {
    return edges;
  }
  
  inline const std::vector<std::string>& Graph::getLabels() const
  {
    return labels;
  }
  
  inline const std::vector<int>* Graph::getEdgeList() const
  {
    return edgelist;
  }
  
  inline const std::string& Graph::getName() const
  {
    return _name;
  }
  
  inline float Graph::calcNodeThreshold(float t) const
  {
    return (1.0 - t * t)*(nodevalue_min) + (t * t)*(nodevalue_max);
  }

  inline float Graph::calcEdgeThreshold(float t) const
  {
    return (1 - t * t)*(edgevalue_min) + t * t * (edgevalue_max);
  }
  
  inline bool* Graph::getIsDrawingNodes() const
  {
    return isdrawingNodes;
  }
  
  inline bool* Graph::getIsDrawingEdges() const
  {
    return isdrawingEdges;
  }
  
  inline bool* Graph::getEdgeAttribute() const
  {
    return edgeAttribute;
  }
  
  inline bool* Graph::getIsNeighbor() const
  {
    return isNeighbor;
  }
  
}

#endif /* defined(__agi3d__Graph__) */
