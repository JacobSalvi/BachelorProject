#include <glm/gtc/matrix_transform.hpp>
#include "sphereBVH.h"

sphereBVH::sphereBVH(std::vector<particle *> particles, int row) : BVH(){
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
