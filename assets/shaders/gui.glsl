/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GUI Shader Source Code
 */

#ifdef VERTEX 
in vec2 aPos;
in vec2 aTex;
in vec4 aCol;

out vec2 TexturePos;
out vec4 Colour;

void main() {
	Texture = aTex;
	Colour = aCol;
	gl_Position = aPos;
}
#endif

#ifdef FRAGMENT
uniform sampler2D Texture;

in vec2 TexturePos;
in vec4 Colour;

out vec4 colour;

void main() {
	colour = texture(Texture, TexturePos) * Colour;
}
#endif
