#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include "../ECS/ECS.hpp"
#include "GraphicsComponentType.h"
#include "Model.h"

enum DrawSpace
{
    DS_WORLD,
    DS_HUD
};

enum Visibility
{
    VS_VISIBLE,
    VS_INVISIBLE
};

class GraphicsComponent : public Component
{
private:
    GraphicsComponentType graphicsComponentType;
    DrawSpace drawSpace;
    Visibility visibility;
    Model *model;

public:
    int orderInLayer;

public:
    GraphicsComponent(
        int order_in_layer = 0,
        GraphicsComponentType graphicsComponentType = GCT_MODEL,
        Visibility visibility = VS_VISIBLE,
        DrawSpace drawSpace = DS_WORLD);

    ~GraphicsComponent();

    GraphicsComponentType GetGraphicsComponentType() const;
    inline void SetDrawSpace(DrawSpace drawSpace);
    DrawSpace GetDrawSpace() const;
    void SetVisibility(Visibility visibility);
    Visibility GetVisibility() const;
    void SetModel(Model *model);
    Model *GetModel() const;
    void ChangeAnimation(const char *name) const;
    float GetAnimationProcentage() const;
    void SetFlip(bool) const;
    bool GetFlip() const;
};

inline GraphicsComponentType
GraphicsComponent::GetGraphicsComponentType() const
{
    return graphicsComponentType;
}

inline void
GraphicsComponent::SetDrawSpace(DrawSpace newDrawSpace)
{
    drawSpace = newDrawSpace;
}

inline DrawSpace
GraphicsComponent::GetDrawSpace() const
{
    return drawSpace;
}

inline Visibility
GraphicsComponent::GetVisibility() const
{
    return visibility;
}

inline Model *
GraphicsComponent::GetModel() const
{
    return model;
}
#endif