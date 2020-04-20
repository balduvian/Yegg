#version 330 core

layout (location = 0) in vec3 vertex;

uniform mat4 model;
uniform mat4 projView;

out vec2 circlePos;
out float circleLevel;

void main() {
    gl_Position = (projView * model) * vec4(vertex, 1);

    circlePos = vertex.xy;

    circleLevel = sqrt(0.25);
}
