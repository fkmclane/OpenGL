#include <stdio.h>
#include <math.h>

#ifdef HAVE_LIBGLEW
#define GLEW_NO_GLU
#include <GL/glew.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <GLFW/glfw3.h>

#include "matrix.h"
#include "object.h"

#define WIDTH 640
#define HEIGHT 480

#define FOV PI / 4
#define NEAR -0.1
#define FAR -50.0

#define CAM_X 0.0
#define CAM_Y 2.0
#define CAM_Z 5.0
#define CENTER_X 0.0
#define CENTER_Y 0.0
#define CENTER_Z 0.0
#define CAM_ROLL 0.0

#define STEP 0.1
#define ROTATE_STEP PI / 48

#define NUM_OBJECTS 1
object * object_list[NUM_OBJECTS] = {
	&(object) {
		.filename = "object.mod",
		.texture_file = "texture.ppm",
		.vert_shader = "object.vert.glsl",
		.frag_shader = "object.frag.glsl",
		.x_rotation_speed = 0 * PI / 4,
		.y_rotation_speed = 0 * PI / 4,
		.z_rotation_speed = 0 * PI / 4
	}
};

long lastsecs = 0;
int framecount = 0;
void printFPS() {
	framecount++; //Increment frame
	float secs = glfwGetTime(); //Get current time
	float time = secs - lastsecs; //Subtract it from the last time and convert to seconds
	if(time > 1) {
		fprintf(stderr, "FPS: %3.2f          \r", framecount / time); //Calculate frames / second
		lastsecs = secs; //Reset everything
		framecount = 0;
	}
}

void animate() { //Set the animation
	//TODO: Put in opengl.c
	for(int i = 0; i < NUM_OBJECTS; i++) {
		glUseProgram(objects[i]->program);
		GLfloat matrix[16];
		createIdentity(matrix); //Load up the identity
		if(objects[i]->x_rotation_speed)
			xRotateMatrix(matrix, glfwGetTime() * objects[i]->x_rotation_speed); //And modify the matrix to rotate the object over time
		if(objects[i]->y_rotation_speed)
			yRotateMatrix(matrix, glfwGetTime() * objects[i]->y_rotation_speed);
		if(objects[i]->z_rotation_speed)
			zRotateMatrix(matrix, glfwGetTime() * objects[i]->z_rotation_speed);
		glUniformMatrix4fv(objects[i]->uniform_obj_transform, 1, GL_FALSE, matrix); //Apply the transformation matrix
	}
}

void resize(GLFWwindow * window, int w, int h) {
	//Reset view matrix
	GLfloat perspective[16];
	createPerspectiveMatrix(perspective, FOV, (float)(w / h), NEAR, FAR);
	for(int i = 0; i < NUM_OBJECTS; i++)
		glUniformMatrix4fv(objects[i]->uniform_perspective, 1, GL_FALSE, perspective);
	glViewport(0, 0, w, h);
}

int main(int argc, char * argv[]) {
	//Initialize GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Error initializing GLFW\n");
		return 1;
	}

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "Object", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Error opening GLFW window\n");
		glfwTerminate();
		return 1;
	}
	glfwSetWindowSizeCallback(window, resize);
	glfwMakeContextCurrent(window);

#ifdef HAVE_LIBGLEW
	//If we have GLEW, initialize it in the window's context
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW\n");
		glfwTerminate();
		return 1;
	}
#endif

	//TODO: Make opengl.c for this stuff
	//Set up OpenGL parameters
	glEnable(GL_BLEND); //For transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Also for transparancy
	glEnable(GL_DEPTH_TEST); //For 3D

	//Set up view matrix and current camera position
	GLfloat perspective[16];
	createPerspectiveMatrix(perspective, FOV, (float)(WIDTH / HEIGHT), NEAR, FAR); //Set our perspective

	GLfloat world_matrix[16];
	createIdentity(world_matrix);
	cameraMatrix(world_matrix, CAM_X, CAM_Y, CAM_Z, CENTER_X, CENTER_Y, CENTER_Z, CAM_ROLL); //Move our world to view from a fake "camera"

	float x = CAM_X;
	float y = CAM_Y;
	float z = CAM_Z;
	float rot_x = atan2(y - CENTER_Y, z - CENTER_Z);
	float rot_y = -atan2(x - CENTER_X, z - CENTER_Z);
	float rot_z = CAM_ROLL;

	if(!initObjects(objects, NUM_OBJECTS)) {
		fprintf(stderr, "Error initializing objects\n");
		destroyObjects();
		glfwTerminate();
		return 1;
	}

	//Apply perspective and camera matrices
	for(int i = 0; i < NUM_OBJECTS; i++) {
		glUniformMatrix4fv(objects[i]->uniform_perspective, 1, GL_FALSE, perspective);
		glUniformMatrix4fv(objects[i]->uniform_world_transform, 1, GL_FALSE, world_matrix);
	}

	while(!glfwWindowShouldClose(window)) {
		displayObjects();
		animate();
		printFPS();

		if(glfwGetKey(window, 'Q')) //Quit
			glfwSetWindowShouldClose(window, GL_TRUE);

		if(glfwGetKey(window, 'W')) //Move forward
			z -= STEP;
		if(glfwGetKey(window, 'A')) //Move left
			x -= STEP;
		if(glfwGetKey(window, 'S')) //Move back
			z += STEP;
		if(glfwGetKey(window, 'D')) //Move right
			x += STEP;
		if(glfwGetKey(window, GLFW_KEY_LEFT)) //Turn left
			rot_y -= ROTATE_STEP;
		if(glfwGetKey(window, GLFW_KEY_RIGHT)) //Turn right
			rot_y += ROTATE_STEP;
		if(glfwGetKey(window, GLFW_KEY_UP)) //Turn up
			rot_x -= ROTATE_STEP;
		if(glfwGetKey(window, GLFW_KEY_DOWN)) //Turn down
			rot_x += ROTATE_STEP;

		//TODO: Make function for this
		//TODO: Put in opengl.c
		//Create a new camera matrix
		createIdentity(world_matrix);
		xRotateMatrix(world_matrix, rot_x);
		yRotateMatrix(world_matrix, rot_y);
		zRotateMatrix(world_matrix, rot_z);
		translateMatrix(world_matrix, -x, -y, -z);

		//Apply camera matrix
		for(int i = 0; i < NUM_OBJECTS; i++)
			glUniformMatrix4fv(objects[i]->uniform_world_transform, 1, GL_FALSE, world_matrix);

		glfwSwapBuffers(window); //Display the buffer
		glfwPollEvents(); //And check events
	}

	glfwTerminate(); //Need to tell GLFW to stop
	destroyObjects(); //Free up memory again
	return 0;
}
