#include <glm/gtc/matrix_transform.hpp>
#include "sphereBVH.h"


sphereBVH::sphereBVH(std::vector<particle *> particles, int row) : p(particles){


    //needed information
    glm::vec3 p0=particles[0]->getPosition();
    glm::vec3 direction = particles[particles.size()-1]->getPosition()-p0;

    //get radius
    float r = glm::length(direction);
    r/=2.0f;
    sphereBVH::radius=r;
    //get center
    glm::vec3 c = p0+r*glm::normalize(direction);

    //no need to divide further
    //create sphere and return
    if(particles.size()==4){
        std::swap(p[2],p[3]);

        //sphere and model matrix for the sphere
        glm::mat4 internalSphereModel= glm::mat4(1);
        //I discovered painstakingly that the order of operations matters
        internalSphereModel = glm::translate(internalSphereModel, c);
        internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0*r,2.0*r,2.0*r));
        sphereBVH::sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f,0.8f,0.8f));
        return;
    }

    //create subSpheres
    int col = (int)particles.size()/row;
    std::vector<particle *> v0;
    std::vector<particle *> v1;
    std::vector<particle *> v2;
    std::vector<particle *> v3;
    int childRow;

    //there probably is a less stupid way to do this, but I can't figure out how
    //vertical structure
    if(col==2){
        for(int i=0;i<row;++i){
            for(int j=0; j<col;++j){
                if(row%2==0){
                    if(i<=row/2){
                        v0.push_back(particles[i*col+j]);
                    }
                    if(i>=row/2-1){
                        v1.push_back(particles[i*col+j]);
                    }
                }else{
                    if(i<=row/2){
                        v0.push_back(particles[i*col+j]);
                    }
                    if(i>=row/2){
                        v1.push_back(particles[i*col+j]);
                    }
                }
            }
        }
        childRow=v0.size()/2;
    }else if(row==2){
        //horizontal structure
        for(int i=0;i<row;++i){
            for(int j=0; j<col;++j){
                if(col%2==0){
                    if(j<=col/2){
                        v0.push_back(particles[i*col+j]);
                    }
                    if(j>=col/2-1){
                        v1.push_back(particles[i*col+j]);
                    }
                }else{
                    if(j<=col/2){
                        v0.push_back(particles[i*col+j]);
                    }
                    if(j>=col/2){
                        v1.push_back(particles[i*col+j]);
                    }
                }
            }
        }
        childRow=2;
    }else{
        //number of rows for children
        childRow=row/2+1;
        //non thin structure
        for(int i=0; i<row;++i){
            for(int j=0; j<col;++j){
                if(col%2==0){
                    if(row%2==0){
                        //even col and row
                        //checking
                        if(j<=col/2 &&i<=row/2){
                            v0.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2 -1&& i<=row/2){
                            v1.push_back(particles[i*col+j]);
                        }
                        if(j<=col/2 && i>=row/2 -1){
                            v2.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2 -1 &&i>=row/2 -1){
                            v3.push_back(particles[i*col+j]);
                        }
                    }else{
                        //even col odd row
                        //should be correct
                        if(j<=col/2 &&i<=row/2){
                            v0.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2-1 && i<=row/2){
                            v1.push_back(particles[i*col+j]);
                        }
                        if(j<=col/2 && i>=row/2){
                            v2.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2-1 && i>=row/2){
                            v3.push_back(particles[i*col+j]);
                        }
                    }
                }else{
                    //col odd row even
                    //should be correct
                    if(row%2==0){
                        if(j<=col/2 &&i<=row/2){
                            v0.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2 && i<=row/2){
                            v1.push_back(particles[i*col+j]);
                        }
                        if(j<=col/2 && i>=row/2-1){
                            v2.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2 && i>=row/2-1){
                            v3.push_back(particles[i*col+j]);
                        }
                    }else{
                        //col and row odd
                        //it works
                        if(j<=col/2 &&i<=row/2){
                            v0.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2 && i<=row/2){
                            v1.push_back(particles[i*col+j]);
                        }
                        if(j<=col/2 && i>=row/2){
                            v2.push_back(particles[i*col+j]);
                        }
                        if(j>=col/2&&i>=row/2){
                            v3.push_back(particles[i*col+j]);
                        }
                    }
                }
            }
        }
    }

    //recursively make children
    child0= !v0.empty()?new sphereBVH(v0, childRow): NULL;
    child1= !v1.empty()?new sphereBVH(v1, childRow): NULL;
    child2= !v2.empty()?new sphereBVH(v2, childRow): NULL;
    child3= !v3.empty()?new sphereBVH(v3, childRow): NULL;

    //sphere and model matrix for the sphere
    glm::mat4 internalSphereModel= glm::mat4(1);
    internalSphereModel = glm::translate(internalSphereModel, c);
    internalSphereModel = glm::scale(internalSphereModel, glm::vec3(2.0*r,2.0*r,2.0*r));

    sphereBVH::sphereShown = new sphere(10, internalSphereModel, glm::vec3(0.8f,0.8f,0.8f));

    //checking stuff
    //std::cout<<"childRow is: "<<childRow<<std::endl;
    /*
    std::cout<<"v0: "<<std::endl;
    for(int i=0; i<v0.size();++i){
       std::cout<< v0[i]->getId()<<std::endl;
    }
    std::cout<<"v1: "<<std::endl;
    for(int i=0; i<v1.size();++i){
        std::cout<< v1[i]->getId()<<std::endl;
    }
    std::cout<<"v2: "<<std::endl;
    for(int i=0; i<v2.size();++i){
        std::cout<< v2[i]->getId()<<std::endl;
    }
    std::cout<<"v3: "<<std::endl;
    for(int i=0; i<v3.size();++i){
        std::cout<< v3[i]->getId()<<std::endl;
    }*/
}

