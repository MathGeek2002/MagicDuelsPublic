#pragma once

#include "Scene.h"
#include "../graphics/RenderEngine.h"

class SceneManager
{
public:
    static Scene *currentScene;
    static Scene *nextScene;
    static bool changedScene;

    static void Init(Scene *startScene);

    template <typename T>
    static void LoadScene()
    {
        RenderEngine::UnregisterAllGraphicsComponents();
        nextScene = new T();
    }
    
    //returns true frame after scene change, false otherwise
    static bool Update();
    static void addObjectToCurrentScene(Object *obj);
};