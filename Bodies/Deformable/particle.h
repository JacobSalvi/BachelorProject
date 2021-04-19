#include <iostream>
#include <vector>

#ifndef CODE_PARTICLE_H
#define CODE_PARTICLE_H

#include <glm/glm.hpp>

class particle {
private:
    float mass;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 position;

    //useless, but I need it to debug stuff
    int id;

    //force
    glm::vec3 force = glm::vec3(0.0f,0.0f,0.0f);
    //collision force
    glm::vec3 collisionForce =glm::vec3(0.0f, 0.0f,0.0f);
public:

    particle(glm::vec3 pos, float mass);

    void setId(int i){
        id=i;
    }

    int getId() const{
        return id;
    }

    void setForce(glm::vec3 newForce){
        force = newForce;
    }

    void addForce(glm::vec3 forceToAdd){
        force+=forceToAdd;
    }

    void setPosition(const glm::vec3 &position);

    virtual ~particle();

    void setVelocity(glm::vec3 newVel){
        velocity=newVel;
    }

    const glm::vec3 &getVelocity() const;

    float getMass() const;

    const glm::vec3 &getForce() const;

    const glm::vec3 &getPosition() const;

    void setMass(float mass);

    void setCollisionForce(const glm::vec3 &newForce);

    void resetCollisionForce();

    const glm::vec3 &getCollisionForce() const;
};


#endif //CODE_PARTICLE_H
