#ifndef SPRITE_H
#define SPRITE_H

#include "Model.h"
#include "Shader.h"
#include "Mesh.h"

class Sprite : public Model
{

public:
    enum type
    {
        ST_TEXTURE = 0,
        ST_ANIMATED,
    };
    bool flipH = false, flipV = false;

protected:
    float dx, dy;  // dim_x, dim_y
    float r = 1.f, g = 1.f, b = 1.f; // TODO: replace with color/glm::vec3 struct
    float offsetX = 0.f, offsetY = 0.f;
    float scaleX = 1.f, scaleY = 1.f;
    Mesh *mesh;
    unsigned int *vaos;
    int type;
    GLuint texture;

public:
    Sprite(float dx_, float dy_, int type);
    Sprite(
        float dx, float dy,
        const char *texturePath,
        ModelType _type = sprite_T);

    void Draw(Shader *);
    void setOffset(float, float);
    void setScale(float, float);
    void setColor(float, float, float);
    void setFlipH(bool);
    void setFlipV(bool);
    void resize(float, float);
    void ChangeTexture(const char* texturePath);
    void ChangeTexture(GLubyte *tex_, int width, int height);
    ~Sprite();

    inline unsigned int Get_VAOS() {  return vaos[0]; }
    inline size_t GetNumberOfInfices() { return mesh->GetNumberOfIndices(); }
    inline glm::mat4 GetModel() { return localTransform; }
    inline glm::vec3 GetColor() { return glm::vec3(r,g,b); }

protected:
    void createMesh();
    void createTexture(const char *texturePath = nullptr);
};

#endif