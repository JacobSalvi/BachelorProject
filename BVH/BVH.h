#ifndef CODE_BVH_H
#define CODE_BVH_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include "../integrators/particle.h"
#include "../shaders/helperStruct.h"

//bounding volume hierarchy needed to avoid
//turning the cpu in a toaster

//TODO: sphere? boxes? convex hull?
class BVH {
protected:
    //four children
    //if child0 is NULL, we have a lead
    BVH * child0=NULL;
    BVH * child1=NULL;
    BVH * child2=NULL;
    BVH * child3=NULL;

    //particles
    std::vector<particle *> p;

public:
    BVH(std::vector<particle *> p);


    BVH(BVH *child0, BVH *child1, BVH *child2, BVH *child3);

    BVH *getChild0() const;

    BVH *getChild1() const;

    BVH *getChild2() const;

    BVH *getChild3() const;

    virtual void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, bool wireFrame, glm::mat4 model);

    //irtual helperStruct rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model);
};


#endif //CODE_BVH_H
