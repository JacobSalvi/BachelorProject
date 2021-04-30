#include "defSphereBVH.h"
#include "../utilities/helperFunctions.h"

defSphereBVH::defSphereBVH(std::vector<particle *> particles, float radius, glm::vec3 center) : BVH(particles) {
    if(radius==0.5){
        //compute centers for the 4 sub spheres
        glm::vec3 c0 = glm::vec3(-0.25f, 0.0f ,0.0f);
        glm::vec3 c1 = glm::vec3(0.25f, 0.0f,0.0f);
        glm::vec3 c2 =  glm::vec3(0.0f, 0.25f,0.0f);
        glm::vec3 c3= glm::vec3(0.0f, -0.25f, 0.0f);

        // radius
        float r0 = 0.3f;

        //compute the sub vectors for the sub spheres
        std::vector<particle *> v0;
        std::vector<particle *> v1;
        std::vector<particle *> v2;
        std::vector<particle *> v3;

        for(int i=0; i<particles.size();++i){
            //are the particles inside the first sub sphere?
            if(glm::length(particles[i]->getPosition()-c0)<=r0){
                v0.push_back(particles[i]);
            }
            //and the second?
            if(glm::length(particles[i]->getPosition()-c1)<=r0){
                v1.push_back(particles[i]);
            }
            //and the third?
            if(glm::length(particles[i]->getPosition()-c2)<=r0){
                v2.push_back(particles[i]);
            }
            //and the last?
            if(glm::length(particles[i]->getPosition()-c3)<=r0){
                v3.push_back(particles[i]);
            }
        }

        child0 = new defSphereBVH(v0, r0, c0);
        child1 = new defSphereBVH(v1, r0, c1);
        child2 = new defSphereBVH(v2, r0, c2);
        child3 = new defSphereBVH(v3, r0, c3);
    }
    glm::mat4 internalSphereModel(1);
    internalSphereModel = glm::translate(internalSphereModel, center);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0f*radius,2.0f*radius,2.0f*radius));
    defSphereBVH::sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0, 0, 0));
}

void defSphereBVH::update() {
    //find the two point that are the furthest apart
    particle *p1;
    particle *p2;
    float furthestDistance = -1.0f;
    for (int i = 0; i < defSphereBVH::p.size(); ++i) {
        for (int j = i + 1; j < defSphereBVH::p.size(); ++j) {
            particle *first = defSphereBVH::p[i];
            particle *second = defSphereBVH::p[j];
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
    defSphereBVH::radius = r;
    //new sphere center
    glm::vec3 c = p1->getPosition() + r * glm::normalize(direction);

    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel = glm::mat4(1);
    //I discovered painstakingly that the order of operations matters
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0 * r, 2.0 * r, 2.0 * r));
    defSphereBVH::sphereShown->setModel(internalSphereModel);

    //update children
    if (defSphereBVH::child0 != nullptr) defSphereBVH::child0->update();
    if (defSphereBVH::child1 != nullptr) defSphereBVH::child1->update();
    if (defSphereBVH::child2 != nullptr) defSphereBVH::child2->update();
    if (defSphereBVH::child3 != nullptr) defSphereBVH::child3->update();
}

void defSphereBVH::detectCollisionSphere(glm::mat4 outModel, collidable *obj) {
    BVH::detectCollisionSphere(outModel, obj);
}

void defSphereBVH::detectCollisionCube(glm::mat4 outModel, collidable *obj) {
    BVH::detectCollisionCube(outModel, obj);
}

