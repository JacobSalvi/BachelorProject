#ifndef CODE_PLANE_H
#define CODE_PLANE_H


#include "../Bodies/Solid/collidable.h"
#include <iostream>

class plane : public collidable {
public:
    plane(float width, float depth, glm::mat4 model, glm::vec3 colour, glm::vec3 lPos);

    int getSize() override;

    int getNumberOfVertices() override;

    void getName() override{
        std::cout<<"plane"<<std::endl;
    }

    mouseIntersectStruct isHovered(glm::vec3 origin, glm::vec3 direction) override;

    int returnType() override;
};


#endif //CODE_PLANE_H
