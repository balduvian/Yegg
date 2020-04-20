#version 330 core

uniform vec4 inColor;

out vec4 color;

in vec2 circlePos;
in float circleLevel;

void main() {
	if (sqrt(pow(circlePos.x - 0.5, 2) + pow(circlePos.y - 0.5, 2)) > circleLevel) {
		discard;

	} else {
		color = inColor;
	}
}
