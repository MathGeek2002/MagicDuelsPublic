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


unsigned int Random::seeds[] = {1677799643,1687441982,1687442239,16126,1687442668,17866};

int main()
{
    if (!Application::Init())
    {
        std::cout << "Unable to initialize game!\n";
    }
    else
    {
        srand((unsigned)time(NULL));
        // seeds: 1677799643
        //  1677692888
        Random::Init(rand());
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