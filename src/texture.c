#include <stdio.h>
#include <stdlib.h>

#include "texture.h"

GLuint texture_load(const char * filename) {
	FILE * file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return 0;
	}

	// get width and height
	GLsizei texture_width, texture_height;
	int texture_maxval;
	int output = fscanf(file, "P6\n%d %d\n%d\n", &texture_width, &texture_height, &texture_maxval);
	if (output != 3 || texture_maxval != 255) {
		fprintf(stderr, "Invalid texture file %s\n", filename);
		fclose(file);
		return 0;
	}

	// get texture size
	long pos = ftell(file);
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file) - pos;
	fseek(file, pos, SEEK_SET);

	// allocate texture
	GLvoid * texture_data = malloc(size);
	if (!texture_data) {
		fprintf(stderr, "Could not load texture (%s): Memory error: ", filename);
		perror("");
		fclose(file);
		return 0;
	}

	// read texture
	size_t size_read = fread(texture_data, 1, size, file);
	if (size_read != size) {
		fprintf(stderr, "Could not load texture (%s): Read error: ", filename);
		perror("");
		fclose(file);
		free(texture_data);
		return 0;
	}

	fclose(file);

	// generate texture
	GLuint texture;
	glGenTextures(1, &texture);

	// fill texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data); // (Target, Level, InternalFormat, Width, Height, Border, Format, Type, Data)

	free(texture_data);

	return texture;
}
