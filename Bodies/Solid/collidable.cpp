#include <glm/gtc/matrix_transform.hpp>
#include "collidable.h"
#include <iostream>

collidable::collidable(glm::vec3 colour, glm::mat4 model, glm::vec3 lPos) : colour(colour), model(model), lightPos(lPos){
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

void collidable::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID, bool wireFrame, glm::mat4 m){
    glm::mat4 tmp = m*getModel();
    //glm::mat4 mvp = ProjectionMatrix * ViewMatrix * tmp;

    //In moment like this I truly believe my iq is negative
    glUseProgram(programID);

    //for the wireframe
    wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE):glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Get a handle for all of the uniforms
    GLuint modelUn = glGetUniformLocation(programID, "modelMatrix");
    GLuint viewUn = glGetUniformLocation(programID, "viewMatrix");
    GLuint projectionUn = glGetUniformLocation(programID, "projectionMatrix");
    GLuint lightUn = glGetUniformLocation(programID, "lightDirection");
    //set the uniforms
    glUniformMatrix4fv(modelUn, 1, GL_FALSE, &tmp[0][0]);
    glUniformMatrix4fv(viewUn, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(projectionUn, 1, GL_FALSE, &ProjectionMatrix[0][0]);
    glUniform3f(lightUn, lightPos[0], lightPos[1], lightPos[2]);

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

    //normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, getCollNormal());
    glVertexAttribPointer(
            2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
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
    glDisableVertexAttribArray(2);
}

void collidable::setGLuint() {
    GLuint v;
    glGenBuffers(1, &v);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glBufferData(GL_ARRAY_BUFFER, getSize(), getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint c;
    glGenBuffers(1, &c);
    glBindBuffer(GL_ARRAY_BUFFER, c);
    glBufferData(GL_ARRAY_BUFFER, getSize(), getColorBuffer(), GL_STATIC_DRAW);

    GLuint n;
    glGenBuffers(1, &n);
    glBindBuffer(GL_ARRAY_BUFFER, n);
    glBufferData(GL_ARRAY_BUFFER, getSize(), getNormalBuffer(), GL_STATIC_DRAW);

    setCollVertex(v);
    setCollColour(c);
    setCollNormal(n);
}

void collidable::move(glm::vec3 mv) {
    model = glm::translate(model, mv);
}

int collidable::returnType() {
    return -1;
}

void collidable::renderShadow(glm::mat4 depthP, glm::mat4 depthV, GLuint programID) {
    //In moment like this I truly believe my iq is negative
    glUseProgram(programID);

    //Get a handle for all of the uniforms
    GLuint MVP = glGetUniformLocation(programID, "depthMVP");
    glm::mat4 tmp = depthP*depthV*getModel();

    //set the uniforms
    glUniformMatrix4fv(MVP, 1, GL_FALSE, &tmp[0][0]);

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

    glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

    glDisableVertexAttribArray(0);
}

void collidable::renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID) {
    glUseProgram(programID);
    //Get a handle for all of the uniforms
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
    GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");

    // Get a handle for our "LightPosition" uniform
    GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");

    glm::mat4 MVP = pr*viw*getModel();

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &getModel()[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viw[0][0]);
    glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &(bias*getModel())[0][0]);
    glUniform1i(ShadowMapID, 0);

    glUniform3f(lightInvDirID, lightPos.x, lightPos.y, lightPos.z);

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

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, getCollNormal());
    glVertexAttribPointer(
            2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, getCollColour());
    glVertexAttribPointer(
            1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

