#version 330 core

in vec3 fCol;
in vec3 fPos;
in vec3 fNor;
in vec2 fTex;

out vec4 fragCol;

struct Material{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

vec4 texCol;

void main(){

  texCol = texture(material.diffuse, fTex);

  if( texCol.a < 0.1 )
    discard;
  
  fragCol = texCol;
}
