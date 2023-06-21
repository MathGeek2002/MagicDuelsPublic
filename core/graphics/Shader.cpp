#include "Shader.h"
#include <string>
#include <iostream>
#include <fstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertexPath, std::ios::in);
  if (VertexShaderStream.is_open())
  {
    std::string Line = "";
    while (getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else
  {
    std::cout << "Impossible to open " << vertexPath << std::endl;
    exit(0);
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragmentPath, std::ios::in);
  if (FragmentShaderStream.is_open())
  {
    std::string Line = "";
    while (getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }
  else
  {
    std::cout << "Impossible to open " << fragmentPath << std::endl;
    exit(0);
  }

  GLint success;
  int infoLength;

  // Compile Vertex Shader
  std::cout << "Compiling vertex shader: " << vertexPath << std::endl;
  char const *VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &success);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &infoLength);
  if (!success)
  {
    char infoLog[infoLength + 1];
    glGetShaderInfoLog(VertexShaderID, infoLength, NULL, infoLog);
    std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
  }

  // Compile Fragment Shader
  std::cout << "Compiling fragment shader: " << fragmentPath << std::endl;
  char const *FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &success);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &infoLength);
  if (!success)
  {
    char infoLog[infoLength + 1];
    glGetShaderInfoLog(FragmentShaderID, infoLength, NULL, infoLog);
    std::cout << "Fragment shader compilation failed: " << infoLog
              << std::endl;
  }

  // Link the program
  std::cout << "Linking program\n";
  program = glCreateProgram();
  glAttachShader(program, VertexShaderID);
  glAttachShader(program, FragmentShaderID);
  glLinkProgram(program);

  // Check the program
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
  if (!success)
  {
    char infoLog[infoLength + 1];
    glGetProgramInfoLog(program, infoLength, NULL, infoLog);
    std::cout << "Shader program linking failed: " << infoLog
              << std::endl;
  }

  glDetachShader(program, VertexShaderID);
  glDetachShader(program, FragmentShaderID);
  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);
}

void Shader::Use()
{
  glUseProgram(program);
}

void Shader::SetScalar1(const GLchar *name, GLint value)
{
  glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::SetScalar1(const GLchar *name, GLfloat value)
{
  glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::SetVector2(const GLchar *name, const GLfloat *value, GLsizei count)
{
  glUniform2fv(glGetUniformLocation(program, name), count, value);
}

void Shader::SetVector3(
    const GLchar *name, const GLfloat *value,
    GLsizei count)
{
  glUniform3fv(glGetUniformLocation(program, name),
               count, value);
}

void Shader::SetMatrix3(
    const GLchar *name, const GLfloat *value,
    GLsizei count, GLboolean trans)
{
  glUniformMatrix3fv(glGetUniformLocation(program, name),
                     count, trans, value);
}

void Shader::SetMatrix4(
    const GLchar *name, const GLfloat *value,
    GLsizei count, GLboolean trans)
{
  glUniformMatrix4fv(glGetUniformLocation(program, name),
                     count, trans, value);
}
