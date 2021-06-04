
#include "deformableCube.h"
#include "../utilities/helperFunctions.h"
#include "../../BVH/defCubeBVH.h"
#include <map>

deformableCube::~deformableCube() {
    std::cout<<"deformable cube destroyed"<<std::endl;
}

// a cube is a sort of grid of inner smaller cubes
// also it is a cube of side 1
deformableCube::deformableCube(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos) :
    size(4), deformableObjects(-5, mod, lPos) {
    //we have 6 faces, each face has size^2 sub squares in it
    // each sub square is formed by 2 triangle
    deformableCube::vertexBuffer = new float[6*2*9*size*size];
    deformableCube::colorBuffer = new float[6*2*9*size*size];
    deformableCube::normalBuffer = new float[6*2*9*size*size];

    float mass = 1.0f;
    int id=0;
    float compar = float(size);
    //setting up particles
    for(float y=0; y<=compar; ++y){
        for(float x=0; x<=compar; ++x){
            for(float z=0; z<=compar; ++z){
                glm::vec3 idk(x/compar, y/compar, z/compar);
                deformableCube::resetPos.push_back(idk);
                auto * toAdd = new particle(idk , mass);
                toAdd->setId(id++);
                deformableCube::particles.push_back(toAdd);
            }
        }
    }

    //initially hung particles
    specialParticles.push_back(deformableCube::particles[(size+1)*(size+1)*size]);
    specialParticles.push_back(deformableCube::particles[(size+1)*(size+1)*size+size]);
    specialParticles.push_back(deformableCube::particles[id-size-1]);
    specialParticles.push_back(deformableCube::particles[id-1]);

    //setting up the springs
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = i + 1; j < particles.size(); ++j) {
            float dist = glm::length(particles[i]->getPosition() - particles[j]->getPosition());
            springs.push_back(new spring(dist, 80, .2, particles[i], particles[j]));
        }
    }

    //colour buffer
    int pos=0;
    for(int i=0; i<6*2*9*size*size;++i){
        if(i%3==0){
            deformableCube::colorBuffer[pos++]=color.x;
        }else if(i%3==1){
            deformableCube::colorBuffer[pos++]=color.y;
        }else{
            deformableCube::colorBuffer[pos++]=color.z;
        }
    }

    //tmp normal buffer
    pos=0;
    for(int i=0; i<6*2*9*size*size;++i){
        if(i%3==0){
            deformableCube::normalBuffer[pos++]=0;
        }else if(i%3==1){
            deformableCube::normalBuffer[pos++]=-1;
        }else{
            deformableCube::normalBuffer[pos++]=0;
        }
    }

    //compute vertex/normal buffer
    updateBuffer();

    //stuff for openGL
    setGLuint();

    //bvh
    //making the bvh
    bvh = new defCubeBVH(particles, sqrt(3.0f/4.0f), glm::vec3(0.5f,0.5f,0.5f));
}

void deformableCube::setGLuint() {
    GLuint deformableCubeVertex;
    glGenBuffers(1, &deformableCubeVertex);
    glBindBuffer(GL_ARRAY_BUFFER, deformableCubeVertex);
    glBufferData(GL_ARRAY_BUFFER, deformableCube::getSize(), deformableCube::getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint deformableCubeColor;
    glGenBuffers(1, &deformableCubeColor);
    glBindBuffer(GL_ARRAY_BUFFER, deformableCubeColor);
    glBufferData(GL_ARRAY_BUFFER, deformableCube::getSize(), deformableCube::getColorBuffer(), GL_STATIC_DRAW);


    GLuint deformableCubeNormal;
    glGenBuffers(1, &deformableCubeNormal);
    glBindBuffer(GL_ARRAY_BUFFER, deformableCubeNormal);
    glBufferData(GL_ARRAY_BUFFER, deformableCube::getSize(), deformableCube::getNormalBuffer(), GL_DYNAMIC_DRAW);

    deformableCube::setVertex(deformableCubeVertex);
    deformableCube::setColour(deformableCubeColor);
    deformableCube::setNormal(deformableCubeNormal);
}

void deformableCube::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) {
    deformableObjects::render(ProjectionMatrix, ViewMatrix, programID);
#if 0
    bvh->render(ProjectionMatrix, ViewMatrix, programID, true, getModelMatrix());
#endif
}

