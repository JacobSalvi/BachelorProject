#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include <GL/glew.h>
#include "../Bodies/Deformable/net.h"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

void computeMatricesFromInputs();
void computeMatricesFromInputs(int * azimuthal, int* polar, float distance);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getCameraPosition();

void setWind(net * net, float wind[]);

glm::vec3 getMouseRay(ImVec2 mousePosition);

#endif
