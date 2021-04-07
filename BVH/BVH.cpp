#include "BVH.h"



BVH *BVH::getChild0() const {
    return child0;
}

BVH *BVH::getChild1() const {
    return child1;
}

BVH::BVH(glm::mat4 *model, BVH *child0, BVH *child1, BVH *child2, BVH *child3) : model(model), child0(child0),
                                                                                       child1(child1), child2(child2),
                                                                                       child3(child3) {}

BVH *BVH::getChild2() const {
    return child2;
}

BVH *BVH::getChild3() const {
    return child3;
}

BVH::BVH(glm::mat4 *model) : model(model) {}
