#ifndef CODE_CUBE_H
#define CODE_CUBE_H


#include "../Bodies/Solid/collidable.h"
#include <iostream>

class cube : public collidable {
public:
    cube(glm::mat4 model, glm::vec3 colour, glm::vec3 lPos);

    int getSize() override;

    int getNumberOfVertices() override;

    void getName() override{
        std::cout<<"cube"<<std::endl;
    }

    mouseIntersectStruct isHovered(glm::vec3 origin, glm::vec3 direction) override;
};


#endif
