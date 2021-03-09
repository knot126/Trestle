#version 420 core

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
	gl_Position = proj * camera * model * vec4(position, 1.0);
}
