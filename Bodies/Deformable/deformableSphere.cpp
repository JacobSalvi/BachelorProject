
#include "deformableSphere.h"
#include "../../utilities/helperFunctions.h"
#include "../../BVH/defSphereBVH.h"

//sorting based on y
bool sortHelperY(particle *a, particle *b) {
    return a->getPosition().y > b->getPosition().y;
}

//sorting based on counterclockwise angle
bool sortHelperAngle(particle *a, particle *b) {
    //a and b should be at the same y, unless I somehow did something wrong
    glm::vec3 center(0, a->getPosition().y, 0);
    float angle1 = atan2(a->getPosition().z - center.z, a->getPosition().x - center.x);
    float angle2 = atan2(b->getPosition().z - center.z, b->getPosition().x - center.x);
    return angle1 < angle2;
}

deformableSphere::deformableSphere(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos) : deformableObjects(-3.0f, mod,
                                                                                                       lPos), size(10) {
    //I fear that a too detailed sphere might
    //burn my cpu, so for now it will have size <10
    vertexBuffer = new float[size * (size) * 18];
    colorBuffer = new float[size * (size) * 18];

    //helper needed to avoid pushing multiple times
    //the same vertex in the particles
    std::vector<particle *> helper;

    int colPos = 0;
    float step = (float) 1.0f / (float)size;
    float u = 0.0f;
    for (int i = 0; i < size; ++i) {
        float v = 0.0f;
        for (int j = 0; j < size; ++j) {
            double t = sin(M_PI * v);
            double x1 = t * cos(2 * M_PI * u);
            double z1 = t * sin(2 * M_PI * u);
            double y1 = cos(M_PI * v);

            double x4 = t * cos(2 * M_PI * (u + step));
            double z4 = t * sin(2 * M_PI * (u + step));
            double y4 = cos(M_PI * v);

            t = sin(M_PI * (v + step));
            double x2 = t * cos(2 * M_PI * u);
            double z2 = t * sin(2 * M_PI * u);
            double y2 = cos(M_PI * (v + step));

            double x3 = t * cos(2 * M_PI * (u + step));
            double z3 = t * sin(2 * M_PI * (u + step));
            double y3 = cos(M_PI * (v + step));

            //add particles
            auto *p1 = new particle(glm::vec3(x1 / 2.0f, y1 / 2.0f, z1 / 2.0f), 1.0f);
            auto *p2 = new particle(glm::vec3(x2 / 2.0f, y2 / 2.0f, z2 / 2.0f), 1.0f);
            auto *p3 = new particle(glm::vec3(x3 / 2.0f, y3 / 2.0f, z3 / 2.0f), 1.0f);
            auto *p4 = new particle(glm::vec3(x4 / 2.0f, y4 / 2.0f, z4 / 2.0f), 1.0f);

            helper.push_back(p1);
            helper.push_back(p2);
            helper.push_back(p3);
            helper.push_back(p4);

            for (int k = 0; k < 6; k++) {
                colorBuffer[colPos++] = color[0];
                colorBuffer[colPos++] = color[1];
                colorBuffer[colPos++] = color[2];
            }
            v += step;
        }
        u += step;
    }

    //clean the helper
    int it = 0;
    while (it < helper.size()) {
        int internal = it + 1;
        while (internal < helper.size()) {
            glm::vec3 curr = helper[it]->getPosition();
            //due to the conversion from double to float
            //I added the second part to find duplicates
            //that wouldn't be found other wise
            if (helper[internal]->getPosition() == curr ||
                glm::length(curr - helper[internal]->getPosition()) < 0.0001) {
                helper.erase(helper.begin() + internal);
            } else {
                internal++;
            }
        }
        it++;
    }

    // now that we successfully removed all duplicates,
    // we can sort the particle by y and counter clock wise rotation
    std::sort(helper.begin(), helper.end(), sortHelperY);
    //it bugged me to see the approximation
    helper[helper.size() - 1]->setPosition(glm::vec3(0, -0.5, 0));

    //sorting based on the counter clockwise order
    //the first and last particle are ok
    for (int i = 0; i < size - 1; ++i) {
        std::sort(helper.begin() + 1 + i * size, helper.begin() + 1 + (i + 1) * size, sortHelperAngle);
    }

    //set the id in a meaningful way
    //and finally set the particle vector
    for (int i = 0; i < helper.size(); ++i) {
        helper[i]->setId(i);
        //printPoint(helper[i]->getPosition(), "point i is");
        //for the reset
        resetPos.push_back(helper[i]->getPosition());

        particles.push_back(helper[i]);
    }

    specialParticles.push_back(particles[0]);

    //springs
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = i + 1; j < particles.size(); ++j) {
            float dist = glm::length(particles[i]->getPosition() - particles[j]->getPosition());
            spring * toADD =new spring(dist, 50, .2, particles[i], particles[j]);
            springs.push_back(toADD);
            //springs.push_back(new spring(dist, 50, .2, particles[i], particles[j]));
        }
    }

    normalBuffer = new float[size*(size)*18];

    updateBuffer();

    //normals are the same as the vertex

    //normalBuffer= vertexBuffer;
    for(int i=0; i<size*(size)*18;++i){
        normalBuffer[i]=vertexBuffer[i];
    }

    setGLuint();

    //making the bvh
    bvh = new defSphereBVH(particles, 0.5f, glm::vec3(0.0f,0.0f,0.0f));
}

