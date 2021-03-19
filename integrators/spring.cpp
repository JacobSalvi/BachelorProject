#include "spring.hpp"

spring::spring( float l0, float ks, float kd, particle * part1, particle * part2) : l0(l0), ks(ks), kd(kd), p1(part1), p2(part2) {}


spring::~spring() {
}

