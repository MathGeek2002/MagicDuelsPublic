#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include "UISprite.h"
#include "Font.h"
#include <string>

class TextLabel : public UISprite
{
public:
    std::string text;
    float scale;
    Font *font;

    TextLabel(float posX_, float posY_, Font *font_);
    ~TextLabel();
    void Update(float dt) override;
    void SetText(const char *text);
    void SetTextScale(float scale_);
};

#endif