#include "camara.h"

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov){
	cameraPos = position;
	cameraFront = direction;
	Sensitivity = sensitivity;
	FOV = fov;
};
//OK
void Camera::DoMovement(GLFWwindow * window) {
	GLfloat currentFrame= glfwGetTime();
	Deltatime = currentFrame - Lastframe;
	Lastframe = currentFrame;
	cameraSpeed = 3 * Deltatime;
	if (glfwGetKey(window, GLFW_KEY_W))
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S))
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A))
		cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D))
		cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
	
};
//OK
void Camera::MouseMove(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		LastMx = xpos;
		LastMy = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - LastMx;
	GLfloat yoffset = LastMy - ypos;

	LastMx = xpos;
	LastMy = ypos;
	
	Sensitivity = 0.05f;

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	YAW += xoffset;
	PITCH += yoffset;

	PITCH = clamp(PITCH, -89.f, 89.f);
	YAW = mod(YAW, 360.f);

	vec3 front;
	front.x = cos(radians(YAW)) * cos(radians(PITCH));
	front.y = sin(radians(PITCH));
	front.z = sin(radians(YAW)) * cos(glm::radians(PITCH));
	
	cameraFront = normalize(front);
	
};
//OK
void Camera::MouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	if (FOV >= 1.0f && FOV <= 45)
		FOV -= yScroll;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 45)
		FOV = 45;
};

mat4 Camera::LookAt() {

//	cameraPos = { camX, 0.f, camZ };
//	cameraTarget = { 0,0,0 };

	cameraTarget = cameraPos + cameraFront;
	cameraDirection = normalize(cameraPos - cameraTarget);
	cameraRight = normalize(cross(Up, cameraDirection));
	cameraUp = cross(cameraDirection, cameraRight);

	mat4 cameraTranslate;
	cameraTranslate[3][0] = -cameraPos.x;
	cameraTranslate[3][1] = -cameraPos.y;
	cameraTranslate[3][2] = -cameraPos.z;

	mat4 LookAtCenter;
	LookAtCenter[0][0] = cameraRight.x;
	LookAtCenter[1][0] = cameraRight.y;
	LookAtCenter[2][0] = cameraRight.z;
	LookAtCenter[0][1] = cameraUp.x;
	LookAtCenter[1][1] = cameraUp.y;
	LookAtCenter[2][1] = cameraUp.z;
	LookAtCenter[0][2] = cameraDirection.x;
	LookAtCenter[1][2] = cameraDirection.y;
	LookAtCenter[2][2] = cameraDirection.z;

	return LookAtCenter*cameraTranslate;
};
//OK
GLfloat Camera::GetFOV() {
	return FOV;
};