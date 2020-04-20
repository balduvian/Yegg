#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in float vertexEggShade;

uniform mat4 model;
uniform mat4 projView;

out float eggShade;

void main() {
    gl_Position = (projView * model) * vec4(vertex, 1);

    eggShade = vertexEggShade;
}
