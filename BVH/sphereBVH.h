#ifndef CODE_SPHEREBVH_H
#define CODE_SPHEREBVH_H

#include "BVH.h"
#include "../integrators/particle.h"
#include <vector>
#include "../Collidables/Sphere.h"

//it is easy to check for collisions with spheres so here we go
class sphereBVH : public BVH {
private:
    //center and radius of the sphere
    glm::vec3 center;
    float radius;

    //actual sphere that will be shown
    //since I already have a class for this I  could as well use it
    sphere *sphereShown;
public:
    sphereBVH(std::vector<particle *> particles, int row);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, bool wireFrame, glm::mat4 model) override;

    sphere *getSphereShown() const;
};


#endif //CODE_SPHEREBVH_H
