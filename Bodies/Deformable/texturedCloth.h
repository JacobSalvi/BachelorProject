#ifndef CODE_TEXTUREDCLOTH_H
#define CODE_TEXTUREDCLOTH_H

#include "net.h"

class texturedCloth : public net{
private:
    GLuint texture;
    GLuint textureId;

    GLuint programID;
public:
    texturedCloth(float mass, GLuint texture, GLuint textureId, const glm::vec3 &color, float gravity,
                  const glm::mat4 &mod, const glm::vec3 &lPos, GLuint pID);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint pID) override;
};


#endif //CODE_TEXTUREDCLOTH_H
