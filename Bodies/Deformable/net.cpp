#include "net.h"
#include "../../utilities/helperFunctions.h"

//constructor
net::net(float mass, int col, int row, int integrator, glm::vec3 color, float gravity,  glm::mat4 mod, glm::vec3 lPos) :
deformableObjects(gravity, mod, lPos), row(row), col(col), integrator(integrator){
    //Eureka, finally I understood, took me only 15 hundred year but I did it in the end
    //number of triangles given by
    //(row-1)*(col-1)*2
    //the number of vertices needed is 3 times the number of triangles
    //-> vertices = 6*(row-1)(col-1)
    //every vertex is specified by 3 floats
    int a = (row-1)*(col-1)*18;
    vertexBuffer = new float[a];
    colorBuffer = new float[a];
    normalBuffer =  new float[a];

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
            float damping = .95f;
            if(j-1>=0){
                spring * toAdd = new spring(1.0f, coeff, damping, particles[i*col+j-1],tmp);
                springs.push_back(toAdd);
            }
            //vertical springs
            if(i-1>=0){
                spring * toAdd = new spring(1.0f, coeff, damping, particles[i*col+j-col],tmp);
                springs.push_back(toAdd);
            }
            //diagonal springs
            if(i>0&&j>0){
                spring * toAdd = new spring(sqrt(2.0f), coeff, damping, particles[i*col+j-(col+1)], tmp);
                springs.push_back(toAdd);
            }
            //diagonal spring but in the other direction
            if(i>0&&j<col-1){
                spring * toAdd = new spring(sqrt(2.0f), coeff, damping, particles[(i-1)*col+j+1], tmp);
                springs.push_back(toAdd);
            }
        }
    }

    //bounding volume hierarchy
    net::bvh=new clothBVH(particles, row);

    //normal
    //the cloth are always initially created as vertical planes
    for(int i=0; i<a;++i){
        if(i%3==2){
            normalBuffer[i]=1.0f;
        }else{
            normalBuffer[i]=0.0f;
        }
    }

    //GLuint
    setGLuint();

    updateBuffer();
}

//orizontal cloth
net::net(int size, glm::mat4 mod, glm::vec3 lPos) : deformableObjects(-1, mod, lPos), orientation(1), row(size), col(size), integrator(1){
    int a = (row-1)*(col-1)*18;
    vertexBuffer = new float[a];
    colorBuffer = new float[a];
    normalBuffer =  new float[a];

    //for now color and vertex buffer are the same
    glm::vec3 color(0,0,1);
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
            particle * tmp = new particle(glm::vec3(float(j), 0.0f, float(i)), 1);
            tmp->setId(col*i+j);
            particles.push_back(tmp);

            //set hanging particles
            if(i*col+j==col*row-1 || i*col+j== col*(row-1) || i*col+j==0 || i*col+j==col-1){
                specialParticles.push_back(tmp);
            }

            //setting up the springs
            //horizontal springs
            float coeff= 40.0f;
            float damping = .95f;
            if(j-1>=0){
                spring * toAdd = new spring(1.0f, coeff, damping, particles[i*col+j-1],tmp);
                springs.push_back(toAdd);
            }
            //vertical springs
            if(i-1>=0){
                spring * toAdd = new spring(1.0f, coeff, damping, particles[i*col+j-col],tmp);
                springs.push_back(toAdd);
            }
            //diagonal springs
            if(i>0&&j>0){
                spring * toAdd = new spring(sqrt(2.0f), coeff, damping, particles[i*col+j-(col+1)], tmp);
                springs.push_back(toAdd);
            }
            //diagonal spring but in the other direction
            if(i>0&&j<col-1){
                spring * toAdd = new spring(sqrt(2.0f), coeff, damping, particles[(i-1)*col+j+1], tmp);
                springs.push_back(toAdd);
            }
        }
    }

    //bounding volume hierarchy
    net::bvh=new clothBVH(particles, row);

    //normal
    for(int i=0; i<a;++i){
        if(i%3==1){
            normalBuffer[i]=1.0f;
        }else{
            normalBuffer[i]=0.0f;
        }
    }
    //GLuint
    setGLuint();

    updateBuffer();

    updateNormalBuffer();
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

    //there is collision now
    //must add collision force
    for(int i=0; i<particles.size();++i){
        particles[i]->addForce(particles[i]->getCollisionForce());
        //once we added it we can reset it to 0
        particles[i]->resetCollisionForce();
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


//second order Runge Kutta integrator
//half the time -> 1/4 the error
void net::rungeKutta(float timeDelta){
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
        particles[i]->addForce(particles[i]->getCollisionForce());
        particles[i]->addForce(particles[i]->getFrictionForce());
        a2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //second evaluation of the forces
    for(int i =0;i< particles.size();++i){
        particles[i]->setForce(net::wind+glm::vec3(0.0f,net::gravity, 0.0f)+particles[i]->getCollisionForce() + particles[i]->getFrictionForce());
        particles[i]->resetCollisionForce();
        particles[i]->resetFrictionForce();
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

    net::updateNormalBuffer();
}

//TODO: rewrite this
void net::updateNormalBuffer() {
    int nVert = (row-1)*(col-1)*6;
    std::vector<glm::vec3> helper;
    //initially of 0 vectors
    for(int i=0; i<nVert;++i){
        helper.push_back(glm::vec3(0.0f,0.0f, 0.0f));
    }
    //update the normal buffer as well
    for(int i=0; i<row-1;++i){
        for(int j=0; j<col-1;++j){
            //the 4 particles
            glm::vec3 bottomLeft = particles[col*i+j]->getPosition();
            glm::vec3 bottomRight = particles[col*i+j+1]->getPosition();
            glm::vec3 topLeft = particles[col*(i+1)+j]->getPosition();
            glm::vec3 topRight = particles[col*(i+1)+j+1]->getPosition();

            //let's calculate the normal
            glm::vec3 topNormal = glm::normalize(glm::cross(bottomLeft-topLeft, bottomLeft-topRight));
            glm::vec3 bottomNormal = glm::normalize(glm::cross(bottomLeft-topRight, bottomLeft-bottomRight));

            //sum to the corresponding vert the computed normal
            helper[col*i+j]+=topNormal+bottomNormal;
            helper[col*i+j+1]+=bottomNormal;
            helper[col*(i+1)+j]+=topNormal;
            helper[col*(i+1)+j+1]+=topNormal+bottomNormal;
        }
    }

    //we need now to normalize the helper
    std::cout<<particles.size()<<std::endl;
    std::cout<<nVert<<std::endl;
    int pos = 0;
    for(int i = 0; i<nVert;++i){
        glm::vec3 tmp = glm::normalize(helper[i]);
        printPoint(tmp, "normal is: ");
        //and finally we can update the buffer
        normalBuffer[pos++]=tmp[0];
        normalBuffer[pos++]=tmp[1];
        normalBuffer[pos++]=tmp[2];
    }

}

void net::setVertexBuffer(float *vb) {
    net::vertexBuffer = vb;
}

//destructor
net::~net() {
    std::cout<<"cloth destroyed"<<std::endl;
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
            orientation == 0 ? particles[i*col+j]->setPosition(glm::vec3(float(j), float(i), 0.0f)) : particles[i*col+j]->setPosition(glm::vec3(float(j), 0.0f, float(i))) ;
            //particles[i*col+j]->setPosition(glm::vec3(float(j), float(i), 0.0f));
            particles[i*col+j]->setVelocity(glm::vec3(0.0f,0.0f, 0.0f));
            particles[i*col+j]->setForce(glm::vec3(0.0f,0.0f,0.0f));
        }
    }

    //reset hanging particles
    //set hanging particles
    net::emptySpecialParticles();
    if(orientation==0) {
        net::setSpecial(net::particles[col * row - 1]);
        net::setSpecial(net::particles[col * (row - 1)]);
    }else{
        net::setSpecial(net::particles[col*row-1]);
        net::setSpecial(net::particles[col*(row-1)]);
        net::setSpecial(net::particles[0]);
        net::setSpecial(net::particles[col-1]);
    }

    net::wind=glm::vec3(0.0f, 0.0f,0.0f);
    net::gravity=-1.0f;

    updateBuffer();

    //reset bvh as well
    getBvh()->update();

    //reset the normal buffer
    int n = (row-1)*(col-1)*18;
    for(int i=0; i<n;++i){
        if(orientation==0){
            if(i%3==2){
                normalBuffer[i]=1.0f;
            }else{
                normalBuffer[i]=0.0f;
            }
        }else {
            if (i % 3 == 1) {
                normalBuffer[i] = 1.0f;
            } else {
                normalBuffer[i] = 0.0f;
            }
        }
    }

}

