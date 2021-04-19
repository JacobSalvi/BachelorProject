#include "plane.h"
#include <iostream>

int plane::getSize() {
    return sizeof(float)*18;
}

int plane::getNumberOfVertices() {
    return 6;
}

plane::plane(float width, float depth, glm::mat4 model, glm::vec3 colour,  glm::vec3 lPos) : collidable(colour, model, lPos){

    vertexBuffer = new float [18];
    colorBuffer = new float[18];
    normalBuffer = new float[18];

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

    //normals
    //the plane is an horizontal plane
    for(int i=0; i<18;++i){
        if(i%3==1){
            normalBuffer[i]=1.0f;
        }else{
            normalBuffer[i]=0.0f;
        }
    }
}

mouseIntersectStruct plane::isHovered(glm::vec3 origin, glm::vec3 direction) {
    {
        //struct to return
        mouseIntersectStruct toReturn;
        toReturn.isMouseOver=false;

        glm::mat4 M_ = glm::inverse(getModel());
        glm::vec4 orHelper = M_*glm::vec4(origin,1.0f);
        glm::vec3 origin_(orHelper.x, orHelper.y, orHelper.z);
        glm::vec4 dirHelper = M_*glm::vec4(direction, 0.0f);
        glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
        d_=glm::normalize(d_);

        glm::vec3 p_;   // intersection point
        glm::vec3 n_;   // normal at intersection point
        float t;     // ray parameter of intersection point

        bool intersect = false;

        // intersection with the {y=0}-plane
        if ( d_.y != 0.0 ) {
            t = -origin_.y / d_.y;
            if ( t > 0.0 ) {
                p_ = origin_ + t * d_;
                if ( p_.x >= -3.0 && p_.x <= 3.0 && p_.z >= -1.0 && p_.z <= 1.0)
                {
                    // set the normal, depending on the viewing direction
                    if ( d_.y > 0.0 )
                        n_ = glm::vec3(0.0, -1.0, 0.0);
                    else
                        n_ = glm::vec3(0.0, 1.0, 0.0);

                    intersect = true;
                }
            }
        }

        if (intersect){
            toReturn.isMouseOver=true;
            toReturn.point=glm::vec3(getModel()*glm::vec4(p_,1.0f));
            return toReturn;
        }else{
            return toReturn;
        }
    }
}

int plane::returnType() {
    return 2;
}
