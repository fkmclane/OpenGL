#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "shader.h"

GLuint loadProgram(const char * vert_filename, const char * frag_filename) {
	//Load shaders
	GLuint vert_shader = loadShader(vert_filename, GL_VERTEX_SHADER);
	GLuint frag_shader = loadShader(frag_filename, GL_FRAGMENT_SHADER);
	if(vert_shader == 0 || frag_shader == 0)
		return 0;

	//Create program
	GLuint program = glCreateProgram();

	//Attach Shaders
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);

	//Link program
	glLinkProgram(program);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	//Check program
	GLint ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	if(ok == GL_FALSE) {
		fprintf(stderr, "Error linking program: ");
		printLog(program);
		return 0;
	}

	return program;
}

GLuint loadShader(const char * filename, GLenum type) {
	//Get glsl code
	GLchar * glsl = fileContents(filename); //Get file contents
	if(!glsl) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return 0;
	}

	//Create shader
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&glsl, NULL);

	free(glsl);

	//Compile shader
	glCompileShader(shader);

	//Check shader
	GLint ok = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if(ok == GL_FALSE) {
		fprintf(stderr, "Error compiling shader %s: ", filename);
		printLog(shader); //Print the log if something goes bad
		glDeleteShader(shader);
		return 0;
	}

	return shader; //Make sure to glDeleteShader this later
}

GLint bindAttrib(GLuint program, const char * name) {
	GLint attrib = glGetAttribLocation(program, name);
	if(attrib < 0)
		fprintf(stderr, "Couldn't bind uniform %s\n", name);

	return attrib;
}

GLint bindUniform(GLuint program, const char * name) {
	GLint uniform = glGetUniformLocation(program, name);
	if(uniform < 0)
		fprintf(stderr, "Couldn't bind uniform %s\n", name);

	return uniform;
}
