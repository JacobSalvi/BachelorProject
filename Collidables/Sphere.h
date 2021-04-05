#ifndef CODE_SPHERE_H
#define CODE_SPHERE_H


#include <glm/glm.hpp>
#include "collidable.h"
#include <iostream>
#include <cmath>

class sphere : public collidable{
private:
    int size;
public:
    sphere(int size, glm::mat4 model, glm::vec3 colour);

    int getSize() override;

    int getNumberOfVertices() override;

    void getName() override{
        std::cout<<"sphere"<<std::endl;
    }

    mouseIntersectStruct isHovered(glm::vec3 origin, glm::vec3 direction) override{
        //return struct
        mouseIntersectStruct toReturn;
        toReturn.isMouseOver=false;

        //i don't even know
        glm::mat4 whatEven = getModel();

        //sphere center
        glm::vec3 center(whatEven[3][0], whatEven[3][1], whatEven[3][2]);

        //transformation into local coordinate system
        glm::mat4 M_ = glm::inverse(whatEven);
        glm::vec4 orHelper = M_*glm::vec4(origin,1.0f);
        glm::vec3 origin_(orHelper.x, orHelper.y, orHelper.z);
        glm::vec4 dirHelper = M_*glm::vec4(direction, 0.0f);
        glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
        d_=glm::normalize(d_);

        float t = -glm::dot(origin_, d_);

        float d2 = glm::dot(origin_,origin_)-t*t;


        if(d2>1.0f){
            //no intersection
            std::cout<<"no sphere intersected"<<std::endl;
            return toReturn;
        }else{
            //intersection
            std::cout<<"sphere intersected"<<std::endl;
            float dt = sqrt(1.0-d2);
            float t1 = t-dt;

            //intersected point on the sphere
            glm::vec3 p_ = origin_+t1*d_;
            std::cout<<p_.x<<" "<<p_.y<<" "<<p_.z<<std::endl;

            //savings the intersection data in the struct
            toReturn.isMouseOver=true;
            toReturn.point=glm::vec3(whatEven*glm::vec4(p_, 1.0f));

            return toReturn;
        }
    };
};


#endif
