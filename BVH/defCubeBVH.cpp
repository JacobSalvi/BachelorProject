
#include "defCubeBVH.h"
#include "../utilities/helperFunctions.h"

defCubeBVH::~defCubeBVH() {}

defCubeBVH::defCubeBVH(std::vector<particle *> particles, float radius, glm::vec3 center) :BVH(particles){
    if(radius==sqrt(3.0f/4.0f)){
        //compute centers for the 6 sub spheres
        //basically one sub sphere for each face
        //0 1 0 - 0.5 0.75 0.5 = 1/4+1/8+1/4
        glm::vec3 c0 = glm::vec3(0.5f, 0.0f ,0.5f);
        glm::vec3 c1 = glm::vec3(0.5f, 1.0f ,0.5f);
        glm::vec3 c2 = glm::vec3(0.0f, 0.5f ,0.5f);
        glm::vec3 c3 = glm::vec3(1.0f, 0.5f ,0.5f);
        glm::vec3 c4 = glm::vec3(0.5f, 0.5f ,0.0f);
        glm::vec3 c5 = glm::vec3(0.5f, 0.5f ,1.0f);

        // radius
        float r0 = sqrt(1.0f/2.0f);

        //compute the sub vectors for the sub spheres
        std::vector<particle *> v0;
        std::vector<particle *> v1;
        std::vector<particle *> v2;
        std::vector<particle *> v3;
        std::vector<particle *> v4;
        std::vector<particle *> v5;

        for(int i=0; i<particles.size();++i){
            //each sub sphere is responsible for one face of the cube
            glm::vec3 tmp = particles[i]->getPosition();
            if(tmp.y==0.0f){
                v0.push_back(particles[i]);
            }
            if(tmp.y==1.0f){
                v1.push_back(particles[i]);
            }
            if(tmp.x==0.0f){
                v2.push_back(particles[i]);
            }
            if(tmp.x==1.0f){
                v3.push_back(particles[i]);
            }
            if(tmp.z==0.0f){
                v4.push_back(particles[i]);
            }
            if(tmp.z==1.0f){
                v5.push_back(particles[i]);
            }
        }
        child0 = new defCubeBVH(v0, r0, c0);
        child1 = new defCubeBVH(v1, r0, c1);
        child2 = new defCubeBVH(v2, r0, c2);
        child3 = new defCubeBVH(v3, r0, c3);
        child4 = new defCubeBVH(v4, r0, c4);
        child5 = new defCubeBVH(v5, r0, c5);
    }else{
        defCubeBVH::bl=particles[0];
        defCubeBVH::tl=particles[sqrt(particles.size())-1];
        defCubeBVH::br=particles[particles.size()-sqrt(particles.size())];
        defCubeBVH::tr=particles[particles.size()-1];
    }
    glm::mat4 internalSphereModel(1);
    internalSphereModel = glm::translate(internalSphereModel, center);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0f*radius,2.0f*radius,2.0f*radius));
    defCubeBVH::sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0, 0, 0));
}

void defCubeBVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame,
                        glm::mat4 model) {
    sphereShown->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    if (child0 != nullptr) {
        child0->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    }
    if (child1 != nullptr) {
        child1->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    }
    if (child2 != nullptr) {
        child2->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    }
    if (child3 != nullptr) {
        child3->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    }
    if (child4 != nullptr) {
        child4->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    }
    if (child5 != nullptr) {
        child5->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
    }
}

bool pointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    //divide the face in two triangles
    //first triangle is V1 V2 V4
    glm::vec3 n=glm::cross((b-a),(c-a));
    //calculate the 3 signed area for the first triangle
    glm::vec3 n1= glm::cross((b-p), (c-p));
    glm::vec3 n2= glm::cross((c-p), (a-p));
    glm::vec3 n3= glm::cross((a-p), (b-p));
    //check if the intersection point lays inside the first triangle
    if(glm::dot(n1,n)>=0&&glm::dot(n2,n)>=0&&glm::dot(n3,n)>=0){
        return true;
    }
    return false;
}

