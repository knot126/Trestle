/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * GUI Shader Source Code
 */

#ifdef VERTEX 
uniform mat4 transform;

in vec2 aPos;
in vec2 aTex;
in vec4 aCol;

out vec2 TexturePos;
out vec4 Colour;

void main() {
	TexturePos = aTex;
	Colour = aCol;
	gl_Position = transform * vec4(aPos, 0.0, 1.0);
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
