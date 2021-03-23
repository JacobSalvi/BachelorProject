#ifndef CODE_CUBE_H
#define CODE_CUBE_H


#include "collidable.h"

class cube : public collidable {
public:
    cube(glm::mat4 model, glm::vec3 colour);

    int getSize() override;

    int getNumberOfVertices() override;
};


#endif
