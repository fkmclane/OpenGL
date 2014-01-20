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

#include "common.h"
#include "opengl.h"

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
		.x_rotation_speed = PI / 4,
		.y_rotation_speed = PI / 4,
		.z_rotation_speed = PI / 4
	}
};

long lastsecs = 0;
int framecount = 0;
void printFPS() {
	framecount++; //Increment frame
	float secs = glfwGetTime(); //Get current running time
	float time = secs - lastsecs; //Subtract it from the last time and convert to seconds
	if(time > 1) {
		fprintf(stderr, "FPS: %3.2f          \r", framecount / time); //Calculate frames / second
		lastsecs = secs; //Reset everything
		framecount = 0;
	}
}

void resize(GLFWwindow * window, int width, int height) {
	resizeGL(width, height);
}

int main(int argc, char * argv[]) {
	//Initialize GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Error initializing GLFW\n");
		return 1;
	}

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "Object", NULL, NULL);
	if(!window) {
		fprintf(stderr, "Error creating GLFW window\n");
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
	if(initGL(FOV, NEAR, FAR, WIDTH, HEIGHT, CAM_X, CAM_Y, CAM_Z, CENTER_X, CENTER_Y, CENTER_Z, CAM_ROLL, object_list, NUM_OBJECTS)) { //Initialize world
		fprintf(stderr, "Error setting up OpenGL world");
		glfwTerminate();
		return 1;
	}

	float x, y, z, rot_x, rot_y, rot_z;

	while(!glfwWindowShouldClose(window)) {
		printFPS();

		x = y = z = rot_x = rot_y = rot_z = 0;

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

		translateGL(x, y, z);

		if(glfwGetKey(window, GLFW_KEY_LEFT)) //Turn left
			rot_y -= ROTATE_STEP;
		if(glfwGetKey(window, GLFW_KEY_RIGHT)) //Turn right
			rot_y += ROTATE_STEP;
		if(glfwGetKey(window, GLFW_KEY_UP)) //Turn up
			rot_x -= ROTATE_STEP;
		if(glfwGetKey(window, GLFW_KEY_DOWN)) //Turn down
			rot_x += ROTATE_STEP;

		rotateGL(rot_x, rot_y, rot_z);

		updateGL(glfwGetTime());

		glfwSwapBuffers(window); //Display the buffer
		glfwPollEvents(); //And check events
	}

	deinitGL();
	glfwTerminate(); //Need to tell GLFW to stop
	return 0;
}