int deformableCube::getSize() {
    return sizeof(float)*6*2*9*size*size;
}

int deformableCube::getNumberOfVertices() {
    return 6*2*9*size*size;
}

int addToBuffer(float * buf, int position, glm::vec3 botL, glm::vec3 botR, glm::vec3 topL, glm::vec3 topR){
    buf[position++]=botL.x;
    buf[position++]=botL.y;
    buf[position++]=botL.z;

    buf[position++]=botR.x;
    buf[position++]=botR.y;
    buf[position++]=botR.z;

    buf[position++]=topL.x;
    buf[position++]=topL.y;
    buf[position++]=topL.z;

    buf[position++]=botR.x;
    buf[position++]=botR.y;
    buf[position++]=botR.z;

    buf[position++]=topR.x;
    buf[position++]=topR.y;
    buf[position++]=topR.z;

    buf[position++]=topL.x;
    buf[position++]=topL.y;
    buf[position++]=topL.z;

    return position;
}

// this is a bit of a mess
//number of particles per face = (size+1)^2
//order top bottom left right front back
void deformableCube::updateBuffer() {
    //a map to (well) map the id of the particles to the normal they should have
    std::map<int, glm::vec3> helper;

    int pos =0;
    //top face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            //bottom left
            glm::vec3 bl = deformableCube::particles[i+(size+1)*j]->getPosition();
            //top left
            glm::vec3 tl = deformableCube::particles[i+(size+1)*j+1]->getPosition();
            //bottom right
            glm::vec3 br = deformableCube::particles[i+(size+1)*(j+1)]->getPosition();
            //top right
            glm::vec3 tr = deformableCube::particles[i+(size+1)*(j+1)+1]->getPosition();

            glm::vec3 bottomNormal = glm::normalize(glm::cross(tl-bl, bl-br));
            glm::vec3 topNormal = glm::normalize(glm::cross(br-bl, tr-br));

            helper[i+(size+1)*j]+=bottomNormal;
            helper[i+(size+1)*j+1]+=bottomNormal+topNormal;
            helper[i+(size+1)*(j+1)]+=bottomNormal+topNormal;
            helper[i+(size+1)*(j+1)+1]+=topNormal;

            pos = addToBuffer(deformableCube::vertexBuffer, pos, bl, br, tl, tr);
        }
    }

    //bottom face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = (size+1)*(size+1)*size;
            //bottom left
            glm::vec3 bl = deformableCube::particles[tmp+i+(size+1)*j]->getPosition();
            //top left
            glm::vec3 tl = deformableCube::particles[tmp+i+(size+1)*j+1]->getPosition();
            //bottom right
            glm::vec3 br = deformableCube::particles[tmp+i+(size+1)*(j+1)]->getPosition();
            //top right
            glm::vec3 tr = deformableCube::particles[tmp+i+(size+1)*(j+1)+1]->getPosition();

            glm::vec3 bottomNormal = glm::normalize(glm::cross(bl-tl, bl-br));
            glm::vec3 topNormal = glm::normalize(glm::cross(tl-br, tr-br));

            helper[tmp+i+(size+1)*j]+=bottomNormal;
            helper[tmp+i+(size+1)*j+1]+=bottomNormal+topNormal;
            helper[tmp+i+(size+1)*(j+1)]+=bottomNormal+topNormal;
            helper[tmp+i+(size+1)*(j+1)+1]+=topNormal;

            pos = addToBuffer(deformableCube::vertexBuffer, pos, bl, br, tl, tr);
        }
    }

    //left face
    for(int i =0; i<size;++i){
        for(int j=0; j<size;++j){
            //bottom left
            glm::vec3 bl = deformableCube::particles[i+(size+1)*(size+1)*j]->getPosition();

            glm::vec3 br = deformableCube::particles[i+(size+1)*(size+1)*(j+1)]->getPosition();

            glm::vec3 tl = deformableCube::particles[i+1+(size+1)*(size+1)*j]->getPosition();

            glm::vec3 tr = deformableCube::particles[i+1+(size+1)*(size+1)*(j+1)]->getPosition();

            glm::vec3 bottomNormal = glm::normalize(glm::cross(bl-tl, bl-br));
            glm::vec3 topNormal = glm::normalize(glm::cross(tl-br, tr-br));

            helper[i+(size+1)*(size+1)*j]+=bottomNormal;
            helper[i+(size+1)*(size+1)*(j+1)]+=bottomNormal+topNormal;
            helper[i+1+(size+1)*(size+1)*j]+=bottomNormal+topNormal;
            helper[i+1+(size+1)*(size+1)*(j+1)]+=topNormal;

            pos = addToBuffer(deformableCube::vertexBuffer, pos, bl, br, tl, tr);
        }
    }

    //right face
    for(int i =0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = (size+1)*size;
            //bottom left
            glm::vec3 bl = deformableCube::particles[tmp+i*(size+1)*(size+1)+j]->getPosition();

            glm::vec3 br = deformableCube::particles[tmp+i*(size+1)*(size+1)+j+1]->getPosition();

            glm::vec3 tl = deformableCube::particles[tmp+(i+1)*(size+1)*(size+1)+j]->getPosition();

            glm::vec3 tr = deformableCube::particles[tmp+(i+1)*(size+1)*(size+1)+j+1]->getPosition();

            glm::vec3 bottomNormal = glm::normalize(glm::cross(bl-tl, bl-br));
            glm::vec3 topNormal = glm::normalize(glm::cross(tl-br, tr-br));

            helper[tmp+i*(size+1)*(size+1)+j]+=bottomNormal;
            helper[tmp+i*(size+1)*(size+1)+j+1]+=bottomNormal+topNormal;
            helper[tmp+(i+1)*(size+1)*(size+1)+j]+=bottomNormal+topNormal;
            helper[tmp+(i+1)*(size+1)*(size+1)+j+1]+=topNormal;

            pos = addToBuffer(deformableCube::vertexBuffer, pos, bl, br, tl, tr);
        }
    }

    //front face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = (size+1)*(size+1)*size;
            glm::vec3 bl = deformableCube::particles[tmp-(size+1)*(size+1)*i+(size+1)*j]->getPosition();

            glm::vec3 br = deformableCube::particles[tmp-(size+1)*(size+1)*i+(size+1)*(j+1)]->getPosition();

            glm::vec3 tl = deformableCube::particles[tmp-(size+1)*(size+1)*(i+1)+(size+1)*j]->getPosition();

            glm::vec3 tr = deformableCube::particles[tmp-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]->getPosition();

            glm::vec3 bottomNormal = glm::normalize(glm::cross(bl-tl, bl-br));
            glm::vec3 topNormal = glm::normalize(glm::cross(tl-br, tr-br));

            helper[tmp-(size+1)*(size+1)*i+(size+1)*j]+=bottomNormal;
            helper[tmp-(size+1)*(size+1)*i+(size+1)*(j+1)]+=bottomNormal+topNormal;
            helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*j]+=bottomNormal+topNormal;
            helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]+=topNormal;

            pos = addToBuffer(deformableCube::vertexBuffer, pos, bl, br, tl, tr);
        }
    }

    //back face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = size+(size+1)*(size+1)*size;
            glm::vec3 bl = deformableCube::particles[tmp-(size+1)*(size+1)*i+(size+1)*j]->getPosition();

            glm::vec3 br = deformableCube::particles[tmp-(size+1)*(size+1)*i+(size+1)*(j+1)]->getPosition();

            glm::vec3 tl = deformableCube::particles[tmp-(size+1)*(size+1)*(i+1)+(size+1)*j]->getPosition();

            glm::vec3 tr = deformableCube::particles[tmp-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]->getPosition();

            glm::vec3 bottomNormal = glm::normalize(glm::cross(tl-bl, bl-br));
            glm::vec3 topNormal = glm::normalize(glm::cross(br-tl, tr-br));

            helper[tmp-(size+1)*(size+1)*i+(size+1)*j]+=bottomNormal;
            helper[tmp-(size+1)*(size+1)*i+(size+1)*(j+1)]+=bottomNormal+topNormal;
            helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*j]+=bottomNormal+topNormal;
            helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]+=topNormal;

            pos = addToBuffer(deformableCube::vertexBuffer, pos, bl, br, tl, tr);
        }
    }

    //adding normals normals
    pos=0;
    //top face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            //bottom left
            glm::vec3 bl = glm::normalize(helper[i+(size+1)*j]);
            //top left
            glm::vec3 tl = glm::normalize(helper[i+(size+1)*j+1]);
            //bottom right
            glm::vec3 br = glm::normalize(helper[i+(size+1)*(j+1)]);
            //top right
            glm::vec3 tr = glm::normalize(helper[i+(size+1)*(j+1)+1]);

            pos = addToBuffer(deformableCube::normalBuffer, pos, bl, br, tl, tr);
        }
    }

    //bottom face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = (size+1)*(size+1)*size;
            //bottom left
            glm::vec3 bl = glm::normalize(helper[tmp+i+(size+1)*j]);
            //top left
            glm::vec3 tl = glm::normalize(helper[tmp+i+(size+1)*j+1]);
            //bottom right
            glm::vec3 br = glm::normalize(helper[tmp+i+(size+1)*(j+1)]);
            //top right
            glm::vec3 tr = glm::normalize(helper[tmp+i+(size+1)*(j+1)+1]);

            pos = addToBuffer(deformableCube::normalBuffer, pos, bl, br, tl, tr);
        }
    }

    //left face
    for(int i =0; i<size;++i){
        for(int j=0; j<size;++j){
            //bottom left
            glm::vec3 bl = glm::normalize(helper[i+(size+1)*(size+1)*j]);

            glm::vec3 br = glm::normalize(helper[i+(size+1)*(size+1)*(j+1)]);

            glm::vec3 tl = glm::normalize(helper[i+1+(size+1)*(size+1)*j]);

            glm::vec3 tr = glm::normalize(helper[i+1+(size+1)*(size+1)*(j+1)]);

            pos = addToBuffer(deformableCube::normalBuffer, pos, bl, br, tl, tr);
        }
    }

    //right face
    for(int i =0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = (size+1)*size;
            //bottom left
            glm::vec3 bl = glm::normalize(helper[tmp+i*(size+1)*(size+1)+j]);

            glm::vec3 br = glm::normalize(helper[tmp+i*(size+1)*(size+1)+j+1]);

            glm::vec3 tl = glm::normalize(helper[tmp+(i+1)*(size+1)*(size+1)+j]);

            glm::vec3 tr = glm::normalize(helper[tmp+(i+1)*(size+1)*(size+1)+j+1]);

            pos = addToBuffer(deformableCube::normalBuffer, pos, bl, br, tl, tr);
        }
    }

    //front face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = (size+1)*(size+1)*size;
            glm::vec3 bl = glm::normalize(helper[tmp-(size+1)*(size+1)*i+(size+1)*j]);

            glm::vec3 br = glm::normalize(helper[tmp-(size+1)*(size+1)*i+(size+1)*(j+1)]);

            glm::vec3 tl = glm::normalize(helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*j]);

            glm::vec3 tr = glm::normalize(helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]);

            pos = addToBuffer(deformableCube::normalBuffer, pos, bl, br, tl, tr);
        }
    }

    //back face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int tmp = size+(size+1)*(size+1)*size;
            glm::vec3 bl = glm::normalize(helper[tmp-(size+1)*(size+1)*i+(size+1)*j]);

            glm::vec3 br = glm::normalize(helper[tmp-(size+1)*(size+1)*i+(size+1)*(j+1)]);

            glm::vec3 tl = glm::normalize(helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*j]);

            glm::vec3 tr = glm::normalize(helper[tmp-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]);

            pos = addToBuffer(deformableCube::normalBuffer, pos, bl, br, tl, tr);
        }
    }
}