void net::setModelMatrix(const glm::mat4 &modelMatrix) {
    net::modelMatrix = modelMatrix;
}

void net::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) {
    deformableObjects::render(ProjectionMatrix, ViewMatrix, programID);
#if 0
    bvh->render(ProjectionMatrix, ViewMatrix, programID, true, getModelMatrix());
#endif
}

GLuint net::getVertex() const {
    return vertex;
}

GLuint net::getColour() const {
    return colour;
}

clothBVH *net::getBvh() const {
    return bvh;
}

helperStruct net::isHovered(glm::vec3 origin, glm::vec3 direction) {
    helperStruct toReturn = net::bvh->rayIntersect(origin, direction, modelMatrix);
    return toReturn;
}

const vec3 &net::getLightPos() const {
    return lightPos;
}

void net::detectCollision(collidable * obj) {
    switch(obj->returnType()){
        case 0:
            //collision with sphere
            net::bvh->detectCollisionSphere(modelMatrix, obj, gravity);
            break;
        case 1:
            //collision with cube
            net::bvh->detectCollisionCube(modelMatrix, obj, gravity);
            break;
        case 2:
            //collision with plane
            net::bvh->detectCollisionPlane(modelMatrix, obj, gravity);
            break;
        default:
            std::cout<<"something went wrong"<<std::endl;
            break;
    }
}

void net::setGLuint() {
    deformableObjects::setGLuint();
    GLuint netVertex;
    glGenBuffers(1, &netVertex);
    glBindBuffer(GL_ARRAY_BUFFER, netVertex);
    glBufferData(GL_ARRAY_BUFFER, net::getSize(), net::getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint netColor;
    glGenBuffers(1, &netColor);
    glBindBuffer(GL_ARRAY_BUFFER, netColor);
    glBufferData(GL_ARRAY_BUFFER, net::getSize(), net::getColorBuffer(), GL_STATIC_DRAW);


    GLuint netNormal;
    glGenBuffers(1, &netNormal);
    glBindBuffer(GL_ARRAY_BUFFER, netNormal);
    glBufferData(GL_ARRAY_BUFFER, net::getSize(), net::getNormalBuffer(), GL_STATIC_DRAW);

    net::setVertex(netVertex);
    net::setColour(netColor);
    net::setNormal(netNormal);
}

int net::getSize() {
    return sizeof(float)*(row-1)*(col-1)*18;
}

int net::getNumberOfVertices() {
    return (row-1)*(col-1)*6;
}
