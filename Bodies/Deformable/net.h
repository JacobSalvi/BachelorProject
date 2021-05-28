
#ifndef CODE_NET_H
#define CODE_NET_H

#include "../../BVH/clothBVH.h"
//#include "../../shaders/helperStruct.h"

#include <glm/gtc/matrix_transform.hpp>
#include "deformableObjects.h"

class clothBVH;
//struct helperStruct;

//simple object made of particles and spring
//should be quite similar to a net/cloth
// X--X--X
// | \| \|
// X--X--X
// | \| \|
// X--X--X
class net: public deformableObjects{
private:
    int col;
    int row;

    //buffer needed for OpenGl
    //vertex and color buffers
    //(i-1)*2*(j-1)

    //integrator to use
    //0 explicit euler
    //1 runge kutta
    int integrator;

    //BVH
    clothBVH * bvh;

    int orientation = 0;

public:
    //assumption:
    //-equal mass particles
    //-equidistant
    //-vertical and horizontal springs have the same properties
    //-diagonal springs have the same properties
    net(float mass, int col, int row, int integrator, glm::vec3 color, float gravity,  glm::mat4 mod, glm::vec3 lPos);

    net(int size, glm::mat4 mod, glm::vec3 lPos);

    void addWind(const glm::vec3 &windToAdd);

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

    virtual ~net();

    void setVertexBuffer(float *vertexBuffer);

    int getSize() override;

    int getNumberOfVertices() override;

    void updateBuffer() override;

    void cleanUp(){
        for(auto & particle : particles){
            delete(particle);
        }
        for(auto & spring : springs){
            delete(&spring);
        }
    }

    void integrate(float timeDelta) override{!integrator ? explicitEuler(timeDelta) : rungeKutta4(timeDelta);}
//    void integrate(float timeDelta) override{
//        rungeKutta4(timeDelta);
//    }

    void reset() override;

    void setModelMatrix(const glm::mat4 &modelMatrix);

    GLuint getVertex() const;

    GLuint getColour() const;

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) override;

    clothBVH *getBvh() const override;

    helperStruct isHovered(glm::vec3 origin, glm::vec3 direction) override;

    const glm::vec3 &getLightPos() const;

    void updateNormalBuffer();

    //collision detection
    //this is gonna be painful
    void detectCollision(collidable * obj) override;

    void setGLuint() override;

    void renderShadow(glm::mat4 depthP, glm::mat4 depthV,  GLuint programId) override;
    void renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID) override;
};


#endif //CODE_NET_H
