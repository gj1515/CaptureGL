#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "shader_s.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

bool isPaused = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		isPaused = !isPaused;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// keyboard callback
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	Shader ourShader("texture.vs", "texture.fs");

	float vertices[] = {
		// ========= texture coord bottom left(0, 0) =========
		// ========= image coord top left(0, 0) ==============
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ============================ Video ============================
	cv::VideoCapture videoCapture("Videos/Teli_3.mp4");
	if (!videoCapture.isOpened())
	{
		std::cout << "Failed to open video file" << std::endl;
		return -1;
	}

	double fps = videoCapture.get(cv::CAP_PROP_FPS);
	int videoWidth = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
	int videoHeight = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
	std::cout << "Video opened: " << videoWidth << "x" << videoHeight << " @ " << fps << "fps" << std::endl;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// ============================ Video ============================
	double frameTime = 1.0 / fps;
	double lastFrameTime = glfwGetTime();

	// video frame
	cv::Mat frame;

	// read first frame
	videoCapture >> frame;
	if (frame.empty())
	{
		std::cout << "Failed to read first video frame" << std::endl;
		return -1;
	}

	// BGR(OpenCV) to RGB(OpenGL)
	cv::Mat frameRGB;
	cv::cvtColor(frame, frameRGB, cv::COLOR_BGR2RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameRGB.cols, frameRGB.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameRGB.data);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// current time
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastFrameTime;

		if (deltaTime >= frameTime && !isPaused)
		{
			videoCapture >> frame;

			if (frame.empty())
			{
				videoCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
				videoCapture >> frame;
				if (frame.empty())
				{
					std::cout << "Failed to loop video" << std::endl;
					break;
				}
			}

			// BGR(OpenCV) to RGB(OpenGL)
			cv::cvtColor(frame, frameRGB, cv::COLOR_BGR2RGB);

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameRGB.cols, frameRGB.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameRGB.data);
			glGenerateMipmap(GL_TEXTURE_2D);

			lastFrameTime = currentTime;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	videoCapture.release();

	glfwTerminate();
	return 0;
}