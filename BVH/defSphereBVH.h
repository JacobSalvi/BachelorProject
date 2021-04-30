#ifndef CODE_DEFSPHEREBVH_H
#define CODE_DEFSPHEREBVH_H
#include "BVH.h"

class defSphereBVH : public BVH{
public:
    //constructors
    defSphereBVH(std::vector<particle *> particles, float radius, glm::vec3 center);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model) override;


    helperStruct rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) override;

    void update() override;

    void detectCollisionSphere(glm::mat4 outModel, collidable * obj) override;

    void detectCollisionCube(glm::mat4 outModel, collidable * obj) override;

    void detectCollisionPlane(glm::mat4 outModel, collidable * obj) override;

};


#endif //CODE_DEFSPHEREBVH_H
