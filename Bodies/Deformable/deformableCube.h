#ifndef CODE_DEFORMABLECUBE_H
#define CODE_DEFORMABLECUBE_H


#include "deformableObjects.h"

class deformableCube : public deformableObjects{
private:
    int size;

    std::vector<glm::vec3> resetPos;

    BVH * bvh;
public:

    deformableCube(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos);

    virtual ~deformableCube();

    void setGLuint() override;

    int getSize() override;

    int getNumberOfVertices() override;

    void integrate(float timeDelta) override;

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) override;

    helperStruct isHovered(glm::vec3 origin, glm::vec3 direction) override;

    BVH * getBvh() const override;

    void rungeKutta(float timeDelta);

    void updateBuffer() override;

    void reset() override;

    void detectCollision(collidable * obj) override;
    void renderShadow(glm::mat4 depthP, glm::mat4 depthV,  GLuint programId) override;
    void renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint programID) override;

};


#endif //CODE_DEFORMABLECUBE_H