void defSphereBVH::detectCollisionPlane(glm::mat4 outModel, collidable *obj) {
    //the internal model plus our model give us the position of the sphere
    glm::mat4 internalModel = sphereShown->getModel();
    glm::mat4 actualModel = outModel * internalModel;
    //get position and radius of the sphere
    glm::vec3 s = glm::vec3(actualModel[3][0], actualModel[3][1], actualModel[3][2]);
    float r = actualModel[0][0] / 2.0f;

    float spherePlaneDistance;
    float width = obj->getVertexBuffer()[6];
    float depth = obj->getVertexBuffer()[8];
    glm::vec3 C1(-width, 0.0, -depth);
    glm::vec3 C2(width, 0.0, depth);
    C1 = glm::vec3(obj->getModel() * (glm::vec4(C1, 1.0)));
    C1.y-=.1;
    C2 = glm::vec3(obj->getModel() * (glm::vec4(C2, 1.0)));

    spherePlaneDistance = glm::dot((s-C2), glm::vec3(0.0,1.0,0.0));

    if (-r<spherePlaneDistance && spherePlaneDistance<r) {
        //plane sphere intersect
        if (child0 == nullptr) {
            //check if any particle is inside the plane
            for (auto i : p) {
                glm::vec3 pos = glm::vec3(outModel*glm::vec4(i->getPosition(), 1));
                if (C1.x <= pos.x && pos.x <= C2.x && C1.y <= pos.y && pos.y <= C2.y && C1.z <= pos.z &&
                    pos.z <= C2.z) {
                    //response for the collision
                    //glm::vec3 intersectionPoint = planeVectorIntersection(pos, glm::normalize(-i->getVelocity()), glm::vec3(0,1,0), C2);
                    //little trick for better visualization
                    //basically the object will be slightly above the plane
                    glm::vec3 trick = C2;
                    trick.y+=0.1;
                    glm::vec3 intersectionPoint = pointPlaneProjection(pos, glm::vec3(0,1,0), trick);
                    i->setCollisionForce(3000.0f*(intersectionPoint-pos));
                }
            }

        } else {
            if (defSphereBVH::child0 != nullptr) defSphereBVH::child0->detectCollisionPlane(outModel, obj);
            if (defSphereBVH::child1 != nullptr) defSphereBVH::child1->detectCollisionPlane(outModel, obj);
            if (defSphereBVH::child2 != nullptr) defSphereBVH::child2->detectCollisionPlane(outModel, obj);
            if (defSphereBVH::child3 != nullptr) defSphereBVH::child3->detectCollisionPlane(outModel, obj);
        }
    }
}

helperStruct defSphereBVH::rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) {
    //to make the compiler happy while I code
    helperStruct toReturn;
    toReturn.isMouseOver = false;

    //do we intersect the sphere?
    mouseIntersectStruct possibleIntersection = defSphereBVH::sphereShown->isHovered(origin, direction, model);
    if (possibleIntersection.isMouseOver) {
        //are we a leaf?
        if (child0 == nullptr) {
            float closestDistance= 1000.0f;
            particle * closest;
            for(int i=0; i<defSphereBVH::p.size();++i){
                if(glm::length(possibleIntersection.point-defSphereBVH::p[i]->getPosition())<closestDistance){
                    closestDistance=glm::length(possibleIntersection.point-defSphereBVH::p[i]->getPosition());
                    closest=defSphereBVH::p[i];
                }
            }
            toReturn.point=closest;
            toReturn.isMouseOver=true;
        } else {
            //recursive call on children
            helperStruct tmp = child0->rayIntersect(origin, direction, model);
            if (tmp.isMouseOver) {
                return tmp;
            }
            if (child1 != nullptr) {
                tmp = child1->rayIntersect(origin, direction, model);
                if (tmp.isMouseOver) {
                    return tmp;
                }
            }
            if (child2 != nullptr) {
                tmp = child2->rayIntersect(origin, direction, model);
                if (tmp.isMouseOver) {
                    return tmp;
                }
            }
            if (child3 != nullptr) {
                tmp = child3->rayIntersect(origin, direction, model);
                if (tmp.isMouseOver) {
                    return tmp;
                }
            }
        }
    }
    return toReturn;
}

void defSphereBVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame,
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
}
