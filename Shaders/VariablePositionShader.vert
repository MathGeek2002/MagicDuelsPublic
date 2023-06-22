#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;
layout(location = 2) in vec3 vTrn;
layout(location = 3) in float vSpd;

uniform float uTime;

out vec3 fCol;

void main(){
  gl_Position.xyz = vPos + vTrn * sin( vSpd * uTime );
  gl_Position.w = 1.f;
  fCol = vCol;
}
