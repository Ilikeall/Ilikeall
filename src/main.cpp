#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/vec2.hpp>

#include<iostream>
#include<chrono>

#include"Game/Game.h"
#include"Resources/ResourceManager.h"

glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);

void isResized(GLFWwindow* window, int width, int height)
{
	g_windowSize.x = width;
	g_windowSize.y = height;
	glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	g_game.setKey(key, action);
}

int main(int argc, char** argv)
{

#pragma region WINDOW INITIALIZED
	if (!glfwInit())
	{
		std::cout << "glfwInit() failed!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Hello Ilikeall", nullptr, nullptr);

	if (!window)
	{
		std::cout << "glfwCreateWindow() failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwSetWindowSizeCallback(window, isResized);
	glfwSetKeyCallback(window, onKeyPressed);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGL())
	{
		std::cout<<"Can't load GLAD!"<<std::endl;
		return -1;
	}
	std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0f, 0.8f, 0.7f, 1.0f);

#pragma endregion
	{
		ResourceManager::setExecutablePath(argv[0]);

		g_game.init();

		auto lastTime = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(window))
		{
			auto currenttTime = std::chrono::high_resolution_clock::now();
			uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currenttTime - lastTime).count();

			lastTime = currenttTime;

			g_game.update(duration);

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			g_game.render();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
		ResourceManager::unloadAllResources();
	}
	glfwTerminate();
	return 0;
}