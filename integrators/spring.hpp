#include <iostream>

#ifndef CODE_SPRING_HPP
#define CODE_SPRING_HPP
#include "particle.h"

class spring{
private:
    //spring properties
    float l0;
    float ks;
    float kd;

    //particles associated with a spring
    particle * p1;
    particle * p2;
public:

    spring(float l0, float ks, float kd, particle * part1, particle* part2);

    void setParticles(particle part1, particle part2){
        p1=&part1;
        p2=&part2;
    }

    void setKs(float ks);

    void printExtremes(){
        std::cout<<"p1 is: "<<p1->getId()<<std::endl;
        std::cout<<"p2 is: "<<p2->getId()<<std::endl;
    }

    //the force on both particle has the same intensity
    //but opposite directions
    void updateParticlesForce(){
        //calculate force
        float length = glm::length(p1->getPosition()-p2->getPosition());
        glm::vec3 helper= (p1->getPosition()-p2->getPosition())/length;
        glm::vec3 computedForce = ks*helper*(length-l0);

        //update force acting on particle1
        p1->addForce(-computedForce);
        //update force acting on particle2
        //same as force acting on particle1 but in opposite direction
        p2->addForce(computedForce);
    }

    //needed for rungeKutta
    void rungeKuttaHelper(glm::vec3 a11, glm::vec3 a12){
        //calculate force
        float length = glm::length((p1->getPosition()+a11)-(p2->getPosition()+a12));
        glm::vec3 helper= ((p1->getPosition()+a11)-(p2->getPosition()+a12))/length;
        glm::vec3 computedForce = ks*helper*(length-l0);

        //update force acting on particle1
        p1->addForce(-computedForce);
        //update force acting on particle2
        //same as force acting on particle1 but in opposite direction
        p2->addForce(computedForce);
    }

    //calculate unified spring force
    glm::vec3 unifiedForce(){
        //calculating fs
        float length = glm::length(p1->getPosition()-p2->getPosition());
        glm::vec3 helper = (p1->getPosition()-p2->getPosition())/length;
        glm::vec3 fs = ks*helper*(length-l0);

        //calculating fd
        glm::vec3 fd = kd*(p1->getVelocity()-p2->getVelocity())*helper;
        return fs+fd;
    }

    int getPart1Id(){
        return p1->getId();
    }

    int getPart2Id(){
        return p2->getId();
    }

    virtual ~spring();
};
#endif //CODE_SPRING_HPP
