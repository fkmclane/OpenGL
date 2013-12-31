#ifndef _GLSL_H
#define _GLSL_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

GLint bindAttrib(GLuint program, const char * name);
GLint bindUniform(GLuint program, const char * name);

#endif
