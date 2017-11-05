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
#include "world.h"

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
#define ROTATE_STEP PI / 128

long lastsecs = 0;
int framecount = 0;
void fps_print() {
	framecount++; // increment frame
	float secs = glfwGetTime(); // get current running time
	float time = secs - lastsecs; // subtract it from the last time and convert to seconds
	if (time >= 1) {
		fprintf(stderr, "FPS: %3.2f          \r", framecount / time); // calculate frames / second
		lastsecs = secs; // reset everything
		framecount = 0;
	}
}

void resize(GLFWwindow * window, int width, int height) {
	world_resize(width, height);
}

int main(int argc, char * argv[]) {
	// initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Error initializing GLFW\n");
		return 1;
	}

	// initialize window
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "Object", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Error creating GLFW window\n");
		glfwTerminate();
		return 1;
	}

	// initialize context
	glfwMakeContextCurrent(window);

#ifdef HAVE_LIBGLEW
	// if we have GLEW, initialize it in the window's context
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW\n");
		glfwTerminate();
		return 1;
	}

#endif
	// initialize world
	if (world_init(FOV, NEAR, FAR, WIDTH, HEIGHT, CAM_X, CAM_Y, CAM_Z, CENTER_X, CENTER_Y, CENTER_Z, CAM_ROLL)) {
		fprintf(stderr, "Error setting up OpenGL world\n");
		glfwTerminate();
		return 1;
	}

	// add callbacks
	glfwSetWindowSizeCallback(window, resize);

	// current action
	float x, y, z, rot_x, rot_y, rot_z;

	while (!glfwWindowShouldClose(window)) {
		fps_print();

		x = y = z = rot_x = rot_y = rot_z = 0;

		if (glfwGetKey(window, 'Q')) // quit
			glfwSetWindowShouldClose(window, GL_TRUE);

		if (glfwGetKey(window, 'W')) // move forward
			z -= STEP;
		if (glfwGetKey(window, 'A')) // move left
			x -= STEP;
		if (glfwGetKey(window, 'S')) // move back
			z += STEP;
		if (glfwGetKey(window, 'D')) // move right
			x += STEP;

		if (x != 0 || y != 0 || z != 0)
			world_translate(x, y, z);

		if (glfwGetKey(window, GLFW_KEY_LEFT)) // turn left
			rot_y -= ROTATE_STEP;
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) // turn right
			rot_y += ROTATE_STEP;
		if (glfwGetKey(window, GLFW_KEY_UP)) // turn up
			rot_x -= ROTATE_STEP;
		if (glfwGetKey(window, GLFW_KEY_DOWN)) // turn down
			rot_x += ROTATE_STEP;

		if (rot_x != 0 || rot_y != 0 || rot_z != 0)
			world_rotate(rot_x, rot_y, rot_z);

		world_animate(glfwGetTime());
		world_update();

		glfwSwapBuffers(window); // display the buffer
		glfwPollEvents(); // and check events
	}

	world_deinit(); // free up OpenGL resources
	glfwTerminate(); // need to tell GLFW to stop
	return 0;
}
