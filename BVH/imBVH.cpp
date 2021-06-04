
#include "imBVH.h"
#include "../utilities/helperFunctions.h"

imBVH::imBVH(std::vector<particle *> part) : BVH(part) {
    particle * p0=part[0];
    particle * p1=part[1];
    float dist = glm::length(p0->getPosition()-p1->getPosition());
    //find the 2 furthest particles
    for(int i =0; i<part.size();++i){
        for(int j=i+1; j<part.size(); ++j){
            if(glm::length(part[i]->getPosition()-part[j]->getPosition())>dist){
                dist = glm::length(part[i]->getPosition()-part[j]->getPosition());
                p0=part[i];
                p1=part[j];
            }
        }
    }
    glm::vec3 dir = glm::normalize(p1->getPosition()-p0->getPosition());
    glm::vec3 c = p[0]->getPosition()+(dist/2.0f)*dir;
    center = c;
    radius = dist/2.0f;

    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel = glm::mat4(1);
    //I discovered painstakingly that the order of operations matters
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(dist,dist,dist));
    sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0, 0, 0));
}

void imBVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame, glm::mat4 model) {
    sphereShown->render(ProjectionMatrix, ViewMatrix, programId, wireFrame, model);
}

void imBVH::detectCollisionPlane(glm::mat4 outModel, collidable *obj, float gravity) {
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

        //cube intersect sphere
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

                    //friction
                    float friction = 100.0f*i->getMass()*gravity;
                    i->setFrictionForce(friction*glm::normalize(i->getVelocity()));
                }
            }

        } else {
            if (child0 != nullptr) child0->detectCollisionPlane(outModel, obj, gravity);
            if (child1 != nullptr) child1->detectCollisionPlane(outModel, obj, gravity);
            if (child2 != nullptr) child2->detectCollisionPlane(outModel, obj, gravity);
            if (child3 != nullptr) child3->detectCollisionPlane(outModel, obj, gravity);
        }
    }
}

void imBVH::update() {
    particle * p0=p[0];
    particle * p1=p[1];
    float dist = glm::length(p0->getPosition()-p1->getPosition());
    //find the 2 furthest particles
    for(int i =0; i<p.size();++i){
        for(int j=i+1; j<p.size(); ++j){
            if(glm::length(p[i]->getPosition()-p[j]->getPosition())>dist){
                dist = glm::length(p[i]->getPosition()-p[j]->getPosition());
                p0=p[i];
                p1=p[j];
            }
        }
    }
    glm::vec3 dir = glm::normalize(p1->getPosition()-p0->getPosition());
    glm::vec3 c = p0->getPosition()+(dist/2.0f)*dir;
    center = c;
    radius = dist/2.0f;

    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel = glm::mat4(1);
    //I discovered painstakingly that the order of operations matters
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(dist,dist,dist));
    sphereShown->setModel(internalSphereModel);
}

void imBVH::detectCollisionSphere(glm::mat4 outModel, collidable *obj, float gravity) {
    //the internal model plus our model give us the position of the sphere
    glm::mat4 internalModel = sphereShown->getModel();
    glm::mat4 actualModel = outModel * internalModel;
    //get position and radius of the sphere
    glm::vec3 s1 = glm::vec3(actualModel[3][0], actualModel[3][1], actualModel[3][2]);
    float r1 = actualModel[0][0] / 2.0f;

    //position and radius of colliding obj
    glm::vec3 s2 = glm::vec3(obj->getModel()[3][0], obj->getModel()[3][1], obj->getModel()[3][2]);
    //printPoint(s2, "???");
    float r2 = obj->getModel()[0][0] / 2.0f;

    //cheating a little
    r2+=0.05;

    if (glm::length(s1 - s2) <= r1 + r2) {
        //we are intersecting the BVH, potentially intersecting the cloth
        //if we are a leaf check if we actually intersect the clot
        if (child0 == nullptr) {
            //instead of doing some complicated sphere-triangle collision detection
            //I simply check if one of the particles is inside the sphere
            for (auto i : p) {
                //get particles translation
                glm::vec3 tr = outModel[0][0]*glm::vec3(outModel[3][0], outModel[3][1], outModel[3][2]);
                glm::vec3 idk = glm::vec3(outModel*glm::vec4(i->getPosition(),1));
                if (glm::length(idk - s2) <= r2) {
                    //let's find the intersection point
                    glm::vec3 point = idk;
                    glm::vec3 direction = glm::normalize(-i->getVelocity());
                    float a = glm::dot(direction,direction);
                    float b = 2.0f*glm::dot((s2-point), direction);
                    float c = glm::dot(s2-point, s2-point)-r2*r2;
                    float t1 = (-b+sqrt(b*b-4.0f*a*c))*(2.0*a);
                    glm::vec3 intersectionPoint = point+t1*direction;

                    //let's set the collision force for the particle
                    //since I have no indication what k should be I will wing it
                    i->setCollisionForce(4000.0f*(intersectionPoint-point));
                }
            }
        } else {
            //otherwise recursively check the children
            if (child0 != nullptr) child0->detectCollisionSphere(outModel, obj, gravity);
            if (child1 != nullptr) child1->detectCollisionSphere(outModel, obj, gravity);
            if (child2 != nullptr) child2->detectCollisionSphere(outModel, obj, gravity);
            if (child3 != nullptr) child3->detectCollisionSphere(outModel, obj, gravity);
        }
    }
}