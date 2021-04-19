#ifndef CODE_COLLIDABLE_H
#define CODE_COLLIDABLE_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <imgui.h>
#include "../../Collidables/mouseIntersectStruct.h"

struct mouseIntersectStruct;

//Simply a super class for every sub class that can be collided with
class collidable {
protected:
    glm::vec3 colour;
    glm::mat4 model;

    float * vertexBuffer{};
    float * colorBuffer{};
    float * normalBuffer{};

    GLuint collVertex{};
    GLuint collColour{};
    GLuint collNormal{};

    glm::vec3 lightPos;

public:
    collidable(glm::vec3 colour, glm::mat4 model, glm::vec3 lPos);

    virtual int getSize();

    GLuint getCollVertex() const;

    GLuint getCollColour() const;

    GLuint getCollNormal() const;

    void setCollVertex(GLuint collVertex);

    void setCollColour(GLuint collColour);

    void setCollNormal(GLuint collNormal);

    float *getVertexBuffer() const;

    float *getColorBuffer() const;

    float *getNormalBuffer() const;

    const glm::mat4 &getModel() const;

    virtual int getNumberOfVertices();

    void handleCollision(collidable * b);

    virtual mouseIntersectStruct isHovered(glm::vec3 origin, glm::vec3 direction);

    //debugging stuff
    virtual void getName();

    void setModel(const glm::mat4 &model);

    void move(glm::vec3 mv);

    void render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID, bool wireFrame, glm::mat4 m=glm::mat4(1));

    void setGLuint();

    virtual int returnType();

};


#endif