helperStruct defCubeBVH::rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) {
    //to make the compiler happy while I code
    helperStruct toReturn;
    toReturn.isMouseOver = false;

    //do we intersect the sphere?
    mouseIntersectStruct possibleIntersection = defCubeBVH::sphereShown->isHovered(origin, direction, model);
    if (possibleIntersection.isMouseOver) {
        //are we a leaf?
        if (child0 == nullptr) {
            //compute possible intersection point
            glm::vec3 norm = glm::normalize(glm::cross(br->getPosition()-bl->getPosition(), tr->getPosition()-bl->getPosition()));
            glm::vec3 possibleIntersectionPoint = planeVectorIntersection(origin, direction, norm, bl->getPosition());

            if(pointInTriangle(possibleIntersectionPoint, bl->getPosition(), br->getPosition(), tl->getPosition())){
                //find closest particle
                toReturn.point=bl;
                if(glm::length(possibleIntersectionPoint-br->getPosition())<glm::length(possibleIntersectionPoint-bl->getPosition())){
                    toReturn.point=br;
                }
                if(glm::length(possibleIntersectionPoint-tl->getPosition())<glm::length(possibleIntersectionPoint-toReturn.point->getPosition())){
                    toReturn.point=tl;
                }
                toReturn.isMouseOver=true;
                return toReturn;
            }
            if(pointInTriangle(possibleIntersectionPoint, br->getPosition(), tr->getPosition(), tl->getPosition())){
                toReturn.point=br;
                if(glm::length(possibleIntersectionPoint-tr->getPosition())<glm::length(possibleIntersectionPoint-toReturn.point->getPosition())){
                    toReturn.point=tr;
                }
                if(glm::length(possibleIntersectionPoint-tl->getPosition())<glm::length(possibleIntersectionPoint-toReturn.point->getPosition())){
                    toReturn.point=tl;
                }
                toReturn.isMouseOver=true;
                return toReturn;
            }
        } else {
            std::vector<helperStruct> tmpIt;
            helperStruct  tmp0 = child0->rayIntersect(origin, direction, model);
            helperStruct  tmp1 = child1->rayIntersect(origin, direction, model);
            helperStruct  tmp2 = child2->rayIntersect(origin, direction, model);
            helperStruct  tmp3 = child3->rayIntersect(origin, direction, model);
            helperStruct  tmp4 = child4->rayIntersect(origin, direction, model);
            helperStruct  tmp5 = child5->rayIntersect(origin, direction, model);
            tmpIt.push_back(tmp0);
            tmpIt.push_back(tmp1);
            tmpIt.push_back(tmp2);
            tmpIt.push_back(tmp3);
            tmpIt.push_back(tmp4);
            tmpIt.push_back(tmp5);
            for(int i=0; i<6;++i){
                if(!toReturn.isMouseOver){
                    toReturn.isMouseOver=tmpIt[i].isMouseOver;
                    toReturn.point=tmpIt[i].point;
                }else{
                    if(tmpIt[i].isMouseOver){
                        if(glm::length(origin-tmpIt[i].point->getPosition())<glm::length(origin-toReturn.point->getPosition())){
                            toReturn.point=tmpIt[i].point;
                        }
                    }
                }
            }
        }
    }
    return toReturn;
}

void defCubeBVH::update() {
    //find the two point that are the furthest apart
    particle *p1;
    particle *p2;
    float furthestDistance = -1.0f;
    for (int i = 0; i < defCubeBVH::p.size(); ++i) {
        for (int j = i + 1; j < defCubeBVH::p.size(); ++j) {
            particle *first = defCubeBVH::p[i];
            particle *second = defCubeBVH::p[j];
            float tmp = glm::length(first->getPosition() - second->getPosition());
            if (tmp > furthestDistance) {
                furthestDistance = tmp;
                p1 = first;
                p2 = second;
            }
        }
    }

    //direction of the vector
    glm::vec3 direction = p2->getPosition() - p1->getPosition();

    //new sphere radius
    float r = glm::length(direction);
    r /= 2.0f;
    defCubeBVH::radius = r;
    //new sphere center
    glm::vec3 c = p1->getPosition() + r * glm::normalize(direction);

    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel = glm::mat4(1);
    //I discovered painstakingly that the order of operations matters
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0 * r, 2.0 * r, 2.0 * r));
    defCubeBVH::sphereShown->setModel(internalSphereModel);

    //update children
    if (defCubeBVH::child0 != nullptr) defCubeBVH::child0->update();
    if (defCubeBVH::child1 != nullptr) defCubeBVH::child1->update();
    if (defCubeBVH::child2 != nullptr) defCubeBVH::child2->update();
    if (defCubeBVH::child3 != nullptr) defCubeBVH::child3->update();
    if (defCubeBVH::child4 != nullptr) defCubeBVH::child4->update();
    if (defCubeBVH::child5 != nullptr) defCubeBVH::child5->update();
}

void defCubeBVH::detectCollisionSphere(glm::mat4 outModel, collidable *obj) {
    BVH::detectCollisionSphere(outModel, obj);
}

void defCubeBVH::detectCollisionCube(glm::mat4 outModel, collidable *obj) {
    BVH::detectCollisionCube(outModel, obj);
}

void defCubeBVH::detectCollisionPlane(glm::mat4 outModel, collidable *obj) {
    BVH::detectCollisionPlane(outModel, obj);
}
