#ifndef _MATRIX_H
#define _MATRIX_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

void matrix_create_identity(GLfloat * matrix);
void matrix_create_perspective(GLfloat * matrix, float fovy, float aspect, float near, float far);
void matrix_camera(GLfloat * matrix, float cam_x, float cam_y, float cam_z, float center_x, float center_y, float center_z, float cam_roll);
void matrix_translate(GLfloat * matrix, float x, float y, float z);
void matrix_rotate_x(GLfloat * matrix, float angle);
void matrix_rotate_y(GLfloat * matrix, float angle);
void matrix_rotate_z(GLfloat * matrix, float angle);
void matrix_rotate(GLfloat * matrix, float angle_x, float angle_y, float angle_z);
void matrix_multiply(GLfloat * matrix, GLfloat * factor);
void matrix_print(GLfloat * matrix);

#endif
