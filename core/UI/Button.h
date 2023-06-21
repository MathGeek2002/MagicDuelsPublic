#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "../graphics/Shader.h"
#include "../graphics/Sprite.h"
#include "../Signals/Signal.hpp"
#include "UISprite.h"
#include "TextLabel.h"

class Button: public UISprite
{
public:
    glm::vec3 color;
    glm::vec3 hoverColor;

    Signal<> onClick;

    Button(float posX_, float posY_, float width_, float height_);
    ~Button();
    void Update(float dt) override;
    void Click();
    bool IsInsideButtonArea(glm::vec2 mousePos);
};

#endif