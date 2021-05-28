#include "multiThreading.h"


bool dragThreadShouldLive = false;
bool shouldSimulate = true;
bool threadShouldLive = false;

std::thread sim;

std::thread collThread;

std::thread updateBvh;

std::thread updateBuffers;

std::thread dragThread;

std::thread otherThread;


void timer_start(unsigned int interval, const std::vector<deformableObjects *> &list, const std::vector<collidable *> &collList, importedModels * a) {
    unsigned int collInterval = 8 * interval;
    //thread that checks collision
    collThread = std::thread([collInterval, list, collList]() {
        while (threadShouldLive) {
            for (auto i : list) {
                for (auto j :collList) {
                    i->detectCollision(j);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(collInterval));
        }
    });
    collThread.detach();

    //update bvh
    updateBvh = std::thread([list, collInterval]() {
        while (threadShouldLive) {
            for (auto i :list) {
                i->getBvh()->update();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(collInterval));
        }
    });
    updateBvh.detach();

    //update buffer
    unsigned int upInt = 16;
    updateBuffers = std::thread([list, upInt] {
        while (threadShouldLive) {
            for (auto i : list) {
                i->updateBuffer();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(upInt));
        }
    });
    updateBuffers.detach();

    //main animation thread
    unsigned int mInt = interval;
    sim = std::thread([mInt, list, collList]() {
        static int counter = 0;
        while (threadShouldLive) {
            auto start = std::chrono::high_resolution_clock::now();
            for (auto i : list) {
                i->integrate((float) mInt/ 1000.0f);
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = duration_cast<std::chrono::microseconds>(stop - start);
            std::cout<<"duration is: "<<duration.count()<<std::endl;
            counter++;
            std::this_thread::sleep_for(std::chrono::milliseconds(mInt));
        }
        std::cout << "The thread should be dead now" << std::endl;
    });
    sim.detach();

    static int robe = 0;
    otherThread = std::thread([a](){
        while(threadShouldLive){
            a->rungeKutta(0.001f);
            robe++;
            if(robe==1){
                robe=0;
                a->updateBuffer();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    otherThread.detach();
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
            //obj.obj->getBvh()->update();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Drag thread should be dead now" << std::endl;
    });
    dragThread.detach();
}

