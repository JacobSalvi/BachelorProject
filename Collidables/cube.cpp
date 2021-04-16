#include "cube.h"
#include <iostream>

cube::cube(glm::mat4 model, glm::vec3 colour, glm::vec3 lPos) : collidable(colour, model, lPos) {
    vertexBuffer = new float[108];
    float helper[] = {
            -0.5, 0.5, 0.5, // front face
            -0.5, -0.5, 0.5,
            0.5, -0.5, 0.5,
            -0.5, 0.5, 0.5,
            0.5, -0.5, 0.5,
            0.5, 0.5, 0.5,
            0.5, -0.5, -0.5,// back face
            -0.5, -0.5, -0.5,
            -0.5, 0.5, -0.5,
            0.5, 0.5, -0.5,
            0.5, -0.5, -0.5,
            -0.5, 0.5, -0.5,
            0.5, -0.5, 0.5,// right face
            0.5, -0.5, -0.5,
            0.5, 0.5, -0.5,
            0.5, 0.5, 0.5,
            0.5, -0.5, 0.5,
            0.5, 0.5, -0.5,
            -0.5, 0.5, -0.5,// left face
            -0.5, -0.5, -0.5,
            -0.5, -0.5, 0.5,
            -0.5, 0.5, -0.5,
            -0.5, -0.5, 0.5,
            -0.5, 0.5, 0.5,
            -0.5, 0.5, 0.5, //top face
            0.5, 0.5, -0.5,
            -0.5, 0.5, -0.5,
            -0.5, 0.5, 0.5,
            0.5, 0.5, 0.5,
            0.5, 0.5, -0.5,
            -0.5, -0.5, 0.5, //bottom face
            -0.5, -0.5, -0.5,
            0.5, -0.5, -0.5,
            -0.5, -0.5, 0.5,
            0.5, -0.5, -0.5,
            0.5, -0.5, 0.5
    };
    for (int i = 0; i < 108; ++i) {
        vertexBuffer[i] = helper[i];
    }
    //vertexBuffer = helper;
    colorBuffer = new float[108];
    int pos = 0;
    for (int i = 0; i < 36; i++) {
        colorBuffer[pos++] = colour[0];
        colorBuffer[pos++] = colour[1];
        colorBuffer[pos++] = colour[2];
    }

    //normal buffer
    normalBuffer = new float[108];
    pos = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            switch (i) {
                case 0:
                    //front face normal
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = 1.0f;
                    break;
                case 1:
                    //backface normal
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = -1.0f;
                    break;
                case 2:
                    //left face normal
                    normalBuffer[pos++] = 1.0f;
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = 1.0f;
                    break;
                case 3:
                    //right face normal
                    normalBuffer[pos++] = -1.0f;
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = 0.0f;
                    break;
                case 4:
                    //top face
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = 1.0f;
                    normalBuffer[pos++] = 0.0f;
                    break;
                case 5:
                    //bottom face
                    normalBuffer[pos++] = 0.0f;
                    normalBuffer[pos++] = -1.0f;
                    normalBuffer[pos++] = 0.0f;
                    break;
            }
        }
    }
}

int cube::getSize() {
    return sizeof(float) * 108;
}

int cube::getNumberOfVertices() {
    return 36;
}

mouseIntersectStruct cube::isHovered(glm::vec3 origin, glm::vec3 direction) {
    //returned struct
    mouseIntersectStruct toReturn;
    toReturn.isMouseOver = false;

    glm::mat4 M_ = glm::inverse(getModel());
    glm::vec4 orHelper = M_ * glm::vec4(origin, 1.0f);
    glm::vec3 origin_(orHelper.x, orHelper.y, orHelper.z);
    glm::vec4 dirHelper = M_ * glm::vec4(direction, 0.0f);
    glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
    d_ = glm::normalize(d_);

    glm::vec3 p_;
    glm::vec3 n_;
    float t = 0.0;

    bool intersect = false;
    bool inside = true;
    float t_;
    glm::vec3 ip_;
    glm::vec3 in_;

    bool yz = false;
    if (origin_.x < -1.0) {
        inside = false;
        if (d_.x > 0.0) {
            t_ = (-1.0 - origin_.x) / d_.x;
            in_ = glm::vec3(-1.0, 0.0, 0.0);
            yz = true;
        }
    }
    if (origin_.x > 1.0) {
        inside = false;
        if (d_.x < 0.0) {
            t_ = (1.0 - origin_.x) / d_.x;
            in_ = glm::vec3(1.0, 0.0, 0.0);
            yz = true;
        }
    }
    // potentially closer (or first) intersection point ?
    if (yz && (!intersect || t_ < t)) {
        ip_ = origin_ + t_ * d_;
        // intersection point inside the cube's face ?
        if (ip_.y >= -1.0 && ip_.y <= 1.0 && ip_.z >= -1.0 && ip_.z <= 1.0) {
            t = t_;
            p_ = ip_;
            n_ = in_;
            intersect = true;
        }
    }

    //
    // intersection with the xz-faces
    //
    bool xz = false;
    if (origin_.y < -1.0) {
        inside = false;
        if (d_.y > 0.0) {
            t_ = (-1.0 - origin_.y) / d_.y;
            in_ = glm::vec3(0.0, -1.0, 0.0);
            xz = true;
        }
    }
    if (origin_.y > 1.0) {
        inside = false;
        if (d_.y < 0.0) {
            t_ = (1.0 - origin_.y) / d_.y;
            in_ = glm::vec3(0.0, 1.0, 0.0);
            xz = true;
        }
    }
    // potentially closer (or first) intersection point ?
    if (xz && (!intersect || t_ < t)) {
        ip_ = origin_ + t_ * d_;
        // intersection point inside the cube's face ?
        if (ip_.x >= -1.0 && ip_.x <= 1.0 && ip_.z >= -1.0 && ip_.z <= 1.0) {
            t = t_;
            p_ = ip_;
            n_ = in_;
            intersect = true;
        }
    }

    //
    // intersection with the xy-faces
    //
    bool xy = false;
    if (origin_.z < -1.0) {
        inside = false;
        if (d_.z > 0.0) {
            t_ = (-1.0 - origin_.z) / d_.z;
            in_ = glm::vec3(0.0, 0.0, -1.0);
            xy = true;
        }
    }
    if (origin_.z > 1.0) {
        inside = false;
        if (d_.z < 0.0) {
            t_ = (1.0 - origin_.z) / d_.z;
            in_ = glm::vec3(0.0, 0.0, 1.0);
            xy = true;
        }
    }
    // potentially closer (or first) intersection point ?
    if (xy && (!intersect || t_ < t)) {
        ip_ = origin_ + t_ * d_;
        // intersection point inside the cube's face ?
        if (ip_.x >= -1.0 && ip_.x <= 1.0 && ip_.y >= -1.0 && ip_.y <= 1.0) {
            t = t_;
            p_ = ip_;
            n_ = in_;
            intersect = true;
        }
    }

    if (intersect) {
        //saving intersection data in the struct
        toReturn.isMouseOver = true;
        toReturn.point = glm::vec3(getModel() * glm::vec4(p_, 1.0f));
        return toReturn;
    } else {
        return toReturn;
    }
}