#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include"Renderer/ShaderProgram.h"
#include"Resources/ResourceManager.h"

GLfloat points[]{
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};
GLfloat colors[]{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

const char* vertex_shader =
"#version 460 core\n"
"layout(location = 0) in vec3 vPos;"
"layout(location = 1) in vec3 vColor;"
"out vec3 color;"
"void main() {"
"	gl_Position = vec4(vPos, 1.0f);"
"	color = vColor;"
"}";

const char* fragment_shader =
"#version 460 core\n"
"in vec3 color;"
"out vec4 fColor;"
"void main() {"
"	fColor = vec4(color, 1.0f);"
"}";

int g_windowSizeX = 1280;
int g_windowSizeY = 720;
void isResized(GLFWwindow* window, int width, int height)
{
	g_windowSizeX = width;
	g_windowSizeY = height;
	glViewport(0, 0, g_windowSizeX, g_windowSizeY);
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
	GLFWwindow* window = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Hello Ilikeall", nullptr, nullptr);

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

		GLuint points_VBO = 0;
		glGenBuffers(1, &points_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		GLuint colors_VBO = 0;
		glGenBuffers(1, &colors_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint VAO = 0;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

#pragma endregion

		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			pDefaultShaderProgram->use();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}