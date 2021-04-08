#ifndef CODE_BVH_H
#define CODE_BVH_H

#include <GL/glew.h>
#include <glm/glm.hpp>

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

    //vertex and color
    float * vertexBuffer;
    float * colorBuffer;


public:
    BVH();


    BVH(BVH *child0, BVH *child1, BVH *child2, BVH *child3);

    BVH *getChild0() const;

    BVH *getChild1() const;

    BVH *getChild2() const;

    BVH *getChild3() const;

    virtual void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, bool wireFrame, glm::mat4 model);
};


#endif //CODE_BVH_H
