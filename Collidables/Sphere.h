#ifndef CODE_SPHERE_H
#define CODE_SPHERE_H


#include <glm/glm.hpp>
#include "../Bodies/Solid/collidable.h"
#include <iostream>
#include <cmath>

class sphere : public collidable{
private:
    int size;
public:
    sphere(int size, glm::mat4 model, glm::vec3 colour, glm::vec3 lPos);

    int getSize() override;

    int getNumberOfVertices() override;

    void getName() override{
        std::cout<<"sphere"<<std::endl;
    }

    bool rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 superModel);

    mouseIntersectStruct isHovered(glm::vec3 origin, glm::vec3 direction) override;
};


#endif
