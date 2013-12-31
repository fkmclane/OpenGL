#ifndef _SHADER_H
#define _SHADER_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

GLuint loadShader(const char * file, GLenum type);

#endif
