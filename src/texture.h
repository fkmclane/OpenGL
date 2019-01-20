#ifndef _TEXTURE_H
#define _TEXTURE_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

GLuint texture_load(const char * filename);

#endif
