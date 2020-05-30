#include "STXR.h"
#include <GLFW/glfw3.h>

bool closeWindow = false;
GLFWwindow* window;
STXR::Text text;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//hit 'ESC' to close window
		closeWindow = true;
	}
}

int main(int argc, char** argv) {
	int w = 1024;
	int h = 720;

	// Create context and window.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(w, h, "TextRenderer Demo", NULL, NULL);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glewInit();
	glfwSetWindowPos(window, 10, 30);

	// Load fonts.
	text.load("examples/2Dumb.bmp", 0.4, 0.065);

	// Render loop.
	while (!glfwWindowShouldClose(window)) {
		if (closeWindow) { glfwSetWindowShouldClose(window, true); }
		processInput(window);
		glClearColor(0.2,0.2,0.2,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render text.
		text.draw(
			"2Dumb",						  //Font name (file name before '.bmp')
			"This is a test example.\nIf you see this it is working.",//String to be rendered
			-0.85,-0.00001,						  //(x,y) coords in window
			glm::vec4(0.8, 0.8, 0.8, 1.0),				  //text color
			0.5,							  //new line spacing
			1.5,							  //spacing between chars
			20.0, 20.0,						  //(x,y) scale values
			0.75, 50.0						  //(x,y) area for block of text
		);
		/*NOTE: if the 'area width' parameter is exceeded by the characters drawn, a newline character
		* will automatically be added to force the text to fit. For this reason, the drawn text reads 
		* like this:
		*
		* This is a test example.
		* If you see this it
		* is working.
		*
		* With two newline characters instead of one newlines, like the original string has.
		*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (window != nullptr)glfwDestroyWindow(window);
	glfwTerminate();
}
