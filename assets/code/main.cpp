#include "SceneManager.h"

#include <mutiny/mutiny.h>


using namespace mutiny::engine;

void mutiny_main()
{
	  
	 Application::loadLevel("mainScene");
	
	 ref<GameObject> sceneObject = GameObject::create();
	 sceneObject->addComponent<SceneManager>();
}