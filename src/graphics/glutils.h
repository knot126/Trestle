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
	/* 
	 * Load and create an OpenGL shader `filename`, of type `type`.
	 */
	char* path = DgEvalPath(filename);
	
	DgLoadBinaryFileInfo* shader_source = DgLoadBinaryFile(path);
	
	if (!shader_source) {
		printf("Error: Failed to load shader '%s'.\n", filename);
		return 0;
	}
	
	// Construct the shader source string from some strings and a file
	GLchar *strings[3];
	GLint strings_lengths[3];
	
	strings[0] = "#version 330 core\n";
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
	strings[2] = (GLchar *) shader_source->data;
	
	strings_lengths[0] = strlen(strings[0]);
	strings_lengths[1] = strlen(strings[1]);
	strings_lengths[2] = shader_source->size;
	
	// Create shader and load source
	GLuint shader = glCreateShader(type);
	glShaderSource(shader,
		3, 
		(const GLchar* const *) strings, 
		(const GLint *) strings_lengths);
	
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
	
	//printf("Loaded shader from '%s'.\n", filename);
	
	return shader;
}

void gl_error_check(char* file, int line) {
	GLenum e = glGetError();
	
	if (e) {
		printf("At line [%s] in [%d]: ", file, line);
	}
	
	switch (e) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("Error: OpenGL: Invalid enum: a bad enum was passed to a function.\n");
			break;
		case GL_INVALID_VALUE:
			printf("Error: OpenGL: Invalid value: a bad value was passed to a function.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("Error: OpenGL: Invalid operation: the operation is not allowed in the current state.\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("Error: OpenGL: Framebuffer not ready: the framebuffer was not ready for the operation.\n");
			break;
		case GL_OUT_OF_MEMORY:
			printf("Error: OpenGL: Out of memory: the system has run out of memory.\n");
			break;
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
		char mesg[1024];
		glGetProgramInfoLog(id, 1024, NULL, mesg);
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
	
	return 0;
}

static void graphicsLoadTextureFromFile(DgOpenGLContext *gl, char *path, GLenum active_texture) {
	(gl->textures_count)++;
	gl->textures = (GLuint *) DgRealloc(gl->textures, sizeof(GLuint) * gl->textures_count);
	
	if (!gl->textures) {
		DgLog(DG_LOG_FATAL, "Texture list allocation failure");
		return;
	}
	
	glGenTextures(1, &gl->textures[gl->textures_count - 1]);
	
	glActiveTexture(active_texture);
	glBindTexture(GL_TEXTURE_2D, gl->textures[gl->textures_count - 1]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	DgImageInfo image = DgLoadImage(path);
	
	if (!image.data) {
		DgFail("Error: Failed to load texture.\n", 100);
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	DgFreeImage(&image);
}

static void graphicsLoadTextureFromBuffer(DgOpenGLContext *gl, DgBitmap *bitmap, GLenum active_texture) {
	(gl->textures_count)++;
	gl->textures = (GLuint *) DgRealloc(gl->textures, sizeof(GLuint) * gl->textures_count);
	
	if (!gl->textures) {
		DgLog(DG_LOG_FATAL, "Texture list allocation failure");
		return;
	}
	
	glGenTextures(1, &gl->textures[gl->textures_count - 1]);
	
	glActiveTexture(active_texture);
	glBindTexture(GL_TEXTURE_2D, gl->textures[gl->textures_count - 1]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLenum mode;
	
	if (bitmap->chan == 1) mode = GL_RED;
	if (bitmap->chan == 2) mode = GL_RG;
	if (bitmap->chan == 3) mode = GL_RGB;
	if (bitmap->chan == 4) mode = GL_RGBA;
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap->width, bitmap->height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap->src);
	glGenerateMipmap(GL_TEXTURE_2D);
}

static void gl_set_format(DgOpenGLContext *gl) {
	// Tell OpenGL about this vertex data
	GLint attr_Position = glGetAttribLocation(gl->programs[0], "position");
	GLint attr_Texture = glGetAttribLocation(gl->programs[0], "texpos");
	GLint attr_Colour = glGetAttribLocation(gl->programs[0], "colour");
	
	if (attr_Position < 0) {
		DgFail("Error: No attribute 'position'.\n", 100);
	}
	
	if (attr_Texture < 0) {
		DgFail("Error: No attribute 'texpos'.\n", 100);
	}
	
	if (attr_Colour < 0) {
		DgFail("Error: No attribute 'colour'.\n", 100);
	}
	
	glVertexAttribPointer(attr_Position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(attr_Position);
	
	gl_error_check(__FILE__, __LINE__);
	
	glVertexAttribPointer(attr_Texture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(attr_Texture);
	
	gl_error_check(__FILE__, __LINE__);
	
	glVertexAttribPointer(attr_Colour, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
	glEnableVertexAttribArray(attr_Colour);
	
	gl_error_check(__FILE__, __LINE__);
}

#endif
