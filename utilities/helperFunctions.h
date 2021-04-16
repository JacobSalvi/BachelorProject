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
#include "../shaders/helperStruct.h"
#include "../Collidables/Sphere.h"
#include "../Collidables/cube.h"
#include "../Collidables/plane.h"

//culture stuff
void culture(std::vector<helperStruct *> *list, glm::vec3 tr);
void drawCulture(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint MatrixID, helperStruct* obj, GLuint texture, GLuint textureId, GLuint program);

//cloth
void addCloth(std::vector<net *> *list, int col, int row, int in, glm::vec3 colour, glm::vec3 tr);

//collidables
void addColl(std::vector<collidable *> * list, int type, glm::vec3 lPos);

//dragging
mouseIntersectStruct isMouseOverColl(glm::vec3 origin, glm::vec3 direction, std::vector<collidable *> * list);
helperStruct isMouseOverDeformable(glm::vec3 origin, glm::vec3 direction, std::vector<net *> * list);

glm::vec3 planeVectorIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeNormal, glm::vec3 planePoint);
glm::vec3 pointPlaneProjection(glm::vec3 point, glm::vec3 planeNormal);

//utilities
//I can't figure out how to use std::find
bool vectorContains(std::vector<particle *> v, particle * e);
void printPoint(glm::vec3 point, char * name);

#endif //CODE_HELPERFUNCTIONS_H
