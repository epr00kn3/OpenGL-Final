#version 330 core
in vec2 uv;
out vec4 FragColor;

uniform vec3 topColor;
uniform vec3 bottomColor;

void main()
{
   FragColor = vec4(mix(bottomColor, topColor, uv.y), 1.0);
}