
#include <functional>

#include "GraphicController.h"
#include "AppDelegete.h"

using namespace agi3d;
using namespace std;

GraphicController::GraphicController(const std::shared_ptr<Graph>& graph) {
  _graph = graph;
  _graphicPanel = AppDelegete::instance().getGraphicPanel();
  
  auto mouseMovedHandler(bind(&GraphicController::mouseMoved, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_MOTION, mouseMovedHandler);
  
  auto mouseLeftDownHandler(bind(&GraphicController::mouseLeftDown, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_LEFT_DOWN, mouseLeftDownHandler);
  
  auto mouseLeftUpHandler(bind(&GraphicController::mouseLeftReleased, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_LEFT_UP, mouseLeftUpHandler);
  
  auto mouseRightDownHandler(bind(&GraphicController::mouseRightDown, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_RIGHT_DOWN, mouseRightDownHandler);
  
  auto mouseRightUpHandler(bind(&GraphicController::mouseRightReleased, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_LEFT_UP, mouseRightUpHandler);
  
  auto resizeHandler(bind(&GraphicController::resized, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_SIZE, resizeHandler);
  
  auto paintHandler(bind(&GraphicController::renderScene, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_PAINT, paintHandler);
  
  auto idleHandler(bind(&GraphicController::onIdle, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_IDLE, idleHandler);
  
  auto mouseScrollHandler(bind(&GraphicController::onIdle, this, placeholders::_1 ));
  this->_graphicPanel->Bind(wxEVT_MOUSEWHEEL, mouseScrollHandler);
}

GraphicController::~GraphicController() {
  
}

void GraphicController::resized(wxSizeEvent& evt) {
  _graphicPanel->resize();
}

void GraphicController::mouseMoved(wxMouseEvent& event) {
  _graphicPanel->moveTo(event.GetX(), event.GetY());
}

void GraphicController::onIdle(wxEvent&) {
  _graphicPanel->refresh();
}

void GraphicController::mouseLeftDown(wxMouseEvent& event) {
  int id = _graphicPanel->pick(event.GetX(), event.GetY());  
  auto sbp = AppDelegete::instance().getControlPanel();
  //@TODO
  //sbp->setTarget(id);
}

void GraphicController::mouseLeftReleased(wxMouseEvent& event) {
  _graphicPanel->releaseLeft();
}

void GraphicController::mouseRightDown(wxMouseEvent& event) {
  int x = event.GetX();
  int y = event.GetY();
  _graphicPanel->downRight(x, y);
}

void GraphicController::mouseRightReleased(wxMouseEvent& event) {
  _graphicPanel->releaseRight();
}

void GraphicController::mouseScroll(wxMouseEvent& event) {
  int delta = event.GetWheelRotation();
  if (delta != 0) {
    _graphicPanel->moveEye(delta);
  }
}

void GraphicController::renderScene(wxPaintEvent& evt) {
  if(_graph->isLoaded()) {
    _graphicPanel->renderScene();
  }
}