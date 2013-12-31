#include <stdio.h>
#include <stdlib.h>

#include "texture.h"

GLuint loadTexture(const char * filename) {
	FILE * file = fopen(filename, "rb");
	if(!file) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return 0;
	}

	GLsizei texture_width, texture_height;
	int texture_maxval;
	int output = fscanf(file, "P6\n%d %d\n%d\n", &texture_width, &texture_height, &texture_maxval);
	if(output != 3 || texture_maxval != 255) {
		fprintf(stderr, "Invalid texture file %s\n", filename);
		fclose(file);
		return 0;
	}
	long pos = ftell(file);
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file) - pos;
	fseek(file, pos, SEEK_SET);
	GLvoid * texture_data = malloc(size);
	if(!texture_data) {
		fprintf(stderr, "Couldn't load texture (%s): Memory error: ", filename);
		perror("");
		fclose(file);
		return 0;
	}
	size_t size_read = fread((void *)texture_data, 1, size, file);
	if(size_read != size) {
		fprintf(stderr, "Couldn't load texture (%s): Read error: ", filename);
		perror("");
		fclose(file);
		free((void *)texture_data);
		return 0;
	}
	fclose(file);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data); //(Target, Level, InternalFormat, Width, Height, Border, Format, Type, Data)
	free(texture_data);

	return texture;
}