void deformableSphere::setGLuint() {
    GLuint deformableSphereVertex;
    glGenBuffers(1, &deformableSphereVertex);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereVertex);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint deformableSphereColor;
    glGenBuffers(1, &deformableSphereColor);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereColor);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getColorBuffer(), GL_STATIC_DRAW);


    GLuint deformableSphereNormal;
    glGenBuffers(1, &deformableSphereNormal);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereNormal);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getNormalBuffer(), GL_DYNAMIC_DRAW);

    deformableSphere::setVertex(deformableSphereVertex);
    deformableSphere::setColour(deformableSphereColor);
    deformableSphere::setNormal(deformableSphereNormal);
}

int deformableSphere::getSize() {
    return sizeof(float) * size * size * 18;
}

int deformableSphere::getNumberOfVertices() {
    return size * size * 18;
}

void deformableSphere::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) {
    deformableObjects::render(ProjectionMatrix, ViewMatrix, programID);
#if 0
    bvh->render(ProjectionMatrix, ViewMatrix, programID, true, getModelMatrix());
#endif
}

helperStruct deformableSphere::isHovered(glm::vec3 origin, glm::vec3 direction) {
    helperStruct toReturn = deformableSphere::bvh->rayIntersect(origin, direction, modelMatrix);
    return toReturn;
}

void deformableSphere::integrate(float timeDelta) {
    //deformableObjects::exEuler(timeDelta);
    //deformableSphere::rungeKutta(timeDelta);
    deformableSphere::rungeKutta4(timeDelta);
}

BVH *deformableSphere::getBvh() const {
    return bvh;
}

void deformableSphere::rungeKutta(float timeDelta) {
    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    //gravity
    for (int i = 0; i < particles.size(); ++i) {
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(deformableSphere::wind + glm::vec3(0.0f, deformableSphere::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for (int i = 0; i < springs.size(); ++i) {
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->addForce(particles[i]->getCollisionForce());
        a2.push_back(particles[i]->getForce() / particles[i]->getMass());
    }

    //second evaluation of the forces
    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->setForce(deformableSphere::wind + glm::vec3(0.0f, deformableSphere::gravity, 0.0f) +
                               particles[i]->getCollisionForce());
        particles[i]->resetCollisionForce();
    }

    for (int i = 0; i < springs.size(); ++i) {
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta / 2.0f) * a1[part1], (timeDelta / 2.0f) * a1[part2],
                                     (timeDelta / 2.0f) * a2[part1], (timeDelta / 2.0f) * a2[part2]);
    }

    for (int i = 0; i < particles.size(); ++i) {
        if (!vectorContains(specialParticles, particles[i])) {
            glm::vec3 vel = particles[i]->getVelocity();

            //updating position
            glm::vec3 pos = particles[i]->getPosition();
            pos += timeDelta * (vel + (timeDelta / 2.0f) * a2[particles[i]->getId()]);
            particles[i]->setPosition(pos);

            //updating velocity
            vel += timeDelta * (particles[i]->getForce() / particles[i]->getMass());
            particles[i]->setVelocity(vel);
        }
    }
}

