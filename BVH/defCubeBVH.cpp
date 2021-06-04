
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
            glm::vec3 bottomRight=glm::vec3(model*glm::vec4(br->getPosition(),1.0));
            glm::vec3 bottomLeft=glm::vec3(model*glm::vec4(bl->getPosition(),1.0));
            glm::vec3 topRight=glm::vec3(model*glm::vec4(tr->getPosition(),1.0));
            glm::vec3 topLeft=glm::vec3(model*glm::vec4(tl->getPosition(),1.0));

            glm::vec3 norm = glm::normalize(glm::cross(bottomRight-bottomLeft, topRight-bottomLeft));
            glm::vec3 possibleIntersectionPoint = planeVectorIntersection(origin, direction, norm, bottomLeft);

            if(pointInTriangle(possibleIntersectionPoint, bottomLeft, bottomRight, topLeft)){
                //find closest particle
                //fix this
                particle * closest= p[0];
                for(int i=1; i<p.size();++i){
                    glm::vec3 candidate = glm::vec3(model*glm::vec4(p[i]->getPosition(),1.0));
                    glm::vec3 closestPos = glm::vec3(model*glm::vec4(closest->getPosition(),1.0));
                    if(glm::length(possibleIntersectionPoint-candidate)<glm::length(possibleIntersectionPoint-closestPos)){
                        closest=p[i];
                    }
                }
                toReturn.isMouseOver=true;
                toReturn.point=closest;
                return toReturn;
            }
            if(pointInTriangle(possibleIntersectionPoint, bottomRight, topRight, topLeft)){
                particle * closest= p[0];
                for(int i=1; i<p.size();++i){
                    glm::vec3 candidate = glm::vec3(model*glm::vec4(p[i]->getPosition(),1.0));
                    glm::vec3 closestPos = glm::vec3(model*glm::vec4(closest->getPosition(),1.0));
                    if(glm::length(possibleIntersectionPoint-candidate)<glm::length(possibleIntersectionPoint-closestPos)){
                        closest=p[i];
                    }
                }
                toReturn.isMouseOver=true;
                toReturn.point=closest;
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

void defCubeBVH::detectCollisionSphere(glm::mat4 outModel, collidable *obj, float gravity) {
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
                    //i->setCollisionForce(1000.0f*(intersectionPoint-point));

                    //friction
                    float cosA = glm::dot(glm::vec3(0,gravity,0), s1-intersectionPoint);
                    float friction = 10.0f*i->getMass()*gravity*cosA;
                    //i->setFrictionForce(friction*glm::normalize(i->getVelocity()));
                    // collision + friction force
                    i->setCollisionForce(1000.0f*(intersectionPoint-point)+friction*glm::normalize(i->getVelocity()));
                }
            }
        } else {
            //otherwise recursively check the children
            if (defCubeBVH::child0 != nullptr) defCubeBVH::child0->detectCollisionSphere(outModel, obj, gravity);
            if (defCubeBVH::child1 != nullptr) defCubeBVH::child1->detectCollisionSphere(outModel, obj, gravity);
            if (defCubeBVH::child2 != nullptr) defCubeBVH::child2->detectCollisionSphere(outModel, obj, gravity);
            if (defCubeBVH::child3 != nullptr) defCubeBVH::child3->detectCollisionSphere(outModel, obj, gravity);
            if (defCubeBVH::child4 != nullptr) defCubeBVH::child4->detectCollisionSphere(outModel, obj, gravity);
            if (defCubeBVH::child5 != nullptr) defCubeBVH::child5->detectCollisionSphere(outModel, obj, gravity);
        }
    }
}

void defCubeBVH::detectCollisionCube(glm::mat4 outModel, collidable *obj, float gravity) {
    //the internal model plus our model give us the position of the sphere
    glm::mat4 internalModel = sphereShown->getModel();
    glm::mat4 actualModel = outModel * internalModel;
    //get position and radius of the sphere
    glm::vec3 s = glm::vec3(actualModel[3][0], actualModel[3][1], actualModel[3][2]);
    float r = actualModel[0][0] / 2.0f;

    float distSquared = r * r;
    //trick
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
                                    cNormal=-cNormal;
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
                                cNormal=-cNormal;
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
                                cNormal=-cNormal;
                            }
                        }
                    }

                    //let's set the collision force for the particle
                    //since I have no indication what k should be I will wing it
                    //i->setCollisionForce(10000.0f*(current-pos));

                    //friction
                    float friction = 10.0f*i->getMass()*gravity;
                    //i->setFrictionForce(friction*glm::normalize(i->getVelocity()));

                    //collision + frictio
                    i->setCollisionForce(10000.0f*(current-pos)+friction*glm::normalize(i->getVelocity()));
                }
            }

        } else {
            if (defCubeBVH::child0 != nullptr) defCubeBVH::child0->detectCollisionCube(outModel, obj, gravity);
            if (defCubeBVH::child1 != nullptr) defCubeBVH::child1->detectCollisionCube(outModel, obj, gravity);
            if (defCubeBVH::child2 != nullptr) defCubeBVH::child2->detectCollisionCube(outModel, obj, gravity);
            if (defCubeBVH::child3 != nullptr) defCubeBVH::child3->detectCollisionCube(outModel, obj, gravity);
            if (defCubeBVH::child4 != nullptr) defCubeBVH::child4->detectCollisionCube(outModel, obj, gravity);
            if (defCubeBVH::child5 != nullptr) defCubeBVH::child5->detectCollisionCube(outModel, obj, gravity);
        }
    }
}

void defCubeBVH::detectCollisionPlane(glm::mat4 outModel, collidable *obj, float gravity) {
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
    C1.y-=0.5f;
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
                    //i->setCollisionForce(10000.0f*(intersectionPoint-pos));

                    //friction
                    float friction = 10.0f*i->getMass()*gravity;
                    //i->setFrictionForce(friction*glm::normalize(i->getVelocity()));

                    //friction + collision
                    i->setCollisionForce(10000.0f*(intersectionPoint-pos)+friction*glm::normalize(i->getVelocity()));
                }
            }

        } else {
            if (defCubeBVH::child0 != nullptr) defCubeBVH::child0->detectCollisionPlane(outModel, obj, gravity);
            if (defCubeBVH::child1 != nullptr) defCubeBVH::child1->detectCollisionPlane(outModel, obj, gravity);
            if (defCubeBVH::child2 != nullptr) defCubeBVH::child2->detectCollisionPlane(outModel, obj, gravity);
            if (defCubeBVH::child3 != nullptr) defCubeBVH::child3->detectCollisionPlane(outModel, obj, gravity);
            if (defCubeBVH::child4 != nullptr) defCubeBVH::child4->detectCollisionPlane(outModel, obj, gravity);
            if (defCubeBVH::child5 != nullptr) defCubeBVH::child5->detectCollisionPlane(outModel, obj, gravity);
        }
    }
}
