#ifndef CODE_SPHEREBVH_H
#define CODE_SPHEREBVH_H

#include "../Collidables/Sphere.h"
#include "../Bodies/Deformable/particle.h"
#include "../shaders/helperStruct.h"

struct helperStruct;

//it is easy to check for collisions with spheres so here we go
class sphereBVH {
private:
    //four children
    //if child0 is NULL, we have a lead
    sphereBVH * child0=NULL;
    sphereBVH * child1=NULL;
    sphereBVH * child2=NULL;
    sphereBVH * child3=NULL;

    //particles
    std::vector<particle *> p;


    //center and radius of the sphere
    glm::vec3 center;
    float radius;

    //actual sphere that will be shown
    //since I already have a class for this I  could as well use it
    sphere *sphereShown;
public:
    sphereBVH(std::vector<particle *> particles, int row);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model);

    sphere *getSphereShown() const;

    helperStruct rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model);

    void update();

    void detectCollisionSphere(glm::mat4 outModel, collidable * obj);

    void detectCollisionCube(glm::mat4 outModel, collidable * obj);

    void detectCollisionPlane(glm::mat4 outModel, collidable * obj);

};


#endif //CODE_SPHEREBVH_H
