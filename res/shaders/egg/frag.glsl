#version 330 core

uniform vec4 inColor;

out vec4 color;

in float eggShade;

void main() {
	color = vec4(inColor.xyz * eggShade, 1);
}
