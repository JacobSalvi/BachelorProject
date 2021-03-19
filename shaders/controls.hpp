#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include "../integrators/net.h"

void computeMatricesFromInputs();
void computeMatricesFromInputs(int * azimuthal, int* polar, int distance);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

void setWind(net * net, float wind[]);

#endif
