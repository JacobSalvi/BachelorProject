#include "skybox.h"

skybox::skybox(GLuint pID) : programID(pID) {
    vertexBuffer = new float[108];
    float helper[] = {
            -0.5, 0.5, 0.5, // front face
            -0.5, -0.5, 0.5,
            0.5, -0.5, 0.5,
            -0.5, 0.5, 0.5,
            0.5, -0.5, 0.5,
            0.5, 0.5, 0.5,
            0.5, -0.5, -0.5,// back face
            -0.5, -0.5, -0.5,
            -0.5, 0.5, -0.5,
            0.5, 0.5, -0.5,
            0.5, -0.5, -0.5,
            -0.5, 0.5, -0.5,
            0.5, -0.5, 0.5,// right face
            0.5, -0.5, -0.5,
            0.5, 0.5, -0.5,
            0.5, 0.5, 0.5,
            0.5, -0.5, 0.5,
            0.5, 0.5, -0.5,
            -0.5, 0.5, -0.5,// left face
            -0.5, -0.5, -0.5,
            -0.5, -0.5, 0.5,
            -0.5, 0.5, -0.5,
            -0.5, -0.5, 0.5,
            -0.5, 0.5, 0.5,
            -0.5, 0.5, 0.5, //top face
            0.5, 0.5, -0.5,
            -0.5, 0.5, -0.5,
            -0.5, 0.5, 0.5,
            0.5, 0.5, 0.5,
            0.5, 0.5, -0.5,
            -0.5, -0.5, 0.5, //bottom face
            -0.5, -0.5, -0.5,
            0.5, -0.5, -0.5,
            -0.5, -0.5, 0.5,
            0.5, -0.5, -0.5,
            0.5, -0.5, 0.5
    };
    for (int i = 0; i < 108; ++i) {
        vertexBuffer[i] = helper[i];
    }

    //faces
    //right, left, top, bottom, front and back
    //in that order
    skybox::faces = {
            "shaders/sb.png",
            "shaders/sb.png",
            "shaders/sb.png",
            "shaders/sb.png",
            "shaders/sb.png",
            "shaders/sb.png"};

    loadTexture();

    glGenBuffers(1, &vertex);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, vertexBuffer, GL_STATIC_DRAW);
}

skybox::~skybox() = default;

void skybox::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix) {

    glUseProgram(skybox::programID);

    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    GLuint viewUn = glGetUniformLocation(programID, "viewMatrix");
    GLuint projectionUn = glGetUniformLocation(programID, "projectionMatrix");
    glm::mat4 model(1);

    glUniformMatrix4fv(viewUn, 1, GL_FALSE, &glm::mat4(glm::mat3(ViewMatrix))[0][0]);
    glUniformMatrix4fv(projectionUn, 1, GL_FALSE, &ProjectionMatrix[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glEnable(GL_DEPTH_TEST);

    glDisableVertexAttribArray(0);
}

void skybox::loadTexture() {
    unsigned int tmp;
    glGenTextures(1, &tmp);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tmp);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         data);
            stbi_image_free(data);
        } else {
            printf("Stb can't load this sh*t: %s\n", faces[i].c_str());
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    skybox::textureID = tmp;
}
