#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec3 texColor;

void main()
{
    vec4 sampled = vec4(1.0,1.0,1.0, texture(tex,TexCoords).r);
    color = vec4(texColor, 1.0) * sampled;
}