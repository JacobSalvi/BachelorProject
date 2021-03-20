// Include standard headers
#include <cstdio>
#include <iostream>
#include <chrono>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "shaders/shader.hpp"
#include "shaders/controls.hpp"
#include "integrators/net.h"
#include "shaders/texture.hpp"
#include <thread>
#include <functional>

void timer_start(unsigned int interval);

//helper struct because I couldn't figure out any better way to do this
struct helperStruct{     
    net * cloth;
    GLuint vertex;
    GLuint color;
    glm::vec3 tr;
};

bool shouldSimulate = true;
std::thread sim;

#if 0
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        std::cout << "Eureka" << std::endl;
        //timer_start(1);
    }//else if(key==GLFW_KEY_C && action==GLFW_PRESS){
//        setWind(cloth);
//        setWind(clothR);
//    }
}
#endif

static bool threadShouldLive = true;
void timer_start(unsigned int interval, const std::vector<helperStruct *>& list) {
    sim = std::thread([interval, list]() {
        static int counter = 0;
        while (threadShouldLive) {
            for(auto i : list){
                i->cloth->integrate((float)(interval)/1000.0f);
            }
            counter++;
            //34 milliseconds ~= 30 frame per second
            if(counter==30){
                for(auto i : list){
                    i->cloth->updateBuffer();
                }
                counter=0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        std::cout<<"The thread should be dead now"<<std::endl;
    });
    sim.detach();
}

//culture
void culture(std::vector<helperStruct *> *list, glm::vec3 tr){
    net * tmp = new net(1.0f, 3,7, 0, glm::vec3(2.0f,0.0f,0.0f), -1.0f);

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

//testing whether I can manage to dynamically create and show a net
void addCloth(std::vector<helperStruct *> *list, int col, int row, int in, glm::vec3 colour, glm::vec3 tr){
    net * clothNew = new net(1.0f, col, row, in, colour, -1.0f);
    GLuint netVertex;
    glGenBuffers(1, &netVertex);
    glBindBuffer(GL_ARRAY_BUFFER, netVertex);
    glBufferData(GL_ARRAY_BUFFER, clothNew->getSize(), clothNew->getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint netColor;
    glGenBuffers(1, &netColor);
    glBindBuffer(GL_ARRAY_BUFFER, netColor);
    glBufferData(GL_ARRAY_BUFFER, clothNew->getSize(), clothNew->getColorBuffer(), GL_STATIC_DRAW);

    auto * tmp = new helperStruct;
    tmp->cloth=clothNew;
    tmp->color=netColor;
    tmp->vertex=netVertex;
    tmp->tr=tr;
    list->push_back(tmp);
}


void drawCloth(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint triangleMatrixID, helperStruct* obj){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, obj->tr);
    glm::mat4 mvp = ProjectionMatrix * ViewMatrix * model;

    glUniformMatrix4fv(triangleMatrixID, 1, GL_FALSE, &mvp[0][0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertex);
    glBufferSubData(GL_ARRAY_BUFFER, 0, obj->cloth->getSize(), obj->cloth->getVertexBuffer());
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) nullptr            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, obj->color);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void *) nullptr                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, obj->cloth->getNumberOfVertices());

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Tutorial 06 - Keyboard and Mouse", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    //glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    //This is likely not gonna work
    //nvm, it worked
    std::vector<helperStruct *> objectList;
//    addCloth(&objectList, 3, 9, 0, glm::vec3(1.0f,0.0f,0.0f), glm::vec3(2.0f,0.0f,0.0f));
//    addCloth(&objectList, 5,8, 0, glm::vec3(2.0f,0.0f,0.0f), glm::vec3(-4.0f,0.0f,0.0f));
//    addCloth(&objectList, 5,3, 1, glm::vec3(2.0f,0.0f,0.0f), glm::vec3(4.0f,0.0f,0.0f));


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //Triangle
    GLuint triangleProgramID = LoadShaders("shaders/vert.vertexshader", "shaders/colorFrag.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint triangleMatrixID = glGetUniformLocation(triangleProgramID, "MVP");


    //infinite wisdom
    GLuint programId = LoadShaders("shaders/vertexShader.vertexshader", "shaders/fragmentShader.fragmentshader");
    GLuint matrixId = glGetUniformLocation(programId, "MVP");
    GLuint texture = loadDDS("shaders/idk.DDS");
    GLuint textureId = glGetUniformLocation(programId, "myTextureSampler");
    GLuint texture2 = loadDDS("shaders/dak2.DDS");
    GLuint textureId2 = glGetUniformLocation(programId, "myTextureSampler");
    GLuint texture3 = loadDDS("shaders/dak3.DDS");
    GLuint textureId3 = glGetUniformLocation(programId, "myTextureSampler");

    std::vector<helperStruct *> cultureList;
    culture(&objectList, glm::vec3(-8.0f, 0.0f,0.0f));
    culture(&objectList, glm::vec3(0.0f, 0.0f,0.0f));
    culture(&objectList, glm::vec3(8.0f, 0.0f,0.0f));

    //glfwSetKeyCallback(window, key_callback);
    std::cout << "about to enter while loop" << std::endl;

    //gui stuff
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    static int azimuthal = 90;
    static int polar = 90;
    static int distance = 10;
    static float wind[3]= {0.0f,0.0f,-1.0f};
    static float mass =1.0f;
    static float gravity = -1.0f;
    static float ks = 40.0f;

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //making the gui
        //I bet that there is a less stupid way to do this
        //anyhow...
        ImGui_ImplGlfwGL3_NewFrame();
        {
            static float f =0.0f;
            static int c=0;
            ImGui::Text("Please god work");
            ImGui::SliderInt("Azimuthal angle", &azimuthal, 0, 360);
            ImGui::SliderInt("Polar angle", &polar, 0, 180);
            ImGui::InputInt("Distance", &distance);
            ImGui::InputFloat3("Wind", wind);
            ImGui::InputFloat("Mass", &mass);
            ImGui::InputFloat("Spring stiffness", &ks);
            ImGui::InputFloat("Gravity", &gravity);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if(ImGui::Button("Begin Simulation") && shouldSimulate){
                threadShouldLive=true;
                shouldSimulate=false;
                timer_start(1, objectList);
            }
            if(ImGui::Button("Set wind")){
                for(auto i: objectList) {
                    setWind(i->cloth, wind);
                }
            }
            if(ImGui::Button("Set mass")){
                for(auto i: objectList) {
                    i->cloth->setMass(mass);
                }
            }
            if(ImGui::Button("Set stiffness")){
                for(auto i: objectList){
                    i->cloth->setStiffness(ks);
                }
            }
            if(ImGui::Button("Set gravity")) {
                for (auto i: objectList) {
                    i->cloth->setGravity(gravity);
                }
            }
                //reset all object and stop the simulation
            if(ImGui::Button("Reset")){
                threadShouldLive=false;
                shouldSimulate=true;
                for(auto i: objectList){
                    i->cloth->reset();
                }
            }
        }

        // Compute the MVP matrix from keyboard and mouse input
        //computeMatricesFromInputs();
        computeMatricesFromInputs(&azimuthal, &polar, distance);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        //Triangle
        glUseProgram(triangleProgramID);
        //to have a double faced surface
        glDisable(GL_CULL_FACE);

        //drawing all of the objects
        for(int i=0; i<objectList.size();++i){
            if(i==0){
                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture, textureId, programId);
            }else if(i==1){
                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture2, textureId2, programId);
            }else{
                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture3, textureId3, programId);
            }
//            if(i==objectList.size()-1){
//                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture, textureId, programId);
//            }else{
//                drawCloth(ProjectionMatrix, ViewMatrix, triangleMatrixID,objectList[i]);
//            }

        }
        //gui stuff
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    std::cout << "while exit" << std::endl;

    //clean up triangle
    glDeleteProgram(triangleProgramID);
    glDeleteVertexArrays(1, &VertexArrayID);

    //clean up the objectList
    for(auto & i : objectList){
        glDeleteBuffers(1, &i->vertex);
        glDeleteBuffers(1, &i->color);
        delete(i);
    }

    //killing the thread, in case it was alive
    threadShouldLive=false;

    std::cout << "i don't even know" << std::endl;
    // Close OpenGL window and terminate GLFW
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}