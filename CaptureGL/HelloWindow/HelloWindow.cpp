#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// OpenGL(-1 ~ 1) to (width, height)
	glViewport(0, 0, width, height); // left down width height
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

int main()
{
	// glfw Initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// Specifies that OpenGL commands will be issued to this window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD manages OpenGL's function point
	// So Before initialize opengl func, initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering
		glClearColor(0.3f, 0.7f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents(); // poll IO events (keys pressed/released, mouse moved etc.)
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}