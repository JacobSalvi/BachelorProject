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

GLuint collidable::getCollVertex() const {
    return collVertex;
}

GLuint collidable::getCollColour() const {
    return collColour;
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