// total number of particles
// 2 + size*(size-1)
//number of triangles (size^2)*2
void deformableSphere::updateBuffer() {
    int currPart = 0;
    int pos = 0;

    //compute center
    glm::vec3 center = particles[0]->getPosition()+0.5f*(glm::normalize(particles[particles.size()-1]->getPosition()-particles[0]->getPosition()));
    int posN = 0;

    while (currPart < particles.size()) {
        //top point
        if (currPart == 0) {
            glm::vec3 top = particles[0]->getPosition();
            for (int i = 1; i <= size; ++i) {
                glm::vec3 l;
                glm::vec3 r;
                if (i == size) {
                    l = particles[i]->getPosition();
                    r = particles[1]->getPosition();
                } else {
                    l = particles[i]->getPosition();
                    r = particles[i + 1]->getPosition();
                }
                vertexBuffer[pos++] = top.x;
                vertexBuffer[pos++] = top.y;
                vertexBuffer[pos++] = top.z;

                vertexBuffer[pos++] = l.x;
                vertexBuffer[pos++] = l.y;
                vertexBuffer[pos++] = l.z;

                vertexBuffer[pos++] = r.x;
                vertexBuffer[pos++] = r.y;
                vertexBuffer[pos++] = r.z;

                //update normal
                normalBuffer[posN++]=top.x-center.x;
                normalBuffer[posN++]=top.y-center.y;
                normalBuffer[posN++]=top.z-center.z;

                normalBuffer[posN++]=l.x -center.x;
                normalBuffer[posN++]=l.y -center.y;
                normalBuffer[posN++]=l.z -center.z;

                normalBuffer[posN++]=r.x -center.x;
                normalBuffer[posN++]=r.y -center.y;
                normalBuffer[posN++]=r.z -center.z;
            }
            currPart++;
        } else if (currPart == particles.size() - 1 - size) {
            //last layer
            glm::vec3 bottom = particles[particles.size() - 1]->getPosition();
            for (int i = currPart; i < currPart + size; ++i) {
                glm::vec3 l;
                glm::vec3 r;
                if (i == particles.size() - 2) {
                    l = particles[i]->getPosition();
                    r = particles[currPart]->getPosition();
                } else {
                    l = particles[i]->getPosition();
                    r = particles[i + 1]->getPosition();
                }
                vertexBuffer[pos++] = l.x;
                vertexBuffer[pos++] = l.y;
                vertexBuffer[pos++] = l.z;

                vertexBuffer[pos++] = bottom.x;
                vertexBuffer[pos++] = bottom.y;
                vertexBuffer[pos++] = bottom.z;

                vertexBuffer[pos++] = r.x;
                vertexBuffer[pos++] = r.y;
                vertexBuffer[pos++] = r.z;

                //update normal
                normalBuffer[posN++]=l.x -center.x;
                normalBuffer[posN++]=l.y -center.y;
                normalBuffer[posN++]=l.z -center.z;

                normalBuffer[posN++]=bottom.x -center.x;
                normalBuffer[posN++]=bottom.y -center.y;
                normalBuffer[posN++]=bottom.z -center.z;

                normalBuffer[posN++]=r.x -center.x;
                normalBuffer[posN++]=r.y -center.y;
                normalBuffer[posN++]=r.z -center.z;
            }
            break;
        } else {
            //layers in between
            glm::vec3 tl;
            glm::vec3 bl;
            glm::vec3 tr;
            glm::vec3 br;
            if(currPart%size==0) {
                tl=particles[currPart]->getPosition();
                bl=particles[currPart+size]->getPosition();
                tr=particles[currPart+1-size]->getPosition();
                br=particles[currPart+1]->getPosition();

            }else{
                tl=particles[currPart]->getPosition();
                tr=particles[currPart+1]->getPosition();
                bl=particles[currPart+size]->getPosition();
                br=particles[currPart+size+1]->getPosition();
            }
            //the two triangles now
            vertexBuffer[pos++] = tl.x;
            vertexBuffer[pos++] = tl.y;
            vertexBuffer[pos++] = tl.z;

            vertexBuffer[pos++] = bl.x;
            vertexBuffer[pos++] = bl.y;
            vertexBuffer[pos++] = bl.z;

            vertexBuffer[pos++] = tr.x;
            vertexBuffer[pos++] = tr.y;
            vertexBuffer[pos++] = tr.z;

            vertexBuffer[pos++] = tr.x;
            vertexBuffer[pos++] = tr.y;
            vertexBuffer[pos++] = tr.z;

            vertexBuffer[pos++] = bl.x;
            vertexBuffer[pos++] = bl.y;
            vertexBuffer[pos++] = bl.z;

            vertexBuffer[pos++] = br.x;
            vertexBuffer[pos++] = br.y;
            vertexBuffer[pos++] = br.z;

            //update normal
            normalBuffer[posN++]=tl.x -center.x;
            normalBuffer[posN++]=tl.y -center.y;
            normalBuffer[posN++]=tl.z -center.z;

            normalBuffer[posN++]=bl.x -center.x;
            normalBuffer[posN++]=bl.y -center.y;
            normalBuffer[posN++]=bl.z -center.z;

            normalBuffer[posN++]=tr.x -center.x;
            normalBuffer[posN++]=tr.y -center.y;
            normalBuffer[posN++]=tr.z -center.z;

            normalBuffer[posN++]=tr.x -center.x;
            normalBuffer[posN++]=tr.y -center.y;
            normalBuffer[posN++]=tr.z -center.z;

            normalBuffer[posN++]=bl.x -center.x;
            normalBuffer[posN++]=bl.y -center.y;
            normalBuffer[posN++]=bl.z -center.z;

            normalBuffer[posN++]=br.x -center.x;
            normalBuffer[posN++]=br.y -center.y;
            normalBuffer[posN++]=br.z -center.z;
            currPart++;
        }
    }

}

