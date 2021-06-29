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

void main() {
	Texture = texpos;
	
	gl_Position = vec4(position, 0.0, 1.0);
}
#endif

#ifdef FRAGMENT
uniform sampler2D font;
uniform vec4 colour;

in vec2 Texture;

out vec4 out_colour;

void main() {
	vec4 inter = texture(font, Texture) * colour;
	
	if (inter.a <= 0.0) discard;
	
	out_colour = inter;
}
#endif
