//this file needs a better name, I know

#include "helperFunctions.h"

//new culture
void culture(std::vector<deformableObjects *> *list, GLuint texture, GLuint textureId, glm::mat4 mod, glm::vec3 lPos, GLuint pID){
    texturedCloth * tmp = new texturedCloth(1.0f, texture, textureId, glm::vec3(2.0f,0.0f,0.0f), -1.0f, mod, lPos, pID);
    list->push_back(tmp);
}

void addCloth(std::vector<deformableObjects *> *list, int col, int row, int in, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos){
    net * clothNew = new net(0.2f, col, row, in, colour, -1.0f, mod, lPos);
    list->push_back(clothNew);
}

void addDefSphere(std::vector<deformableObjects *> *list, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos){
    deformableSphere * newSphere = new deformableSphere(mod, colour, lPos);
    list->push_back(newSphere);
}

void addDefCube(std::vector<deformableObjects *> *list, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos){
    deformableCube * newCube = new deformableCube(mod, colour, lPos);
    list->push_back(newCube);
}


//create and add sphere
//type 0->sphere, 1->cube, 2 plane
void addColl(std::vector<collidable *> * list, int type, glm::vec3 lPos, glm::vec3 tr){
    collidable * coll;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, tr);
    switch(type){
        //sphere
        case 0:
            //model = glm::translate(model, glm::vec3(1.0f,1.0f,-1.0f));
            //model = glm::translate(model, glm::vec3(-3.0f,1.0f,0.0f));
            coll = new sphere(10, model,glm::vec3(1.0f,0.2f,0.2f), lPos);
            break;
            //cube
        case 1:
            //model = glm::translate(model, glm::vec3(-3.0f,1.0f,0.0f));
            coll = new cube(model,glm::vec3(1.0f,1.0f,0.4f), lPos);
            break;
            //plane
        case 2:
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,-2.0f,0.0f));
            glm::mat4 modelP = glm::scale(translate, glm::vec3(5.0f,5.0f,5.0f));
            coll = new plane(3.0f,1.0f, modelP, glm::vec3(0.4f,1.0f,0.6f), lPos);
            break;
    }

    list->push_back(coll);
}

//dragging
mouseIntersectStruct isMouseOverColl(glm::vec3 origin, glm::vec3 direction, std::vector<collidable *> * list){
    mouseIntersectStruct closestObject;
    closestObject.isMouseOver=false;
    for(auto i : *list){
        mouseIntersectStruct tmp = i->isHovered(origin, direction);
        //check whether we intersect the object
        if(tmp.isMouseOver){
            if(!closestObject.isMouseOver){
                closestObject = tmp;
                closestObject.object=i;
            }else{
                //check whether this object is closer than the last object we intersected
                if(glm::length(origin-tmp.point)<glm::length(origin-closestObject.point)){
                    closestObject=tmp;
                    closestObject.object=i;
                }
            }
        }
    }
    return closestObject;
}

helperStruct isMouseOverDeformable(glm::vec3 origin, glm::vec3 direction, std::vector<deformableObjects *> * list){
    helperStruct closestObject;
    closestObject.isMouseOver=false;
    for(auto i : *list){
        helperStruct tmp = i->isHovered(origin, direction);
        if(tmp.isMouseOver){
            closestObject.isMouseOver=true;
            closestObject.obj=i;
            closestObject.point=tmp.point;
        }
    }
    return closestObject;
}

//compute the intersection with the plane
glm::vec3 planeVectorIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeNormal, glm::vec3 planePoint){
    float t = glm::dot(planePoint-rayOrigin,planeNormal)/glm::dot(rayDirection,planeNormal);
    return rayOrigin+t*rayDirection;
}

//compute projection of point onto plane
glm::vec3 pointPlaneProjection(glm::vec3 point, glm::vec3 planeNormal, glm::vec3 planePoint){
    return point - glm::dot(point - planePoint, planeNormal) * planeNormal;
}

bool vectorContains(std::vector<particle *> v, particle * e){
    bool toReturn=false;
    for(auto p : v){
        if(p==e){
            toReturn=true;
            break;
        }
    }
    return toReturn;
}

void printPoint(glm::vec3 point, char * name){
    std::cout<<name<<std::endl;
    std::cout<<point[0]<<" "<<point[1]<<" "<<point[2]<<std::endl;
}

//to do various stuff
void setWind(deformableObjects * net, float wind[]){
    //net->addForce(glm::vec3(0.0f,0.0f, -1.0f));
    net->setWind(glm::vec3(wind[0],wind[1], wind[2]));
}