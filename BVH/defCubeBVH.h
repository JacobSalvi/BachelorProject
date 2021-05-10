#ifndef CODE_DEFCUBEBVH_H
#define CODE_DEFCUBEBVH_H


#include "BVH.h"

class defCubeBVH : public BVH{
private:
    //additionally children
    defCubeBVH * child4 = NULL;
    defCubeBVH * child5 = NULL;

    // 4 extremes of face
    particle * tl;
    particle * tr;
    particle * bl;
    particle * br;
public:
    defCubeBVH(std::vector<particle *> particles, float radius, glm::vec3 center);

    virtual ~defCubeBVH();

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model) override;


    helperStruct rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) override;

    void update() override;

    void detectCollisionSphere(glm::mat4 outModel, collidable * obj) override;

    void detectCollisionCube(glm::mat4 outModel, collidable * obj) override;

    void detectCollisionPlane(glm::mat4 outModel, collidable * obj) override;
};


#endif //CODE_DEFCUBEBVH_H
