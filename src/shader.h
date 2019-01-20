#ifndef _SHADER_H
#define _SHADER_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

GLuint program_load(const char ** shader_filenames, unsigned int shader_num);
GLuint shader_load(const char * file, GLenum type);
GLint attrib_bind(GLuint program, const char * name);
GLint uniform_bind(GLuint program, const char * name);

#endif
