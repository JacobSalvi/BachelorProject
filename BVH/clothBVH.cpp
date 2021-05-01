#include <glm/gtc/matrix_transform.hpp>
#include "clothBVH.h"
#include "../utilities/helperFunctions.h"


clothBVH::clothBVH(std::vector<particle *> particles, int row) : BVH(particles) {
    //needed information
    glm::vec3 p0 = particles[0]->getPosition();
    glm::vec3 direction = particles[particles.size() - 1]->getPosition() - p0;

    //get radius
    float r = glm::length(direction);
    r /= 2.0f;
    clothBVH::radius = r;
    //get center
    glm::vec3 c = p0 + r * glm::normalize(direction);

    //no need to divide further
    //create sphere and return
    if (particles.size() == 4) {
        std::swap(p[2], p[3]);

        //sphere and model matrix for the sphere
        glm::mat4 internalSphereModel = glm::mat4(1);
        //I discovered painstakingly that the order of operations matters
        internalSphereModel = glm::translate(internalSphereModel, c);
        internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0 * r, 2.0 * r, 2.0 * r));
        clothBVH::sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0, 0, 0));
        return;
    }

    //create subSpheres
    int col = (int) particles.size() / row;
    std::vector<particle *> v0;
    std::vector<particle *> v1;
    std::vector<particle *> v2;
    std::vector<particle *> v3;
    int childRow;

    //there probably is a less stupid way to do this, but I can't figure out how
    //vertical structure
    if (col == 2) {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (row % 2 == 0) {
                    if (i <= row / 2) {
                        v0.push_back(particles[i * col + j]);
                    }
                    if (i >= row / 2 - 1) {
                        v1.push_back(particles[i * col + j]);
                    }
                } else {
                    if (i <= row / 2) {
                        v0.push_back(particles[i * col + j]);
                    }
                    if (i >= row / 2) {
                        v1.push_back(particles[i * col + j]);
                    }
                }
            }
        }
        childRow = v0.size() / 2;
    } else if (row == 2) {
        //horizontal structure
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (col % 2 == 0) {
                    if (j <= col / 2) {
                        v0.push_back(particles[i * col + j]);
                    }
                    if (j >= col / 2 - 1) {
                        v1.push_back(particles[i * col + j]);
                    }
                } else {
                    if (j <= col / 2) {
                        v0.push_back(particles[i * col + j]);
                    }
                    if (j >= col / 2) {
                        v1.push_back(particles[i * col + j]);
                    }
                }
            }
        }
        childRow = 2;
    } else {
        //number of rows for children
        childRow = row / 2 + 1;
        //non thin structure
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (col % 2 == 0) {
                    if (row % 2 == 0) {
                        //even col and row
                        //checking
                        if (j <= col / 2 && i <= row / 2) {
                            v0.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 - 1 && i <= row / 2) {
                            v1.push_back(particles[i * col + j]);
                        }
                        if (j <= col / 2 && i >= row / 2 - 1) {
                            v2.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 - 1 && i >= row / 2 - 1) {
                            v3.push_back(particles[i * col + j]);
                        }
                    } else {
                        //even col odd row
                        //should be correct
                        if (j <= col / 2 && i <= row / 2) {
                            v0.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 - 1 && i <= row / 2) {
                            v1.push_back(particles[i * col + j]);
                        }
                        if (j <= col / 2 && i >= row / 2) {
                            v2.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 - 1 && i >= row / 2) {
                            v3.push_back(particles[i * col + j]);
                        }
                    }
                } else {
                    //col odd row even
                    //should be correct
                    if (row % 2 == 0) {
                        if (j <= col / 2 && i <= row / 2) {
                            v0.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 && i <= row / 2) {
                            v1.push_back(particles[i * col + j]);
                        }
                        if (j <= col / 2 && i >= row / 2 - 1) {
                            v2.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 && i >= row / 2 - 1) {
                            v3.push_back(particles[i * col + j]);
                        }
                    } else {
                        //col and row odd
                        //it works
                        if (j <= col / 2 && i <= row / 2) {
                            v0.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 && i <= row / 2) {
                            v1.push_back(particles[i * col + j]);
                        }
                        if (j <= col / 2 && i >= row / 2) {
                            v2.push_back(particles[i * col + j]);
                        }
                        if (j >= col / 2 && i >= row / 2) {
                            v3.push_back(particles[i * col + j]);
                        }
                    }
                }
            }
        }
    }

    //recursively make children
    child0 = !v0.empty() ? new clothBVH(v0, childRow) : nullptr;
    child1 = !v1.empty() ? new clothBVH(v1, childRow) : nullptr;
    child2 = !v2.empty() ? new clothBVH(v2, childRow) : nullptr;
    child3 = !v3.empty() ? new clothBVH(v3, childRow) : nullptr;

    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel = glm::mat4(1);
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0 * r, 2.0 * r, 2.0 * r));

    clothBVH::sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(-4, 10, 5));

}

void clothBVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programId, bool wireFrame,
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

helperStruct clothBVH::rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) {
    //to make the compiler happy while I code
    helperStruct toReturn;
    toReturn.isMouseOver = false;

    //do we intersect the sphere?
    if (clothBVH::sphereShown->rayIntersect(origin, direction, model)) {
        //are we a leaf?
        if (child0 == nullptr) {
            //must invert the model matrix
            glm::mat4 M_ = glm::inverse(model);
            glm::vec4 orHelper = M_ * glm::vec4(origin, 1.0f);
            glm::vec3 o_(orHelper.x, orHelper.y, orHelper.z);
            glm::vec4 dirHelper = M_ * glm::vec4(direction, 0.0f);
            glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
            d_ = glm::normalize(d_);


            //do we intersect the part of the net inside the sphere?
            //find normal
            glm::vec3 normal = glm::cross(p[1]->getPosition() - p[0]->getPosition(),
                                          p[3]->getPosition() - p[0]->getPosition());
            normal = glm::normalize(normal);

            //parallel, no intersection
            if (glm::dot(d_, normal) == 0) {
                return toReturn;
            }

            //point plane intersection
            glm::vec3 diff = o_ - p[0]->getPosition();
            float prod1 = glm::dot(diff, normal);
            float prod2 = glm::dot(d_, normal);
            float prod3 = prod1 / prod2;
            glm::vec3 inter = o_ - d_ * prod3;

            //divide the face in two triangles
            //first triangle is V1 V2 V4
            glm::vec3 n = glm::cross(p[1]->getPosition() - p[0]->getPosition(),
                                     p[3]->getPosition() - p[0]->getPosition());
            //calculate the 3 signed area for the first triangle
            glm::vec3 n1 = glm::cross(p[1]->getPosition() - inter, p[3]->getPosition() - inter);
            glm::vec3 n2 = glm::cross(p[3]->getPosition() - inter, p[0]->getPosition() - inter);
            glm::vec3 n3 = glm::cross(p[0]->getPosition() - inter, p[1]->getPosition() - inter);
            //check if the intersection point lays inside the first triangle
            if (glm::dot(n1, n) >= 0 && glm::dot(n2, n) >= 0 && glm::dot(n3, n) >= 0) {
                //find closest particle
                particle *closest = p[0];
                if (glm::length(p[1]->getPosition() - inter) < glm::length(p[0]->getPosition() - inter)) {
                    closest = p[1];
                }
                if (glm::length(p[3]->getPosition() - inter) < glm::length(closest->getPosition() - inter)) {
                    closest = p[3];
                }
                toReturn.isMouseOver = true;
                toReturn.point = closest;
                return toReturn;
            }

            //calculate the 3 signed area for the second triangle
            n = glm::cross(p[2]->getPosition() - p[1]->getPosition(), p[3]->getPosition() - p[1]->getPosition());
            n1 = glm::cross(p[2]->getPosition() - inter, p[3]->getPosition() - inter);
            n2 = glm::cross(p[3]->getPosition() - inter, p[1]->getPosition() - inter);
            n3 = glm::cross(p[1]->getPosition() - inter, p[2]->getPosition() - inter);
            if (glm::dot(n1, n) >= 0 && glm::dot(n2, n) >= 0 && glm::dot(n3, n) >= 0) {
                //find closest particle
                particle *closest = p[1];
                if (glm::length(p[2]->getPosition() - inter) < glm::length(p[1]->getPosition() - inter)) {
                    closest = p[2];
                }
                if (glm::length(p[3]->getPosition() - inter) < glm::length(closest->getPosition() - inter)) {
                    closest = p[3];
                }
                toReturn.isMouseOver = true;
                toReturn.point = closest;
                return toReturn;
            }
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

void clothBVH::update() {
    //basically recreate the sphere
    //find the two point that are the furthest apart
    particle *p1;
    particle *p2;
    float furthestDistance = -1.0f;
    //TODO: maybe size -1
    for (int i = 0; i < clothBVH::p.size(); ++i) {
        for (int j = i + 1; j < clothBVH::p.size(); ++j) {
            particle *first = clothBVH::p[i];
            particle *second = clothBVH::p[j];
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
    clothBVH::radius = r;
    //new sphere center
    glm::vec3 c = p1->getPosition() + r * glm::normalize(direction);


    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel = glm::mat4(1);
    //I discovered painstakingly that the order of operations matters
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0 * r, 2.0 * r, 2.0 * r));
    clothBVH::sphereShown->setModel(internalSphereModel);

    //update children
    if (clothBVH::child0 != nullptr) clothBVH::child0->update();
    if (clothBVH::child1 != nullptr) clothBVH::child1->update();
    if (clothBVH::child2 != nullptr) clothBVH::child2->update();
    if (clothBVH::child3 != nullptr) clothBVH::child3->update();
}

void clothBVH::detectCollisionSphere(glm::mat4 outModel, collidable *obj) {
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
    r2+=0.1;

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
                    i->setCollisionForce(100.0f*(intersectionPoint-point));
                }
            }
        } else {
            //otherwise recursively check the children
            if (clothBVH::child0 != nullptr) clothBVH::child0->detectCollisionSphere(outModel, obj);
            if (clothBVH::child1 != nullptr) clothBVH::child1->detectCollisionSphere(outModel, obj);
            if (clothBVH::child2 != nullptr) clothBVH::child2->detectCollisionSphere(outModel, obj);
            if (clothBVH::child3 != nullptr) clothBVH::child3->detectCollisionSphere(outModel, obj);
        }
    }
}

