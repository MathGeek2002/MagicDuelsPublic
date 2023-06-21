#pragma once

#include "../Object.h"
#include "../UI/UISprite.h"
#include <vector>

class Scene
{
public:
    std::vector<Object *> sceneObjects;

    virtual ~Scene();
    virtual void Init();
    virtual void Update(float dt);
    void addObject(Object *obj);
    void removeObject(Object *obj);
};