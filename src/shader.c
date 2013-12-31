#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "shader.h"

GLuint loadShader(const char * filename, GLenum type) {
	const GLchar * glsl = fileContents(filename); //Get file contents
	if(!glsl) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return 0;
	}

	GLint ok = GL_FALSE; //Flag for checking compile status
	GLuint shader = glCreateShader(type); //Create the shader
	glShaderSource(shader, 1, &glsl, NULL); //Load the source
	free((void *)glsl); //Avoid memory leaks
	glCompileShader(shader); //And compile the source
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if(ok == GL_FALSE) {
		fprintf(stderr, "Error compiling shader %s: ", filename);
		printLog(shader); //Print the log if something goes bad
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}
