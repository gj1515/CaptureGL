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

	Shader ourShader("texture.vs", "texture.fs");

	float vertices[] = {
		// ========= texture coord bottom left(0, 0) =========
		// ========= image coord top left(0, 0) ==============
		// positions          // colors           // texture coords
		// left window
		 0.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 0.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // top left 

		// right window
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
		 0.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
		 0.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7
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

	// ==================== first texture ====================
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ============================ Video ============================
	cv::VideoCapture videoCapture1("Videos/Teli_3.mp4");
	if (!videoCapture1.isOpened())
	{
		std::cout << "Failed to open video file" << std::endl;
		return -1;
	}

	double fps1 = videoCapture1.get(cv::CAP_PROP_FPS);
	int videoWidth1 = static_cast<int>(videoCapture1.get(cv::CAP_PROP_FRAME_WIDTH));
	int videoHeight1 = static_cast<int>(videoCapture1.get(cv::CAP_PROP_FRAME_HEIGHT));
	std::cout << "Video opened: " << videoWidth1 << "x" << videoHeight1 << " @ " << fps1 << "fps" << std::endl;

	// ==================== second texture ====================
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ============================ Video ============================
	cv::VideoCapture videoCapture2("Videos/Teli_2.mp4");
	if (!videoCapture2.isOpened())
	{
		std::cout << "Failed to open video file" << std::endl;
		return -1;
	}

	double fps2 = videoCapture2.get(cv::CAP_PROP_FPS);
	int videoWidth2 = static_cast<int>(videoCapture2.get(cv::CAP_PROP_FRAME_WIDTH));
	int videoHeight2 = static_cast<int>(videoCapture2.get(cv::CAP_PROP_FRAME_HEIGHT));
	std::cout << "Video opened: " << videoWidth2 << "x" << videoHeight2 << " @ " << fps2 << "fps" << std::endl;

	ourShader.use();

	// ============================ Video ============================
	double frameTime = std::min(1.0 / fps1, 1.0 / fps2); 
	double lastFrameTime = glfwGetTime();

	// video frame
	cv::Mat frame1, frame2;

	// read first frame
	videoCapture1 >> frame1;
	if (frame1.empty())
	{
		std::cout << "Failed to read first video frame" << std::endl;
		return -1;
	}

	videoCapture2 >> frame2;
	if (frame2.empty())
	{
		std::cout << "Failed to read first frame from video 2" << std::endl;
		return -1;
	}

	// BGR(OpenCV) to RGB(OpenGL)
	cv::Mat frameRGB1, frameRGB2;
	cv::cvtColor(frame1, frameRGB1, cv::COLOR_BGR2RGB);
	cv::cvtColor(frame2, frameRGB2, cv::COLOR_BGR2RGB);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameRGB1.cols, frameRGB1.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameRGB1.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameRGB2.cols, frameRGB2.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameRGB2.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastFrameTime;

		if (deltaTime >= frameTime && !isPaused)
		{
			videoCapture1 >> frame1;
			if (frame1.empty())
			{
				videoCapture1.set(cv::CAP_PROP_POS_FRAMES, 0);
				videoCapture1 >> frame1;
				if (frame1.empty())
				{
					std::cout << "Failed to loop first video" << std::endl;
					break;
				}
			}

			videoCapture2 >> frame2;
			if (frame2.empty())
			{
				videoCapture2.set(cv::CAP_PROP_POS_FRAMES, 0);
				videoCapture2 >> frame2;
				if (frame2.empty())
				{
					std::cout << "Failed to loop second video" << std::endl;
					break;
				}
			}

			// BGR to RGB (OpenCV -> OpenGL)
			cv::cvtColor(frame1, frameRGB1, cv::COLOR_BGR2RGB);
			cv::cvtColor(frame2, frameRGB2, cv::COLOR_BGR2RGB);

			glBindTexture(GL_TEXTURE_2D, texture1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameRGB1.cols, frameRGB1.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameRGB1.data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, texture2);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameRGB2.cols, frameRGB2.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameRGB2.data);
			glGenerateMipmap(GL_TEXTURE_2D);

			lastFrameTime = currentTime;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		ourShader.use();
		glBindVertexArray(VAO);

		// left window
		glBindTexture(GL_TEXTURE_2D, texture1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// right window
		glBindTexture(GL_TEXTURE_2D, texture2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	videoCapture1.release();
	videoCapture2.release();

	glfwTerminate();
	return 0;
}