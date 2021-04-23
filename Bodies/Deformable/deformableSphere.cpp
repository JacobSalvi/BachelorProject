
#include "deformableSphere.h"

deformableSphere::deformableSphere(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos) : deformableObjects(-1.0f, mod, lPos), size(10){
    //I fear that a too detailed sphere might
    //burn my cpu, so for now it will have size of 10
    vertexBuffer = new float[size * size * 18];
    colorBuffer = new float[size * size * 18];

    int vertPos = 0;
    int colPos = 0;
    float step = (float) 1 / size;
    float u = 0.0f;
    for (int i = 0; i < size; ++i) {
        float v = 0.0f;
        for (int j = 0; j < size; ++j) {
            double t = sin(M_PI * v);
            double x1 = t * cos(2 * M_PI * u);
            double z1 = t * sin(2 * M_PI * u);
            double y1 = cos(M_PI * v);

            double x4 = t * cos(2 * M_PI * (u + step));
            double z4 = t * sin(2 * M_PI * (u + step));
            double y4 = cos(M_PI * v);

            t = sin(M_PI * (v + step));
            double x2 = t * cos(2 * M_PI * u);
            double z2 = t * sin(2 * M_PI * u);
            double y2 = cos(M_PI * (v + step));

            double x3 = t * cos(2 * M_PI * (u + step));
            double z3 = t * sin(2 * M_PI * (u + step));
            double y3 = cos(M_PI * (v + step));

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            vertexBuffer[vertPos++] = x2;
            vertexBuffer[vertPos++] = y2;
            vertexBuffer[vertPos++] = z2;

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x4;
            vertexBuffer[vertPos++] = y4;
            vertexBuffer[vertPos++] = z4;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            //add particles
            particle * p1 = new particle(glm::vec3(x1, y1, z1), 1.0f);
            particle * p2 = new particle(glm::vec3(x2, y2, z2), 1.0f);
            particle * p3 = new particle(glm::vec3(x3, y3, z3), 1.0f);
            particle * p4 = new particle(glm::vec3(x4, y4, z4), 1.0f);
            deformableObjects::particles.push_back(p1);
            deformableObjects::particles.push_back(p2);
            deformableObjects::particles.push_back(p3);
            deformableObjects::particles.push_back(p4);

            for (int k = 0; k < 6; k++) {
                colorBuffer[colPos++] = color[0];
                colorBuffer[colPos++] = color[1];
                colorBuffer[colPos++] = color[2];
            }
            v += step;
        }
        u += step;
    }

    //unit sphere
    for (int i = 0; i < size * size * 18; ++i) {
        vertexBuffer[i] = vertexBuffer[i] / 2.0f;
    }

    //normals are the same as the vertex
    normalBuffer=vertexBuffer;

    setGLuint();
}

void deformableSphere::setGLuint() {
    GLuint deformableSphereVertex;
    glGenBuffers(1, &deformableSphereVertex);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereVertex);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint deformableSphereColor;
    glGenBuffers(1, &deformableSphereColor);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereColor);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getColorBuffer(), GL_STATIC_DRAW);


    GLuint deformableSphereNormal;
    glGenBuffers(1, &deformableSphereNormal);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereNormal);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getNormalBuffer(), GL_STATIC_DRAW);

    deformableSphere::setVertex(deformableSphereVertex);
    deformableSphere::setColour(deformableSphereColor);
    deformableSphere::setNormal(deformableSphereNormal);
}

int deformableSphere::getSize() {
    return sizeof(float) * size * size * 18;
}

int deformableSphere::getNumberOfVertices() {
    return size * size * 18;
}

void deformableSphere::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) {
    deformableObjects::render(ProjectionMatrix, ViewMatrix, programID);
}

helperStruct deformableSphere::isHovered(glm::vec3 origin, glm::vec3 direction) {
    helperStruct toReturn;
    toReturn.isMouseOver = false;
    return toReturn;
}
