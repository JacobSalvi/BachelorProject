
#include "deformableObjects.h"

deformableObjects::deformableObjects(float gravity, const glm::mat4 &modelMatrix, const glm::vec3 &lightPos) :
gravity(gravity), modelMatrix(modelMatrix), lightPos(lightPos), wind(glm::vec3(0.0f,0.0f,0.0f)) {}

int deformableObjects::getSize() {
    return 0;
}

int deformableObjects::getNumberOfVertices() {
    return 0;
}

void deformableObjects::setGLuint() {}

float *deformableObjects::getVertexBuffer(){
    return vertexBuffer;
}

float *deformableObjects::getColorBuffer(){
    return colorBuffer;
}

float *deformableObjects::getNormalBuffer() const {
    return normalBuffer;
}

void deformableObjects::setVertex(GLuint newVert) {
    deformableObjects::vertex = newVert;
}

void deformableObjects::setColour(GLuint newColour) {
    deformableObjects::colour = newColour;
}

void deformableObjects::setNormal(GLuint newNormal) {
    deformableObjects::normal = newNormal;
}

void deformableObjects::emptySpecialParticles() {
    deformableObjects::specialParticles.clear();
}

void deformableObjects::setSpecial(particle *p) {
    deformableObjects::specialParticles.push_back(p);
}

deformableObjects::~deformableObjects() {}

void deformableObjects::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) {
    //just to be safe
    glUseProgram(programID);

    //Get a handle for all of the uniforms
    GLuint modelUn = glGetUniformLocation(programID, "modelMatrix");
    GLuint viewUn = glGetUniformLocation(programID, "viewMatrix");
    GLuint projectionUn = glGetUniformLocation(programID, "projectionMatrix");
    GLuint lightUn = glGetUniformLocation(programID, "lightDirection");
    //set the uniforms
    glUniformMatrix4fv(modelUn, 1, GL_FALSE, &getModelMatrix()[0][0]);
    glUniformMatrix4fv(viewUn, 1, GL_FALSE, &ViewMatrix[0][0]);
    glUniformMatrix4fv(projectionUn, 1, GL_FALSE, &ProjectionMatrix[0][0]);
    glUniform3f(lightUn, lightPos[0], lightPos[1], lightPos[2]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, deformableObjects::vertex);
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
    glBindBuffer(GL_ARRAY_BUFFER, deformableObjects::colour);
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
    glBindBuffer(GL_ARRAY_BUFFER, deformableObjects::normal);
    glVertexAttribPointer(
            2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) nullptr                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    //render wireframe only when desired
}

const glm::mat4 &deformableObjects::getModelMatrix() const {
    return modelMatrix;
}

void deformableObjects::integrate(float timeDelta) {}

void deformableObjects::updateBuffer() {}

void deformableObjects::detectCollision(collidable *obj) {}

BVH *deformableObjects::getBvh() const {
    return nullptr;
}

void deformableObjects::setWind(const glm::vec3 &wind) {
    deformableObjects::wind = wind;
}

void deformableObjects::setMass(float mass) {
    for(auto i:deformableObjects::particles){
        i->setMass(mass);
    }
}

void deformableObjects::setStiffness(float stiffness){
    for(auto i : deformableObjects::springs){
        i->setKs(stiffness);
    }
}

void deformableObjects::setGravity(float gravity) {
    deformableObjects::gravity = gravity;
}

void deformableObjects::reset() {}

helperStruct deformableObjects::isHovered(glm::vec3 origin, glm::vec3 direction) {
    return helperStruct();
}
