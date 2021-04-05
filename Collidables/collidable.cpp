#include "collidable.h"

collidable::collidable(glm::vec3 colour, glm::mat4 model) : colour(colour), model(model){
}

float *collidable::getVertexBuffer() const {
    return vertexBuffer;
}

float *collidable::getColorBuffer() const {
    return colorBuffer;
}

void collidable::setCollVertex(GLuint collVertex) {
    collidable::collVertex = collVertex;
}

void collidable::setCollColour(GLuint collColour) {
    collidable::collColour = collColour;
}

void collidable::setCollNormal(GLuint collNormal) {
    collidable::collNormal = collNormal;
}

GLuint collidable::getCollVertex() const {
    return collVertex;
}

GLuint collidable::getCollColour() const {
    return collColour;
}

GLuint collidable::getCollNormal() const {
    return collNormal;
}

const glm::mat4 &collidable::getModel() const {
    return model;
}

int collidable::getSize() {
    return 0;
}

int collidable::getNumberOfVertices() {
    return 0;
}

float *collidable::getNormalBuffer() const {
    return normalBuffer;
}

void collidable::handleCollision(collidable *b) {

}

mouseIntersectStruct collidable::isHovered(glm::vec3 origin, glm::vec3 direction) {
    mouseIntersectStruct toReturn;
    return toReturn;
}

void collidable::getName() {

}

void collidable::setModel(const glm::mat4 &model) {
    collidable::model = model;
}

