#ifndef CODE_PLANE_H
#define CODE_PLANE_H


#include "collidable.h"
#include <iostream>

class plane : public collidable {
public:
    plane(float width, float depth, glm::mat4 model, glm::vec3 colour);

    int getSize() override;

    int getNumberOfVertices() override;

    void getName() override{
        std::cout<<"plane"<<std::endl;
    }

    mouseIntersectStruct isHovered(glm::vec3 origin, glm::vec3 direction) override{
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
};


#endif //CODE_PLANE_H
