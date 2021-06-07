
#include "texturedCloth.h"
#include "../../shaders/controls.hpp"
#include "../../shaders/texture.hpp"

float tw=14;
float th=14;

texturedCloth::texturedCloth(float mass, GLuint texture, GLuint textureId, const glm::vec3 &color, float gravity,
                             const glm::mat4 &mod, const glm::vec3 &lPos, GLuint pID) : net( tw+1, mod, lPos),
                             texture(texture), textureId(textureId), programID(pID) {
    //the only difference is that we have to change the color buffer
    float * uv = new float[int(th)*int(tw)*12];
    int pos = 0;
    for(int i=0; i<th; ++i){
        for(int j=0; j<tw; ++j){
            //first triangle first vertex

            uv[pos++]=float(j)/tw;
            uv[pos++]=1.0f-float(i)/th;

            //first triangle second vertex
            uv[pos++]=float(j)/tw;
            uv[pos++]=1.0f-float(i+1)/th;

            //first triangle third vertex
            uv[pos++]=float(j+1)/tw;
            uv[pos++]=1.0f-float(i+1)/th;

            //second triangle first vertex
            uv[pos++]=float(j)/tw;
            uv[pos++]=1.0f-float(i)/th;

            //second triangle second vertex
            uv[pos++]=float(j+1)/tw;
            uv[pos++]=1.0f-float(i+1)/th;

            //second triangle third vertex
            uv[pos++]=float(j+1)/tw;
            uv[pos++]=1.0f-float(i)/th;
        }
    }
    texturedCloth::colorBuffer=uv;
    GLuint netColor;
    glGenBuffers(1, &netColor);
    glBindBuffer(GL_ARRAY_BUFFER, netColor);
    glBufferData(GL_ARRAY_BUFFER, net::getSize(), net::getColorBuffer(), GL_STATIC_DRAW);
    net::setColour(netColor);

    ntId = glGetUniformLocation(programID, "normalMap");
    nt = loadDDS("shaders/normalMap.DDS");
    computeTangents();
}

void texturedCloth::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint pID) {
    //here is were the real change happens
    glUseProgram(texturedCloth::programID);
    GLuint matrixID = glGetUniformLocation(texturedCloth::programID, "MVP");
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * getModelMatrix();
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(textureId, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, texturedCloth::vertex);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texturedCloth::getSize(), texturedCloth::vertexBuffer);
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
    glBindBuffer(GL_ARRAY_BUFFER, texturedCloth::colour);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, texturedCloth::getNumberOfVertices()); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}

void texturedCloth::renderShadow(glm::mat4 pr, glm::mat4 viw, glm::mat4 bias, GLuint pID) {
    glUseProgram(programID);
    //glCullFace(GL_BACK);
    //Get a handle for all of the uniforms
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    GLuint DepthBiasID = glGetUniformLocation(programID, "DepthBiasMVP");
    GLuint ShadowMapID = glGetUniformLocation(programID, "shadowMap");

    // Get a handle for our "LightPosition" uniform
    GLuint lightInvDirID = glGetUniformLocation(programID, "LightInvDirection_worldspace");

    glm::mat4 MVP = pr*viw*modelMatrix;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viw[0][0]);
    glUniformMatrix4fv(DepthBiasID, 1, GL_FALSE, &(bias*modelMatrix)[0][0]);
    glUniform1i(ShadowMapID, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureId, 1);

    //normal texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, nt);
    glUniform1i(ntId, 2);

    glUniform3f(lightInvDirID, lightPos.x, lightPos.y, lightPos.z);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
    glBufferSubData(GL_ARRAY_BUFFER, 0, getSize(), getVertexBuffer());
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal);
    glBufferSubData(GL_ARRAY_BUFFER, 0, getSize(), getNormalBuffer());
    glVertexAttribPointer(
            2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colour);
    glVertexAttribPointer(
            1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    //tangent
    GLuint tan;
    glGenBuffers(1, &tan);
    glBindBuffer(GL_ARRAY_BUFFER, tan);
    glBufferData(GL_ARRAY_BUFFER, tangent.size()*sizeof(glm::vec3), &tangent[0], GL_DYNAMIC_DRAW);
    GLuint bitan;
    glGenBuffers(1, &bitan);
    glBindBuffer(GL_ARRAY_BUFFER, bitan);
    glBufferData(GL_ARRAY_BUFFER, bitangent.size()*sizeof(glm::vec3), &bitangent[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tan);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tangent.size()*sizeof(glm::vec3), &tangent[0]);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, tan);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bitangent.size()*sizeof(glm::vec3), &bitangent[0]);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);


    glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glUniform1i(ShadowMapID, 0);
}

void texturedCloth::computeTangents() {
    tangent.clear();
    bitangent.clear();
    //vertices = 6*14*14
    for(int i=0; i<14*14*2;++i){
        //conversion from float arr to vec
        glm::vec3 v0(vertexBuffer[9*i],vertexBuffer[9*i+1],vertexBuffer[9*i+2]);
        glm::vec3 v1(vertexBuffer[3+9*i],vertexBuffer[3+9*i+1],vertexBuffer[3+9*i+2]);
        glm::vec3 v2(vertexBuffer[6+9*i],vertexBuffer[6+9*i+1],vertexBuffer[6+9*i+2]);

        glm::vec2 uv0(colorBuffer[6*i], colorBuffer[6*i+1]);
        glm::vec2 uv1(colorBuffer[6*i+2], colorBuffer[6*i+1+2]);
        glm::vec2 uv2(colorBuffer[6*i+4], colorBuffer[6*i+1+4]);

        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v1-v0;
        glm::vec3 deltaPos2 = v2-v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1-uv0;
        glm::vec2 deltaUV2 = uv2-uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tan = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        glm::vec3 bitan = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

        tangent.push_back(tan);
        tangent.push_back(tan);
        tangent.push_back(tan);

        bitangent.push_back(bitan);
        bitangent.push_back(bitan);
        bitangent.push_back(bitan);
    }
}

void texturedCloth::updateBuffer() {
    net::updateBuffer();
    computeTangents();
}
