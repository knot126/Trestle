/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Main shader source code
 */

#ifdef VERTEX
uniform mat4 model;
uniform mat4 camera;
uniform mat4 proj;

in vec3 position;
in vec2 texturepos;
in vec3 colour;

out vec2 Texture;
out vec3 Colour;

void main() {
	Colour = colour;
	Texture = texturepos;
	gl_Position = proj * (camera * (model * vec4(position, 1.0)));
}
#endif

#ifdef FRAGMENT
uniform float mixValue;
uniform sampler2D image;
uniform sampler2D image2;

in vec2 Texture;
in vec3 Colour;

out vec4 out_colour;

void main() {
	out_colour = mix(texture(image, Texture), texture(image2, Texture), mixValue) * vec4(Colour, 1.0);
}
#endif
