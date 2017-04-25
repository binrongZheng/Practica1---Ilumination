#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;
#include "myShader.h"

class Object {

public:
	mat4 Model;

	enum FigureType {
		cube = 0,
	};

	Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef);
	~Object();

	void Draw();
	void Move(vec3 translation);
	void Rotate(GLfloat rotaX,GLfloat rotaY);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	vec3 GetPosition();

private:
	GLuint VBO, VAO, EBO;
	vec3 position;
	vec3 scale;
	vec3 rotation;
	GLfloat angleX;
	GLfloat angleY;
};

