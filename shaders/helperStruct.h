#ifndef CODE_HELPERSTRUCT_H
#define CODE_HELPERSTRUCT_H

#include <GLFW/glfw3.h>
#include "../Bodies/Deformable/deformableObjects.h"

class deformableObjects;

struct helperStruct{
    bool isMouseOver;
    deformableObjects * obj;
    particle * point;
};

#endif //CODE_HELPERSTRUCT_H
