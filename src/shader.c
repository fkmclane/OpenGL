#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "shader.h"

GLuint program_load(const char ** shader_filenames, unsigned int shader_num) {
	// shaders
	GLuint shaders[shader_num];

	// load and attach shaders
	for (unsigned int i = 0; i < shader_num; i++) {
		GLenum type;

		if (strstr(shader_filenames[i], ".vert.glsl") == shader_filenames[i] + strlen(shader_filenames[i]) - strlen(".vert.glsl")) {
			type = GL_VERTEX_SHADER;
		}
		else if (strstr(shader_filenames[i], ".frag.glsl") == shader_filenames[i] + strlen(shader_filenames[i]) - strlen(".frag.glsl")) {
			type = GL_FRAGMENT_SHADER;
		}
		else {
			fprintf(stderr, "Invalid shader name: %s\n", shader_filenames[i]);
			return 0;
		}

		shaders[i] = shader_load(shader_filenames[i], type);

		if (shaders[i] == 0)
			return 0;
	}

	// create program
	GLuint program = glCreateProgram();

	for (unsigned int i = 0; i < shader_num; i++)
		glAttachShader(program, shaders[i]);

	// link program
	glLinkProgram(program);

	// delete shaders
	for (unsigned int i = 0; i < shader_num; i++)
		glDeleteShader(shaders[i]);

	// check program
	GLint ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	if (ok == GL_FALSE) {
		fprintf(stderr, "Error linking program: ");
		log_print(program);
		return 0;
	}

	return program;
}

GLuint shader_load(const char * filename, GLenum type) {
	// get glsl code
	GLchar * glsl = file_read(filename); // get file contents
	if (!glsl) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return 0;
	}

	// create shader
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&glsl, NULL);

	free(glsl);

	// compile shader
	glCompileShader(shader);

	// check shader
	GLint ok = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (ok == GL_FALSE) {
		fprintf(stderr, "Error compiling shader %s: ", filename);
		log_print(shader); // print the log if something goes bad
		glDeleteShader(shader);
		return 0;
	}

	return shader; // make sure to glDeleteShader this later
}

GLint attrib_bind(GLuint program, const char * name) {
	GLint attrib = glGetAttribLocation(program, name);
	if (attrib < 0)
		fprintf(stderr, "Couldn't bind uniform %s\n", name);

	return attrib;
}

GLint uniform_bind(GLuint program, const char * name) {
	GLint uniform = glGetUniformLocation(program, name);
	if (uniform < 0)
		fprintf(stderr, "Couldn't bind uniform %s\n", name);

	return uniform;
}
