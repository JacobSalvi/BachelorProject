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
#include "Collidables/Sphere.h"
#include <thread>
#include <functional>

#include "shaders/helperStruct.h"
#include "shaders/helperFunctions.h"

void timer_start(unsigned int interval);

void dragMouse();

bool shouldSimulate = true;
std::thread sim;

bool dragThreadShouldLive = false;
std::thread dragThread;

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
void timer_start(unsigned int interval, const std::vector<helperStruct *>& list, const std::vector<collidable *> &collList) {
    sim = std::thread([interval, list, collList]() {
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

                for(int i=0; i<collList.size();++i){
                    for(int j=i+1; j<collList.size(); ++j){
                        collList[i]->handleCollision(collList[j]);
                    }
                }
                counter=0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        std::cout<<"The thread should be dead now"<<std::endl;
    });
    sim.detach();
}

void dragMouse(){
    dragThread = std::thread([](){
        while(dragThreadShouldLive){
            ImVec2 tmp = ImGui::GetMousePos();
            //std::cout<<tmp[0]<<" "<<tmp[1]<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
    dragThread.detach();
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
    window = glfwCreateWindow(1024, 768, "Please God Work", nullptr, nullptr);
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
    addCloth(&objectList, 3, 9, 0, glm::vec3(1.0f,0.0f,0.0f), glm::vec3(2.0f,0.0f,0.0f));
    addCloth(&objectList, 5,8, 0, glm::vec3(2.0f,0.0f,0.0f), glm::vec3(-4.0f,0.0f,0.0f));
    addCloth(&objectList, 5,3, 1, glm::vec3(2.0f,0.0f,0.0f), glm::vec3(6.0f,0.0f,0.0f));


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
//    culture(&objectList, glm::vec3(-8.0f, 0.0f,0.0f));
//    culture(&objectList, glm::vec3(0.0f, 0.0f,0.0f));
//    culture(&objectList, glm::vec3(8.0f, 0.0f,0.0f));


    //phong lighting system
    //GLuint lightSysId = LoadShaders("shaders/shadowVert.vertexshader", "shaders/shadowFrag.fragmentshader");
    //glm::vec3 lightPosition(0.0f, 3.0f, 5.0f);

    //glfwSetKeyCallback(window, key_callback);
    std::cout << "about to enter while loop" << std::endl;

    //Collidables
    std::vector<collidable *> collObjects;
    addColl(&collObjects, 2);
    addColl(&collObjects, 0);
    addColl(&collObjects,1);
    std::cout<<"init successful"<<std::endl;

    //gui stuff
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    static int azimuthal = 90;
    static int polar = 90;
    static float distance = 10.0f;
    static float wind[3]= {0.0f,0.0f,-1.0f};
    static float mass =1.0f;
    static float gravity = -1.0f;
    static float ks = 40.0f;


    int idk =0;
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
            ImGui::InputFloat("Distance", &distance);
            ImGui::InputFloat3("Wind", wind);
            ImGui::InputFloat("Mass", &mass);
            ImGui::InputFloat("Spring stiffness", &ks);
            ImGui::InputFloat("Gravity", &gravity);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if(ImGui::Button("Begin Simulation") && shouldSimulate){
                threadShouldLive=true;
                shouldSimulate=false;
                timer_start(1, objectList, collObjects);
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

        if(ImGui::IsMouseClicked(0) && !dragThreadShouldLive){
            dragThreadShouldLive=true;
            glm::vec3 origin(0.0f,0.0f,10.0f);
            glm::vec3 pixelRay = getMouseRay(ImGui::GetMousePos());
            glm::vec3 pos(0.0f,0.0f, 0);
            collidable * tmp = isMouseOverObject(getCameraPosition(), pixelRay, &collObjects);
//            if(tmp !=NULL){
//                std::cout<< "not null" <<std::endl;
//            }else{
//                std::cout<<"Null" <<std::endl;
//            }
            dragMouse();
        }
        if(ImGui::IsMouseReleased(0)){
            dragThreadShouldLive =false;
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
//            if(i==0){
//                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture, textureId, programId);
//            }else if(i==1){
//                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture2, textureId2, programId);
//            }else{
//                drawCulture(ProjectionMatrix, ViewMatrix, matrixId, objectList[i], texture3, textureId3, programId);
//            }
            drawCloth(ProjectionMatrix, ViewMatrix, triangleMatrixID,objectList[i]);
        }

        for(auto i : collObjects){
            drawColl(ProjectionMatrix, ViewMatrix, triangleMatrixID, i);
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

    //clean up
    for(auto &i : collObjects){
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