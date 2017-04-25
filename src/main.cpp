//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "myShader.h"
#include "camara.h"
#include "object.h" 
//para textura
#include <SOIL.h>
//para vector
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 800;
bool WIREFRAME = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
vec3 mov, rot, scal;
vec3 movement;
vec3 rotation = {0,0,0};
GLfloat mixValor;
//GLfloat radX = 0;
//GLfloat radY = 0;

int main() {
	
	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		cout << "Error al inicializar glew" << endl;
		glfwTerminate();
		return NULL;
	}

	//set function when callback
	glfwSetKeyCallback(window, key_callback);

	//set windows and viewport
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	//fondo	
	
	//cargamos los shader
	glEnable(GL_DEPTH_TEST);
	Shader myShader("./src/coordVertex.vertexshader", "./src/coordFragment.fragmentshader");
	Shader LIGHTShader("./src/LightVertex.vertexshader", "./src/LightFragment.fragmentshader");

	Object cubA({ 0.3f,0.3f,0.3f }, { 0.f,0.f,0.f }, { 0.f,3.f,1.f }, Object::cube);
	Object cubB({ 0.1,0.1,0.1 }, { 0.f,0.f,0.f }, { 0.f,3.f,5.f }, Object::cube);

	//declarar vector
	cubA.Move({ 0.f,0.3f,0.1f });
	cubB.Move({ 0.f,-0.3f,0.5f });
	
	cubA.Rotate({ 1.f,0.f,0.0f });
	cubB.Rotate({ -1.f,0.f,0.0f });

	cubA.Scale({ 0.3,0.3,0.3 });
	cubB.Scale({ 0.1,0.1,0.1 });
	//bucle de dibujado

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
				
		//Establecer el color de fondo
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//establecer el shader
		myShader.USE();

		GLint objectColorLoc = glGetUniformLocation(myShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(myShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(myShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(myShader.Program, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		
		glUniform3f(lightPosLoc, cubB.GetPosition().x, cubB.GetPosition().y, cubB.GetPosition().z);
		glUniform3f(viewPosLoc, 0, 0, -3.f);

		mat4 proj;		mat4 view;		mat4 model;
		
		proj = perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
		view = glm::translate(view, vec3(0.0f, 0.0f, -3.f));
		
		
		model = cubA.GetModelMatrix();
		

		GLint modelLoc = glGetUniformLocation(myShader.Program,"model");
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		GLint viewLoc = glGetUniformLocation(myShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		GLint projectionLoc = glGetUniformLocation(myShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

		//pintar el VAO
		cubA.Draw();

		LIGHTShader.USE();

		if (rotation != vec3 {0, 0, 0}) {
			cubB.Rotate(rotation);
			rotation = { 0,0,0 };
		}
		cubB.Move(movement);
		movement = {0,0,0};
		
		model = glm::translate(model, cubB.GetPosition());

		model = cubB.GetModelMatrix();

		modelLoc = glGetUniformLocation(LIGHTShader.Program, "model");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		viewLoc = glGetUniformLocation(LIGHTShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		projectionLoc = glGetUniformLocation(LIGHTShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));

		//pintar el VAO
		cubB.Draw();
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// liberar la memoria de los VAO, EBO y VBO
	cubA.Delete();
	cubB.Delete();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		WIREFRAME = !WIREFRAME;

	if (key == GLFW_KEY_KP_4)
		rotation = { 0.f,-1.f,0.f };
	if (key == GLFW_KEY_KP_6)
		rotation = { 0.f,1.f,0.f };
	if (key == GLFW_KEY_KP_8)
		rotation = { -1.f,0.f,0.f};
	if (key == GLFW_KEY_KP_2)
		rotation = { 1.f,0.f,0.f };
	
	if (key == GLFW_KEY_LEFT )
		movement = { -0.05,0,0 };
	if (key == GLFW_KEY_RIGHT)
		movement = { 0.05,0,0 };
	if (key == GLFW_KEY_UP )
		movement = { 0,0.05,0 };
	if (key == GLFW_KEY_DOWN )
		movement = {0, -0.05, 0};
}
