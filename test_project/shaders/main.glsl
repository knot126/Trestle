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

const float near = 0.001;
const float far = 100;
const float left = -near;
const float right = near;
const float bottom = -near;
const float top = near;

float LightResolve() {
	return min((1.0 - LightFactor) + LightFactor * (position.x * LightDirection.x + position.y * LightDirection.y + position.z * LightDirection.z), 1.0);
}

float Map(float t, float a, float b) {
	/**
	 * Linear map from t in [a, b] to t in [-1, 1]
	 */
	
	return (2.0 * (t - a)) / (b - a) - 1;
}

void main() {
	Texture = texpos;
	Colour = colour * LightResolve();
	gl_Position = proj * camera * model * vec4(position, 1.0);
	
// 	gl_Position.z = -Map(gl_Position.z, near, far);
// 	gl_Position.x = Map(gl_Position.x / gl_Position.z, left, right);
// 	gl_Position.y = Map(gl_Position.y / gl_Position.z, bottom, top);
// 	gl_Position.w = 1.0;
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
