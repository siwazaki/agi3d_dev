#include "UserDefault.h"

using namespace agi3d;

UserDefault::UserDefault() : _layout(E_Layout::D3), _rotation(E_Rotation::None), _nodeSize(1.0f), _edgeThickness(1.0f),_isDrawNode(true), _isDrawEdge(true) {
}

UserDefault::~UserDefault() {}

void UserDefault::changeNodeSize(float nodeSize) {
  _nodeSize = nodeSize;
  this->notify(E_ObserveType::RefreshOnly);
}

void UserDefault::changeEdgeThickness(float edgeThickness) {
  _edgeThickness = edgeThickness;
  this->notify(E_ObserveType::RefreshOnly);
}

void UserDefault::changeIsDrawEdge(bool isDraw) {
  _isDrawEdge = isDraw;
  this->notify(E_ObserveType::RefreshOnly);
}

void UserDefault::changeIsDrawNode(bool isDraw) {
  _isDrawNode = isDraw;
  this->notify(E_ObserveType::RefreshOnly);
}

void UserDefault::changeLayout(E_Layout layout) {
  _layout = layout;
  this->notify(E_ObserveType::NeedRelayout);
}

void UserDefault::changeRotation(E_Rotation rotation) {
  _rotation = rotation;
  this->notify(E_ObserveType::ChangeAutoRotation);
}