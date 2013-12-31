#ifndef _TEXTURE_H
#define _TEXTURE_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

GLuint loadTexture(const char * filename);

#endif
