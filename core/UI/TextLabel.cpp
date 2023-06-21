#include "TextLabel.h"

TextLabel::TextLabel(float posX_, float posY_, Font *font_) : UISprite(posX_, posY_, 1.f, 1.f)
{
    scale = 1;
    font = font_;
}

TextLabel::~TextLabel()
{
}

void TextLabel::Update(float dt)
{
}

void TextLabel::SetText(const char *text)
{
    TextTexture tTexture = font->RenderText(text);

    width = tTexture.width * scale;
    height = tTexture.height * scale;

    sprite->ChangeTexture(tTexture.tex, tTexture.width, tTexture.height);
    sprite->resize(width, height);
}

void TextLabel::SetTextScale(float scale_)
{
    float scalingCoef = scale_/scale;
    scale = scale_;

    width *= scalingCoef;
    height *= scalingCoef;

    sprite->resize(width, height);
}