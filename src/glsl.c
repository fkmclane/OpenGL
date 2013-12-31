#include <stdio.h>

#include "glsl.h"

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
