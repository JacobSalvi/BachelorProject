
#include "texturedCloth.h"

float tw=2;
float th=6;

texturedCloth::texturedCloth(float mass, GLuint texture, GLuint textureId, const glm::vec3 &color, float gravity,
                             const glm::mat4 &mod, const glm::vec3 &lPos, GLuint pID) : net(mass, tw+1, th+1, 1, color,
                                                                                gravity, mod, lPos), texture(texture), textureId(textureId), programID(pID) {
    //the only difference is that we have to change the color buffer
    float * uv = new float[144];
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
