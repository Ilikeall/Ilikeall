#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/vec2.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<iostream>
#include"Renderer/ShaderProgram.h"
#include"Resources/ResourceManager.h"
#include"Renderer/Texture2D.h"
#include"Renderer/Sprite.h"

GLfloat points[]{
	 0.0f,  50.f, 0.0f,
	 50.f, -50.f, 0.0f,
	-50.f, -50.f, 0.0f
};
GLfloat colors[]{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

GLfloat texCoord[]{
	0.5f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

glm::ivec2 g_windowSize(640, 480);

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
}

char resultInfo[1024];
int res;

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

#pragma region BUFFER INITIALIZATION
	{

		ResourceManager resourceManager(argv[0]);
		auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/Shaders/vertex.vert", "res/Shaders/fragment.frag");
		if (!pDefaultShaderProgram)
		{
			std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
			return -1;
		}

		auto ptrSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/Shaders/vert_Sprite.vert", "res/Shaders/frag_Sprite.frag");
		if (!ptrSpriteShaderProgram)
		{
			std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
			return -1;
		}

		auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

		auto ptrSprite = resourceManager.loadSprite("NewSprite", "DefaultTexture", "SpriteShader", 50, 100);
		ptrSprite->setPosition(glm::vec2(300, 100));

		GLuint points_VBO = 0;
		glGenBuffers(1, &points_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		GLuint colors_VBO = 0;
		glGenBuffers(1, &colors_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint texCoord_VBO = 0;
		glGenBuffers(1, &texCoord_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

		GLuint VAO = 0;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_VBO);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

#pragma endregion

		pDefaultShaderProgram->use();
		pDefaultShaderProgram->setInt("tex", 0);

		glm::mat4 modelMatrix_1 = glm::mat4(1.f);
		modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));
		glm::mat4 modelMatrix_2 = glm::mat4(1.f);
		modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

		glm::mat4 projectionMatrix = glm::ortho(0.f,static_cast<float>(g_windowSize.x),0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

		pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

		ptrSpriteShaderProgram->use();
		ptrSpriteShaderProgram->setInt("tex", 0);
		ptrSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			pDefaultShaderProgram->use();
			glBindVertexArray(VAO);
			tex->bind();

			pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			ptrSprite->render();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}