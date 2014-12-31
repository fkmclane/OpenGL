#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "shader.h"
#include "texture.h"
#include "object.h"

int initObjects(object_list * objects) {
	for(int i = 0; i < objects->num; i++) {
		if(!initObject(objects->list[i]))
			return 0;
	}

	return 1;
}

int initObject(object * current_object) {
	//Load shader program
	current_object->program = loadProgram(current_object->vert_shader, current_object->frag_shader);

	//Set attributes
	current_object->attribute_coords = bindAttrib(current_object->program, "coords");
	current_object->attribute_texture_coords = bindAttrib(current_object->program, "texture_coords");

	//Set uniforms
	current_object->uniform_obj_transform = bindUniform(current_object->program, "obj_transform");
	current_object->uniform_world_transform = bindUniform(current_object->program, "world_transform");
	current_object->uniform_perspective = bindUniform(current_object->program, "perspective");
	current_object->uniform_texture = bindUniform(current_object->program, "texture");

	//Check bound variables
	if(current_object->attribute_coords < 0 || current_object->attribute_texture_coords < 0 || current_object->uniform_obj_transform < 0 || current_object->uniform_world_transform < 0 || current_object->uniform_perspective < 0 || current_object->uniform_texture < 0) {
		fprintf(stderr, "Couldn't bind variables in %s and %s\n", current_object->vert_shader, current_object->frag_shader);
		return 0;
	}

	//Load the object model file
	current_object->buffers = loadObject(current_object->filename);
	if(current_object->buffers.vbo == 0 || current_object->buffers.ibo == 0) {
		fprintf(stderr, "Couldn't load object model %s\n", current_object->filename);
		return 0;
	}

	//Load texture file
	current_object->texture = loadTexture(current_object->texture_filename);
	if(current_object->texture == 0) {
		fprintf(stderr, "Couldn't load texture %s\n", current_object->texture_filename);
		return 0;
	}

	return 1;
}

buffer loadObject(const char * filename) {
	FILE * file = fopen(filename, "r");
	if(!file) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return (buffer){0, 0};
	}

	//Get number of vertices and elements
	int vertices_size, elements_size;
	int output = fscanf(file, "%d %d\n", &vertices_size, &elements_size);
	if(output != 2) {
		fprintf(stderr, "Invalid object file %s\n", filename);
		fclose(file);
		return (buffer){0, 0};
	}

	//Copy vertex data
	vertex * vertices = (vertex *)malloc(vertices_size * sizeof(vertex));
	for(int i = 0; i < vertices_size; i++) {
		output = fscanf(file, "%f %f %f|%f %f\n", &vertices[i].coords[0], &vertices[i].coords[1], &vertices[i].coords[2], &vertices[i].texture_coords[0], &vertices[i].texture_coords[1]);
		if(output != 5) {
			fprintf(stderr, "Invalid object file %s\n", filename);
			fclose(file);
			free(vertices);
			return (buffer){0, 0};
		}
	}

	//Copy element data
	GLushort * elements = (GLushort *)malloc(elements_size * sizeof(GLushort));
	for(int i = 0; i < elements_size; i++) {
		output = fscanf(file, "%hu\n", &elements[i]);
		if(output != 1) {
			fprintf(stderr, "Invalid object file %s\n", filename);
			fclose(file);
			free(vertices);
			free(elements);
			return (buffer){0, 0};
		}
	}

	fclose(file);

	//Generate buffers
	buffer buffers;
	glGenBuffers(1, &buffers.vbo);
	glGenBuffers(1, &buffers.ibo);

	//Fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(vertex), vertices, GL_STATIC_DRAW);

	//Fill element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size * sizeof(GLushort), elements, GL_STATIC_DRAW);

	free(vertices);
	free(elements);

	return buffers;
}

void displayObjects(object_list * objects) {
	for(int i = 0; i < objects->num; i++)
		displayObject(objects->list[i]);
}

void displayObject(object * current_object) {
	//Use the shader program
	glUseProgram(current_object->program);

	//Specify vertex attribute arrays and bind to vertex buffer
	glEnableVertexAttribArray(current_object->attribute_coords);
	glEnableVertexAttribArray(current_object->attribute_texture_coords);
	glBindBuffer(GL_ARRAY_BUFFER, current_object->buffers.vbo);
	glVertexAttribPointer(current_object->attribute_coords, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0); //(attribute, # of elements per vertex, type, take values as they are, stride, offset)
	glVertexAttribPointer(current_object->attribute_texture_coords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, texture_coords));

	//Bind 2D texture to the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, current_object->texture);
	glUniform1i(current_object->uniform_texture, 0);

	//Bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_object->buffers.ibo);

	//Get size of array buffer
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	//Draw all of the elements
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	//Unspecify vertex attribute arrays
	glDisableVertexAttribArray(current_object->attribute_coords);
	glDisableVertexAttribArray(current_object->attribute_texture_coords);
}

void destroyObjects(object_list * objects) {
	for(int i = 0; i < objects->num; i++)
		destroyObject(objects->list[i]);
}

void destroyObject(object * current_object) {
	glDeleteProgram(current_object->program);
	glDeleteBuffers(1, &current_object->buffers.vbo);
	glDeleteBuffers(1, &current_object->buffers.ibo);
	glDeleteTextures(1, &current_object->texture);
}
