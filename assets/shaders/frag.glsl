#version 420 core

uniform sampler2D image;

in vec2 Texture;
in vec3 Colour;

out vec4 out_colour;

void main() {
	out_colour = texture(image, Texture);
}
