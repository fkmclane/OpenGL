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
	GLfloat coords[3]; //Coordinates
	GLfloat texture_coords[2]; //Texture Coordinates
} vertex;

typedef struct {
	GLuint vbo, ibo; //Vertex Buffer Object and Index Buffer Object
} buffer;

typedef struct {
	const char * filename, * texture_filename, * vert_shader, * frag_shader; //Files
	float x_rotation_speed, y_rotation_speed, z_rotation_speed;
	GLuint program; //Shader program
	buffer buffers; //Vertex Buffer and Index Buffer Objects
	GLuint texture; //Texture
	GLint attribute_coords, attribute_texture_coords; //Attributes
	GLint uniform_obj_transform, uniform_world_transform, uniform_perspective, uniform_texture; //Uniforms
} object;

typedef struct {
	object ** list;
	int num;
} object_list;

int initObjects(object_list * objects);
int initObject(object * current_object);
buffer loadObject(const char * filename);
void displayObjects(object_list * objects);
void displayObject(object * current_object);
void destroyObjects(object_list * objects);
void destroyObject(object * current_object);

#endif
