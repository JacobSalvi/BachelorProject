//this file needs a better name, I know

#include "helperFunctions.h"
#include <fstream>
#include <iostream>
#include<sstream>

//new culture
void culture(std::vector<deformableObjects *> *list, GLuint texture, GLuint textureId, glm::mat4 mod, glm::vec3 lPos, GLuint pID){
    texturedCloth * tmp = new texturedCloth(1.0f, texture, textureId, glm::vec3(2.0f,0.0f,0.0f), -1.0f, mod, lPos, pID);
    list->push_back(tmp);
}

void addCloth(std::vector<deformableObjects *> *list, int col, int row, int in, glm::vec3 colour, glm::mat4 mod, glm::vec3 lPos, int orientation){
    net * clothNew;
    orientation == 0 ? clothNew =new net(0.2f, col, row, in, colour, -1.0f, mod, lPos) : clothNew = new net(col, mod, lPos);
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
            coll = new plane(3.0f,10.0f, modelP, glm::vec3(0.4f,1.0f,0.6f), lPos);
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

void loadObj(const char * path,
             std::vector<float> & linearVerts,
             std::vector<float> & linearUVs,
             std::vector<float> & linearNormals,
             std::vector<float> & linearTangents,
             std::vector<float> & linearBitangents,
             std::vector<unsigned int> & tris){
    std::cout<<"loading object "<<path<<std::endl;
    //needed variables to store the object information
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<int> vert_ids;
    std::vector<glm::ivec3> indices;

    std::fstream file;
    file.open(path, std::ios::in);
    if(!file.is_open()){
        std::cout<<"File couldn't be loaded"<<std::endl;
        return;
    }

    //the file opened successfully
    //read the content
    std::string line;
    //read file line by line
    while(getline(file, line)){
        //std::cout<<line<<std::endl;
        std::string s;
        std::istringstream ss(line);
        std::vector<std::string> arguments;
        //divide line based on spaces
        while(std::getline(ss, s, ' ')){
            arguments.push_back(s);
        }
        // what kind of information are we reading?
        // i am not going to make an enumerator to
        // switch on strings
        if(arguments[0]=="v"){
            //push vert information in the array
            verts.push_back(glm::vec3(std::stof(arguments[1]),
                                      std::stof(arguments[2]),
                                      std::stof(arguments[3])));
        }else if(arguments[0]=="vn"){
            normals.push_back(glm::vec3(std::stof(arguments[1]),
                                      std::stof(arguments[2]),
                                      std::stof(arguments[3])));
        }else if(arguments[0]=="vt"){
            uvs.push_back(glm::vec2(std::stof(arguments[1]),
                                      1.0f-std::stof(arguments[2])));
        }else if(arguments[0]=="f"){
            for(int j = 1; j<=3;++j){
                std::vector<int> index;
                std::string tmp;
                std::istringstream iStream(arguments[j]);
                //divide line based on "/"
                while(std::getline(iStream, tmp, '/')){
                    index.push_back(std::stoi(tmp));
                }
                // need to adjust since we start from 0
                indices.push_back(glm::ivec3(index[0]-1,
                                             index[1]-1,
                                             index[2]-1));
            }
            //if mesh is composed of quads I will probably cry
            if(arguments.size()==5){
                for(int j =3; j>=3 && j<=5;++j){
                    int it = j;
                    if(it==5){
                        it=1;
                    }
                    std::vector<int> index;
                    std::string tmp;
                    std::istringstream iStream(arguments[it]);
                    //divide line based on "/"
                    while(std::getline(iStream, tmp, '/')){
                        index.push_back(std::stoi(tmp));
                    }
                    indices.push_back(glm::ivec3(index[0]-1,
                                                 index[1]-1,
                                                 index[2]-1));
                }
            }
        }
    }

    int numVerts = 0;
    // something about vertices duplicates
    for(int i=0; i<indices.size();i+=3){
        // compute tangent and bitangent
        glm::vec3 v1 = verts[indices[i+0].x];
        glm::vec3 v2 = verts[indices[i+1].x];
        glm::vec3 v3 = verts[indices[i+2].x];

        glm::vec3 w1 = verts[indices[i+0].y];
        glm::vec3 w2 = verts[indices[i+1].y];
        glm::vec3 w3 = verts[indices[i+2].y];

        float x1 = v2[0] - v1[0];
        float x2 = v3[0] - v1[0];
        float y1 = v2[1] - v1[1];
        float y2 = v3[1] - v1[1];
        float z1 = v2[2] - v1[2];
        float z2 = v3[2] - v1[2];

        float s1 = w2[0] - w1[0];
        float s2 = w3[0] - w1[0];
        float t1 = w2[1] - w1[1];
        float t2 = w3[1] - w1[1];

        float r = 1.0f/(s1*s2-s2*t1);

        glm::vec3 face_tangent((t2*x1 - t1*x2)*r,
                (t2*y1 - t1*y2)*r,
                (t2*z1 - t1*z2)*r);
        glm::vec3 face_bitangent = -glm::vec3((s1*x2 - s2*x1)*r,
                (s1*y2 - s2*y1)*r,
                (s1*z2 - s2*z1)*r);

        //add triangle
        for(int j=0; j<3;++j){
            glm::ivec3 P = indices[i+j];

            glm::vec3 normal = normals[P.y];

            //idk, I am scared
            glm::vec3 tangent(1,0,0);
            tangent = glm::dot(normal, face_tangent)*normal;
            tangent = face_tangent -tangent;
            tangent = glm::normalize(tangent);

            glm::vec3 bitangent;
            bitangent = glm::cross(normal, tangent);

            float handedness = glm::dot(bitangent, face_bitangent) < 0.0f ? -1.0f : 1.0f;

            // set bitangent
            bitangent = glm::cross(normal, tangent);
            bitangent = handedness*bitangent;
            bitangent = glm::normalize(bitangent);

            //pushing stuff
            linearVerts.push_back(verts[P.x][0]);
            linearVerts.push_back(verts[P.x][1]);
            linearVerts.push_back(verts[P.x][2]);

            linearNormals.push_back(normals[P.z][0]);
            linearNormals.push_back(normals[P.z][1]);
            linearNormals.push_back(normals[P.z][2]);

            linearTangents.push_back(tangent[0]);
            linearTangents.push_back(tangent[1]);
            linearTangents.push_back(tangent[2]);

            linearBitangents.push_back(bitangent[0]);
            linearBitangents.push_back(bitangent[1]);
            linearBitangents.push_back(bitangent[2]);

            linearUVs.push_back(uvs[P.y][0]);
            linearUVs.push_back(uvs[P.y][1]);
            tris.push_back(numVerts++);
        }
    }

    //we need to return
    // linearVerts linearNormals linearUvs, linear Tangents, linearBitangents and tris
    std::cout<<"object should have been loaded correctly, maybe"<<std::endl;
}

void renderModel( std::vector<float> & out_vertices,
                  std::vector<float> & out_uvs,
                  std::vector<float> & out_normals,
                  std::vector<unsigned int> & out_tris,
                  GLuint pId,
                  GLuint t,
                  GLuint tId,
                  glm::mat4 ProjectionMatrix,
                  glm::mat4 ViewMatrix,
                  glm::mat4 modelMatrix){
    // trying to render
    GLuint v;
    glGenBuffers(1, &v);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glBufferData(GL_ARRAY_BUFFER, out_vertices.size()*sizeof(float), &out_vertices[0], GL_STATIC_DRAW);

    GLuint uv;
    glGenBuffers(1, &uv);
    glBindBuffer(GL_ARRAY_BUFFER, uv);
    glBufferData(GL_ARRAY_BUFFER, out_uvs.size()*sizeof(float), &out_uvs[0], GL_STATIC_DRAW);

    GLuint ind;
    glGenBuffers(1, &ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_tris.size()*sizeof(unsigned int), &out_tris[0], GL_STATIC_DRAW);

    GLuint n;
    glGenBuffers(1, &n);
    glBindBuffer(GL_ARRAY_BUFFER, n);
    glBufferData(GL_ARRAY_BUFFER, out_normals.size(), &out_normals[0], GL_STATIC_DRAW);

    glUseProgram(pId);
    GLuint matrixID = glGetUniformLocation(pId, "MVP");
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * modelMatrix;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(tId, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    //i do not know for which law of the universe nullptr fixes the problem
    //but somehow it does. I swear that nothing makes less sense that this
    glDrawElements(GL_TRIANGLES, out_tris.size(), GL_UNSIGNED_INT, nullptr);
    //glDrawArrays(GL_TRIANGLES, 0, out_vertices.size()); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}