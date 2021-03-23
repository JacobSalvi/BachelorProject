#ifndef CODE_PLANE_H
#define CODE_PLANE_H


#include "collidable.h"

class plane : public collidable {
public:
    plane(float width, float depth, glm::mat4 model, glm::vec3 colour);

    int getSize() override;

    int getNumberOfVertices() override;
};


#endif //CODE_PLANE_H
