/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Text shader source code
 */

#ifdef VERTEX
in vec2 position;
in vec2 texpos;

out vec2 Texture;
out vec3 Colour;

void main() {
	Texture = texpos;
	
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT
uniform sampler2D font;

in vec2 Texture;
in vec3 Colour;

out vec4 out_colour;

void main() {
	out_colour = texture(font, Texture) * vec4(Colour, 1.0);
}
#endif
