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

uniform float LightFactor/* = 0.15*/;
uniform vec3 LightDirection/* = vec3(-0.8, 0.8, 0.3)*/;

in vec3 position;
in vec2 texpos;
in vec3 colour;

out vec2 Texture;
out vec3 Colour;

float LightResolve() {
	return (1.0 - LightFactor) + LightFactor * (position.x * LightDirection.x + position.y * LightDirection.y + position.z * LightDirection.z);
}

void main() {
	Texture = texpos;
	Colour = colour * LightResolve();
	gl_Position = proj * camera * model * vec4(position, 1.0);
}
#endif

#ifdef FRAGMENT
uniform sampler2D image;

in vec2 Texture;
in vec3 Colour;

out vec4 out_colour;

void main() {
	out_colour = texture(image, Texture) * vec4(Colour, 1.0);
}
#endif
