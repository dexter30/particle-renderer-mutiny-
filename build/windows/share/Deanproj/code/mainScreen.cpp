
#include "mainScreen.h"
#include "dynnEmitter.h"
#include <iostream>
;
using namespace mutiny::engine;

ref<GameObject> mainScreen::create()
{
  Debug::log("onAwake()");
  ref<GameObject> mainGo = GameObject::create("mainScene");
  mainGo->addComponent<mainScreen>();
	
  return mainGo;
}

void mainScreen::onAwake()
{
	 TwInit(TW_OPENGL, NULL);
	 TwWindowSize(800, 600);
	cameraGo = GameObject::create("MainCamera");
  ref<Camera> camera = cameraGo->addComponent<Camera>();
  cameraGo->getTransform()->setPosition(Vector3(0, 0, -10));

  //cube1Go = GameObject::createPrimitive(PrimitiveType::CUBE);
 // cube1Go->getTransform()->setPosition(Vector3(-2, -2, 5));
 // cube1Go->setName("cube1Go");
   //cube1Go->getTransform()->setScale(Vector3(2, 2, 5));
	//ref<dynnRenderer> part = cube1Go->addComponent<dynnRenderer>();
	logs = GameObject::create("logs");
	  ref<MeshRenderer> playerMr = logs->addComponent<MeshRenderer>();
	   playerMr->setMaterial(Resources::load<Material>("shaders/Internal-MeshRendererTexture"));
 playerMr->getMaterial()->setMainTexture(Resources::load<Texture2d>("models/loggg"));
	
	
	  ref<Mesh> mesh = Resources::load<Mesh>("models/hotova");
  ref<MeshFilter> playerMf = logs->addComponent<MeshFilter>();
  playerMf->setMesh(mesh);
	logs->getTransform()->setPosition(Vector3(-4, -3, 5));
	
	floor = GameObject::create("floor");
	ref<MeshRenderer> floorMr = floor->addComponent<MeshRenderer>();
	   floorMr->setMaterial(Resources::load<Material>("shaders/Internal-MeshRendererTexture"));
	ref<Mesh> floorM = 	Resources::load<Mesh>("models/floor");
	ref<MeshFilter> floorMf = floor->addComponent<MeshFilter>();
	floorMf->setMesh(floorM);
	floor->getTransform()->setPosition(Vector3(-3, -4, 5));
	
	cloud = GameObject::create("cloud");
	ref<MeshRenderer> cloudMr = cloud ->addComponent<MeshRenderer>();
	cloudMr->setMaterial(Resources::load<Material>("shaders/Internal-MeshRendererTexture"));
	ref<Mesh> cloudM = Resources::load<Mesh>("models/cloud");
	ref<MeshFilter> cloudMf = cloud->addComponent<MeshFilter>();
	cloudMf->setMesh(cloudM);
	cloud->getTransform()->setPosition(Vector3(6,0,5));
	
	myBar = TwNewBar("NameOfMyTweakBar");
}

void mainScreen::onUpdate()
{
	
}

void mainScreen::onGui()
{
	 int middle = Screen::getWidth() / 2;
  int padding = 5;
	 TwDraw();
	  if(Gui::button(Rect(middle + padding, 300, 200, 50), "Add a particle FX") == true)
	{
    createEmitter();
	}
	//if(Gui::button(Rect(middle + padding, 400, 200, 50), "edit FX") == true)
	//{
    //makeGui();
	//}
}

void TW_CALL mainScreen::butt(void *P)
{
		
}

void mainScreen::createEmitter()
{
	ref<GameObject> part = GameObject::create("particle");
	ref<dynnRenderer> parti = part->addComponent<dynnRenderer>();
	parti->makeInterface(myBar);
	particleContainer.push_back(part);
}

void mainScreen::makeGui()
{
	
	for(int i=0; i < particleContainer.size(); i++)
	{
		
		TwAddVarRW(myBar, "particle"+i, TW_TYPE_BOOLCPP, &particleContainer.at(i)->getComponent<dynnRenderer>()->fin, " label='edit the effect' ");
	}
}