#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

void frameCallBack(GLFWwindow* window, int w, int h);
void processEvent(GLFWwindow* window);

const char* vertexShaderSource =
"#version 330 core\n"
"layout(location=0) in vec3 a_Pos;\n"
"layout(location=1) in vec3 a_Color;\n"
"out vec3 colour;\n"
"void main()\n"
"{\n"
"	colour = a_Color;\n"
"	gl_Position = vec4(a_Pos,1.0f);\n"
"}\n";
const char* framentShaderSource =
"#version 330 core\n"
"in vec3 colour;\n"
"out vec4 FramentColor;"
"void main()\n"
"{\n"
"	FramentColor = vec4(colour,1.0f);\n"
"}\n";

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);

	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameCallBack);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "初始化laod 失败" << std::endl;
		system("pause");
		return -1;
	}


	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int framentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(framentShader, 1, &framentShaderSource, NULL);

	glCompileShader(vertexShader);
	glCompileShader(framentShader);

	char loginfo[512];
	int success;

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, loginfo);
		std::cout << loginfo << std::endl;
		system("pause");
		return -1;
	}
	glGetShaderiv(framentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(framentShader, 512, NULL, loginfo);
		std::cout << loginfo << std::endl;
		system("pause");
		return -1;
	}
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, framentShader);

	glBindAttribLocation(shaderProgram, 0, "a_Pos");
	glBindAttribLocation(shaderProgram, 1, "a_Color");

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, loginfo);
		std::cout << loginfo << std::endl;
		system("pause");
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(framentShader);

	float vertexs[] = {
		-.5f,-.5f,0.0f,
		.0f,-.5f,.0f,
		.0f,.5f,.0f,
		.5f,-.5f,.0f,
	};
	unsigned int indexs[] = {
		2,1,0,
		1,3,2
	};
	float vertexColor[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f
	};

	GLuint EBO, VBO, VAO, VBO_color;

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_color);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1); 

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	while (!glfwWindowShouldClose(window))
	{
		processEvent(window);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);


	glfwTerminate();
	return 0;
}

void frameCallBack(GLFWwindow * window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processEvent(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
