
#ifndef CODE_DEFORMABLESPHERE_H
#define CODE_DEFORMABLESPHERE_H
#include "deformableObjects.h"

class deformableSphere : public deformableObjects{
private:
    int size;

    sphereBVH * bvh;
public:
    deformableSphere(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos);

    void setGLuint() override;

    int getSize() override;

    int getNumberOfVertices() override;

    void integrate(float timeDelta) override;

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) override;

    helperStruct isHovered(glm::vec3 origin, glm::vec3 direction) override;

    sphereBVH * getBvh() const override;

    void rungeKutta(float timeDelta);

    void updateBuffer() override;

    void reset() override;

};


#endif //CODE_DEFORMABLESPHERE_H
