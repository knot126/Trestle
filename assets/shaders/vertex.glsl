#version 420 core

uniform vec3 distort;

in vec3 position;
in vec3 colour;

out vec3 Colour;

void main() {
	Colour = colour;
	gl_Position = vec4(position + distort, 1.0);
}
