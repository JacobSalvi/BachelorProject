
#include "BVH.h"

BVH::BVH(std::vector<particle *> part) :p(part){}

sphere *BVH::getSphereShown() const {
    return sphereShown;
}

void BVH::update() {}

void BVH::detectCollisionSphere(glm::mat4 outModel, collidable *obj, float gravity) {}

void BVH::detectCollisionCube(glm::mat4 outModel, collidable *obj, float gravity) {}

void BVH::detectCollisionPlane(glm::mat4 outModel, collidable *obj, float gravity) {}

void BVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model) {}

helperStruct BVH::rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) {
    return helperStruct();
}


