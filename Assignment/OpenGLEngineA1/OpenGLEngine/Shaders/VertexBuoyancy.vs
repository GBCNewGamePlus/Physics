#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float currentHeight;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec4 worldPosition = model * vec4(aPos, 1.0);
    currentHeight = worldPosition.y;
}