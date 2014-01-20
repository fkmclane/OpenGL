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

char * fileContents(const char * filename);
void printLog(GLuint object);

#endif
