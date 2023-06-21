#ifndef FONT_H
#define FONT_H

#include <GL/glew.h>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"

struct Character
{
    glm::ivec2 size;      // Size of glyph
    glm::ivec2 bearing;   // Offset from baselines to left nad top of glyph
    unsigned int advance; // Offset to next glyph
    GLubyte *tex;
};

struct TextTexture
{
    int width;
    int height;
    GLubyte *tex;
};

class Font
{
protected:
    int nChars = 128;
    Character characters[128];

public:
    std::string fontName;
    int fontSize;
    Font(std::string fontName_, int fontSize_);
    ~Font();
    TextTexture RenderText(std::string text);
};

#endif
