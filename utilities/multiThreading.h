#ifndef CODE_MULTITHREADING_H
#define CODE_MULTITHREADING_H

#include "../Bodies/Solid/mouseIntersectStruct.h"
#include "../shaders/helperStruct.h"
#include "helperFunctions.h"
#include "../shaders/controls.hpp"
#include "../Bodies/Deformable/importedModels.h"
#include <thread>

class collidable;
struct helperStruct;
struct mouseIntersectStruct;

extern std::thread sim;

extern std::thread dragThread;

extern bool dragThreadShouldLive;
extern bool shouldSimulate;
extern bool threadShouldLive;
void timer_start(unsigned int interval,  const std::vector<deformableObjects *>& list, const std::vector<collidable *> &collList, importedModels * a);

void checkCollision();

void dragMouse(mouseIntersectStruct obj);
void dragMouse(helperStruct obj);
#endif //CODE_MULTITHREADING_H
