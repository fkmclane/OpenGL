#ifndef _WORLD_H
#define _WORLD_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "object.h"

GLfloat perspective[16];
GLfloat world[16];

GLfloat temp[16];

float cam_x, cam_y, cam_z, x, y, z, rot_x, rot_y, rot_z;
float fov, width, height, near, far;

int world_init(float fov, float near, float far, float width, float height, float cam_x, float cam_y, float cam_z, float center_x, float center_y, float center_z, float cam_roll);
void world_deinit();
void world_resize(int width, int height);
void world_animate(float secs);
void world_update();
void world_matrix();
void world_translate(float dx, float dy, float dz);
void world_rotate(float drot_x, float drot_y, float drot_z);

#endif
