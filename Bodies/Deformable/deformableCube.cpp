
#include "deformableCube.h"
#include "../../BVH/defSphereBVH.h"
#include "../utilities/helperFunctions.h"

deformableCube::~deformableCube() {
    std::cout<<"deformable cube destroyed"<<std::endl;
}

// a cube is a sort of grid of inner smaller cubes
// also it is a cube of side 1
deformableCube::deformableCube(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos) :
    size(3), deformableObjects(-1, mod, lPos) {
    //we have 6 faces, each face has size^2 sub squares in it
    // each sub square is formed by 2 triangle
    deformableCube::vertexBuffer = new float[6*2*3*size*size];
    deformableCube::colorBuffer = new float[6*2*3*size*size];
    deformableCube::normalBuffer = new float[6*2*3*size*size];

    float mass = 1.0f;
    int id=0;
    float compar = 3.0f;
    //setting up particles
    for(float y=0; y<=compar; ++y){
        for(float x=0; x<=compar; ++x){
            for(float z=0; z<=compar; ++z){
                glm::vec3 idk(x/compar, y/compar, z/compar);
                printPoint(idk, "");
                deformableCube::resetPos.push_back(idk);
                auto * toAdd = new particle(idk , mass);
                toAdd->setId(id++);
                deformableCube::particles.push_back(toAdd);
            }
        }
    }

    //setting up the springs
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = i + 1; j < particles.size(); ++j) {
            float dist = glm::length(particles[i]->getPosition() - particles[j]->getPosition());
            springs.push_back(new spring(dist, 50, .2, particles[i], particles[j]));
        }
    }

    //colour buffer
    int pos=0;
    for(int i=0; i<36*size*size;++i){
        if(i%3==0){
            deformableCube::colorBuffer[pos++]=color.x;
        }else if(i%3==1){
            deformableCube::colorBuffer[pos++]=color.y;
        }else{
            deformableCube::colorBuffer[pos++]=color.z;
        }
    }

    //normal buffer
    pos=0;
    for(int i=0; i<36*size*size;++i){
        if(i%3==0){
            deformableCube::normalBuffer[pos++]=color.x;
        }else if(i%3==1){
            deformableCube::normalBuffer[pos++]=color.y;
        }else{
            deformableCube::normalBuffer[pos++]=color.z;
        }
    }


    //stuff for openGL
    setGLuint();

    //compute vertex/normal buffer
    updateBuffer();

    //bvh
    //making the bvh
    bvh = new defSphereBVH(particles, 0.5f, glm::vec3(0.0f,0.0f,0.0f));
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
    glBufferData(GL_ARRAY_BUFFER, deformableCube::getSize(), deformableCube::getNormalBuffer(), GL_STATIC_DRAW);

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
    return sizeof(float)*36*size*size;
}

int deformableCube::getNumberOfVertices() {
    return 36*size*size;
}

