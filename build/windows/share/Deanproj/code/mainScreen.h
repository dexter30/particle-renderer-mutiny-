#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "AntTweakBar.h"
#include <mutiny/mutiny.h>
#include "dynnRenderer.h"
#include <memory>

using namespace mutiny::engine;

class mainScreen : public Behaviour
{
public:
  static ref<GameObject> create();
  virtual void onAwake();
  virtual void onUpdate();
  virtual void onGui();
  void TW_CALL butt(void *P);
	void createEmitter();
	void makeGui();
private:
	TwBar *myBar;
	std::vector<ref<GameObject>> particleContainer;
  ref<GameObject> cameraGo;
  ref<GameObject> cube1Go;
  ref<GameObject> logs;
  ref<GameObject> floor;
  ref<GameObject> cloud;
  
  }
#endif