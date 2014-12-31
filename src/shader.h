#ifndef _SHADER_H
#define _SHADER_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

GLuint loadProgram(const char * vert_filename, const char * frag_filename);
GLuint loadShader(const char * file, GLenum type);
GLint bindAttrib(GLuint program, const char * name);
GLint bindUniform(GLuint program, const char * name);

#endif
