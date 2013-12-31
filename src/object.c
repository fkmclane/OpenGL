#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "glsl.h"
#include "shader.h"
#include "texture.h"
#include "object.h"

int initObjects(object * init_objects[], int init_num_objects) {
	objects = init_objects;
	num_objects = init_num_objects;

	for(int i = 0; i < num_objects; i++) {
		if(!initObject(objects[i]))
			return 0;
	}

	return 1;
}

int initObject(object * current_object) {
	//Load shaders
	GLuint vertshader = loadShader(current_object->vert_shader, GL_VERTEX_SHADER);
	GLuint fragshader = loadShader(current_object->frag_shader, GL_FRAGMENT_SHADER);
	if(vertshader == 0 || fragshader == 0)
		return 0;

	//Create program and link shaders
	current_object->program = glCreateProgram();
	glAttachShader(current_object->program, vertshader);
	glAttachShader(current_object->program, fragshader);
	glLinkProgram(current_object->program);
	GLint ok = GL_FALSE;
	glGetProgramiv(current_object->program, GL_LINK_STATUS, &ok);
	if(ok == GL_FALSE) {
		fprintf(stderr, "Error linking program: ");
		printLog(current_object->program);
		return 0;
	}

	//Set attributes - Coordinates and Texture Coordinates
	current_object->attribute_coords = bindAttrib(current_object->program, "coords");
	current_object->attribute_texture_coords = bindAttrib(current_object->program, "texture_coords");

	//Set uniforms
	current_object->uniform_obj_transform = bindUniform(current_object->program, "obj_transform");
	current_object->uniform_world_transform = bindUniform(current_object->program, "world_transform");
	current_object->uniform_perspective = bindUniform(current_object->program, "perspective");
	current_object->uniform_texture = bindUniform(current_object->program, "texture");

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

	//Add texture
	current_object->texture = loadTexture(current_object->texture_file);
	if(current_object->texture == 0) {
		fprintf(stderr, "Couldn't load texture %s\n", current_object->texture_file);
		return 0;
	}

	return 1;
}

buffer loadObject(const char * filename) {
	FILE * file = fopen(filename, "r"); //Open file
	if(!file) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return (buffer){0, 0};
	}

	int vertices_size, elements_size;
	int output = fscanf(file, "%d %d\n", &vertices_size, &elements_size);
	if(output != 2) {
		fprintf(stderr, "Invalid object file %s\n", filename);
		fclose(file);
		return (buffer){0, 0};
	}

	vertex * vertices = (vertex *)malloc(vertices_size * sizeof(vertex));
	GLushort * elements = (GLushort *)malloc(elements_size * sizeof(GLushort));

	//Copy over the vertex data
	for(int i = 0; i < vertices_size; i++) {
		output = fscanf(file, "%f %f %f|%f %f\n", &vertices[i].coords[0], &vertices[i].coords[1], &vertices[i].coords[2], &vertices[i].texture_coords[0], &vertices[i].texture_coords[1]);
		if(output != 5) {
			fprintf(stderr, "Invalid object file %s\n", filename);
			fclose(file);
			free(vertices);
			free(elements);
			return (buffer){0, 0};
		}
	}

	//Copy the element data
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

	//Make, bind, and set the buffers
	buffer buffers;
	glGenBuffers(1, &buffers.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(vertex), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &buffers.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size * sizeof(GLushort), elements, GL_STATIC_DRAW);

	free(vertices);
	free(elements);

	return buffers;
}

void displayObjects() {
	//Set a black background
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < num_objects; i++)
		displayObject(objects[i]);
}

void displayObject(object * current_object) {
	//Set the shader program with buffers, attributes, and uniforms
	glUseProgram(current_object->program);
	glEnableVertexAttribArray(current_object->attribute_coords);
	glEnableVertexAttribArray(current_object->attribute_texture_coords);
	glBindBuffer(GL_ARRAY_BUFFER, current_object->buffers.vbo);
	glVertexAttribPointer(current_object->attribute_coords, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0); //(attribute, # of elements per vertex, type, take values as they are, stride, offset)
	glVertexAttribPointer(current_object->attribute_texture_coords, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, texture_coords));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, current_object->texture);
	glUniform1i(current_object->uniform_texture, 0);

	//Draw everything then disable the attributes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_object->buffers.ibo);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(current_object->attribute_coords);
	glDisableVertexAttribArray(current_object->attribute_texture_coords);
}

void destroyObjects() {
	for(int i = 0; i < num_objects; i++) {
		glDeleteProgram(objects[i]->program);
		glDeleteBuffers(1, &objects[i]->buffers.vbo);
		glDeleteBuffers(1, &objects[i]->buffers.ibo);
		glDeleteTextures(1, &objects[i]->texture);
	}
}
