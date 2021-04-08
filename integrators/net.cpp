#include "net.h"
#include "../BVH/sphereBVH.h"
#include "../shaders/helperFunctions.h"

//constructor
net::net(float mass, int col, int row, int integrator, glm::vec3 color, float gravity,  glm::vec3 tr) :
row(row), col(col), gravity(gravity), integrator(integrator){
    //Eureka, finally I understood, took me only 15 hundred year but I did it in the end
    //number of triangles given by
    //(row-1)*(col-1)*2
    //the number of vertices needed is 3 times the number of triangles
    //-> vertices = 6*(row-1)(col-1)
    //every vertex is specified by 3 floats
    int a = (row-1)*(col-1)*18;
    vertexBuffer = new float[a];
    colorBuffer = new float[a];


    //adding the vertices to the vertex buffer
    //given in counter clock wise order
    //helper to push in the array
    int pos = 0;
    for(int i=0; i<row-1; ++i){
        for(int j=0; j<col-1; ++j){
            //first triangle first vertex
            vertexBuffer[pos++]=(float)j;
            vertexBuffer[pos++]=(float)i;
            vertexBuffer[pos++]=0.0f;

            //first triangle second vertex
            vertexBuffer[pos++]=(float)j;
            vertexBuffer[pos++]=(float)(i+1);
            vertexBuffer[pos++]=0.0f;

            //first triangle third vertex
            vertexBuffer[pos++]=(float)(j+1);
            vertexBuffer[pos++]=(float)(i+1);
            vertexBuffer[pos++]=0.0f;

            //second triangle first vertex
            vertexBuffer[pos++]=(float)j;
            vertexBuffer[pos++]=(float)i;
            vertexBuffer[pos++]=0.0f;

            //second triangle second vertex
            vertexBuffer[pos++]=(float)(j+1);
            vertexBuffer[pos++]=(float)(i+1);
            vertexBuffer[pos++]=0.0f;

            //second triangle third vertex
            vertexBuffer[pos++]=(float)(j+1);
            vertexBuffer[pos++]=(float)i;
            vertexBuffer[pos++]=0.0f;
        }
    }

    //for now color and vertex buffer are the same
    if(color[0]==2) {
        colorBuffer = vertexBuffer;
    }else{
        for(int i=0; i<(col-1)*(row-1)*18;++i){
            if(i%3==0){
                colorBuffer[i]=color[0];
            }else if(i%3==1){
                colorBuffer[i]=color[1];
            }else{
                colorBuffer[i]=color[2];
            }
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            //initial position in a net, equidistant
            particle * tmp = new particle(glm::vec3(float(j), float(i), 0.0f), mass);
            tmp->setId(col*i+j);
            particles.push_back(tmp);

            //set hanging particles
            if(i*col+j==col*row-1 || i*col+j== col*(row-1)){
                specialParticles.push_back(tmp);
            }

            //setting up the springs
            //horizontal springs
            float coeff= 40.0f;
            if(j-1>=0){
                //TODO choose initial spring coefficient that make sense
                spring * toAdd = new spring(1.0f, coeff,1.0f, particles[i*col+j-1],tmp);
                springs.push_back(toAdd);
            }
            //vertical springs
            if(i-1>=0){
                spring * toAdd = new spring(1.0f, coeff,1.0f, particles[i*col+j-col],tmp);
                springs.push_back(toAdd);
            }
            //diagonal springs
            if(i>0&&j>0){
                spring * toAdd = new spring(sqrt(2), coeff,1.0f, particles[i*col+j-(col+1)], tmp);
                springs.push_back(toAdd);
            }
            //diagonal spring but in the other direction
            if(i>0&&j<col-1){
                spring * toAdd = new spring(sqrt(2), coeff,1.0f, particles[(i-1)*col+j+1], tmp);
                springs.push_back(toAdd);
            }
        }
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, tr);
    net::modelMatrix=model;

    //bounding volume hierarchy
    net::bvh=new sphereBVH(particles, row);
}

