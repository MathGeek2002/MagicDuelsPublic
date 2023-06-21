#ifndef MATERIALS_H
#define MATERIALS_H

#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"
#include "Shader.h"

class Material
{
public:
  virtual ~Material(){}
  virtual void SetMaterial(Shader* shader) = 0;
};

class UniformMaterial : public Material
{
public:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

  UniformMaterial(
    float dr = 0.5f, float dg = 0.5f, float db = 0.5f,
    float ss = 32.f,
    float ar = 0.5f, float ag = 0.5f, float ab = 0.5f,
    float sr = 1.f, float sg = 1.f, float sb = 1.f
  ) :
    ambient(ar, ag, ab),
    diffuse(dr, dg, db),
    specular(sr, sg, sb),
    shininess(ss)
  {}

  virtual ~UniformMaterial(){}

  void SetMaterial(Shader* shader)
  {
    shader->SetVector3("material.ambient", glm::value_ptr(ambient));
    shader->SetVector3("material.diffuse", glm::value_ptr(diffuse));
    shader->SetVector3("material.specular", glm::value_ptr(specular));
    shader->SetScalar1("material.shininess", shininess);
  }
};

class TextureMaterial : public Material
{
private:
  GLuint ambient;
  GLuint diffuse;
  GLuint specular;
  float shininess;
public:
  TextureMaterial(
    const char *difName,
    float shi = 32.f,
    const char *speName = nullptr,
    const char *ambName = nullptr
  );
  virtual ~TextureMaterial();
  void SetMaterial(Shader* shader);
};

#endif