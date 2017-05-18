#include "SceneManager.h"

void SceneManager::onAwake()
{
  Debug::log("Awake");
    loadLevel();

}

void SceneManager::onLevelWasLoaded()
{
	Debug::log("Level ");
  loadLevel();
}

void SceneManager::loadLevel()
{
  Debug::log("Level loaded: " + Application::getLoadedLevelName());
  if(Application::getLoadedLevelName() == "mainScene")
  {
	 mainScreen::create();
  }
}