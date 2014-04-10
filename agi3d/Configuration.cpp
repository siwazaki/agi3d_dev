#include "Configuration.h"

using namespace agi3d;

Configuration::Configuration() :
_label(),
_neiborLabels(),
_nodethreshold_t(10000.0f),
_nodethreshold_b(-1.0f),
_edgethreshold_t(10000.0f),
_edgethreshold_b(-1.0f),
_fps(0.0f),
_labelMap(),
_selectedId(-1),
_prevSelectedId(-1),
_nodeThresholdAttrID(0),
_edgeThresholdAttrID(0)
{ 
  
}

Configuration::~Configuration() {
  
}

void Configuration::changeNodeThreshholdAtrr(int id)
{
  _nodeThresholdAttrID = id;
  this->notify(E_ObserveType::RefreshOnly);
}

void Configuration::changeEdgeThreshholdAttr(int id)
{
  _edgeThresholdAttrID = id;
  this->notify(E_ObserveType::RefreshOnly);
}

void Configuration::changeColor(int selectedNodeId)
{
  this->setSelectedId(selectedNodeId);
  this->notify(E_ObserveType::ColorOnly);
}

void Configuration::changeTarget(const std::string& label, const std::vector<std::string>& neiborLabels, const std::vector<int>& neiborIds) {
  this->_labelMap.clear();
  this->_label = label;
  this->_neiborLabels.clear();
  typedef std::vector<std::string>::const_iterator vi;
  int key = 0;
  for(vi itr = neiborLabels.begin(), end = neiborLabels.end(); itr != end; ++itr){
    _labelMap.insert(std::map<int,int>::value_type(key, neiborIds[key]));
    _neiborLabels.push_back(*itr);
    ++key;
  }
  this->notify(E_ObserveType::ConfigurationOnly);
}