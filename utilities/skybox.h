#ifndef CODE_SKYBOX_H
#define CODE_SKYBOX_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "stdbi_image.h"
#include <vector>
#include <string>

class skybox {
private:
    GLuint programID;
    GLuint vertex;
    unsigned int textureID;

    float * vertexBuffer;
    std::vector<std::string> faces;
public:
    //constructor and destructor
    skybox(GLuint pId);
    ~skybox();

    //unexpectedly the render functions renders stuff
    void render();

    //to load the skybox texture
    void loadTexture();
};


#endif //CODE_SKYBOX_H
