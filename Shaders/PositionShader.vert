#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;

out vec3 fCol;

void main(){
  gl_Position.xyz = vPos;
  gl_Position.w = 1.f;
  fCol = vCol;
}
