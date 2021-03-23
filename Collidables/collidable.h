#ifndef CODE_COLLIDABLE_H
#define CODE_COLLIDABLE_H
#include <glm/glm.hpp>
#include <GL/glew.h>



//Simply a super class for every sub class that can be collided with
class collidable {
protected:
    glm::vec3 colour;
    glm::mat4 model;

    float * vertexBuffer;
    float * colorBuffer;

    GLuint collVertex;
    GLuint collColour;

public:
    collidable(glm::vec3 colour, glm::mat4 model);

    virtual int getSize();

    GLuint getCollVertex() const;

    GLuint getCollColour() const;

    void setCollVertex(GLuint collVertex);

    void setCollColour(GLuint collColour);

    float *getVertexBuffer() const;

    float *getColorBuffer() const;

    const glm::mat4 &getModel() const;

    virtual int getNumberOfVertices();
};


#endif