#ifndef CODE_CLOTHBVH_H
#define CODE_CLOTHBVH_H


#include "BVH.h"

//struct helperStruct;

//it is easy to check for collisions with spheres so here we go
class clothBVH : public BVH {
private:
//    //four children
//    //if child0 is NULL, we have a lead
//    clothBVH * child0=NULL;
//    clothBVH * child1=NULL;
//    clothBVH * child2=NULL;
//    clothBVH * child3=NULL;

//    //particles
//    std::vector<particle *> p;
//
//
//    //center and radius of the sphere
//    glm::vec3 center;
//    float radius;
//
//    //actual sphere that will be shown
//    //since I already have a class for this I  could as well use it
//    sphere *sphereShown;
public:
    clothBVH(std::vector<particle *> particles, int row);

    //for debugging stuff
    //clothBVH();

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model) override;

    //sphere *getSphereShown() const;

    helperStruct rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) override;

    void update() override;

    void detectCollisionSphere(glm::mat4 outModel, collidable * obj, float gravity) override;

    void detectCollisionCube(glm::mat4 outModel, collidable * obj, float gravity) override;

    void detectCollisionPlane(glm::mat4 outModel, collidable * obj, float gravity) override;

};


#endif //CODE_CLOTHBVH_H
