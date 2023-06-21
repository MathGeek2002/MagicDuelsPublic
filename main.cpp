#include <iostream>
#include "core/Application.h"
#include "core/graphics/RenderEngine.h"
#include "core/Random/Random.hpp"
#include "core/Scene/Scene.h"
#include "game/Scenes/MainScene.hpp"
#include "game/Scenes/MenuScene.hpp"
#include "core/Scene/SceneManager.h"

void InitData();
void CreateGround(glm::vec3);

int main()
{
    if (!Application::Init())
    {
        std::cout << "Unable to initialize game!\n";
    }
    else
    {
        // seeds: 1677799643
        //  1677692888
        Random::Init(1677799643);
        RenderEngine::InitData();
        InitData();
        Application::Run();
        Application::Close();
    }
    
    return 0;
}

void InitData()
{
    //define initial scene below
    SceneManager::Init(new MenuScene()); 
}