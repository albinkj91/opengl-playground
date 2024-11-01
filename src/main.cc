#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "stb_image.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#define window_width 800
#define window_height 800
#define triangle_count 18

GLuint shader_program;
GLuint vbo;
GLuint vao;

glm::vec4 vertex_data[] = {
	glm::vec4{0.0f, 0.5f, 0.0f, 1.0f},
	glm::vec4{0.5f, -0.5f, 0.5f, 1.0f},
	glm::vec4{-0.5f, -0.5f, 0.5f, 1.0f},

	glm::vec4{0.0f, 0.5f, 0.0f, 1.0f},
	glm::vec4{0.5f, -0.5f, -0.5f, 1.0f},
	glm::vec4{0.5f, -0.5f, 0.5f, 1.0f},

	glm::vec4{0.0f, 0.5f, 0.0f, 1.0f},
	glm::vec4{-0.5f, -0.5f, 0.5f, 1.0f},
	glm::vec4{-0.5f, -0.5f, -0.5f, 1.0f},

	glm::vec4{0.0f, 0.5f, 0.0f, 1.0f},
	glm::vec4{-0.5f, -0.5f, -0.5f, 1.0f},
	glm::vec4{0.5f, -0.5f, -0.5f, 1.0f},

	glm::vec4{0.5f, -0.5f, 0.5f, 1.0f},
	glm::vec4{0.5f, -0.5f, -0.5f, 1.0f},
	glm::vec4{-0.5f, -0.5f, -0.5f, 1.0f},

	glm::vec4{0.5f, -0.5f, 0.5f, 1.0f},
	glm::vec4{-0.5f, -0.5f, -0.5f, 1.0f},
	glm::vec4{-0.5f, -0.5f, 0.5f, 1.0f},

	//colors
	glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
	glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
	glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},

	glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
	glm::vec4{1.0f, 1.0f, 0.0f, 1.0f},
	glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},

	glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
	glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
	glm::vec4{1.0f, 0.0f, 1.0f, 1.0f},

	glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
	glm::vec4{1.0f, 0.0f, 1.0f, 1.0f},
	glm::vec4{1.0f, 1.0f, 0.0f, 1.0f},

	glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
	glm::vec4{1.0f, 1.0f, 0.0f, 1.0f},
	glm::vec4{1.0f, 0.0f, 1.0f, 1.0f},

	glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
	glm::vec4{1.0f, 0.0f, 1.0f, 1.0f},
	glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},

	//texture coordinates
	glm::vec4{0.5f, 1.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},
	glm::vec4{1.0f, 0.0f, 0.0f, 0.0f},

	glm::vec4{0.5f, 1.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},
	glm::vec4{1.0f, 0.0f, 0.0f, 0.0f},

	glm::vec4{0.5f, 1.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},
	glm::vec4{1.0f, 0.0f, 0.0f, 0.0f},

	glm::vec4{0.5f, 1.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},
	glm::vec4{1.0f, 0.0f, 0.0f, 0.0f},

	glm::vec4{1.0f, 1.0f, 0.0f, 0.0f},
	glm::vec4{1.0f, 0.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},

	glm::vec4{1.0f, 1.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},
	glm::vec4{0.0f, 1.0f, 0.0f, 0.0f},
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

	shader_program = CreateProgram(shaderList);
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}


void InitializeVertexBuffer()
{
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init()
{
	InitializeProgram();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	InitializeVertexBuffer();
	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);

	// Create texture reference
	unsigned int texture{};
	glGenTextures(1, &texture);

	// Load image
	int width{}, height{}, nrChannels{};
	unsigned char *data = stbi_load("/home/albin/dev/cpp/opengl-playground/assets/gripen.jpg", &width, &height, &nrChannels, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "position"));
	glVertexAttribPointer(glGetAttribLocation(shader_program, "position"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "color"));
	glVertexAttribPointer(glGetAttribLocation(shader_program, "color"), 4, GL_FLOAT, GL_FALSE, 0, (void*)(triangle_count * 4 * sizeof(float)));
	glEnableVertexAttribArray(glGetAttribLocation(shader_program, "textureCoord"));
	glVertexAttribPointer(glGetAttribLocation(shader_program, "textureCoord"), 4, GL_FLOAT, GL_FALSE, 0, (void*)(2*triangle_count * 4 * sizeof(float)));
	glDrawArrays(GL_TRIANGLES, 0, triangle_count);
}

int main()
{
	if (!glfwInit())
	{
		cerr << "GLFW init failed" << endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Playground", NULL, NULL);
	glfwSetWindowAspectRatio(window, window_width, window_height);
	if(!window)
	{
		glfwTerminate();
		cerr << "GLFW window creation failed" << endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	init();

	glm::mat4 rot_y{1.0f};
	rot_y = glm::rotate(rot_y, 0.01f, glm::vec3{0.0, 1.0, 0.0});
	glm::mat4 rot_x{1.0f};
	rot_x = glm::rotate(rot_x, 0.01f, glm::vec3{1.0, 0.0, 0.0});
	glm::mat4 rot_z{1.0f};
	rot_z = glm::rotate(rot_z, 0.01f, glm::vec3{0.0, 0.0, 1.0});

	while (!glfwWindowShouldClose(window))
	{
		for(int i{}; i < triangle_count; ++i)
			vertex_data[i] = rot_x * rot_y * rot_z * vertex_data[i];

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
