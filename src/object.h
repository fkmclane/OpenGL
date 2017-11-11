#ifndef _OBJECT_H
#define _OBJECT_H

#ifdef GLEW
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

typedef struct {
	GLfloat coords[3]; // coordinates
	GLfloat texture_coords[2]; // texture coordinates
	GLfloat normal[3]; // normal vector
} vertex;

typedef struct {
	GLuint vbo, ibo; // vertex buffer object and index buffer object
} buffer;

typedef struct {
	const char * filename, * texture_filename; // files
	const char * shaders[64]; // shaders
	float x_rotation_speed, y_rotation_speed, z_rotation_speed;
	GLuint program; // shader program
	buffer buffers; // vertex buffer and index buffer objects
	GLuint texture; // texture
	GLint attribute_coords, attribute_texture_coords, attribute_normal; // attributes
	GLint uniform_obj_transform, uniform_world_transform, uniform_perspective, uniform_texture; // uniforms
} object;

typedef struct {
	object ** list;
	int num;
} object_list;

int object_init_list(object_list * objects);
int object_init(object * current_object);
buffer object_load(const char * filename);
void object_display_list(object_list * objects);
void object_display(object * current_object);
void object_destroy_list(object_list * objects);
void object_destroy(object * current_object);

#endif