void deformableSphere::reset() {
    //reset particles position velocity and force
    for(int i=0; i<particles.size();++i){
        particles[i]->setPosition(resetPos[i]);
        particles[i]->setVelocity(glm::vec3(0.0f,0.0f,0.0f));
        particles[i]->setForce(glm::vec3(0.0f,0.0f,0.0f));
    }

    //reset wind and gravity
    deformableSphere::wind= glm::vec3(0,0,0);
    deformableSphere::gravity=-3;

    //reset hanging particle
    deformableSphere::specialParticles.clear();
    deformableSphere::specialParticles.push_back(particles[0]);

    //reset vertexbuffer
    deformableSphere::updateBuffer();

    //reset bvh
    bvh->update();
}

deformableSphere::~deformableSphere() {
    std::cout<<"def sphere destroyed"<<std::endl;
}

void deformableSphere::detectCollision(collidable *obj) {
    switch(obj->returnType()){
        case 0:
            //collision with sphere
            deformableSphere::bvh->detectCollisionSphere(modelMatrix, obj, gravity);
            break;
        case 1:
            //collision with cube
            deformableSphere::bvh->detectCollisionCube(modelMatrix, obj, gravity);
            break;
        case 2:
            //collision with plane
            deformableSphere::bvh->detectCollisionPlane(modelMatrix, obj, gravity);
            break;
        default:
            std::cout<<"something went wrong"<<std::endl;
            break;
    }
}

void deformableSphere::renderShadow(glm::mat4 depthP, glm::mat4 depthV, GLuint programId) {
    setCullFace(GL_FRONT);
    deformableObjects::renderShadow(depthP, depthV, programId);
}

void deformableSphere::renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID) {
    setCullFace(GL_BACK);
    deformableObjects::renderShadow(pr, viw, bias, programID);
}
