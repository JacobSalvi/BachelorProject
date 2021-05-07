#ifndef CODE_HELPERFUNCTIONS_H
#define CODE_HELPERFUNCTIONS_H

#include <iostream>


// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

using namespace glm;

#include "../Bodies/Deformable/net.h"
#include "../Bodies/Deformable/texturedCloth.h"
#include "../Bodies/Deformable/deformableSphere.h"
#include "../Bodies/Deformable/deformableCube.h"
#include "../shaders/helperStruct.h"
#include "../Bodies/Solid/Sphere.h"
#include "../Bodies/Solid/cube.h"
#include "../Bodies/Solid/plane.h"

//culture stuff
void culture(std::vector<deformableObjects *> *list, GLuint texture, GLuint textureId, glm::mat4 mod, glm::vec3 lPos, GLuint pID);

//cloth
void addCloth(std::vector<deformableObjects *> *list, int col, int row, int in, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos);
void addDefSphere(std::vector<deformableObjects *> *list, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos);
void addDefCube(std::vector<deformableObjects *> *list, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos);

//collidables
void addColl(std::vector<collidable *> * list, int type, glm::vec3 lPos);

//dragging
mouseIntersectStruct isMouseOverColl(glm::vec3 origin, glm::vec3 direction, std::vector<collidable *> * list);
helperStruct isMouseOverDeformable(glm::vec3 origin, glm::vec3 direction, std::vector<deformableObjects *> * list);

glm::vec3 planeVectorIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeNormal, glm::vec3 planePoint);
glm::vec3 pointPlaneProjection(glm::vec3 point, glm::vec3 planeNormal, glm::vec3 planePoint);

//utilities
//I can't figure out how to use std::find
bool vectorContains(std::vector<particle *> v, particle * e);
void printPoint(glm::vec3 point, char * name);

void setWind(deformableObjects * net, float wind[]);

#endif //CODE_HELPERFUNCTIONS_H
