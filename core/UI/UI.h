#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include <vector>
#include "Button.h"
#include "UISprite.h"
#include "Font.h"
#include "TextLabel.h"

class UI
{

public:
    static std::vector<UISprite *> uiObjects;
    
    static bool Init();
    static void RegisterUIObject(UISprite *uiObject);
    static void UnregiterUIObject(UISprite *uiObject);
    static void Update(float dt);
    static void Close();
};

#endif