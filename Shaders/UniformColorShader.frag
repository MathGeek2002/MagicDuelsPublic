#version 330 core

in vec3 fCol;

uniform vec3 uCol;

out vec3 oCol;

void main(){
  oCol = (uCol + fCol) * 0.5f;
}