void sphereBVH::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, bool wireFrame, glm::mat4 model) {
    sphereShown->render(ProjectionMatrix, ViewMatrix, triangleMatrixID, true, model);
    if(child0!=NULL){
        child0->render(ProjectionMatrix, ViewMatrix, triangleMatrixID, wireFrame, model);
    }
    if(child1!=NULL){
        child1->render(ProjectionMatrix, ViewMatrix, triangleMatrixID, wireFrame, model);
    }
    if(child2!=NULL){
        child2->render(ProjectionMatrix, ViewMatrix, triangleMatrixID, wireFrame, model);
    }
    if(child3!=NULL){
        child3->render(ProjectionMatrix, ViewMatrix, triangleMatrixID, wireFrame, model);
    }

}

sphere *sphereBVH::getSphereShown() const {
    return sphereShown;
}

helperStruct sphereBVH::rayIntersect(glm::vec3 origin, glm::vec3 direction, glm::mat4 model) {
    //to make the compiler happy while I code
    helperStruct toReturn;
    toReturn.isMouseOver=false;

    //do we intersect the sphere?
    if(sphereBVH::sphereShown->rayIntersect(origin, direction, model)){
        //are we a leaf?
        if(child0==NULL){
            //must invert the model matrix
            glm::mat4 M_ = glm::inverse(model);
            glm::vec4 orHelper = M_*glm::vec4(origin,1.0f);
            glm::vec3 o_(orHelper.x, orHelper.y, orHelper.z);
            glm::vec4 dirHelper = M_*glm::vec4(direction, 0.0f);
            glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
            d_=glm::normalize(d_);


            //do we intersect the part of the net inside the sphere?
            //find normal
            glm::vec3 normal = glm::cross(p[1]->getPosition()-p[0]->getPosition(), p[3]->getPosition()-p[0]->getPosition());
            normal = glm::normalize(normal);

            //parallel, no intersection
            if(glm::dot(d_,normal)==0){
                return toReturn;
            }

            //point plane intersection
            glm::vec3 diff = o_-p[0]->getPosition();
            float prod1= glm::dot(diff,normal);
            float prod2= glm::dot(d_,normal);
            float prod3 = prod1/prod2;
            glm::vec3 inter=o_-d_*prod3;

            //divide the face in two triangles
            //first triangle is V1 V2 V4
            glm::vec3 n=glm::cross(p[1]->getPosition()-p[0]->getPosition(), p[3]->getPosition()-p[0]->getPosition());
            //calculate the 3 signed area for the first triangle
            glm::vec3 n1=glm::cross(p[1]->getPosition()-inter,p[3]->getPosition()-inter);
            glm::vec3 n2=glm::cross(p[3]->getPosition()-inter,p[0]->getPosition()-inter);
            glm::vec3 n3=glm::cross(p[0]->getPosition()-inter,p[1]->getPosition()-inter);
            //check if the intersection point lays inside the first triangle
            if(glm::dot(n1,n)>=0&& glm::dot(n2,n)>=0&& glm::dot(n3,n)>=0){
                //find closest particle
                particle * closest = p[0];
                if(glm::length(p[1]->getPosition()-inter)<glm::length(p[0]->getPosition()-inter)){
                    closest=p[1];
                }
                if(glm::length(p[3]->getPosition()-inter)<glm::length(closest->getPosition()-inter)){
                    closest=p[3];
                }
                toReturn.isMouseOver= true;
                toReturn.point=closest;
                return toReturn;
            }

            //calculate the 3 signed area for the second triangle
            n=glm::cross(p[2]->getPosition()-p[1]->getPosition(),p[3]->getPosition()-p[1]->getPosition());
            n1=glm::cross(p[2]->getPosition()-inter,p[3]->getPosition()-inter);
            n2=glm::cross(p[3]->getPosition()-inter, p[1]->getPosition()-inter);
            n3=glm::cross(p[1]->getPosition()-inter, p[2]->getPosition()-inter);
            if(glm::dot(n1,n)>=0 && glm::dot(n2,n)>=0 && glm::dot(n3,n)>=0){
                //find closest particle
                particle * closest = p[1];
                if(glm::length(p[2]->getPosition()-inter)<glm::length(p[1]->getPosition()-inter)){
                    closest=p[2];
                }
                if(glm::length(p[3]->getPosition()-inter)<glm::length(closest->getPosition()-inter)){
                    closest=p[3];
                }
                toReturn.isMouseOver= true;
                toReturn.point=closest;
                return toReturn;
            }
        }else{
            //recursive call on children
            helperStruct tmp = child0->rayIntersect(origin, direction, model);
            if(tmp.isMouseOver){
                return tmp;
            }
            if(child1!=NULL){
                tmp= child1->rayIntersect(origin, direction, model);
                if(tmp.isMouseOver){
                    return tmp;
                }
            }
            if(child2!=NULL){
                tmp= child2->rayIntersect(origin, direction, model);
                if(tmp.isMouseOver){
                    return tmp;
                }
            }
            if(child3!=NULL){
                tmp= child3->rayIntersect(origin, direction, model);
                if(tmp.isMouseOver){
                    return tmp;
                }
            }
        }
    }
    return toReturn;
}
