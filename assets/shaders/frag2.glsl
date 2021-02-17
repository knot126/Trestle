#version 420 core

uniform vec3 solid;

in vec3 Colour;

out vec4 out_colour;

void main() {
	// 0.4, 0.6, 0.2
	out_colour = vec4(solid, 1.0);
}
