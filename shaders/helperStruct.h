#ifndef CODE_HELPERSTRUCT_H
#define CODE_HELPERSTRUCT_H

#include <GLFW/glfw3.h>
#include "../integrators/net.h"

struct helperStruct{
    net * cloth;
    GLuint vertex;
    GLuint color;
    glm::vec3 tr;
};

#endif //CODE_HELPERSTRUCT_H
