//this file needs a better name, I know

#include "helperFunctions.h"

//TODO: rework how to do cultural stuff
#if 0
//culture
void culture(std::vector<helperStruct *> *list, glm::vec3 tr){
    net * tmp = new net(1.0f, 3,7, 0, glm::vec3(2.0f,0.0f,0.0f), -1.0f, tr);

    //my wisdom knows no limit
    // = 12*(row-1)(col-1)
    float uv[144];

    int pos = 0;
    for(int i=0; i<6; ++i){
        for(int j=0; j<2; ++j){
            //first triangle first vertex

            uv[pos++]=float(j)/2.0f;
            uv[pos++]=1.0f-float(i)/6.0f;

            //first triangle second vertex
            uv[pos++]=float(j)/2.0f;
            uv[pos++]=1.0f-float(i+1)/6.0f;

            //first triangle third vertex
            uv[pos++]=float(j+1)/2.0f;
            uv[pos++]=1.0f-float(i+1)/6.0f;

            //second triangle first vertex
            uv[pos++]=float(j)/2.0f;
            uv[pos++]=1.0f-float(i)/6.0f;

            //second triangle second vertex
            uv[pos++]=float(j+1)/2.0f;
            uv[pos++]=1.0f-float(i+1)/6.0f;

            //second triangle third vertex
            uv[pos++]=float(j+1)/2.0f;
            uv[pos++]=1.0f-float(i)/6.0f;
        }
    }

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, tmp->getSize(), tmp->getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_DYNAMIC_DRAW);

    auto * str = new helperStruct;
    str->cloth=tmp;
    str->vertex=vertexbuffer;
    str->color=uvbuffer;
    str->tr=tr;
    list->push_back(str);
}

void drawCulture(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint MatrixID, helperStruct* obj, GLuint texture, GLuint textureId, GLuint program){
    glUseProgram(program);
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix=glm::translate(ModelMatrix, obj->tr);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(textureId, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertex);
    glBufferSubData(GL_ARRAY_BUFFER, 0, obj->cloth->getSize(), obj->cloth->getVertexBuffer());
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
    glBindBuffer(GL_ARRAY_BUFFER, obj->color);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, obj->cloth->getNumberOfVertices()); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
#endif

void addCloth(std::vector<net *> *list, int col, int row, int in, glm::vec3 colour, glm::vec3 tr){
    net * clothNew = new net(1.0f, col, row, in, colour, -1.0f, tr);
    GLuint netVertex;
    glGenBuffers(1, &netVertex);
    glBindBuffer(GL_ARRAY_BUFFER, netVertex);
    glBufferData(GL_ARRAY_BUFFER, clothNew->getSize(), clothNew->getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint netColor;
    glGenBuffers(1, &netColor);
    glBindBuffer(GL_ARRAY_BUFFER, netColor);
    glBufferData(GL_ARRAY_BUFFER, clothNew->getSize(), clothNew->getColorBuffer(), GL_STATIC_DRAW);

    clothNew->setVertex(netVertex);
    clothNew->setColour(netColor);

    list->push_back(clothNew);
}

//create and add sphere
//type 0->sphere, 1->cube, 2 plane
void addColl(std::vector<collidable *> * list, int type){
    collidable * coll;
    glm::mat4 model = glm::mat4(1.0f);
    switch(type){
        //sphere
        case 0:
            model = glm::translate(model, glm::vec3(6.0f,0.0f,2.0f));
            coll = new sphere(100, model,glm::vec3(1.0f,0.8f,0.8f));
            break;
            //cube
        case 1:
            model = glm::translate(model, glm::vec3(-3.0f,0.0f,2.0f));
            coll = new cube(model,glm::vec3(1.0f,1.0f,0.4f));
            break;
            //plane
        case 2:
            glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,-2.0f,0.0f));
            model = glm::scale(translate, glm::vec3(5.0f,5.0f,5.0f));
            coll = new plane(3.0f,1.0f, model, glm::vec3(0.4f,1.0f,0.6f));
            break;

    }

    GLuint collVertex;
    glGenBuffers(1, &collVertex);
    glBindBuffer(GL_ARRAY_BUFFER, collVertex);
    glBufferData(GL_ARRAY_BUFFER, coll->getSize(), coll->getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint collColour;
    glGenBuffers(1, &collColour);
    glBindBuffer(GL_ARRAY_BUFFER, collColour);
    glBufferData(GL_ARRAY_BUFFER, coll->getSize(), coll->getColorBuffer(), GL_STATIC_DRAW);

    coll->setCollVertex(collVertex);
    coll->setCollColour(collColour);

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

helperStruct isMouseOverDeformable(glm::vec3 origin, glm::vec3 direction, std::vector<net *> * list){
    helperStruct closestObject;
    closestObject.isMouseOver=false;
    for(auto i : *list){
        helperStruct tmp = i->isHovered(origin, direction);
        if(tmp.isMouseOver){
            closestObject=tmp;
            closestObject.obj=i;
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
glm::vec3 pointPlaneProjection(glm::vec3 point, glm::vec3 planeNormal){
    return point - glm::dot(point, planeNormal) * planeNormal;
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
