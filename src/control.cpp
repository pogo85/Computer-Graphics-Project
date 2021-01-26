/**
	* File:    control.cpp
	*
	* Author1:  Akshay Tiwari (2018A7PS0201H)
	* Author2:  Mushkan Surekha  (2018B5A70477H)
	* Author3:  Mahesh Swaminathan  (2018A4PS0982H)
	* Date:     09-04-2020
	* Course:   Computer Graphics (IS F311)
	*
	* Summary of File:
	*
	*   Program to control the movement of camera around the scene using
	*	either mouse or keyboard inputs as desired by the user.
*/
#include <GLFW/glfw3.h>
extern GLFWwindow* window; ///<Current context window
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <iostream>
#include "control.hpp"

glm::mat4 ViewMatrix;///<Output View Matrix
glm::mat4 ProjectionMatrix;///<Output Projection Matrix
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);///<Initial camera location
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);///<Initial camera front location (view direction)
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);///<Initial camera up direction
glm::vec3 position = glm::vec3(0, 0, 2);///<Initial position on +Z
float horizontalAngle = 3.14f;///<Initial horizontal angle : toward -Z
float verticalAngle = 0.0f;///<Initial vertical angle : none
float initialFoV = 45.0f;///<Initial Field of View
float speed = 10.0f;///<Speed of viweing
float mouseSpeed = 0.005f;///<Cursor movement speed
float yaw = 0;///<Initial yaw
float pitch = 0;///<Initial pitch
float roll = 0;///<Initial roll
float FoV = initialFoV;///<Initial Field Of View
glm::vec3 center;///<Center vector
glm::vec3 up;//Up vector
glm::vec3 direction;//Direction
/**
	*   glm::mat4 getViewMatrix()
	*
	*   Summary of getViewMatrix() function:
	*
	*       Returns the current ViewMatrix
	*
	*   Parameters  :  
	*
	*		None
	*
	*   Return Value : 
	*
	*		Current ViewMatrix
	*
	*   Description :
	*
	*       Returns the current ViewMatrix.
*/
glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
/**
	*   glm::mat4 getProjectionMatrix()
	*
	*   Summary of getProjectionMatrix() function:
	*
	*       Returns the current ProjectionMatrix
	*
	*   Parameters  :
	*
	*		None
	*
	*   Return Value :
	*
	*		Current ProjectionMatrix
	*
	*   Description :
	*
	*       Returns the current ProjectionMatrix.
*/
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}
/**
	*   glm::mat4 getPosition()
	*
	*   Summary of getPosition() function:
	*
	*       Returns the current Position of eye
	*
	*   Parameters  :
	*
	*		None
	*
	*   Return Value :
	*
	*		Current Position
	*
	*   Description :
	*
	*       Returns the current Position of eye
*/
glm::vec3 getPosition() {
	return position;
}
/**
	*   void computeMatricesFromInputs(GLFWwindow* window, int* movType)
	*
	*   Summary of computeMatricesFromInputs(GLFWwindow* window, int* movType) function:
	*
	*       Computes the View matrix and Projection matrix from current cursor position or from current keyboard inputs
	*
	*   Parameters  :
	*
	*		GLFWwindow* window - Current context window
	*		int* movType - Movement Type (Restricted / Free)
	*
	*   Return Value :
	*
	*		None ( Void function)
	*
	*   Description :
	*
	*       Computes the View matrix and Projection matrix from current curs0r position or from current keyboard inputs.
	*		I to toggle movement type.
	*		If movType is 1, free movement through mouse and  W S A D keys.
	*		If movType is 0, restricted movement : 
	*		
	*			W and S to zoom.
	*			A and D to pan.
	*			Q and E to roll.
	*			Up and Down arrow keys to pitch.
	*			Right and Left arrow keys to yaw.
*/
void computeMatricesFromInputs(GLFWwindow* window, int* movType) {
	static double lastTime = glfwGetTime();
	double xpos, ypos;
	double currentTime = glfwGetTime();
	if(*movType){
		// Compute time difference between current and last frame
		float deltaTime = float(currentTime - lastTime);
		// Get mouse position
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, 1920 / 2, 1080 / 2);
		// Compute new orientation
		horizontalAngle += mouseSpeed * float(1920 / 2 - xpos);
		verticalAngle += mouseSpeed * float(1080 / 2 - ypos);
		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction=glm::vec3(cos(verticalAngle) * sin(horizontalAngle),sin(verticalAngle),cos(verticalAngle) * cos(horizontalAngle));
		direction = glm::normalize(direction);
		glm::vec3 right = glm::normalize(glm::vec3(sin(horizontalAngle - 3.14f / 2.0f),0,cos(horizontalAngle - 3.14f / 2.0f)));
		up = glm::normalize(glm::cross(right, direction));
		if (*movType) {
			// Move forward
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				position += direction * deltaTime * speed;
			}
			// Move backward
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				position -= direction * deltaTime * speed;
			}
			// Strafe right
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				position += right * deltaTime * speed;
			}
			// Strafe left
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				position -= right * deltaTime * speed;

			}
		}
		//Toggle Movement Type
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
				{
					*movType = !(*movType);
					cameraPos = position;
					cameraFront =direction;
					cameraUp = up;
				}
			}
		}
		center = position + direction;
		ViewMatrix = glm::lookAt(position, position + direction, up);
		lastTime = currentTime;
	}
	if (!*movType) {
		glm::vec3 right = glm::cross(cameraFront, cameraUp);
		const float cameraSpeed = 0.1f;
		//ZOOM
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;

		//PAN
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		//PITCH
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			pitch = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			pitch = -1.0f;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
			pitch = 0.0f;

		//YAW
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			yaw = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			yaw = -1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
			yaw = 0.0f;
		

		//ROLL
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			roll = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			roll = -1.0f;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
			roll = 0.0f;

		//Toggle Movement Type
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
				{
					*movType = !(*movType);
					up = cameraUp;
					position = cameraPos;
					direction = cameraFront;	
				}
			}
		}
		if (pitch) {
			cameraFront = glm::normalize(cameraFront * glm::cos(glm::radians(0.1f * pitch)) + cameraUp * glm::sin(glm::radians(0.1f * pitch)));
			cameraUp = glm::cross(right, cameraFront);
		}
		if (yaw) {
			cameraFront = glm::normalize(cameraFront * glm::cos(glm::radians(0.1f * yaw)) + right * glm::sin(glm::radians(0.1f * yaw)));
		}
		if (roll) {
			cameraUp = glm::normalize(cameraUp * glm::cos(glm::radians(0.1f * roll)) + right * glm::sin(glm::radians(0.1f * roll)));
		}

		ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		lastTime = currentTime;
	}
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 16.0f / 9.0f, 0.1f, 100.0f);
}

