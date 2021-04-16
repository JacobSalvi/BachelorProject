#include "spring.hpp"
#include "../../utilities/helperFunctions.h"

spring::spring( float l0, float ks, float kd, particle * part1, particle * part2) : l0(l0), ks(ks), kd(kd), p1(part1), p2(part2) {}


spring::~spring() {
}

void spring::setKs(float ks) {
    spring::ks = ks;
}

glm::vec3 spring::unifiedForce() {
    //calculating fs
    float length = glm::length(p1->getPosition()-p2->getPosition());
    glm::vec3 helper = (p1->getPosition()-p2->getPosition())/length;
    glm::vec3 fs = ks*helper*(length-l0);

    //calculating fd
    //let's pretend this is correct
    glm::vec3 fd = (kd)*(p1->getVelocity()-p2->getVelocity());
    return fs+fd;
}

void spring::updateParticlesForce(){
    //unified spring force
    glm::vec3 f = unifiedForce();
    //update force acting on particle1
    p1->addForce(-f);
    //update force acting on particle2
    //same as force acting on particle1 but in opposite direction
    p2->addForce(f);
}

void spring::rungeKuttaHelper(glm::vec3 a11, glm::vec3 a12, glm::vec3 a21, glm::vec3 a22){
    //calculate force
    float length = glm::length((p1->getPosition()+a11)-(p2->getPosition()+a12));
    glm::vec3 helper= ((p1->getPosition()+a11)-(p2->getPosition()+a12))/length;
    glm::vec3 fs = ks*helper*(length-l0);

    //calculate fd
    glm::vec3 fd = (kd)*(p1->getVelocity()+a21-p2->getVelocity()-a22);

    //update force acting on particle1
    p1->addForce(-fs-fd);
    //update force acting on particle2
    //same as force acting on particle1 but in opposite direction
    p2->addForce(fs+fd);
}

