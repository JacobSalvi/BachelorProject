#include "multiThreading.h"


bool dragThreadShouldLive = false;
bool shouldSimulate = true;
bool threadShouldLive = false;

std::thread sim;

std::thread dragThread;


void timer_start(unsigned int interval, const std::vector<net *> &list, const std::vector<collidable *> &collList) {
    sim = std::thread([interval, list, collList]() {
        static int counter = 0;
        while (threadShouldLive) {
            for (auto i : list) {
                i->integrate((float) (interval) / 1000.0f);
            }
            for (auto i : list) {
                for (auto j :collList){
                    i->detectCollision(j);
                }
            }
            counter++;

            //every 15 millisecond rebuild the bvh
            if (counter % 8 == 0) {
                for (auto i:list) {
                    i->getBvh()->update();
                }
            }

            //16 milliseconds ~= 60 frame per second
            if (counter == 16) {
                for (auto i : list) {
                    i->updateBuffer();
                }
                counter = 0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        std::cout << "The thread should be dead now" << std::endl;
    });
    sim.detach();
}


void dragMouse(mouseIntersectStruct obj) {
    dragThread = std::thread([obj]() {
        glm::vec3 helper = obj.point;
        while (dragThreadShouldLive) {
//this piece of code is a crime against humanity and honestly it brings shame
//on my family
#if 0
            glm::vec3 pixelRay = getMouseRay(ImGui::GetMousePos());
            glm::vec3 rayOrigin = getCameraPosition();
            glm::vec3 planeNormal = glm::normalize(rayOrigin);
            glm::vec3 p1 = planeVectorIntersection(rayOrigin, pixelRay, planeNormal, glm::vec3(0.0,0.0,0.0));

            //get object pos
            glm::mat4 tmp = obj.object->getModel();
            glm::vec3 p2 = planeVectorIntersection(rayOrigin, glm::normalize(helper-rayOrigin), planeNormal, glm::vec3(0.0,0.0,0.0));

            //translating the object accordingly
            glm::vec3 translation = p1-p2;
            tmp[3][0]+=translation[0];
            tmp[3][1]+=translation[1];
            tmp[3][2]+=translation[2];

            std::cout<<"translation: "<<translation[0]<<" ";
            std::cout<<translation[1]<<" ";
            std::cout<<translation[2]<<std::endl;

            //updating obj
            obj.object->setModel(tmp);
            helper+=translation;
#endif
            //this code also brings shame upon my family, but less
            glm::vec3 pixelRay = getMouseRay(ImGui::GetMousePos());
            glm::vec3 rayOrigin = getCameraPosition();
            glm::vec3 planeNormal = glm::normalize(rayOrigin);
            glm::vec3 p1 = planeVectorIntersection(rayOrigin, pixelRay, planeNormal, helper);

            glm::vec3 translation = p1 - helper;
            glm::mat4 tmp = obj.object->getModel();
            tmp[3][0] += translation[0];
            tmp[3][1] += translation[1];
            tmp[3][2] += translation[2];
            obj.object->setModel(tmp);
            helper += translation;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Drag thread should be dead now" << std::endl;
    });
    dragThread.detach();
}

//dragging deformable objects
//TODO: problem, the BVH go insane when I move it around
//sol1: rebuilt the sphere for each bvh element, probably won't work correctly
void dragMouse(helperStruct obj) {
    dragThread = std::thread([obj]() {
        //make all of the special point normal
        obj.obj->emptySpecialParticles();
        //set point as special point
        obj.obj->setSpecial(obj.point);

        glm::vec3 firstPlaneIntersection = obj.point->getPosition();
        while (dragThreadShouldLive) {
            //this code also brings shame upon my family, but less
            glm::vec3 pixelRay = getMouseRay(ImGui::GetMousePos());
            glm::vec3 rayOrigin = getCameraPosition();
            glm::vec3 planeNormal = glm::normalize(rayOrigin);
            glm::vec3 p1 = planeVectorIntersection(rayOrigin, pixelRay, planeNormal, firstPlaneIntersection);

            glm::vec3 translation =
                    p1 - glm::vec3(obj.obj->getModelMatrix() * glm::vec4(obj.point->getPosition(), 1.0f));

            obj.point->setPosition(obj.point->getPosition() + translation);

            //update bvh
            obj.obj->getBvh()->update();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Drag thread should be dead now" << std::endl;
    });
    dragThread.detach();
}

