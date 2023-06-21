#include "GraphicsComponent.h"
#include "RenderEngine.h"
#include "AnimatedSprite.h"
#include "Sprite.h"

GraphicsComponent::GraphicsComponent(
    int order_in_layer,
    GraphicsComponentType _graphicsComponentType,
    Visibility _visibility, 
    DrawSpace _drawSpace) : Component(CT_GRAPHICS), graphicsComponentType(_graphicsComponentType),
                                                    drawSpace(_drawSpace), visibility(_visibility), model(nullptr), 
                                                    orderInLayer(order_in_layer)
{ }

GraphicsComponent::~GraphicsComponent()
{
    SetVisibility(VS_INVISIBLE);
}

void GraphicsComponent::SetVisibility(Visibility new_visibility_mode)
{
    if (visibility == new_visibility_mode)
        return;

    visibility = new_visibility_mode;

    if (visibility == VS_INVISIBLE)
        RenderEngine::UnRegisterObject(this);
    else
        RenderEngine::RegisterObject(this);
}

void GraphicsComponent::SetModel(Model *new_model)
{
    model = new_model;
    RenderEngine::RegisterObject(this);
}

void GraphicsComponent::ChangeAnimation(const char *name) const
{
    if (model->GetType() != animated_sprite_T)
        return;

    static_cast<AnimatedSprite *>(model)->setAnimation(name);
}

float GraphicsComponent::GetAnimationProcentage() const
{
    if (model->GetType() != animated_sprite_T)
        return 0.f;

    return static_cast<AnimatedSprite *>(model)->GetAnimationProcentage();
}

void GraphicsComponent::SetFlip(bool isTurnedLeft) const
{
    if (model->GetType() != animated_sprite_T && model->GetType() != sprite_T)
        return;

    static_cast<Sprite *>(model)->setFlipH(isTurnedLeft);
}

bool GraphicsComponent::GetFlip() const
{
    if (model->GetType() != animated_sprite_T && model->GetType() != sprite_T)
        return false;

    return static_cast<Sprite *>(model)->flipH;
}