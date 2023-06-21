#version 330 core

in vec3 fCol;
in vec3 fPos;
in vec3 fNor;
in vec2 fTex;

out vec4 fragCol;

uniform vec2 offset;
uniform vec2 scale;
uniform vec3 color;

vec4 texCol;

void main(){


  fragCol.r *= color.r;
  fragCol.g *= color.g;
  fragCol.b *= color.b;

    fragCol.r = fCol.r;
    fragCol.g = fCol.g;
    fragCol.b = fCol.b;
}