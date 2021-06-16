
#include "deformableObjects.h"
#include "../utilities/helperFunctions.h"

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

deformableObjects::~deformableObjects() = default;

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
    //glBufferSubData(GL_ARRAY_BUFFER, 0, getSize(), getNormalBuffer());
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

void deformableObjects::setWind(const glm::vec3 &w) {
    deformableObjects::wind = w;
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

void deformableObjects::setGravity(float g) {
    deformableObjects::gravity = g;
}

void deformableObjects::reset() {}

helperStruct deformableObjects::isHovered(glm::vec3 origin, glm::vec3 direction) {
    return helperStruct();
}

float deformableObjects::getGravity() const {
    return gravity;
}


void deformableObjects::renderShadow(glm::mat4 depthP, glm::mat4 depthV, GLuint programID) {
    //In moment like this I truly believe my iq is negative
    glUseProgram(programID);

    //glCullFace(GL_BACK);
    //Get a handle for all of the uniforms
    GLuint MVP = glGetUniformLocation(programID, "depthMVP");
    glm::mat4 tmp = depthP*depthV*modelMatrix;

    //set the uniforms
    glUniformMatrix4fv(MVP, 1, GL_FALSE, &tmp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
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

void deformableObjects::renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID) {
    glUseProgram(programID);
    //glCullFace(GL_BACK);
    //Get a handle for all of the uniforms
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
    GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");

    // Get a handle for our "LightPosition" uniform
    GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");

    glm::mat4 MVP = pr*viw*modelMatrix;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viw[0][0]);
    glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &(bias*modelMatrix)[0][0]);
    glUniform1i(ShadowMapID, 0);

    glUniform3f(lightInvDirID, lightPos.x, lightPos.y, lightPos.z);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
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
    glBindBuffer(GL_ARRAY_BUFFER, normal);
    glBufferSubData(GL_ARRAY_BUFFER, 0, getSize(), getNormalBuffer());
    glVertexAttribPointer(
            2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colour);
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

void deformableObjects::setCullFace(GLuint in) {
    glCullFace(in);
}

void deformableObjects::rungeKutta4(float timeDelta) {
    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    std::vector<glm::vec3> b1;
    std::vector<glm::vec3> b2;
    std::vector<glm::vec3> c1;
    std::vector<glm::vec3> c2;
    std::vector<glm::vec3> d1;
    std::vector<glm::vec3> d2;

    //gravity
    for(int i =0;i< particles.size();++i){
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(deformableObjects::wind+glm::vec3(0.0f,deformableObjects::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for(int i=0; i<springs.size(); ++i){
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for(int i=0; i<particles.size();++i){
        particles[i]->addForce(particles[i]->getCollisionForce());
        //particles[i]->addForce(particles[i]->getFrictionForce());
        a2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //second eval
    for(int i =0; i<particles.size();++i){
        b1.push_back(particles[i]->getVelocity()+(timeDelta/2.0f)*a2[i]);
        particles[i]->setForce(deformableObjects::wind+glm::vec3(0.0f,deformableObjects::gravity, 0.0f));
        particles[i]->addForce(particles[i]->getCollisionForce());
        //particles[i]->addForce(particles[i]->getFrictionForce());
    }

    for(int i=0; i<springs.size(); ++i){
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta/2.0f)*a1[part1], (timeDelta/2.0f)*a1[part2],
                                     (timeDelta/2.0f)*a2[part1], (timeDelta/2.0f)*a2[part2]);
    }

    //b2
    for(int i =0; i<particles.size();++i){
        b2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //third eval
    for(int i =0; i<particles.size();++i){
        c1.push_back(particles[i]->getVelocity()+(timeDelta/2.0f)*b2[i]);
        particles[i]->setForce(deformableObjects::wind+glm::vec3(0.0f,deformableObjects::gravity, 0.0f));
        particles[i]->addForce(particles[i]->getCollisionForce());
       // particles[i]->addForce(particles[i]->getFrictionForce());
    }

    for(int i=0; i<springs.size(); ++i){
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta/2.0f)*b1[part1], (timeDelta/2.0f)*b1[part2],
                                     (timeDelta/2.0f)*b2[part1], (timeDelta/2.0f)*b2[part2]);
    }

    for(int i=0; i<particles.size(); ++i){
        c2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //last eval
    for(int i =0; i<particles.size();++i){
        d1.push_back(particles[i]->getVelocity()+(timeDelta)*c2[i]);
        particles[i]->setForce(deformableObjects::wind+glm::vec3(0.0f,deformableObjects::gravity, 0.0f));
        particles[i]->addForce(particles[i]->getCollisionForce());
        //particles[i]->addForce(particles[i]->getFrictionForce());
    }

    for(int i=0; i<springs.size(); ++i){
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta)*c1[part1], (timeDelta)*c1[part2],
                                     (timeDelta)*c2[part1], (timeDelta)*c2[part2]);
    }

    for(int i=0; i<particles.size(); ++i){
        d2.push_back(particles[i]->getForce()/particles[i]->getMass());
        //particles[i]->resetFrictionForce();
        particles[i]->resetCollisionForce();
    }

    //update vel and pos
    for(int i=0; i<particles.size(); ++i){
        if(!vectorContains(specialParticles, particles[i])){
            glm::vec3 pos = particles[i]->getPosition();
            pos += (timeDelta/6.0f)*(a1[i]+2.0f*b1[i]+2.0f*c1[i]+d1[i]);
            particles[i]->setPosition(pos);

            glm::vec3 vel = particles[i]->getVelocity();
            vel+=(timeDelta/6.0f)*(a2[i]+2.0f*b2[i]+2.0f*c2[i]+d2[i]);
            particles[i]->setVelocity(vel);
        }
    }
}

void deformableObjects::exEuler(float timeDelta) {
    //update force for all particles
    //adding gravity
    for(int i=0; i<particles.size(); ++i){
        particles[i]->setForce(wind+glm::vec3(0.0f,gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for(int i=0; i<springs.size(); ++i){
        //update both particles
        springs[i]->updateParticlesForce();
    }

    //there is collision now
    //must add collision force
    for(int i=0; i<particles.size();++i){
        particles[i]->addForce(particles[i]->getCollisionForce());
        //particles[i]->addForce(particles[i]->getFrictionForce());
        //once we added it we can reset it to 0
        particles[i]->resetCollisionForce();
        //particles[i]->resetFrictionForce();
    }

    //update velocity
    for(int i=0; i<particles.size();++i){
        particle * tmp = particles[i];
        glm::vec3 vel = tmp->getVelocity();
        vel+= timeDelta*tmp->getForce()/tmp->getMass();
        //life is void of meaning
        particles[i]->setVelocity(vel);
    }

    //update position
    for(int i=0; i<particles.size();i++){
        //the top right and left particle are fixed in place
        //like if the cloth was hung
        //i!=col*row-1 && i!= col*(row-1)
        //i!=0 && i!= col-1
        if(!vectorContains(specialParticles, particles[i])){
            particle * tmp = particles[i];
            glm::vec3 pos = tmp->getPosition();
            pos+= tmp->getVelocity()*timeDelta;
            particles[i]->setPosition(pos);
        }
    }
}

void deformableObjects::rungeKutta2(float timeDelta) {
    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    //gravity
    for(int i =0;i< particles.size();++i){
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(wind+glm::vec3(0.0f,gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for(int i=0; i<springs.size(); ++i){
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for(int i=0; i<particles.size();++i){
        particles[i]->addForce(particles[i]->getCollisionForce());
        //particles[i]->addForce(particles[i]->getFrictionForce());
        a2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //second evaluation of the forces
    for(int i =0;i< particles.size();++i){
        particles[i]->setForce(wind+glm::vec3(0.0f, gravity, 0.0f)+particles[i]->getCollisionForce());
        particles[i]->resetCollisionForce();
        //particles[i]->resetFrictionForce();
    }

    for(int i=0; i<springs.size(); ++i){
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta/2.0f)*a1[part1], (timeDelta/2.0f)*a1[part2],
                                     (timeDelta/2.0f)*a2[part1], (timeDelta/2.0f)*a2[part2]);
    }

    for(int i=0; i<particles.size(); ++i){
        if(!vectorContains(specialParticles, particles[i])){
            glm::vec3 vel = particles[i]->getVelocity();

            //updating position
            glm::vec3 pos = particles[i]->getPosition();
            pos+=timeDelta*(vel+(timeDelta/2.0f)*a2[particles[i]->getId()]);
            particles[i]->setPosition(pos);

            //updating velocity
            vel+=timeDelta*(particles[i]->getForce()/particles[i]->getMass());
            particles[i]->setVelocity(vel);
        }
    }
}
