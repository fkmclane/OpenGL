#ifndef _COMMON_H
#define _COMMON_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#define PI 3.1415926535898
#define EPSILON 0.01

unsigned int arrlen(const char ** arr);
char * file_read(const char * filename);
void log_print(GLuint object);

#endif
