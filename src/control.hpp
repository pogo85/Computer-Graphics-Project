#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(GLFWwindow* window, int* movType);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getPosition();
#endif