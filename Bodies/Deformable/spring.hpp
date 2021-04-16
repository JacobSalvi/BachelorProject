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
    void updateParticlesForce();

    //needed for rungeKutta
    void rungeKuttaHelper(glm::vec3 a11, glm::vec3 a12, glm::vec3 a21, glm::vec3 a22);

    //calculate unified spring force
    glm::vec3 unifiedForce();

    int getPart1Id(){
        return p1->getId();
    }

    int getPart2Id(){
        return p2->getId();
    }

    virtual ~spring();
};
#endif //CODE_SPRING_HPP
