#pragma once

#include "../../core/ECS/ECS.hpp"
#include "../../core/TransformComponent/TransformComponent.h"
#include "../../core/graphics/TextureSprite.h"
#include "../../core/graphics/GraphicsComponent.h"
#include "../../core/Application.h"
#include "../../thirdparty/glm/glm.hpp"
#include "../../core/Signals/Signal.hpp"

class Bar
{
private:
    Entity *bar;
    TextureSprite *fillTexture;
    glm::vec3 offset;
    glm::vec3 color;

public:
    Signal <float> onValueChange;

    Bar(glm::vec3 color ,glm::vec3 offset_ = glm::vec3(0)) : offset(offset_)
    {
        bar = &Application::Instantiate(glm::vec3(0,0,0.2f));
        GraphicsComponent &gCmp = bar->addComponent<GraphicsComponent>(100);
        fillTexture = new TextureSprite(0.12f, 0.01f, 50, 1);
        fillTexture->setColor( color.r, color.g, color.b);
        gCmp.SetModel(fillTexture);
        UpdateTexture( 1.f );

        onValueChange.connect_member(this, &Bar::UpdateTexture);
    }

    ~Bar()
    {
        delete fillTexture;
    }

    void UpdateTexture(float fillAmount)
    {
        int res = 50;
        GLubyte *tex = new GLubyte[res * 4];

        for (int i = 0; i < res; i++)
        {
            float part = (float)i / res;
            int val = part > fillAmount ? 0 : 255;

            tex[i * 4 + 0] = val;
            tex[i * 4 + 1] = val;
            tex[i * 4 + 2] = val;
            tex[i * 4 + 3] = 255;
        }

        fillTexture->UpdateTexture(tex);

        delete[] tex;
    }

    void UpdateBarPosition(glm::vec3 parrentPos)
    {
        glm::vec3 newPos = parrentPos + offset; 
        bar->getComponent<TransformComponent>().SetPosition(newPos);
    }
};