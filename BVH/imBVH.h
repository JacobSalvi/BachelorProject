
#ifndef CODE_IMBVH_H
#define CODE_IMBVH_H


#include "BVH.h"

class imBVH : public BVH{

public:
    imBVH(std::vector<particle *> part);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model) override;

    void detectCollisionPlane(glm::mat4 outModel, collidable *obj, float gravity) override;

    void detectCollisionSphere(glm::mat4 outModel, collidable * obj, float gravity) override;

    void update() override;

};


#endif //CODE_IMBVH_H
