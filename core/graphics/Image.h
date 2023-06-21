#ifndef IMAGE_H
#define IMAGE_H

#include <FreeImage.h>
#include <GL/glew.h>

class Image {
  FIBITMAP *img;
  FIBITMAP *img32;
  int bPP;
public:
  GLubyte *tex;
  int w, h;
  Image(const char *name);
  ~Image();
};

#endif
