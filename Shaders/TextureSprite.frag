#version 330 core

in vec3 fCol;
in vec3 fPos;
in vec3 fNor;
in vec2 fTex;

out vec4 fragCol;

uniform sampler2D tex;
uniform vec2 offset;
uniform vec2 scale;
uniform vec3 color;
uniform vec2 flip;

vec4 texCol;

void main(){

  vec2 texPos;
  texPos.x = (flip.x > 0) ? -fTex.x : fTex.x;
  texPos.y = (flip.y > 0) ? -fTex.y : fTex.y;

  texCol = texture(tex, (texPos / scale) + offset);
  texCol.r *= color.r;
  texCol.g *= color.g;
  texCol.b *= color.b;
  
  fragCol = texCol;
}
