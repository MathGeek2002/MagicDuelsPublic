#include "Scene.h"
#include <iostream>

void Scene::Init()
{
}

void Scene::Update(float dt)
{
}

Scene::~Scene()
{
    for (auto it = sceneObjects.rbegin(); it != sceneObjects.rend(); ++it)
    {
        (*it)->destroy();
    }

    sceneObjects.clear();
}

void Scene::addObject(Object *obj)
{
    sceneObjects.push_back(obj);
}

void Scene::removeObject(Object *obj)
{
    std::vector<Object *>::iterator it;
    for (it = sceneObjects.begin(); it < sceneObjects.end(); ++it)
    {
        if (*it == obj)
        {
            sceneObjects.erase(it);
            break;
        }
    }
}