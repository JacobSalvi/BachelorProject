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
    //the internal model plus our model give us the position of the sphere
    glm::mat4 internalModel = sphereShown->getModel();
    glm::mat4 actualModel = outModel * internalModel;
    //get position and radius of the sphere
    glm::vec3 s1 = glm::vec3(actualModel[3][0], actualModel[3][1], actualModel[3][2]);
    float r1 = actualModel[0][0] / 2.0f;

    //position and radius of colliding obj
    glm::vec3 s2 = glm::vec3(obj->getModel()[3][0], obj->getModel()[3][1], obj->getModel()[3][2]);
    float r2 = obj->getModel()[0][0] / 2.0f;

    //cheating a little
    r2+=0.01;

    if (glm::length(s1 - s2) <= r1 + r2) {
        //we are intersecting the BVH, potentially intersecting the cloth
        //if we are a leaf check if we actually intersect the clot
        if (child0 == nullptr) {
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
                    i->setCollisionForce(1000.0f*(intersectionPoint-point));
                }
            }
        } else {
            //otherwise recursively check the children
            if (defSphereBVH::child0 != nullptr) defSphereBVH::child0->detectCollisionSphere(outModel, obj);
            if (defSphereBVH::child1 != nullptr) defSphereBVH::child1->detectCollisionSphere(outModel, obj);
            if (defSphereBVH::child2 != nullptr) defSphereBVH::child2->detectCollisionSphere(outModel, obj);
            if (defSphereBVH::child3 != nullptr) defSphereBVH::child3->detectCollisionSphere(outModel, obj);
        }
    }
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
    C1.y-=0.5;
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
                    glm::vec3 trick = C2;
                    trick.y+=0.1;
                    glm::vec3 intersectionPoint = pointPlaneProjection(pos, glm::vec3(0,1,0), trick);
                    i->setCollisionForce(30000.0f*(intersectionPoint-pos));
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

void defSphereBVH::detectCollisionCube(glm::mat4 outModel, collidable *obj) {
    //the internal model plus our model give us the position of the sphere
    glm::mat4 internalModel = sphereShown->getModel();
    glm::mat4 actualModel = outModel * internalModel;
    //get position and radius of the sphere
    glm::vec3 s = glm::vec3(actualModel[3][0], actualModel[3][1], actualModel[3][2]);
    float r = actualModel[0][0] / 2.0f;

    float distSquared = r * r;
    glm::vec3 C1(-0.6, -0.6, -0.6);
    glm::vec3 C2(0.6, 0.6, 0.6);
    C1 = glm::vec3(obj->getModel() * (glm::vec4(C1, 1.0)));
    C2 = glm::vec3(obj->getModel() * (glm::vec4(C2, 1.0)));

    float dmin= 0.0f;
    for(int i =0; i<3;++i){
        if(s[i]<C1[i]) {
            dmin +=(s[i] - C1[i])*(s[i]-C1[i]);
        }else if (s[i]>C2[i]){
            dmin +=(s[i]-C2[i])*(s[i]-C2[i]);
        }
    }

    if (dmin <= distSquared) {
        //cube intersect sphere
        if (child0 == nullptr) {
            //check if any particle is inside the cube
            for (auto i : p) {
                glm::vec3 pos = glm::vec3(outModel*glm::vec4(i->getPosition(), 1));
                if (C1.x <= pos.x && pos.x <= C2.x && C1.y <= pos.y && pos.y <= C2.y && C1.z <= pos.z &&
                    pos.z <= C2.z) {
                    //let's find the intersection point between the velocity of the particle and
                    //the cube

                    //front and back faces
                    glm::vec3 cNormal(0,0, 1);
                    glm::vec3 rDir = glm::normalize(-i->getVelocity());
                    glm::vec3 current=pos;
                    //checking if the vectors are parallel
                    if(glm::dot(cNormal, rDir)!=0){
                        //front face
                        glm::vec3 front = pointPlaneProjection(pos, cNormal, C2);
                        //inside face?
                        if(!(front.x<C1.x || front.x > C2.x || front.y<C1.y || front.y>C2.y)){
                            current = front;
                            glm::vec3 back= pointPlaneProjection(pos, cNormal, C1);
                            //inside face?
                            if(!(back.x < C1.x || back.x > C2.x || back.y < C1.y || back.y > C2.y)){
                                //closer?
                                if(glm::length(pos - back) < glm::length(pos - current)){
                                    current = back;
                                }
                            }
                        }
                    }

                    //left and right faces
                    cNormal = glm::vec3(1,0,0);
                    if(glm::dot(cNormal, rDir)!=0){
                        glm::vec3 left = pointPlaneProjection(pos, cNormal, C1);
                        glm::vec3 right = pointPlaneProjection(pos, cNormal, C2);
                        //inside left face?
                        if(!(left.z<C1.z || left.z > C2.z || left.y<C1.y || left.y>C2.y)) {
                            if(glm::length(pos-left)<glm::length(pos-current)){
                                current = left;
                            }
                        }
                        //inside right face
                        if(!(right.z<C1.z || right.z > C2.z || right.y<C1.y || right.y>C2.y)) {
                            if(glm::length(pos-right)<glm::length(pos-current)){
                                current = right;
                            }
                        }
                    }

                    //top and bottom faces
                    cNormal = glm::vec3(0,1,0);
                    if(glm::dot(cNormal, rDir)!=0){
                        glm::vec3 top = pointPlaneProjection(pos, cNormal, C1);
                        glm::vec3 bottom = pointPlaneProjection(pos, cNormal, C2);
                        //inside top face?
                        if(!(top.x<C1.x || top.x > C2.x || top.z<C1.z || top.z>C2.z)) {
                            if(glm::length(pos-top)<glm::length(pos-current)){
                                current = top;
                            }
                        }
                        //inside bottom face?
                        if(!(bottom.x<C1.x || bottom.x > C2.x || bottom.z<C1.z || bottom.z>C2.z)) {
                            if(glm::length(pos-bottom)<glm::length(pos-current)){
                                current = bottom;
                            }
                        }
                    }

                    //let's set the collision force for the particle
                    //since I have no indication what k should be I will wing it
                    i->setCollisionForce(10000.0f*(current-pos));
                }
            }

        } else {
            if (defSphereBVH::child0 != nullptr) defSphereBVH::child0->detectCollisionCube(outModel, obj);
            if (defSphereBVH::child1 != nullptr) defSphereBVH::child1->detectCollisionCube(outModel, obj);
            if (defSphereBVH::child2 != nullptr) defSphereBVH::child2->detectCollisionCube(outModel, obj);
            if (defSphereBVH::child3 != nullptr) defSphereBVH::child3->detectCollisionCube(outModel, obj);
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
