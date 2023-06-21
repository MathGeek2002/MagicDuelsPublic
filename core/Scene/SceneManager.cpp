#include "SceneManager.h"

Scene *SceneManager::currentScene = nullptr;
Scene *SceneManager::nextScene = nullptr;
bool SceneManager::changedScene = false;

void SceneManager::Init(Scene *startScene)
{
    currentScene = startScene;
    currentScene->Init();
}

bool SceneManager::Update()
{
    //scene changed last update
    if (changedScene) {
        changedScene = false;
        return true;
    }

    //scene changing
    if (nextScene == nullptr)
    {
        return false;
    }

    if (currentScene != nullptr)
    {
        delete currentScene;
    }

    currentScene = nextScene;
    nextScene = nullptr;
    currentScene->Init();
    changedScene = true;

    return false;
}

void SceneManager::addObjectToCurrentScene(Object *obj)
{
    currentScene->addObject(obj);
}