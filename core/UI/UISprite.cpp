#include "UISprite.h"
#include "UI.h"
#include "../graphics/RenderEngine.h"
#include "../Application.h"
//#include "../Debug/Debug.hpp"

UISprite::UISprite(float posX_, float posY_, float width_, float height_,const char * texturePath, AnchorType _anchor)
{
    posX = posX_;
    posY = posY_;
    width = width_;
    height = height_;
    visible = true;
    sprite = new Sprite(width, height, texturePath, sprite_T);
    anchor = _anchor;

    UI::RegisterUIObject(this);
    Application::onWindowResized.connect_member(this, &UISprite::RecalculatePosition);
}

UISprite::~UISprite()
{
    UI::UnregiterUIObject(this);
    delete sprite;
    Application::onWindowResized.disconnect_all();
}

void UISprite::destroy()
{
    delete this;
}

void UISprite::Draw(Shader *shader)
{
    if (!visible)
    {
        return;
    }

    glm::mat4 model = glm::mat4(1.f);
    glm::vec3 position{posX + width / 2 + origin.x, RenderEngine::screenHeight - posY - height / 2 - origin.y, 0};
    anchoredPositionX = posX + origin.x;
    anchoredPositionY = posY + origin.y;
    model = glm::translate(model, position);
    sprite->SetLocalTransform(model);
    sprite->Draw(shader);
}

void UISprite::Update(float dt)
{
}

void UISprite::SetPosition(float newX, float newY)
{
    posX = newX;
    posY = newY;
}

void UISprite::SetAnchor(AnchorType newAnchorType)
{
    anchor = newAnchorType;

    RecalculatePosition();
}

void UISprite::RecalculatePosition()
{
    switch (anchor)
    {
    case CENTER:
        origin = glm::vec3(RenderEngine::screenWidth - width,RenderEngine::screenHeight - height,0) * 0.5f;
        break;
    case LEFT_TOP:
        origin = glm::vec3(0,0,0);
        break;
    case BOTTOM:
        origin = glm::vec3(RenderEngine::screenWidth * 0.5f,RenderEngine::screenHeight,0);
        break;
    case TOP:
        origin = glm::vec3(RenderEngine::screenWidth * 0.5f, 0,0);
        break;
    case RIGHT_TOP:
        origin = glm::vec3(RenderEngine::screenWidth, 0,0);
        break;
    }
}

void UISprite::SetColor(float r_,float g_,float b_)
{
    r = r_;
    g = g_;
    b = b_;
    sprite->setColor(r,g,b);
}