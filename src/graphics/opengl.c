/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL-related graphics stuff
 */

#if !defined(DG_GLEW_INCLUDED_OK)
	#include "../glew/glew.h"
	#define DG_GLEW_INCLUDED_OK
#endif
#include <GLFW/glfw3.h>

#include "../util/alloc.h"
#include "../io/load.h"

#include "opengl.h"

GLuint gl_load_shader(char* filename, GLenum type) {
	/* 
	 * Load and create an OpenGL shader `filename`, of type `type`.
	 */
	char* path = DgEvalPath(filename);
	
	DgLoadBinaryFileInfo* shader_source = DgLoadBinaryFile(path);
	
	if (!shader_source) {
		printf("Failed to load shader '%s'.\n", filename);
		return 0;
	}
	
	// Create shader and load source
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, 
		(const GLchar* const *) &shader_source->data, 
		(const GLint *) &shader_source->size);
	
	// Cleanup path
	DgUnloadBinaryFile(shader_source);
	DgFree(path);
	
	// Compile the shader
	glCompileShader(shader);
	
	// Check that the shader was compiled successfully
	GLint status;
	char mesg[512];
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shader, 512, NULL, mesg);
	
	if (mesg[0]) {
		printf("%s\n", mesg);
	}
	
	if (status != GL_TRUE) {
		return 0;
	}
	
	printf("Loaded shader at '%s'.\n", filename);
	
	return shader;
}
