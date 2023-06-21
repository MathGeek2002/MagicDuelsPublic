#ifndef FONT_H
#define FONT_H

#include <GL/glew.h>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"
#include "Shader.h"

struct Character
{
  glm::ivec2 size; // Size of glyph
  glm::ivec2 bearing; // Offset from baselines to left nad top of glyph
  unsigned int advance; // Offset to next glyph
  glm::vec2 texCoord;
  glm::vec2 texSize;
};

class Font {
protected:
  std::string fontName;
  int fontSize;
  Character characters[128];
  GLuint VAO, VBO; // OpenGL attribute and buffer objects
  GLuint textureID; // OpenGL texture object
public:
  Font(std::string fontName_, int fontSize_);
  ~Font();
  void RenderText(
    Shader *shader_,
    std::string text_, float posX_, float posY_, 
    float scale_ = 1.f, glm::vec3 color_ = {1.f, 1.f, 1.f}
  );
};

#endif
