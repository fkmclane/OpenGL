#include <math.h>
#include <stdio.h>

#include "matrix.h"
#include "scene.h"

#include "world.h"

int world_init(float fov_v, float near_v, float far_v, float width, float height, float cam_x_v, float cam_y_v, float cam_z_v, float center_x, float center_y, float center_z, float cam_roll) {
	// set up OpenGL parameters
	glEnable(GL_BLEND); // for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // also for transparancy
	glEnable(GL_DEPTH_TEST); // for 3D

	// set a black background
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// set up view matrix
	fov = fov_v;
	near = near_v;
	far = far_v;
	world_resize(width, height);

	// prepare world variables
	cam_x = cam_x_v;
	cam_y = cam_y_v;
	cam_z = cam_z_v;

	x = 0;
	y = 0;
	z = 0;

	rot_x = atan2(y - center_y, z - center_z);
	rot_y = -atan2(x - center_x, z - center_z);
	rot_z = cam_roll;

	// create world matrix
	world_matrix();

	// initialize scene
	if (scene_init()) {
		fprintf(stderr, "Error setting up scene\n");
		return 1;
	}

	// run everything once to initialize it
	world_animate(0);
	world_update();

	return 0;
}

void world_deinit() {
	scene_deinit();
}

void world_resize(int width_v, int height_v) {
	width = width_v;
	height = height_v;

	// reset view matrix
	matrix_create_perspective(perspective, fov, width / height, near, far);
	for (int i = 0; i < objects->num; i++)
		glUniformMatrix4fv(objects->list[i]->uniform_perspective, 1, GL_FALSE, perspective);

	glViewport(0, 0, width, height);
}

void world_animate(float secs) {
	scene_animate(secs);
}

void world_update() {
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_update();

	// apply camera matrix
	for (int i = 0; i < objects->num; i++)
		glUniformMatrix4fv(objects->list[i]->uniform_world_transform, 1, GL_FALSE, world);
}

void world_matrix() {
	matrix_create_identity(world);
	matrix_rotate(world, rot_x, rot_y, rot_z);
	matrix_translate(world, -x, -y, -z);
	matrix_translate(world, -cam_x, -cam_y, -cam_z);
}

void world_translate(float dx, float dy, float dz) {
	x += dx;
	y += dy;
	z += dz;

	world_matrix();
}

void world_rotate(float drot_x, float drot_y, float drot_z) {
	rot_x += drot_x;
	rot_y += drot_y;
	rot_z += drot_z;

	world_matrix();
}
