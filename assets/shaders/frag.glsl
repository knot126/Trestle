#version 420 core

uniform float mixValue;
uniform sampler2D image;
uniform sampler2D image2;

in vec2 Texture;
in vec3 Colour;

out vec4 out_colour;

void main() {
	out_colour = mix(texture(image, Texture), texture(image2, Texture), mixValue) * vec4(Colour, 1.0);
}
