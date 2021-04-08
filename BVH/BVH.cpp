#include "BVH.h"

#include <utility>



BVH *BVH::getChild0() const {
    return child0;
}

BVH *BVH::getChild1() const {
    return child1;
}

BVH::BVH(BVH *child0, BVH *child1, BVH *child2, BVH *child3) :  child0(child0), child1(child1), child2(child2),
                                                                                       child3(child3) {}

BVH *BVH::getChild2() const {
    return child2;
}

BVH *BVH::getChild3() const {
    return child3;
}

BVH::BVH(std::vector<particle *> p) :p(std::move(p)){}

void BVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, bool wireFrame, glm::mat4 model) {

}


