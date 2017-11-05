#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

unsigned int arrlen(const char ** arr) {
	unsigned int len = 0;

	while(arr++)
		len++;

	return len;
}

char * file_read(const char * filename) {
	FILE * file = fopen(filename, "r"); // open file
	if(!file) {
		fprintf(stderr, "Error opening %s: ", filename);
		perror("");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file); // get file size
	rewind(file);

	char * buffer = (char *)malloc(size + 1); // allocate space for it
	if(!buffer) {
		fprintf(stderr, "Couldn't load file (%s): Memory error: ", filename);
		perror("");
		fclose(file);
		return NULL;
	}
	buffer[size] = '\0'; // and null-terminate it for uses outside of this function

	size_t size_read = fread(buffer, 1, size, file); // read the file into the buffer
	if(size_read != size) { // and double check its read size to actual size
		fprintf(stderr, "Couldn't load file (%s): Read error: ", filename);
		perror("");
		fclose(file);
		free(buffer);
		return NULL;
	}

	fclose(file);
	return buffer; // be sure to free the data when done with it
}

void log_print(GLuint object) { // print logs of shaders or programs
	GLint length = 0;
	char * log;
	if(glIsShader(object)) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetShaderInfoLog(object, length, NULL, log);
	}
	else if(glIsProgram(object)) {
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetProgramInfoLog(object, length, NULL, log);
	}
	else {
		fprintf(stderr, "Error getting log: Not a shader or a program\n");
		return;
	}

	fprintf(stderr, "%s\n", log);
	free(log);
}