// this is a bit of a mess
void deformableCube::updateBuffer() {
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

            //first triangle
            deformableCube::vertexBuffer[pos++]=bl.x;
            deformableCube::vertexBuffer[pos++]=bl.y;
            deformableCube::vertexBuffer[pos++]=bl.z;

            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;

            //second triangle
            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tr.x;
            deformableCube::vertexBuffer[pos++]=tr.y;
            deformableCube::vertexBuffer[pos++]=tr.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;
        }
    }

    //bottom face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int helper = (size+1)+(size+1)*size;
            //bottom left
            glm::vec3 bl = deformableCube::particles[helper+i+(size+1)*j]->getPosition();
            //top left
            glm::vec3 tl = deformableCube::particles[helper+i+(size+1)*j+1]->getPosition();
            //bottom right
            glm::vec3 br = deformableCube::particles[helper+i+(size+1)*(j+1)]->getPosition();
            //top right
            glm::vec3 tr = deformableCube::particles[helper+i+(size+1)*(j+1)+1]->getPosition();

            //first triangle
            deformableCube::vertexBuffer[pos++]=bl.x;
            deformableCube::vertexBuffer[pos++]=bl.y;
            deformableCube::vertexBuffer[pos++]=bl.z;

            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;

            //second triangle
            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tr.x;
            deformableCube::vertexBuffer[pos++]=tr.y;
            deformableCube::vertexBuffer[pos++]=tr.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;
        }
    }

    //left face
    for(int i =0; i<size;++i){
        for(int j=0; j<size;++j){
            //bottom left
            glm::vec3 bl = deformableCube::particles[i*(size+1)*(size+1)+j]->getPosition();


            glm::vec3 br = deformableCube::particles[i*(size+1)*(size+1)+j+1]->getPosition();

            glm::vec3 tl = deformableCube::particles[(i+1)*(size+1)*(size+1)+j]->getPosition();

            glm::vec3 tr = deformableCube::particles[(i+1)*(size+1)*(size+1)+j+1]->getPosition();

            //first triangle
            deformableCube::vertexBuffer[pos++]=bl.x;
            deformableCube::vertexBuffer[pos++]=bl.y;
            deformableCube::vertexBuffer[pos++]=bl.z;

            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;



            //second triangle
            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tr.x;
            deformableCube::vertexBuffer[pos++]=tr.y;
            deformableCube::vertexBuffer[pos++]=tr.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;
        }
    }

    //right face
    for(int i =0; i<size;++i){
        for(int j=0; j<size;++j){
            int helper = (size+1)*size;
            //bottom left
            glm::vec3 bl = deformableCube::particles[helper+i*(size+1)*(size+1)+j]->getPosition();

            glm::vec3 br = deformableCube::particles[helper+i*(size+1)*(size+1)+j+1]->getPosition();

            glm::vec3 tl = deformableCube::particles[helper+(i+1)*(size+1)*(size+1)+j]->getPosition();

            glm::vec3 tr = deformableCube::particles[helper+(i+1)*(size+1)*(size+1)+j+1]->getPosition();

            //first triangle
            deformableCube::vertexBuffer[pos++]=bl.x;
            deformableCube::vertexBuffer[pos++]=bl.y;
            deformableCube::vertexBuffer[pos++]=bl.z;

            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;

            //second triangle
            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tr.x;
            deformableCube::vertexBuffer[pos++]=tr.y;
            deformableCube::vertexBuffer[pos++]=tr.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;
        }
    }

    //front face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int helper = (size+1)*(size+1)*size;
            glm::vec3 bl = deformableCube::particles[helper-(size+1)*(size+1)*i+(size+1)*j]->getPosition();

            glm::vec3 br = deformableCube::particles[helper-(size+1)*(size+1)*i+(size+1)*(j+1)]->getPosition();

            glm::vec3 tl = deformableCube::particles[helper-(size+1)*(size+1)*(i+1)+(size+1)*j]->getPosition();

            glm::vec3 tr = deformableCube::particles[helper-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]->getPosition();

            //first triangle
            deformableCube::vertexBuffer[pos++]=bl.x;
            deformableCube::vertexBuffer[pos++]=bl.y;
            deformableCube::vertexBuffer[pos++]=bl.z;

            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;

            //second triangle
            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tr.x;
            deformableCube::vertexBuffer[pos++]=tr.y;
            deformableCube::vertexBuffer[pos++]=tr.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;
        }
    }

    //back face
    for(int i=0; i<size;++i){
        for(int j=0; j<size;++j){
            int helper = size+(size+1)*(size+1)*size;
            glm::vec3 bl = deformableCube::particles[helper-(size+1)*(size+1)*i+(size+1)*j]->getPosition();

            glm::vec3 br = deformableCube::particles[helper-(size+1)*(size+1)*i+(size+1)*(j+1)]->getPosition();

            glm::vec3 tl = deformableCube::particles[helper-(size+1)*(size+1)*(i+1)+(size+1)*j]->getPosition();

            glm::vec3 tr = deformableCube::particles[helper-(size+1)*(size+1)*(i+1)+(size+1)*(j+1)]->getPosition();

            //first triangle
            deformableCube::vertexBuffer[pos++]=bl.x;
            deformableCube::vertexBuffer[pos++]=bl.y;
            deformableCube::vertexBuffer[pos++]=bl.z;

            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;

            //second triangle
            deformableCube::vertexBuffer[pos++]=br.x;
            deformableCube::vertexBuffer[pos++]=br.y;
            deformableCube::vertexBuffer[pos++]=br.z;

            deformableCube::vertexBuffer[pos++]=tr.x;
            deformableCube::vertexBuffer[pos++]=tr.y;
            deformableCube::vertexBuffer[pos++]=tr.z;

            deformableCube::vertexBuffer[pos++]=tl.x;
            deformableCube::vertexBuffer[pos++]=tl.y;
            deformableCube::vertexBuffer[pos++]=tl.z;
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
    deformableCube::gravity=-1;

    //reset hanging particle
    deformableCube::specialParticles.clear();
    deformableCube::specialParticles.push_back(particles[0]);

    //reset vertexbuffer
    deformableCube::updateBuffer();

    //reset bvh
    bvh->update();
}

void deformableCube::integrate(float timeDelta) {
    deformableObjects::integrate(timeDelta);
}

void deformableCube::rungeKutta(float timeDelta) {

}

void deformableCube::detectCollision(collidable *obj) {
    deformableObjects::detectCollision(obj);
}

BVH *deformableCube::getBvh() const {
    return deformableObjects::getBvh();
}

helperStruct deformableCube::isHovered(glm::vec3 origin, glm::vec3 direction) {
    return deformableObjects::isHovered(origin, direction);
}


