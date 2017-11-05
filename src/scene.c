#include <stdio.h>

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "common.h"
#include "matrix.h"
#include "object.h"
#include "world.h"

#include "scene.h"

object_list * objects = & (object_list) {
	.list = & (object *) {
		& (object) {
			.filename = "object.obj",
			.texture_filename = "crate.ppm",
			.shaders = {"object.vert.glsl", "object.frag.glsl", NULL},
			.x_rotation_speed = PI / 4,
			.y_rotation_speed = PI / 4,
			.z_rotation_speed = PI / 4,
		},
	},
	.num = sizeof(objects->list) / sizeof(object *),
};

int scene_init() {
	// init objects
	if(!object_init_list(objects)) {
		fprintf(stderr, "Error initializing objects\n");
		object_destroy_list(objects);
		return 1;
	}

	return 0;
}

void scene_deinit() {
	object_destroy_list(objects);
}

void scene_update() {
	// put objects in buffer
	object_display_list(objects);
}

void scene_animate(float secs) {
	// set the animation
	for(int i = 0; i < objects->num; i++) {
		glUseProgram(objects->list[i]->program);
		GLfloat matrix[16];
		matrix_create_identity(matrix); // load up the identity
		if(objects->list[i]->x_rotation_speed)
			matrix_rotate_x(matrix, secs * objects->list[i]->x_rotation_speed); // and modify the matrix to rotate the object over time
		if(objects->list[i]->y_rotation_speed)
			matrix_rotate_y(matrix, secs * objects->list[i]->y_rotation_speed);
		if(objects->list[i]->z_rotation_speed)
			matrix_rotate_z(matrix, secs * objects->list[i]->z_rotation_speed);
		glUniformMatrix4fv(objects->list[i]->uniform_obj_transform, 1, GL_FALSE, matrix); // apply the transformation matrix
	}
}
