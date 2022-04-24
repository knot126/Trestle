/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * OpenGL Utilities
 * 
 * Kept in a seprate file for organisational purposes
 */

#ifndef TRESTLE_OPENGL_UTILITIES_INCLUDED
#define TRESTLE_OPENGL_UTILITIES_INCLUDED

GLuint gl_load_shader(char* filename, GLenum type) {
	/**
	 * Load and create an OpenGL shader `filename`, of type `type`.
	 */
	
	uint8_t* shader_source = DgFileLoad(filename, NULL);
	
	if (!shader_source) {
		printf("Error: Failed to load shader '%s'.\n", filename);
		return 0;
	}
	
	// Construct the shader source string from some strings and a file
	GLchar *strings[3];
	GLint strings_lengths[3];
	
	// Add version string
	strings[0] = "#version 330 core\n";
	
	// Add shader type string
	switch (type) {
		case GL_VERTEX_SHADER:
			strings[1] = "#define VERTEX\n\n";
			break;
		case GL_FRAGMENT_SHADER:
			strings[1] = "#define FRAGMENT\n\n";
			break;
		case GL_GEOMETRY_SHADER:
			strings[1] = "#define GEOMETRY\n\n";
			break;
	}
	
	// Add data string
	strings[2] = (GLchar *) shader_source;
	
	// Set string lengths
	strings_lengths[0] = strlen(strings[0]);
	strings_lengths[1] = strlen(strings[1]);
	strings_lengths[2] = strlen(strings[2]);
	
	// Create shader and load source
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 3, (const GLchar* const *) strings, (const GLint *) strings_lengths);
	
	// Cleanup source (it is not needed after pushing source)
	DgFree(shader_source);
	
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
	
	/// @todo ^^ Resource leak ??
	
	return shader;
}

void gl_error_check(char* file, int line) {
	GLenum e = glGetError();
	
	if (e) {
		char *msg;
		
		switch (e) {
			case GL_INVALID_ENUM:
				msg = "OpenGL: Invalid enum: a bad enum was passed to a function.";
				break;
			case GL_INVALID_VALUE:
				msg = "OpenGL: Invalid value: a bad value was passed to a function.";
				break;
			case GL_INVALID_OPERATION:
				msg = "OpenGL: Invalid operation: the operation is not allowed in the current state.";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				msg = "OpenGL: Framebuffer not ready: the framebuffer was not ready for the operation.";
				break;
			case GL_OUT_OF_MEMORY:
				msg = "OpenGL: Out of memory: the system has run out of memory.";
				break;
		}
		
		DgLog(DG_LOG_ERROR, "%s:%d: %s", file, line, msg);
	}
}

uint32_t graphicsLoadShader(DgOpenGLContext *gl, char* source_path) {
	/**
	 * Load shader(s) from a GLSL shader file and mark it as the active program.
	 * 
	 * This now adds the program to the list of programs automatically.
	 * 
	 * Returns 0 on success, returns 1 on non-fatal failure, returns 2 on fatal
	 * failure where the game needs to exit (because resources have been lost)
	 */
	
	// Vertex Shader
	GLuint vertex = gl_load_shader(source_path, GL_VERTEX_SHADER);
	if (!vertex) {
		return 1;
	}
	
	(gl->shader_count)++;
	gl->shaders = DgRealloc(gl->shaders, sizeof(GLuint) * gl->shader_count);
	
	if (!gl->shaders) {
		DgLog(DG_LOG_ERROR, "Failed to allocate memory for shaders list.");
		return 2;
	}
	
	gl->shaders[gl->shader_count - 1] = vertex;
	
	// Fragment Shader
	GLuint fragment = gl_load_shader(source_path, GL_FRAGMENT_SHADER);
	if (!fragment) {
		return 1;
	}
	
	(gl->shader_count)++;
	gl->shaders = DgRealloc(gl->shaders, sizeof(GLuint) * gl->shader_count);\
	
	if (!gl->shaders) {
		DgLog(DG_LOG_ERROR, "Failed to allocate memory for shaders list.");
		return 2;
	}
	
	gl->shaders[gl->shader_count - 1] = fragment;
	
	/* *** Creating the actual program *** */
	
	GLuint id = glCreateProgram();
	
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	
	glLinkProgram(id);
	
	// Check for any errors making program
	GLint link_stat;
	glGetProgramiv(id, GL_LINK_STATUS, &link_stat);
	
	if (!link_stat) {
		char mesg[1536];
		glGetProgramInfoLog(id, 1536, NULL, mesg);
		if (mesg[0]) {
			printf("%s\n", mesg);
		}
		return 1;
	}
	
	glUseProgram(id);
	
	// Add to the list
	++(gl->programs_count);
	gl->programs = DgRealloc(gl->programs, sizeof(GLuint) * gl->programs_count);
	gl->programs[gl->programs_count - 1] = id;
	
	// Free shader, keep program, not needed anymore
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
	return 0;
}

#endif
