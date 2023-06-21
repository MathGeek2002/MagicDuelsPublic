#include "UI.h"
#include "../Input/Input.h"
#include "../Scene/SceneManager.h"

std::vector<UISprite *> UI::uiObjects;

bool UI::Init()
{
    return true;
}

void UI::Close()
{
    for (UISprite *obj : uiObjects)
    {
        delete obj;
    }
    uiObjects.clear();
}

void UI::RegisterUIObject(UISprite *obj)
{
    uiObjects.push_back(obj);
    SceneManager::addObjectToCurrentScene(obj);
}

void UI::UnregiterUIObject(UISprite *obj)
{
    std::vector<UISprite *>::iterator it;
    for (it = uiObjects.begin(); it < uiObjects.end(); ++it)
    {
        if (*it == obj)
        {
            uiObjects.erase(it);
            break;
        }
    }

    SceneManager::currentScene->removeObject(obj);
}

void UI::Update(float dt)
{
    for (UISprite *obj : uiObjects)
    {
        if (!obj->visible)
        {
            continue;
        }

        obj->Update(dt);
    }
}