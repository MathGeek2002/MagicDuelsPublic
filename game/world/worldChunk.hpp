#ifndef WORLD_CHUNK_HPP
#define WORLD_CHUNK_HPP

#include "../../core/ECS/ECS.hpp"
#include "../../thirdparty/glm/glm.hpp"
#include "../../core/TransformComponent/TransformComponent.h"
#include "../../core/Collisions/ColliderAABB.h"
#include "../../core/Application.h"
#include "../../core/Utils/PolygonSimplify.h"
#include "../../core/Utils/ConvexDecomp.h"
#include "../../core/Collisions/MeshCollider.h"
#include "../../core/Collisions/ColliderSet.h"
//#include "../../core/Debug/Debug.hpp"
#include "../../core/Random/Random.hpp"
#include "../../core/ParticleSystem/ParticleData.h"
#include "world.hpp"


class World;

class WorldChunk : public Entity
{
public:
    glm::vec3 pos;
    float chunkWidth;
    int size;
    int *chunkData;
    bool chunkDataChanged;
    ColliderSet *collidersCmp;
    World *worldRef;
    TextureSprite *chunkTexture;

    WorldChunk(glm::vec3 pos_, float width_, int size_, int *chunkData_, World *worldRef_)
    {
        pos = pos_;
        chunkWidth = width_;
        size = size_;
        chunkData = chunkData_;
        collidersCmp = nullptr;
        worldRef = worldRef_;
        chunkDataChanged = false;

        this->tag = "Ground";

        this->addComponent<TransformComponent>(pos);
        updateChunkColliders();

        chunkTexture = new TextureSprite(chunkWidth, chunkWidth, size, size);
        GraphicsComponent &graphicsCmp = this->addComponent<GraphicsComponent>(2);
        graphicsCmp.SetModel(chunkTexture);

        updateTexture();

        this->onTriggerEnter.connect_member(this, &WorldChunk::OnCollision);

        Application::Register(this);
    }

    void OnCollision(Entity *other)
    {
        if (other->tag == "orange" || other->tag == "blue")
        {
            if(other->getComponent<ParticleData>().destroyTerrainChance <= Random::Range(0,100))
                return;

            TransformComponent &trnCmp = other->getComponent<TransformComponent>();
            glm::vec3 particlePos = trnCmp.GetPosition();

            glm::vec3 hitPos = particlePos;
            glm::vec2 chunkPixPos = getIndexAt(hitPos.x, hitPos.y);
            // find closest pixel
            glm::vec2 closestPos = closestSolid(chunkPixPos, 5);
            int pixValue = getPixelAtIndex(closestPos.x, closestPos.y);
            if (pixValue && pixValue < 240)
            {
                setPixel(closestPos.x, closestPos.y, 0);
            }

            chunkDataChanged = true;
        }
    }

    void UserUpdate(float dt)
    {
        if (chunkDataChanged)
        {
            updateTexture();
            updateChunkColliders();
            chunkDataChanged = false;
        }
    }

    void updateTexture()
    {
        GLubyte *textureData = new GLubyte[size * size * 4];

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int index = j * size + i;
                int value = chunkData[index];

                glm::vec4 color;
                if (value < 160)
                {
                    color = glm::vec4{0x5c, 0x34, 0x15, 255};
                }
                else if (value < 175)
                {
                    color = glm::vec4{0x48, 0x28, 0x10, 255};
                }
                else
                {
                    color = glm::vec4{0x2e, 0x1a, 0x0a, 255};
                }

                textureData[index * 4 + 0] = color.r;
                textureData[index * 4 + 1] = color.g;
                textureData[index * 4 + 2] = color.b;
                textureData[index * 4 + 3] = value > 0 ? 255 : 0;
            }
        }

        chunkTexture->UpdateTexture(textureData);
        delete[] textureData;
    }

    void updateChunkColliders()
    {
        contoursData chunkContours = PolygonSimplify::getSimplifiedContours(chunkData, size, size, 0.85f);

        if (chunkContours.size() == 0 && collidersCmp != nullptr)
        {
            collidersCmp->clearColliders();
        }

        if (chunkContours.size() > 0)
        {
            if (collidersCmp == nullptr)
            {
                collidersCmp = &this->addComponent<ColliderSet>();
            }
            collidersCmp->clearColliders();

            for (std::vector<glm::vec2> contour : chunkContours)
            {
                ConvexDecomp decomp;
                decomp.makeCCW(contour);
                decomp.decomposePoly(contour);
                for (std::vector<glm::vec2> c : decomp.polys)
                // for (std::vector<glm::vec2> c : chunkContours)
                {
                    int pointsNum = c.size();
                    Vertex *vertices = new Vertex[pointsNum];
                    int i = 0;
                    float pixelWidth = chunkWidth / size;
                    for (glm::vec2 point : c)
                    {
                        vertices[i].position = glm::vec3(point.x, point.y, 0) * pixelWidth + glm::vec3(-chunkWidth / 2, -chunkWidth / 2, 0);
                        i++;
                    }

                    MeshCollider *tempColl = new MeshCollider(vertices, pointsNum);
                    tempColl->SetStaticMode(true);
                    collidersCmp->addCollider(tempColl);
                }
            }
        }
    }

    glm::vec2 closestSolid(glm::vec2 pos, int radius)
    {
        int loopRadius = radius - 1;
        glm::vec2 closestPos = pos;
        float minDist = radius * 2;
        for (int i = (int)pos.x - loopRadius; i < (int)pos.x + loopRadius; i++)
        {
            for (int j = (int)pos.y - loopRadius; j < (int)pos.y + loopRadius; j++)
            {
                // outside of chunk skip condition

                if (i >= size || i < 0 || j >= size || j < 0)
                {
                    continue;
                }
                int pixValue = getPixelAtIndex(i, j);
                if (pixValue > 0)
                {
                    float dist = glm::length(glm::vec2(i, j) - pos);
                    if (dist < minDist)
                    {
                        closestPos = glm::vec2(i, j);
                        minDist = dist;
                    }
                }
            }
        }

        return closestPos;
    }

    glm::vec2 getIndexAt(float x, float y)
    {
        int indexX = glm::round(((x - pos.x) / chunkWidth) * size + ((float)size - 1) / 2);
        indexX = glm::clamp(indexX, 0, size - 1);
        int indexY = glm::round(((y - pos.y) / chunkWidth) * size + ((float)size - 1) / 2);
        indexY = glm::clamp(indexY, 0, size - 1);

        return glm::vec2(indexX, indexY);
    }

    int getPixelAt(float x, float y)
    {
        glm::vec2 pos = getIndexAt(x, y);
        int index = pos.y * size + pos.x;
        return chunkData[index];
    }

    int getPixelAtIndex(int x, int y)
    {
        int index = y * size + x;
        return chunkData[index];
    }

    void setPixel(int x, int y, int value)
    {
        int index = glm::clamp(y, 0, size - 1) * size + glm::clamp(x, 0, size - 1);
        chunkData[index] = value;
    }

    ~WorldChunk()
    {
        delete chunkTexture;
        delete [] chunkData;
    }
};

#endif