void clothBVH::detectCollisionCube(glm::mat4 outModel, collidable *obj) {
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
            //TODO: stupid triangle triangle intersection
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
                    i->setCollisionForce(256000.0f*(current-pos));
                }
            }

        } else {
            if (clothBVH::child0 != nullptr) clothBVH::child0->detectCollisionCube(outModel, obj);
            if (clothBVH::child1 != nullptr) clothBVH::child1->detectCollisionCube(outModel, obj);
            if (clothBVH::child2 != nullptr) clothBVH::child2->detectCollisionCube(outModel, obj);
            if (clothBVH::child3 != nullptr) clothBVH::child3->detectCollisionCube(outModel, obj);
        }
    }
}

//with this one I am gonna cheat a lot
//since it would be difficult to check the collision between a plane
//that has no height, for the sake of the collision detection
//I am gonna give it an artificial height
//so basically it is the same as a cube/box
void clothBVH::detectCollisionPlane(glm::mat4 outModel, collidable *obj) {
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
                }
            }

        } else {
            if (clothBVH::child0 != nullptr) clothBVH::child0->detectCollisionPlane(outModel, obj);
            if (clothBVH::child1 != nullptr) clothBVH::child1->detectCollisionPlane(outModel, obj);
            if (clothBVH::child2 != nullptr) clothBVH::child2->detectCollisionPlane(outModel, obj);
            if (clothBVH::child3 != nullptr) clothBVH::child3->detectCollisionPlane(outModel, obj);
        }
    }
}
