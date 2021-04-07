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

void collidable::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, bool wireFrame){
    glm::mat4 mvp = ProjectionMatrix * ViewMatrix * getModel();

    //for the wireframe
    wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE):glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUniformMatrix4fv(triangleMatrixID, 1, GL_FALSE, &mvp[0][0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, getCollVertex());
    glBufferSubData(GL_ARRAY_BUFFER, 0, getSize(), getVertexBuffer());
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, getCollColour());
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) nullptr                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

    //reset to default GL_FILL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

