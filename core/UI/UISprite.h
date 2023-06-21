#ifndef UI_SPRITE_H
#define UI_SPRITE_H

#include "../graphics/Shader.h"
#include "../graphics/Sprite.h"
#include "../Object.h"
#include "../../thirdparty/glm/glm.hpp"

enum AnchorType
{
    CENTER = 0,
    LEFT_TOP,
    TOP,
    BOTTOM,
    RIGHT_TOP
};

class UISprite : public Object
{
protected:
    float r = 1.f,g = 1.f,b = 1.f;
public:
    float posX, posY;
    float anchoredPositionX, anchoredPositionY;
    float width, height;
    bool visible;
    Sprite *sprite;
    AnchorType anchor;
    glm::vec3 origin = glm::vec3(0,0,0);

    UISprite(float posX_, float posY_, float width_, float height_,const char * texturePath = nullptr, AnchorType _anchor = LEFT_TOP);
    virtual ~UISprite();
    virtual void destroy();
    virtual void Draw(Shader *shader);
    virtual void Update(float dt);
    void SetPosition(float newX, float newY);
    void SetAnchor(AnchorType newAnchorType);
    void RecalculatePosition();
    void SetColor(float r_,float g_,float b_);
};

#endif