
#ifndef CODE_IMPORTEDMODELS_H
#define CODE_IMPORTEDMODELS_H

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include "../utilities/helperFunctions.h"
#include <map>
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "particle.h"
#include "spring.hpp"
#include "../../BVH/imBVH.h"

class importedModels {
private:
    //model matrix
    glm::mat4 modelMatrix;
    float wind=0.0f;
    float gravity=-4.0f;

    //stuff we get when importing
    std::vector<float> linearVerts;
    std::vector<float> linearUVs;
    std::vector<float> linearNormals;
    std::vector<float> linearTangents;
    std::vector<float> linearBitangents;
    std::vector<unsigned int> tris;

    //vector of particles and springs
    std::map<particle *, std::vector<int>> p;
    std::vector<particle *> particles;
    std::vector<particle *> specialParticles;
    std::vector<spring *> springs;

    //bvh
    imBVH * bvh;
public:
    importedModels(const char * path, glm::mat4 mod);

    void render(glm::mat4 pm, glm::mat4 vm, GLuint pi, GLuint t, GLuint ti);

    void generateMSS();

    void meshToTethrahedon();

    void updateBuffer();

    void rungeKutta(float timeDelta);

    void drop();

    void detectCollision(collidable * obj);

    void detectCollision(deformableObjects * obj);

    imBVH *getBvh() const;

};


#endif //CODE_IMPORTEDMODELS_H
