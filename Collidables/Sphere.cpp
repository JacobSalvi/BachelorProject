#include "Sphere.h"
#include <cmath>
#include <iostream>

//number of vertices:
//size^2*18
sphere::sphere(int size, glm::mat4 model, glm::vec3 colour, glm::vec3 lPos) : collidable(colour, model, lPos),
                                                                              size(size) {
    vertexBuffer = new float[size * size * 18];
    colorBuffer = new float[size * size * 18];

    int vertPos = 0;
    int colPos = 0;
    float step = (float) 1 / size;
    float u = 0.0f;
    for (int i = 0; i < size; ++i) {
        float v = 0.0f;
        for (int j = 0; j < size; ++j) {
            double t = sin(M_PI * v);
            double x1 = t * cos(2 * M_PI * u);
            double z1 = t * sin(2 * M_PI * u);
            double y1 = cos(M_PI * v);

            double x4 = t * cos(2 * M_PI * (u + step));
            double z4 = t * sin(2 * M_PI * (u + step));
            double y4 = cos(M_PI * v);

            t = sin(M_PI * (v + step));
            double x2 = t * cos(2 * M_PI * u);
            double z2 = t * sin(2 * M_PI * u);
            double y2 = cos(M_PI * (v + step));

            double x3 = t * cos(2 * M_PI * (u + step));
            double z3 = t * sin(2 * M_PI * (u + step));
            double y3 = cos(M_PI * (v + step));

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            vertexBuffer[vertPos++] = x2;
            vertexBuffer[vertPos++] = y2;
            vertexBuffer[vertPos++] = z2;

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x4;
            vertexBuffer[vertPos++] = y4;
            vertexBuffer[vertPos++] = z4;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            for (int k = 0; k < 6; k++) {
                colorBuffer[colPos++] = colour[0];
                colorBuffer[colPos++] = colour[1];
                colorBuffer[colPos++] = colour[2];
            }
            v += step;
        }
        u += step;
    }

    //unit sphere
    for (int i = 0; i < size * size * 18; ++i) {
        vertexBuffer[i] = vertexBuffer[i] / 2.0f;
    }

    //normals are the same as the vertex
    normalBuffer=vertexBuffer;

    setGLuint();
}

int sphere::getSize() {
    return sizeof(float) * size * size * 18;
}

int sphere::getNumberOfVertices() {
    return size * size * 18;
}


bool sphere::rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 superModel) {
    //return struct
    mouseIntersectStruct toReturn;
    toReturn.isMouseOver = false;

    //i don't even know
    glm::mat4 whatEven = superModel * getModel();

    //sphere center
    glm::vec3 center(whatEven[3][0], whatEven[3][1], whatEven[3][2]);

    //transformation into local coordinate system
    glm::mat4 M_ = glm::inverse(whatEven);
    glm::vec4 orHelper = M_ * glm::vec4(origin, 1.0f);
    glm::vec3 origin_(orHelper.x, orHelper.y, orHelper.z);
    glm::vec4 dirHelper = M_ * glm::vec4(direction, 0.0f);
    glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
    d_ = glm::normalize(d_);

    float t = -glm::dot(origin_, d_);

    float d2 = glm::dot(origin_, origin_) - t * t;


    if (d2 <= 1.0f) {
        return true;
    }
    return false;
}

mouseIntersectStruct sphere::isHovered(glm::vec3 origin, glm::vec3 direction) {
    //return struct
    mouseIntersectStruct toReturn;
    toReturn.isMouseOver = false;

    //i don't even know
    glm::mat4 whatEven = getModel();

    //sphere center
    glm::vec3 center(whatEven[3][0], whatEven[3][1], whatEven[3][2]);

    //transformation into local coordinate system
    glm::mat4 M_ = glm::inverse(whatEven);
    glm::vec4 orHelper = M_ * glm::vec4(origin, 1.0f);
    glm::vec3 origin_(orHelper.x, orHelper.y, orHelper.z);
    glm::vec4 dirHelper = M_ * glm::vec4(direction, 0.0f);
    glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
    d_ = glm::normalize(d_);

    float t = -glm::dot(origin_, d_);

    float d2 = glm::dot(origin_, origin_) - t * t;

    if (d2 > 1.0f) {
        //no intersection
        return toReturn;
    } else {
        //intersection
        float dt = sqrt(1.0 - d2);
        float t1 = t - dt;

        //intersected point on the sphere
        glm::vec3 p_ = origin_ + t1 * d_;

        //savings the intersection data in the struct
        toReturn.isMouseOver = true;
        toReturn.point = glm::vec3(whatEven * glm::vec4(p_, 1.0f));

        return toReturn;
    }
}

