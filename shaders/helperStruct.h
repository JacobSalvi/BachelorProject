#ifndef CODE_HELPERSTRUCT_H
#define CODE_HELPERSTRUCT_H

#include <GLFW/glfw3.h>
#include "../Bodies/Deformable/net.h"

class net;

struct helperStruct{
    bool isMouseOver;
    net * obj;
    particle * point;
};

#endif //CODE_HELPERSTRUCT_H
