#ifndef CODE_TEXTUREDCLOTH_H
#define CODE_TEXTUREDCLOTH_H

#include "net.h"

class texturedCloth : public net{
private:
    GLuint texture;
    GLuint textureId;

    GLuint nt;
    GLuint ntId;

    GLuint programID;

    std::vector<glm::vec3> tangent;
    std::vector<glm::vec3> bitangent;
public:
    texturedCloth(float mass, GLuint texture, GLuint textureId, const glm::vec3 &color, float gravity,
                  const glm::mat4 &mod, const glm::vec3 &lPos, GLuint pID);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint pID) override;

    void renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint pID) override;

    void computeTangents();

    void updateBuffer() override;
};


#endif //CODE_TEXTUREDCLOTH_H
