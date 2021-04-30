#ifndef CODE_BVH_H
#define CODE_BVH_H
#include <glm/gtc/matrix_transform.hpp>
#include "vector"
#include "../Bodies/Deformable/particle.h"
#include "../Bodies/Solid/Sphere.h"
#include "../shaders/helperStruct.h"


struct helperStruct;

class BVH {
protected:
    //particles
    std::vector<particle *> p;


    //center and radius of the sphere
    glm::vec3 center;
    float radius;

    //actual sphere that will be shown
    //since I already have a class for this I  could as well use it
    sphere *sphereShown;

    //four children
    //if child0 is NULL, we have a lead
    BVH * child0=NULL;
    BVH * child1=NULL;
    BVH * child2=NULL;
    BVH * child3=NULL;

public:
    BVH(std::vector<particle *> part);

    sphere *getSphereShown() const;

    //virtual stuff
    virtual void update();

    virtual void detectCollisionSphere(glm::mat4 outModel, collidable * obj);

    virtual void detectCollisionCube(glm::mat4 outModel, collidable * obj);

    virtual void detectCollisionPlane(glm::mat4 outModel, collidable * obj);

    virtual void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model);

    virtual helperStruct rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model);

};


#endif //CODE_BVH_H
