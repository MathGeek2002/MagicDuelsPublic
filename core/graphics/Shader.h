#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

class Shader
{
public:
  GLuint program;

  Shader(const char *vertexPath,
         const char *fragmentPath);
  void Use();
  void SetScalar1(const GLchar *name, GLint value);
  void SetScalar1(const GLchar *name, GLfloat value);
  void SetVector2(const GLchar *name, const GLfloat *value, GLsizei count = 1);
  void SetVector3(const GLchar *name, const GLfloat *value,
                  GLsizei count = 1);
  void SetMatrix3(const GLchar *name, const GLfloat *value,
                  GLsizei count = 1, GLboolean trans = GL_FALSE);
  void SetMatrix4(const GLchar *name, const GLfloat *value,
                  GLsizei count = 1, GLboolean trans = GL_FALSE);
};

#endif
