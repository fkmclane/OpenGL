#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "shader.h"
#include "texture.h"
#include "object.h"

typedef struct {
	GLfloat coords[3];
} obj_v;

typedef struct {
	GLfloat texture_coords[2];
} obj_vt;

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
		for (unsigned int i = 1; i < arrlen(current_object->shaders); i++)
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

	// get number of vertices and indices
	int vertices_size = 0, indices_size = 0;
	while (!feof(file)) {
		if (fscanf(file, "v %*[^\r\n]%*[\r\n]"))
			vertices_size++;

		if (fscanf(file, "f %*[^\r\n]%*[\r\n]"))
			indices_size++;
	}

	// get data
	obj_v * obj_vertices = (obj_v *)malloc(vertices_size * sizeof(obj_v));
	unsigned int ovi = 0;
	obj_vt * obj_texture_coords = (obj_vt *)malloc(vertices_size * sizeof(obj_vt));
	unsigned int oti = 0;

	GLushort * indices = (GLushort *)malloc(indices_size * 3 * sizeof(GLushort));
	GLushort * texture_indices = (GLushort *)malloc(indices_size * 3 * sizeof(GLushort));
	unsigned int oii = 0;

	// read full file
	while (!feof(file)) {
		int output;
		char type[16];

		// get type
		output = fscanf(file, "%15s", type);

		if (output != 1)
			continue;

		// coordinate
		if (strcmp(type, "v") == 0) {
			output = fscanf(file, "%f %f %f", &obj_vertices[ovi].coords[0], &obj_vertices[ovi].coords[1], &obj_vertices[ovi].coords[2]);
			if (output != 3) {
				fprintf(stderr, "Invalid object file %s\n", filename);
				fclose(file);
				free(obj_vertices);
				free(obj_texture_coords);
				free(indices);
				free(texture_indices);
				return (buffer){0, 0};
			}

			ovi++;
		}
		// texture coordinate
		else if (strcmp(type, "vt") == 0) {
			output = fscanf(file, "%f %f", &obj_texture_coords[oti].texture_coords[0], &obj_texture_coords[oti].texture_coords[1]);
			if (output != 2) {
				fprintf(stderr, "Invalid object file %s\n", filename);
				fclose(file);
				free(obj_vertices);
				free(obj_texture_coords);
				free(indices);
				free(texture_indices);
				return (buffer){0, 0};
			}

			oti++;
		}
		// face
		else if (strcmp(type, "f") == 0) {
			output = fscanf(file, "%hu/%hu/%*u %hu/%hu/%*u %hu/%hu/%*u", &indices[oii], &texture_indices[oii], &indices[oii + 1], &texture_indices[oii + 1], &indices[oii + 2], &texture_indices[oii + 2]);
			if (output != 9) {
				fprintf(stderr, "Invalid object file %s\n", filename);
				fclose(file);
				free(obj_vertices);
				free(obj_texture_coords);
				free(indices);
				free(texture_indices);
				return (buffer){0, 0};
			}

			oii += 3;
		}

		output = fscanf(file, "%*[^\r\n]%*[\r\n]");
	}

	fclose(file);

	// create vertices
	vertex * vertices = (vertex *)malloc(indices_size * sizeof(vertex));
	for (int i = 0; i < indices_size; i++) {
		vertices[i].coords[0] = obj_vertices[indices[i] - 1].coords[0];
		vertices[i].coords[1] = obj_vertices[indices[i] - 1].coords[1];
		vertices[i].coords[2] = obj_vertices[indices[i] - 1].coords[2];
		vertices[i].texture_coords[0] = obj_texture_coords[texture_indices[i] - 1].texture_coords[0];
		vertices[i].texture_coords[1] = obj_texture_coords[texture_indices[i] - 1].texture_coords[1];
	}

	free(obj_vertices);
	free(obj_texture_coords);
	free(texture_indices);

	// generate buffers
	buffer buffers;
	glGenBuffers(1, &buffers.vbo);
	glGenBuffers(1, &buffers.ibo);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
	glBufferData(GL_ARRAY_BUFFER, indices_size * sizeof(vertex), vertices, GL_STATIC_DRAW);

	// fill element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(GLushort), indices, GL_STATIC_DRAW);

	free(vertices);
	free(indices);

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
