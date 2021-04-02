#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include "../integrators/net.h"

void computeMatricesFromInputs();
void computeMatricesFromInputs(int * azimuthal, int* polar, float distance);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getCameraPosition();

void setWind(net * net, float wind[]);

glm::vec3 getMouseRay(ImVec2 mousePosition);

#endif
