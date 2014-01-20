#ifndef _MATRIX_H
#define _MATRIX_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

void createIdentity(GLfloat * matrix);
void createPerspectiveMatrix(GLfloat * matrix, float fovy, float aspect, float near, float far);
void cameraMatrix(GLfloat * matrix, float cam_x, float cam_y, float cam_z, float center_x, float center_y, float center_z, float cam_roll);
void translateMatrix(GLfloat * matrix, float x, float y, float z);
void xRotateMatrix(GLfloat * matrix, float angle);
void yRotateMatrix(GLfloat * matrix, float angle);
void zRotateMatrix(GLfloat * matrix, float angle);
void multiplyMatrices(GLfloat * matrix, GLfloat * factor);
void printMatrix(GLfloat * matrix);

#endif
