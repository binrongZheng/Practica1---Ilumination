#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>




class Shader {
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void USE();
};