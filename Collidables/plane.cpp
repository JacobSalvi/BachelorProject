#include "plane.h"
#include <iostream>

int plane::getSize() {
    return sizeof(float)*18;
}

int plane::getNumberOfVertices() {
    return 6;
}

plane::plane(float width, float depth, glm::mat4 model, glm::vec3 colour) : collidable(colour, model){

    vertexBuffer = new float [18];
    colorBuffer = new float[18];

    vertexBuffer[0] = -width;
    vertexBuffer[1] = 0.0f;
    vertexBuffer[2] = -depth;

    vertexBuffer[3] = width;
    vertexBuffer[4] = 0.0f;
    vertexBuffer[5] = -depth;

    vertexBuffer[6] = width;
    vertexBuffer[7] = 0.0f;
    vertexBuffer[8] = depth;

    vertexBuffer[9] = width;
    vertexBuffer[10] = 0.0f;
    vertexBuffer[11] = depth;

    vertexBuffer[12] = -width;
    vertexBuffer[13] = 0.0f;
    vertexBuffer[14] = depth;

    vertexBuffer[15] = -width;
    vertexBuffer[16] = 0.0f;
    vertexBuffer[17] = -depth;

    int pos=0;
    for(int i=0; i<6; ++i){
        colorBuffer[pos++]=colour[0];
        colorBuffer[pos++]=colour[1];
        colorBuffer[pos++]=colour[2];
    }
}
