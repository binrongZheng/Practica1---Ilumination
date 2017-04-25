#pragma once
//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
//para textura
#include <SOIL.h>
//para vector
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Camera{
private:
	vec3 cameraPos;
	vec3 cameraFront;
	vec3 Up = {0,1,0};
	vec3 cameraTarget= { 0,0,0 };
	vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	vec3 cameraRight = normalize(cross(Up, cameraDirection));
	vec3 cameraUp = cross(cameraDirection, cameraRight);
	GLfloat radius = 8.0f;
	GLfloat camX;
	GLfloat camZ;
	GLfloat Deltatime=0;
	GLfloat Lastframe=0;
	GLfloat LastMx=400;
	GLfloat LastMy=400;
	GLfloat Sensitivity;
	GLboolean firstMouse=true;
	GLfloat PITCH=0;
	GLfloat YAW=-90;
	GLfloat FOV;
	GLfloat cameraSpeed=3*Deltatime;
	GLfloat currentFrame = glfwGetTime();

public:
	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov);
	void DoMovement(GLFWwindow * window);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xScroll, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();
};

