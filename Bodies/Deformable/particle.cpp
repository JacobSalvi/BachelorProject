#include "particle.h"

particle::particle(glm::vec3 pos, float mass) : position(pos), mass(mass){}

const glm::vec3 &particle::getVelocity() const {
    return velocity;
}

const glm::vec3 &particle::getPosition() const {
    return position;
}

const glm::vec3 &particle::getForce() const {
    return force;
}

float particle::getMass() const {
    return mass;
}

void particle::setPosition(const glm::vec3 &newPosition) {
    particle::position = newPosition;
}

particle::~particle() = default;

void particle::setMass(float newMass) {
    particle::mass = newMass;
}

void particle::setCollisionForce(const glm::vec3 &newForce) {
    particle::collisionForce = newForce;
}

void particle::resetCollisionForce() {
    particle::setCollisionForce(glm::vec3(0,0,0));
}

const glm::vec3 &particle::getCollisionForce() const {
    return collisionForce;
}

