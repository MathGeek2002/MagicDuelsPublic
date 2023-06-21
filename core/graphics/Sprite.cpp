#include "Sprite.h"
#include "Image.h"
#include "Vertex.hpp"
#include "../Memory/TextureLibrary.h"

Sprite::Sprite(
    float dx_, float dy_,
    int type_) : dx(dx_), dy(dy_)
{
    type = type_;
    r = 1.f;
    g = 1.f;
    b = 1.f;
    createMesh();
    createTexture();
    SetModelType(sprite_T);
}

Sprite::Sprite(
    float dx_, float dy_,
    const char *texturePath,ModelType _type) : dx(dx_), dy(dy_)
{
    type = ST_TEXTURE;
    r = 1.f;
    g = 1.f;
    b = 1.f;
    createMesh();
    createTexture(texturePath);
    SetModelType(_type);
}

Sprite::~Sprite()
{
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, vaos);
    delete[] vaos;
    delete mesh;
}

void Sprite::createMesh()
{
    vaos = new unsigned int[1];
    glGenVertexArrays(1, vaos);

    int nVertices = 4;
    int nIndices = 6;

    Vertex *vertices = new Vertex[nVertices];
    unsigned int *indices = new unsigned int[nIndices];

    float dx2 = dx * 0.5f;
    float dy2 = dy * 0.5f;

    vertices[0].position = glm::vec3(-dx2, -dy2, 0.f);
    vertices[0].color = glm::vec3(r, g, b);
    vertices[0].normal = glm::vec3(0.f, 0.f, 1.f);
    vertices[0].texcoord = glm::vec2(0.f, 0.f);

    vertices[1].position = glm::vec3(dx2, -dy2, 0.f);
    vertices[1].color = glm::vec3(r, g, b);
    vertices[1].normal = glm::vec3(0.f, 0.f, 1.f);
    vertices[1].texcoord = glm::vec2(1.f, 0.f);

    vertices[2].position = glm::vec3(-dx2, dy2, 0.f);
    vertices[2].color = glm::vec3(r, g, b);
    vertices[2].normal = glm::vec3(0.f, 0.f, 1.f);
    vertices[2].texcoord = glm::vec2(0.f, 1.f);

    vertices[3].position = glm::vec3(dx2, dy2, 0.f);
    vertices[3].color = glm::vec3(r, g, b);
    vertices[3].normal = glm::vec3(0.f, 0.f, 1.f);
    vertices[3].texcoord = glm::vec2(1.f, 1.f);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 1;

    mesh = new Mesh(
        nVertices, vertices,
        nIndices, indices,
        vaos[0]);

    delete[] vertices;
    delete[] indices;
}

void Sprite::Draw(Shader *shader_)
{
    glm::mat4 model = localTransform * mesh->localTransform;
    shader_->SetMatrix4("model", glm::value_ptr(model));
    glm::mat3 normal = glm::mat3(glm::inverse(glm::transpose(model)));
    shader_->SetMatrix3("normal", glm::value_ptr(normal));
    glm::vec2 offset = glm::vec2(offsetX, offsetY);
    shader_->SetVector2("offset", glm::value_ptr(offset));
    glm::vec2 scale = glm::vec2(scaleX, scaleY);
    shader_->SetVector2("scale", glm::value_ptr(scale));
    glm::vec3 color = glm::vec3(r,g,b);
    shader_->SetVector3("color", glm::value_ptr(color));
    glm::vec2 flip = glm::vec2(flipH, flipV);
    shader_->SetVector2("flip", glm::value_ptr(flip));
     
    shader_->SetScalar1("tex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vaos[0]);
    glDrawElements(GL_TRIANGLES, mesh->GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sprite::createTexture(const char *texturePath)
{
    GLubyte *tex = nullptr;
    Image *image = nullptr;
    GLenum colorFormat = GL_RGBA;
    int width = 1;
    int height = 1;
    if (texturePath == nullptr)
    {
        tex = new GLubyte[4]{255, 255, 255, 255}; // 1x1 white texture
    }
    else
    {
        image = TextureLibrary::GetTexture(texturePath);
        tex = image->tex;
        width = image->w;
        height = image->h;
        colorFormat = GL_BGRA;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, tex);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (texturePath == nullptr)
    {
        delete[] tex;
    }
}

void Sprite::setOffset(float x, float y)
{
    offsetX = x;
    offsetY = y;
}

void Sprite::setScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

void Sprite::setColor(float r_, float g_, float b_)
{
    r = r_;
    g = g_;
    b = b_;
}

void Sprite::setFlipH(bool flag)
{
    flipH = flag;
}

void Sprite::setFlipV(bool flag)
{
    flipV = flag;
}

void Sprite::resize(float dx_, float dy_)
{
    dx = dx_;
    dy = dy_;

    int nVertices = 4;
    Vertex *vertices = new Vertex[nVertices];

    float dx2 = dx_ * 0.5f;
    float dy2 = dy_ * 0.5f;

    vertices[0].position = glm::vec3(-dx2, -dy2, 0.f);
    vertices[0].texcoord = glm::vec2(0.f, 0.f);

    vertices[1].position = glm::vec3(dx2, -dy2, 0.f);
    vertices[1].texcoord = glm::vec2(1.f, 0.f);

    vertices[2].position = glm::vec3(-dx2, dy2, 0.f);
    vertices[2].texcoord = glm::vec2(0.f, 1.f);
    
    vertices[3].position = glm::vec3(dx2, dy2, 0.f);
    vertices[3].texcoord = glm::vec2(1.f, 1.f);

    mesh->SetVertices(vertices, nVertices);
    delete[] vertices;
}

void Sprite::ChangeTexture(const char* texturePath)
{
    Image *image = TextureLibrary::GetTexture(texturePath);
    GLenum colorFormat = GL_BGRA;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, colorFormat, GL_UNSIGNED_BYTE, image->tex);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Sprite::ChangeTexture(GLubyte *tex, int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    glGenerateMipmap(GL_TEXTURE_2D);
}