#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

GLuint theProgram;
GLuint positionBufferObject;
GLuint vao;

glm::vec4 vertexPositions[] = {
	glm::vec4{0.75f, 0.75f, 0.0f, 1.0f},
	glm::vec4{0.75f, -0.75f, 0.0f, 1.0f},
	glm::vec4{-0.75f, -0.75f, 0.0f, 1.0f}
};

string load_shader(string const& filename)
{
	string shader{};
	string tmp{};
	ifstream ifs{filename};

	while(getline(ifs, tmp, '\n'))
	{
		shader.append(tmp)
			.append("\n");
	}
	return shader;
}

const string strVertexShader{load_shader("shaders/shader.vert")};
const string strFragmentShader{load_shader("shaders/shader.frag")};

GLuint CreateShader(GLenum eShaderType, const string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t i = 0; i < shaderList.size(); i++)
		glAttachShader(program, shaderList[i]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t i = 0; i < shaderList.size(); i++)
		glDetachShader(program, shaderList[i]);

	return program;
}

void InitializeProgram()
{
	vector<GLuint> shaderList{};

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = CreateProgram(shaderList);

	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}


void InitializeVertexBuffer()
{
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
	InitializeProgram();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	InitializeVertexBuffer();
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(theProgram);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
	if (!glfwInit())
	{
		cerr << "GLFW init failed" << endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Playground", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		cerr << "GLFW window creation failed" << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	init();

	glm::mat4 rot_z{1.0};
	rot_z = glm::rotate(rot_z, 0.01f, glm::vec3{0.0, 0.0, 1.0});

	while (!glfwWindowShouldClose(window))
	{
		for(int i{}; i < 3; ++i)
			vertexPositions[i] = rot_z * vertexPositions[i];

		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
