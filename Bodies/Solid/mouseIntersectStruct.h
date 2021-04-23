#ifndef CODE_MOUSEINTERSECTSTRUCT_H
#define CODE_MOUSEINTERSECTSTRUCT_H


#include "collidable.h"
//needed due to circular dependency
class collidable;

struct mouseIntersectStruct{
    bool isMouseOver;
    collidable * object;
    glm::vec3 point;
};

#endif //CODE_MOUSEINTERSECTSTRUCT_H
