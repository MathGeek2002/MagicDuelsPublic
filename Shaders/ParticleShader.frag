#version 330 core

// input data from previous stage
in vec3 fPos;
in vec3 fCol;
in vec3 fNor;
in vec2 fTex;

// output color
out vec3 col;

//uniform vec3 color;

void main(){

    col = fCol;

}
