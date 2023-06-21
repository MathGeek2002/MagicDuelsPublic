#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include <cmath>
#include "Random.hpp"

int PERLIN_YWRAPB = 4;
long long PERLIN_YWRAP = (1 << PERLIN_YWRAPB);
long long PERLIN_ZWRAPB = 8;
long long PERLIN_ZWRAP = (1 << PERLIN_ZWRAPB);
long long PERLIN_SIZE = 4095;

int perlin_octaves = 4; // default to medium smooth
float perlin_amp_falloff = 0.5f; // 50% reduction/octave

float scaled_cosine(float arg)
{
    return 0.5f * (1.0f - cos(arg * M_PI));
}

struct lcg {
    const unsigned long long m = 4294967296;
    const unsigned long long a = 1664525;
    const unsigned long long c = 1013904223;
    int seed, z;

    void SetSeed()
    {
        z = seed = (int)(Random::Range(0.f,1.f) * m);
    }
    
    void setSeed(int val) {
        z = seed = val;
    }

    int getSeed() {
        return seed;
    }

    float rand() {
        z = (a * z + c) % m;
        return (float)z / m;
    }
};

class PerlinNoise
{
private:
    float * perlin;
    lcg _lcg;

public:

    float noise(float x,float y = 0.f,float z = 0.f)
    {
        if (perlin == nullptr) 
        {
            perlin = new float[PERLIN_SIZE + 1];

            for (int i = 0; i < PERLIN_SIZE + 1; i++)
            {
                perlin[i] = Random::Range(0.f,1.f);
            }
        }
        
        x = abs(x);
        y = abs(y);
        z = abs(z);

        int xi = floor(x);
        int yi = floor(y);
        int zi = floor(z);
        float xf = x - xi;
        float yf = y - yi;
        float zf = z - zi;
        float rxf, ryf;

        float r = 0.f;
        float ampl = 0.5f;

        float n1 = 0.f, n2 = 0.f, n3 = 0.f;

        for (int o = 0; o < perlin_octaves; o++) 
        {
            int of = xi + (yi << PERLIN_YWRAPB) + (zi << PERLIN_ZWRAPB);

            rxf = scaled_cosine(xf);
            ryf = scaled_cosine(yf);

            n1 = perlin[(of & PERLIN_SIZE)];
            n1 += rxf * (perlin[((of + 1) & PERLIN_SIZE)] - n1);
            n2 = perlin[((of + PERLIN_YWRAP) & PERLIN_SIZE)];
            n2 += rxf * (perlin[((of + PERLIN_YWRAP + 1) & PERLIN_SIZE)] - n2);
            n1 += ryf * (n2 - n1);

            of += PERLIN_ZWRAP;
            n2 = perlin[(of & PERLIN_SIZE)];
            n2 += rxf * (perlin[((of + 1) & PERLIN_SIZE)] - n2);
            n3 = perlin[((of + PERLIN_YWRAP) & PERLIN_SIZE)];
            n3 += rxf * (perlin[((of + PERLIN_YWRAP + 1) & PERLIN_SIZE)] - n3);
            n2 += ryf * (n3 - n2);

            n1 += scaled_cosine(zf) * (n2 - n1);

            r += n1 * ampl;
            ampl *= perlin_amp_falloff;
            xi <<= 1;
            xf *= 2;
            yi <<= 1;
            yf *= 2;
            zi <<= 1;
            zf *= 2;

            if (xf >= 1.0) {
                xi++;
                xf--;
            }
            if (yf >= 1.0) {
                yi++;
                yf--;
            }
            if (zf >= 1.0) {
                zi++;
                zf--;
            }
        }
        return r;
    }

    void noiseDetail(float lod,float falloff) 
    {
        if (lod > 0.f) {
            perlin_octaves = lod;
        }
        if (falloff > 0.f) {
            perlin_amp_falloff = falloff;
        }
    }

    void noiseSeed(int seed) 
    {
        _lcg.setSeed(seed);
        perlin = new float[PERLIN_SIZE + 1];
        for (int i = 0; i < PERLIN_SIZE + 1; i++) 
        {
            perlin[i] = _lcg.rand();
        }
    }
};

PerlinNoise perlinNoise;

#endif