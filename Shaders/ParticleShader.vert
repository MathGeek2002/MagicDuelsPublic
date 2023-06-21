#version 330 core
// Vertex attributes
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;
layout(location = 2) in vec3 vNor;
layout(location = 3) in vec2 vTex;
layout(location = 4) in vec2 vInsPos;
layout(location = 5) in vec3 vInsCol;

// Uniforms
uniform mat4 model;
uniform mat4 view;

// Outs
out vec3 fPos;
out vec3 fCol;
out vec3 fNor;
out vec2 fTex;

// Main function
void main(){
  vec4 _vPos = model * vec4( vPos.x + vInsPos.x,vPos.y + vInsPos.y, vPos.z, 1.0 );

  gl_Position = view * _vPos;

  fPos = vec3(_vPos);
  fCol = vInsCol;
  fNor = vNor;
  fTex = vTex;
}
