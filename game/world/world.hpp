#ifndef WORLD_HPP
#define WORLD_HPP

#include "../../core/ECS/ECS.hpp"
#include "../../core/graphics/RenderEngine.h"
#include "../../core/graphics/GraphicsComponent.h"
#include "../../core/graphics/TextureSprite.h"
#include "../../core/graphics/Sprite.h"
#include "../../core/Application.h"
#include "../../core/Input/Input.h"
#include "../../core/TransformComponent/TransformComponent.h"
#include "../../core/graphics/Image.h"
#include "../../core/Random/PerlinNoise.hpp"
#include "../../core/Collisions/ColliderAABB.h"
#include "worldChunk.hpp"
#include "../../core/Debug/Debug.hpp"

class World : public Entity
{
public:
    int width = 300, height = 188; // dimension in pixels
    int *worldData;
    float accTime = 0;
    float tick = 0.05f;
    float noiseOffset = 0;
    float worldWidth;
    float worldHeight;
    WorldChunk *testChunk;

    World()
    {
        worldData = new int[width * height];
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int index = j * width + i;
                // terrain from noise
                int pointValue = (int)(perlinNoise.noise((float)i * 0.015f, (float)j * 0.015f) * 255);

                if (pointValue > 140)
                {
                    pointValue += perlinNoise.noise((float)i * 0.1f, (float)j * 0.2f) * 20;
                }

                // add border
                int borderWidth = 7;
                int jBorder = (j <= borderWidth) || (j >= height - 1 - borderWidth);
                int iBorder = (i <= borderWidth) || (i >= width - 1 - borderWidth);
                if (iBorder || jBorder)
                {
                    pointValue = 255;
                }

                pointValue = (pointValue > 140) ? pointValue : 0;
                worldData[index] = pointValue;
            }
        }

        worldWidth = 3.f;
        worldHeight = 3.f * (float)height / (float)width;

        // worldTexture = new TextureSprite(worldWidth, worldHeight, width, height);
        updateWorld();

        this->addComponent<TransformComponent>(glm::vec3(0, 0, 0));
        // GraphicsComponent &graphicsCmp = this->addComponent<GraphicsComponent>(1);
        // graphicsCmp.SetModel(worldTexture);
        Application::Register(this);

        // generate chunks with colliders
        int chunkSize = 17;
        float chunkWidth = worldWidth / width * chunkSize;
        for (int i = 0; i < width; i += chunkSize)
        {
            for (int j = 0; j < height; j += chunkSize)
            {
                int *chunkData = new int[chunkSize * chunkSize];
                std::fill_n(chunkData, chunkSize * chunkSize, 0);

                for (int chunkX = i; chunkX < i + chunkSize; chunkX++)
                {
                    for (int chunkY = j; chunkY < j + chunkSize; chunkY++)
                    {
                        int index = chunkY * width + chunkX;

                        int chunkIndex = (chunkY - j) * chunkSize + (chunkX - i);
                        int value = (chunkX >= width || chunkY >= height) ? 0 : worldData[index];
                        chunkData[chunkIndex] = value;
                    }
                }
                float chunkPosX = -worldWidth / 2 + (float)i * (chunkWidth / chunkSize) + chunkWidth / 2;
                float chunkPosY = -worldHeight / 2 + (float)j * (chunkWidth / chunkSize) + chunkWidth / 2;
                glm::vec3 pos = glm::vec3(chunkPosX, chunkPosY, 0.0f);

                testChunk = new WorldChunk(pos, chunkWidth, chunkSize, chunkData, this);
            }
        }
        perlinNoise.resetNoise();
    }

    void UserUpdate(float dt)
    {
        accTime += dt;

        if (accTime > tick)
        {
            accTime -= tick;
            // updateWorld();
        }
    }

    void updateWorld()
    {

        // for (int i = 0; i < height; i++)
        // {
        //     for (int j = 0; j < width; j++)
        //     {
        //         int index = i * height + j;
        //         int indexBelow = (i-1) * height + j;
        //         int indexBelowR = (i-1) * height + j + 1;
        //         int indexBelowL = (i-1) * height + j - 1;
        //         if (!worldData[index]) {
        //             continue;
        //         }
        //         if ((indexBelow > 0) && !worldData[indexBelow])
        //         {
        //             worldData[index] = 0;
        //             worldData[indexBelow] = 1;
        //         }
        //         else if (indexBelowR > 0 && indexBelowR < height * width && !worldData[indexBelowR])
        //         {
        //             worldData[index] = 0;
        //             worldData[indexBelowR] = 1;
        //         }
        //         else if (indexBelowL > 0 && indexBelowL < height * width && !worldData[indexBelowL])
        //         {
        //             worldData[index] = 0;
        //             worldData[indexBelowL] = 1;
        //         }
        //     }
        // }
    }

    glm::vec2 getIndexAt(float x, float y)
    {
        int indexX = glm::round((x / worldWidth) * width + ((float)width - 1) / 2);
        indexX = glm::clamp(indexX, 0, width - 1);
        int indexY = glm::round((y / worldHeight) * height + ((float)height - 1) / 2);
        indexY = glm::clamp(indexY, 0, height - 1);

        return glm::vec2(indexX, indexY);
    }

    void setPixel(int x, int y, int value)
    {
        int index = glm::clamp(y, 0, height - 1) * width + glm::clamp(x, 0, width - 1);
        worldData[index] = value;
    }

    int getPixelAt(float x, float y)
    {
        glm::vec2 pos = getIndexAt(x, y);
        int index = pos.y * width + pos.x;
        return worldData[index];
    }

    ~World()
    {
        delete [] worldData;
    }
};

#endif