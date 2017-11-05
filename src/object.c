#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "shader.h"
#include "texture.h"
#include "object.h"

int object_init_list(object_list * objects) {
	for (int i = 0; i < objects->num; i++) {
		if (!object_init(objects->list[i]))
			return 0;
	}

	return 1;
}

int object_init(object * current_object) {
	// load shader program
	current_object->program = program_load(current_object->shaders, arrlen(current_object->shaders));

	// set attributes
	current_object->attribute_coords = attrib_bind(current_object->program, "coords");
	current_object->attribute_texture_coords = attrib_bind(current_object->program, "texture_coords");

	// set uniforms
	current_object->uniform_obj_transform = uniform_bind(current_object->program, "obj_transform");
	current_object->uniform_world_transform = uniform_bind(current_object->program, "world_transform");
	current_object->uniform_perspective = uniform_bind(current_object->program, "perspective");
	current_object->uniform_texture = uniform_bind(current_object->program, "texture");

	// check bound variables
	if (current_object->attribute_coords < 0 || current_object->attribute_texture_coords < 0 || current_object->uniform_obj_transform < 0 || current_object->uniform_world_transform < 0 || current_object->uniform_perspective < 0 || current_object->uniform_texture < 0) {
		fprintf(stderr, "Couldn't bind variables in ");
		fprintf(stderr, "%s", current_object->shaders[0]);
		for (int i = 1; i < arrlen(current_object->shaders); i++)
			fprintf(stderr, ",%s", current_object->shaders[i]);
		fprintf(stderr, "\n");
		return 0;
	}

	// load the object model file
	current_object->buffers = object_load(current_object->filename);
	if (current_object->buffers.vbo == 0 || current_object->buffers.ibo == 0) {
		fprintf(stderr, "Couldn't load object model %s\n", current_object->filename);
		return 0;
	}

	// load texture file
	current_object->texture = texture_load(current_object->texture_filename);
	if (current_object->texture == 0) {
		fprintf(stderr, "Couldn't load texture %s\n", current_object->texture_filename);
		return 0;
	}

	return 1;
}

buffer object_load(const char * filename) {
	FILE * file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return (buffer){0, 0};
	}

	// get number of vertices and elements
	int vertices_size, elements_size;
	int output = fscanf(file, "%d %d\n", &vertices_size, &elements_size);
	if (output != 2) {
		fprintf(stderr, "Invalid object file %s\n", filename);
		fclose(file);
		return (buffer){0, 0};
	}

	// copy vertex data
	vertex * vertices = (vertex *)malloc(vertices_size * sizeof(vertex));
	for (int i = 0; i < vertices_size; i++) {
		output = fscanf(file, "%f %f %f|%f %f\n", &vertices[i].coords[0], &vertices[i].coords[1], &vertices[i].coords[2], &vertices[i].texture_coords[0], &vertices[i].texture_coords[1]);
		if (output != 5) {
			fprintf(stderr, "Invalid object file %s\n", filename);
			fclose(file);
			free(vertices);
			return (buffer){0, 0};
		}
	}

	// copy element data
	GLushort * elements = (GLushort *)malloc(elements_size * sizeof(GLushort));
	for (int i = 0; i < elements_size; i++) {
		output = fscanf(file, "%hu\n", &elements[i]);
		if (output != 1) {
			fprintf(stderr, "Invalid object file %s\n", filename);
			fclose(file);
			free(vertices);
			free(elements);
			return (buffer){0, 0};
		}
	}

	fclose(file);

	// generate buffers
	buffer buffers;
	glGenBuffers(1, &buffers.vbo);
	glGenBuffers(1, &buffers.ibo);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(vertex), vertices, GL_STATIC_DRAW);

	// fill element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size * sizeof(GLushort), elements, GL_STATIC_DRAW);

	free(vertices);
	free(elements);

	return buffers;
}

void object_display_list(object_list * objects) {
	for (int i = 0; i < objects->num; i++)
		object_display(objects->list[i]);
}

void object_display(object * current_object) {
	// use the shader program
	glUseProgram(current_object->program);

	// specify vertex attribute arrays and bind to vertex buffer
	glEnableVertexAttribArray(current_object->attribute_coords);
	glEnableVertexAttribArray(current_object->attribute_texture_coords);
	glBindBuffer(GL_ARRAY_BUFFER, current_object->buffers.vbo);
	glVertexAttribPointer(current_object->attribute_coords, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0); // (attribute, # of elements per vertex, type, take values as they are, stride, offset)
	glVertexAttribPointer(current_object->attribute_texture_coords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, texture_coords));

	// bind 2D texture to the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, current_object->texture);
	glUniform1i(current_object->uniform_texture, 0);

	// bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_object->buffers.ibo);

	// get size of array buffer
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	// draw all of the elements
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	// unspecify vertex attribute arrays
	glDisableVertexAttribArray(current_object->attribute_coords);
	glDisableVertexAttribArray(current_object->attribute_texture_coords);
}

void object_destroy_list(object_list * objects) {
	for (int i = 0; i < objects->num; i++)
		object_destroy(objects->list[i]);
}

void object_destroy(object * current_object) {
	glDeleteProgram(current_object->program);
	glDeleteBuffers(1, &current_object->buffers.vbo);
	glDeleteBuffers(1, &current_object->buffers.ibo);
	glDeleteTextures(1, &current_object->texture);
}
