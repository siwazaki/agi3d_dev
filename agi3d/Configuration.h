#ifndef __agi3d__Configuration__
#define __agi3d__Configuration__

#include <string>
#include <map>

#include "Observable.h"

namespace agi3d {
  
  class ConfigurationController;
  /**
   * ControlPanelで変更可能なデータを保持するモデル。
   * @TODO: Jsonにシリアライズ
   */
  class Configuration : public Observable
  {
    
    friend class ConfigurationController;
    
  public:
    Configuration();
    virtual ~Configuration();
    const std::string& graphName() const;
    int getNodeThreshHold_t() const;
    int getNodeThreshHold_b() const;
    int getEdgeThreshHold_t() const;
    int getEdgeThreshHold_b() const;
    const std::string& getNodeLabel() const;
    const std::vector<std::string>& getNeiborLabels() const;
    int getNodeIdinLabels(int ithInTheLabel);
    float getFPS() const;
    int getPickedId() const;
    void setPickedId(int i);
    int getSelectedId() const;
    int getPrevSelectedId() const;
    void setSelectedId(int i);
    
    void changeTarget(const std::string& label, const std::vector<std::string>& neiborLabels, const std::vector<int>& neiborIds);
    void changeFPS(float fps);
    void changeColor(int selectedNodeId);
    void changeNodeThreshholdAtrr(int id);
    void changeEdgeThreshholdAttr(int id);
    
  private:
    std::string _graphName;
    std::string _label;
    std::vector<std::string> _neiborLabels;
    
    float _nodethreshold_t;
    float _nodethreshold_b;
    float _edgethreshold_t;
    float _edgethreshold_b;
    float _fps;
    //TODO:要らないかもしれないので削除
    int _pickid;
    
    std::map<int, int> _labelMap;
    int _selectedId;
    int _prevSelectedId;
    
    int _nodeThresholdAttrID;
    int _edgeThresholdAttrID;
    
  };
  
  inline const std::string& Configuration::graphName() const
  {
    return _graphName;
  }
  
  inline int Configuration::getNodeThreshHold_t() const
  {
    return _nodethreshold_t;
  }
  
  inline int Configuration::getNodeThreshHold_b() const
  {
    return _nodethreshold_b;
  }
  
  inline int Configuration::getEdgeThreshHold_t() const
  {
    return _edgethreshold_t;
  }
  
  inline int Configuration::getEdgeThreshHold_b() const
  {
    return _edgethreshold_b;
  }
  
  inline const std::string& Configuration::getNodeLabel() const
  {
    return _label;
  }
  
  inline const std::vector<std::string>& Configuration::getNeiborLabels() const
  {
    return _neiborLabels;
  }
  
  inline float Configuration::getFPS() const
  {
    return _fps;
  }
  
  inline void Configuration::changeFPS(float fps)
  {
    _fps = fps;
    this->notify(E_ObserveType::ConfigurationFPSOnly);
  }
  
  inline int Configuration::getNodeIdinLabels(int ithInTheLabel)
  {
    return _labelMap[ithInTheLabel];
  }
  
  inline int Configuration::getSelectedId() const
  {
    return _selectedId;
  }
  
  inline int Configuration::getPrevSelectedId() const
  {
    return _prevSelectedId;
  }
  
  inline void Configuration::setSelectedId(int i)
  {
    _prevSelectedId = _selectedId;
    _selectedId = i;
  }
  
  inline int Configuration::getPickedId() const {
    return _pickid;
  }
  
  inline void Configuration::setPickedId(int i) {
    _pickid=i;
  }
  
}

#endif /* defined(__agi3d__Configuration__) */