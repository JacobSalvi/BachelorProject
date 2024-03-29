
#ifndef CODE_DEFORMABLEOBJECTS_H
#define CODE_DEFORMABLEOBJECTS_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"
#include "spring.hpp"
#include "../Solid/collidable.h"
#include "../../BVH/BVH.h"
#include "../../shaders/helperStruct.h"


class BVH;
struct helperStruct;

class deformableObjects {
protected:
    //list of particles
    std::vector<particle* > particles;
    //list of springs
    std::vector<spring *> springs;

    //particles that are hung or dragged
    //should not update their position normally
    std::vector<particle *> specialParticles;

    float * vertexBuffer;
    float * colorBuffer;
    float * normalBuffer;

    //environmental forces, gravity and wind
    float gravity;
    glm::vec3 wind;

    //model matrix
    glm::mat4 modelMatrix;

    //stuff needed for opengl
    GLuint vertex;
    GLuint colour;
    GLuint normal;

    //lightPosition
    glm::vec3 lightPos;

public:
    deformableObjects(float gravity, const glm::mat4 &modelMatrix, const glm::vec3 &lightPos);

    virtual ~deformableObjects();

    virtual int getSize();

    virtual int getNumberOfVertices();

    void setVertex(GLuint newVert);

    void setColour(GLuint newColour);

    void setNormal(GLuint newNormal);

    virtual void setGLuint();

    float *getVertexBuffer();

    float *getColorBuffer();

    float *getNormalBuffer() const;

    void emptySpecialParticles();

    void setSpecial(particle * p);

    const glm::mat4 &getModelMatrix() const;

    void setWind(const glm::vec3 &wind);

    void setMass(float mass);

    void setStiffness(float stiffness);

    void setGravity(float gravity);

    float getGravity() const;

    virtual void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID);

    virtual void integrate(float timeDelta);

    virtual void updateBuffer();

    virtual void detectCollision(collidable * obj);

    virtual BVH *getBvh() const;

    virtual void reset();

    virtual helperStruct isHovered(glm::vec3 origin, glm::vec3 direction);

    //shadow maps
    virtual void renderShadow(glm::mat4 depthP, glm::mat4 depthV,  GLuint programId);
    virtual void renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID);

    static void setCullFace(GLuint in);

    //integrators
    void rungeKutta2(float timeDelta);

    void rungeKutta4(float timeDelta);

    void exEuler(float timeDelta);
};


#endif //CODE_DEFORMABLEOBJECTS_H
