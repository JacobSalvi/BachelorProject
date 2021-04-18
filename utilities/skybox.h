#ifndef CODE_SKYBOX_H
#define CODE_SKYBOX_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::vec3 cPos);

    //to load the skybox texture
    void loadTexture();
};


#endif //CODE_SKYBOX_H
