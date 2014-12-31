#ifndef _OPENGL_H
#define _OPENGL_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "object.h"

GLfloat perspective[16];
GLfloat world_matrix[16];

float x, y, z, rot_x, rot_y, rot_z;
float fov, width, height, near, far;
object_list * objects;

int initGL(float fov, float near, float far, float width, float height, float cam_x, float cam_y, float cam_z, float center_x, float center_y, float center_z, float cam_roll, object_list * objects);
void deinitGL();
void resizeGL(int width, int height);
void animateGL(float secs);
void updateGL();
void translateGL(float dx, float dy, float dz);
void rotateGL(float drot_x, float drot_y, float drot_z);

#endif
