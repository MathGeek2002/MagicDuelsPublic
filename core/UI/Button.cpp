#include "Button.h"
#include "UI.h"
#include "../graphics/RenderEngine.h"
#include "../Input/Input.h"

Button::Button(float posX_, float posY_, float width_, float height_) : UISprite(posX_, posY_, width_, height_)
{
    color = glm::vec3{1.f};
    hoverColor = glm::vec3{.8f};
}

Button::~Button()
{
}

void Button::Update(float dt)
{
    glm::vec2 mousePos = Input::getMousePosition();
    bool mousePressed = Input::mouseDown(Input::MB_LEFT);

    glm::vec3 activeColor = IsInsideButtonArea(mousePos) ? hoverColor : color;
    sprite->setColor(activeColor.r, activeColor.g, activeColor.b);

    if (mousePressed && IsInsideButtonArea(mousePos))
    {
        Click();
    }
}

void Button::Click()
{
    onClick.emit();
}

bool Button::IsInsideButtonArea(glm::vec2 mousePos)
{
    if (mousePos.x < anchoredPositionX || mousePos.x > anchoredPositionX + width)
    {
        return false;
    }
    if (mousePos.y < anchoredPositionY || mousePos.y > anchoredPositionY + height)
    {
        return false;
    }
    return true;
}