void deformableCube::reset() {
    //reset particles position velocity and force
    for(int i=0; i<particles.size();++i){
        particles[i]->setPosition(resetPos[i]);
        particles[i]->setVelocity(glm::vec3(0.0f,0.0f,0.0f));
        particles[i]->setForce(glm::vec3(0.0f,0.0f,0.0f));
    }

    //reset wind and gravity
    deformableCube::wind= glm::vec3(0,0,0);
    deformableCube::gravity=-5;

    //reset hanging particle
    deformableCube::specialParticles.clear();
    specialParticles.push_back(deformableCube::particles[(size+1)*(size+1)*size]);
    specialParticles.push_back(deformableCube::particles[(size+1)*(size+1)*size+size]);
    specialParticles.push_back(deformableCube::particles[(size+1)*(size+1)*(size+1)-1-size]);
    specialParticles.push_back(deformableCube::particles[(size+1)*(size+1)*(size+1)-1]);

    //reset vertex buffer
    deformableCube::updateBuffer();

    //reset bvh
    bvh->update();
}

void deformableCube::integrate(float timeDelta) {
    deformableCube::rungeKutta4(timeDelta);
}

void deformableCube::rungeKutta(float timeDelta) {
    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    //gravity
    for (int i = 0; i < particles.size(); ++i) {
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(deformableCube::wind + glm::vec3(0.0f, deformableCube::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for (int i = 0; i < springs.size(); ++i) {
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->addForce(particles[i]->getCollisionForce());
        //a2.push_back(particles[i]->getForce() / particles[i]->getMass());
    }

    //adding fore due to collision
    for(int i=0; i<particles.size();++i){
        //particles[i]->addForce(particles[i]->getFrictionForce());
        a2.push_back(particles[i]->getForce() / particles[i]->getMass());
    }

    //second evaluation of the forces
    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->setForce(deformableCube::wind + glm::vec3(0.0f, deformableCube::gravity, 0.0f) +
                               particles[i]->getCollisionForce());
        particles[i]->resetCollisionForce();
        //particles[i]->resetFrictionForce();
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

void deformableCube::detectCollision(collidable *obj) {
    switch(obj->returnType()){
        case 0:
            //collision with sphere
            deformableCube::bvh->detectCollisionSphere(modelMatrix, obj, gravity);
            break;
        case 1:
            //collision with cube
            deformableCube::bvh->detectCollisionCube(modelMatrix, obj, gravity);
            break;
        case 2:
            //collision with plane
            deformableCube::bvh->detectCollisionPlane(modelMatrix, obj, gravity);
            break;
        default:
            std::cout<<"something went wrong"<<std::endl;
            break;
    }
}

BVH *deformableCube::getBvh() const {
    return deformableCube::bvh;
}

helperStruct deformableCube::isHovered(glm::vec3 origin, glm::vec3 direction) {
    helperStruct toReturn = deformableCube::bvh->rayIntersect(origin, direction, modelMatrix);
    return toReturn;
}

void deformableCube::renderShadow(glm::mat4 depthP, glm::mat4 depthV, GLuint programId) {
    setCullFace(GL_FRONT);
    deformableObjects::renderShadow(depthP, depthV, programId);
}

void deformableCube::renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID) {
    setCullFace(GL_BACK);
    deformableObjects::renderShadow(pr, viw, bias, programID);
}


