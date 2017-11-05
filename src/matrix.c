#include <math.h>
#include <stdio.h>

#include "matrix.h"

void matrix_create_identity(GLfloat * matrix) {
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = matrix[6] = matrix[7] = matrix[8] = matrix[9] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0;
}

void matrix_create_perspective(GLfloat * matrix, float fovy, float aspect, float near, float far) {
	float f = 1 / tan(fovy / 2);
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = matrix[6] = matrix[7] = matrix[8] = matrix[9] = matrix[12] = matrix[13] = matrix[15] = 0;
	matrix[11] = -1;
	matrix[0] = f / aspect;
	matrix[5] = f;
	matrix[10] = (far + near) / (near - far);
	matrix[14] = 2 * far * near / (far - near);
}

void matrix_camera(GLfloat * matrix, float cam_x, float cam_y, float cam_z, float center_x, float center_y, float center_z, float cam_roll) {
	// put camera at origin then rotate world accordingly
	if(cam_roll)
		matrix_rotate_z(matrix, cam_roll);
	if(cam_x - center_x)
		matrix_rotate_y(matrix, -atan2(cam_x - center_x, cam_z - center_z));
	if(cam_y - center_y)
		matrix_rotate_x(matrix, atan2(cam_y - center_y, cam_z - center_z));
	matrix_translate(matrix, -cam_x, -cam_y, -cam_z);
}

void matrix_translate(GLfloat * matrix, float x, float y, float z) {
	for(int i = 0; i < 4; i++) { // go through each row and change the last element by the translation matrix
		matrix[i + 12] = matrix[i] * x + matrix[i + 4] * y + matrix[i + 8] * z + matrix[i + 12];
	}
}

void matrix_rotate_x(GLfloat * matrix, float angle) {
	float sintheta = sinf(angle);
	float costheta = cosf(angle);
	for(int i = 4; i < 8; i++) { // go through each row and change according to the rotation matrix (2nd and 3rd columns for rotation about the x-axis)
		GLfloat temp = matrix[i];
		matrix[i] = matrix[i] * costheta + matrix[i + 4] * sintheta;
		matrix[i + 4] = matrix[i + 4] * costheta - temp * sintheta;
	}
}

void matrix_rotate_y(GLfloat * matrix, float angle) {
	float sintheta = sinf(angle);
	float costheta = cosf(angle);
	for(int i = 0; i < 4; i++) { // go through each row and change according to the rotation matrix (1st and 3rd columns for rotation about the y-axis)
		GLfloat temp = matrix[i];
		matrix[i] = matrix[i] * costheta - matrix[i + 8] * sintheta;
		matrix[i + 8] = matrix[i + 8] * costheta + temp * sintheta;
	}
}

void matrix_rotate_z(GLfloat * matrix, float angle) {
	float sintheta = sinf(angle);
	float costheta = cosf(angle);
	for(int i = 0; i < 4; i++) { // go through each row and change according to the rotation matrix (1st and 2nd columns for rotation about the z-axis)
		GLfloat temp = matrix[i];
		matrix[i] = matrix[i] * costheta + matrix[i + 4] * sintheta;
		matrix[i + 4] = matrix[i + 4] * costheta - temp * sintheta;
	}
}

void matrix_rotate(GLfloat * matrix, float angle_x, float angle_y, float angle_z) {
	matrix_rotate_z(matrix, angle_z);
	matrix_rotate_y(matrix, angle_y);
	matrix_rotate_x(matrix, angle_x);
}

void matrix_multiply(GLfloat * matrix, GLfloat * factor) {
	for(int i = 0; i < 4; i++) { // go through each row
		GLfloat value[4] = {0, 0, 0, 0};
		for(int j = 0; j < 4; j++) // and then through each column of the first matrix and second matrix
			for(int k = 0; k < 4; k++) // then through each row in the second matrix
				value[j] += matrix[i + k * 4] * factor[j * 4 + k]; // and multiply them together, adding these up for a whole row of values
		for(int l = 0; l < 4; l++)
			matrix[i + l * 4] = value[l]; // then copy those values back to the first matrix
	}
}

void matrix_print(GLfloat * matrix) {
	for(int i = 0; i < 4; i++) {
		printf("%f", matrix[i]);
		for(int j = 1; j < 4; j++)
			printf(" %f", matrix[j * 4 + i]);
		printf("\n");
	}
}