//update the particles following the
//explicit Euler integrator
void net::explicitEuler(float timeDelta) {
    //update force for all particles
    //adding gravity
    for(int i=0; i<particles.size(); ++i){
        particles[i]->setForce(net::wind+glm::vec3(0.0f,net::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for(int i=0; i<springs.size(); ++i){
        //update both particles
        springs[i]->updateParticlesForce();
    }

//    for(int i=0;i<particles.size();++i){
//        glm::vec3 tmp = particles[i]->getPosition();
//        //std::cout<<"please god work"<<std::endl;
//        std::cout<<tmp[0]<<std::endl;
//        std::cout<<tmp[1]<<std::endl;
//        std::cout<<tmp[2]<<std::endl;
//    }

    //there is no collision yet
    //can't add force due to collision

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


//second order Runge Kutta integrator
//half the time -> 1/4 the error
void net::rungeKutta(float timeDelta){
//    std::cout<<"Runge Kutta"<<std::endl;
    //TODO consider whether there is a better way to do this

    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    //gravity
    for(int i =0;i< particles.size();++i){
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(net::wind+glm::vec3(0.0f,net::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for(int i=0; i<springs.size(); ++i){
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for(int i=0; i<particles.size();++i){
        a2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //second evaluation of the forces
    for(int i =0;i< particles.size();++i){
        particles[i]->setForce(net::wind+glm::vec3(0.0f,net::gravity, 0.0f));
    }

    for(int i=0; i<springs.size(); ++i){
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta/2.0f)*a1[part1], (timeDelta/2.0f)*a1[part2]);
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


//#----#----#---...
//|    |    |
//|    |    |
//#----#----#---...
//.    .    .
//.    .    .
//.    .    .
void net::updateBuffer() {
    int pos =0;
    for(int i=0; i<row-1;++i){
        for(int j=0; j<col-1;++j){
            //first triangle first vertex
            glm::vec3 topLeft = particles[col*i+j]->getPosition();
            vertexBuffer[pos++]=topLeft[0];
            vertexBuffer[pos++]=topLeft[1];
            vertexBuffer[pos++]=topLeft[2];

            //first triangle second vertex
            glm::vec3 bottomLeft = particles[col*(i+1)+j]->getPosition();
            vertexBuffer[pos++]=bottomLeft[0];
            vertexBuffer[pos++]=bottomLeft[1];
            vertexBuffer[pos++]=bottomLeft[2];

            //first triangle third vertex
            glm::vec3 bottomRight = particles[col*(i+1)+j+1]->getPosition();
            vertexBuffer[pos++]=bottomRight[0];
            vertexBuffer[pos++]=bottomRight[1];
            vertexBuffer[pos++]=bottomRight[2];

            //second triangle first vertex
            vertexBuffer[pos++]=topLeft[0];
            vertexBuffer[pos++]=topLeft[1];
            vertexBuffer[pos++]=topLeft[2];

            //second triangle second vertex
            vertexBuffer[pos++]=bottomRight[0];
            vertexBuffer[pos++]=bottomRight[1];
            vertexBuffer[pos++]=bottomRight[2];

            //second triangle third vertex
            glm::vec3 topRight = particles[col*i+j+1]->getPosition();
            vertexBuffer[pos++]=topRight[0];
            vertexBuffer[pos++]=topRight[1];
            vertexBuffer[pos++]=topRight[2];
        }
    }
}

void net::setVertexBuffer(float *vb) {
    net::vertexBuffer = vb;
}

float *net::getVertexBuffer(){
    return vertexBuffer;
}

float *net::getColorBuffer() {
    return colorBuffer;
}


//destructor
net::~net() {
    std::cout<<"cloth destroyed"<<std::endl;
}

void net::setWind(const glm::vec3 &newWind) {
    net::wind = newWind;
}

void net::addWind(const glm::vec3 &windToAdd) {
    net::wind+=windToAdd;
}

//reset the cloth to its initial position
void net::reset() {
    //adding the vertices to the vertex buffer
    //given in counter clock wise order
    //helper to push in the array
    int pos = 0;
    for(int i=0; i<row; ++i){
        for(int j=0; j<col; ++j) {
            //have to rest particle positions and speed
            particles[i*col+j]->setPosition(glm::vec3(float(j), float(i), 0.0f));
            particles[i*col+j]->setVelocity(glm::vec3(0.0f,0.0f, 0.0f));
            particles[i*col+j]->setForce(glm::vec3(0.0f,0.0f,0.0f));
        }
    }

    net::wind=glm::vec3(0.0f, 0.0f,0.0f);
    net::gravity=-1.0f;

    for(int i=0; i<row-1; ++i){
        for(int j=0; j<col-1; ++j){
            //first triangle first vertex
            vertexBuffer[pos++]=(float)j;
            vertexBuffer[pos++]=(float)i;
            vertexBuffer[pos++]=0.0f;

            //first triangle second vertex
            vertexBuffer[pos++]=(float)j;
            vertexBuffer[pos++]=(float)(i+1);
            vertexBuffer[pos++]=0.0f;

            //first triangle third vertex
            vertexBuffer[pos++]=(float)(j+1);
            vertexBuffer[pos++]=(float)(i+1);
            vertexBuffer[pos++]=0.0f;

            //second triangle first vertex
            vertexBuffer[pos++]=(float)j;
            vertexBuffer[pos++]=(float)i;
            vertexBuffer[pos++]=0.0f;

            //second triangle second vertex
            vertexBuffer[pos++]=(float)(j+1);
            vertexBuffer[pos++]=(float)(i+1);
            vertexBuffer[pos++]=0.0f;

            //second triangle third vertex
            vertexBuffer[pos++]=(float)(j+1);
            vertexBuffer[pos++]=(float)i;
            vertexBuffer[pos++]=0.0f;
        }
    }
}

void net::setMass(float mass) {
    for(auto i:net::particles){
        i->setMass(mass);
    }
}

void net::setGravity(const float newGravity) {
    net::gravity=newGravity;
}

void net::setStiffness(float stiffness) {
    for(auto i : net::springs){
        i->setKs(stiffness);
    }
}

void net::setModelMatrix(const glm::mat4 &modelMatrix) {
    net::modelMatrix = modelMatrix;
}

void net::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID) {

    glm::mat4 mvp = ProjectionMatrix * ViewMatrix * getModelMatrix();

    glUniformMatrix4fv(triangleMatrixID, 1, GL_FALSE, &mvp[0][0]);
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

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colour);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) nullptr                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    bvh->render(ProjectionMatrix, ViewMatrix, triangleMatrixID, true, getModelMatrix());
}

const glm::mat4 &net::getModelMatrix() const {
    return modelMatrix;
}

GLuint net::getVertex() const {
    return vertex;
}

void net::setVertex(GLuint newVert) {
    net::vertex = newVert;
}

GLuint net::getColour() const {
    return colour;
}

void net::setColour(GLuint newColour) {
    net::colour = newColour;
}

sphereBVH *net::getBvh() const {
    return bvh;
}

