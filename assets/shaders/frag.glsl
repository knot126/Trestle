#version 420 core

uniform sampler2D image;
uniform sampler2D image2;

in vec2 Texture;
in vec3 Colour;

out vec4 out_colour;

void main() {
	out_colour = texture(image, Texture) * vec4(Colour, 1.0);
}
