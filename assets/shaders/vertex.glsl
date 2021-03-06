#version 420 core

uniform mat4 xform;

in vec3 position;
in vec2 texturepos;
in vec3 colour;

out vec2 Texture;
out vec3 Colour;

void main() {
	Colour = colour;
	Texture = texturepos;
	gl_Position = xform * vec4(position, 1.0);
}
