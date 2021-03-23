#ifndef CODE_SPHERE_H
#define CODE_SPHERE_H


#include <glm/glm.hpp>
#include "collidable.h"

class sphere : public collidable{
private:
    float ray;
    glm::vec3 colour;
    glm::mat4 model;
    int size;
public:
    sphere(int size, glm::mat4 model, glm::vec3 colour);

    int getSize() override;

    int getNumberOfVertices() override;
};


#endif
