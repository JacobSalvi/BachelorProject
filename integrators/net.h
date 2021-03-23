
#ifndef CODE_NET_H
#define CODE_NET_H
//#include <GL/glew.h>
#include "particle.h"
#include "spring.hpp"
#include <vector>

//simple object made of particles and spring
//should be quite similar to a net/cloth
// X--X--X
// | \| \|
// X--X--X
// | \| \|
// X--X--X
class net{
private:
    int col;
    int row;
    //list of particles
    std::vector<particle* > particles;
    //list of springs
    std::vector<spring *> springs;

    //buffer needed for OpenGl
    //vertex and color buffers
    //(i-1)*2*(j-1)
    float * vertexBuffer;
    float * colorBuffer;

    //integrator to use
    //0 explicit euler
    //1 runge kutta
    int integrator;

    //environmental forces, gravity and wind
    float gravity;
    glm::vec3 wind;

public:
    //assumption:
    //-equal mass particles
    //-equidistant
    //-vertical and horizontal springs have the same properties
    //-diagonal springs have the same properties
    net(float mass, int col, int row, int integrator, glm::vec3 color, float gravity);

    void setWind(const glm::vec3 &newWind);
    void addWind(const glm::vec3 &windToAdd);

    void setGravity(const float newGravity);

    //integrators
    void explicitEuler(float timeDelta);
    void rungeKutta(float timeDelta);

    //testing that the initialization went well
    void printStuff(){
        for(int i=0; i<(row-1)*(col-1)*18; ++i){
            if(i%3==0){
                std::cout<<"vertex: "<<std::endl;
                std::cout<<vertexBuffer[i]<<", ";
            }
            if(i%3==1){
                std::cout<<vertexBuffer[i]<<", ";
            }
            if(i%3==2){
                std::cout<<vertexBuffer[i]<<std::endl;
            }
        }
    }

    float *getVertexBuffer();

    float *getColorBuffer();

    virtual ~net();

    void setVertexBuffer(float *vertexBuffer);

    int getSize() const{
        return sizeof(float)*(row-1)*(col-1)*18;
    }

    int getNumberOfVertices() const{
        return (row-1)*(col-1)*18;
    }

    void updateBuffer();

    void cleanUp(){
        for(auto & particle : particles){
            delete(particle);
        }
        for(auto & spring : springs){
            delete(&spring);
        }
    }

    void integrate(float timeDelta){!integrator ? explicitEuler(timeDelta) : rungeKutta(timeDelta);}

    void reset();

    void setMass(float mass);

    void setStiffness(float stiffness);

};


#endif //CODE_NET_H
