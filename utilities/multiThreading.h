#ifndef CODE_MULTITHREADING_H
#define CODE_MULTITHREADING_H

#include "../Collidables/mouseIntersectStruct.h"
#include "../shaders/helperStruct.h"
#include "helperFunctions.h"
#include "../shaders/controls.hpp"
#include <thread>

class collidable;
struct helperStruct;
struct mouseIntersectStruct;

extern std::thread sim;

extern std::thread dragThread;

extern bool dragThreadShouldLive;
extern bool shouldSimulate;
extern bool threadShouldLive;
void timer_start(unsigned int interval,  const std::vector<net *>& list, const std::vector<collidable *> &collList);

void dragMouse(mouseIntersectStruct obj);
void dragMouse(helperStruct obj);
#endif //CODE_MULTITHREADING_H
