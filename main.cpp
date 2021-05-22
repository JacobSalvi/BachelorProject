// Include standard headers
#include <cstdio>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>

using namespace glm;

#include "shaders/shader.hpp"
#include "shaders/controls.hpp"
#include "Bodies/Deformable/net.h"
#include "shaders/texture.hpp"
#include <functional>

#include "shaders/helperStruct.h"
#include "utilities/helperFunctions.h"
#include "Bodies/Solid/mouseIntersectStruct.h"

#include "utilities/multiThreading.h"
//I have no idea why including this make it compile,
//I mean it isn't used, Clion even complains about it
//but without the linker throws a fit
//whatever hocus pocus it does I don't care as long as it works
// -> Good Job - Claudio Maggioni, an idiot
#include "utilities/stb_image.h"
#include "utilities/skybox.h"
#include "Bodies/Deformable/deformableSphere.h"
#include "Bodies/Deformable/importedModels.h"

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

    glm::vec3 lightPosition(-4.0f, 10.0f, 5.0f);
    //glm::vec3 lightPosition(0.0f, 10.0f, 0.0f);

    //This is likely not gonna work
    //nvm, it worked
    std::vector<deformableObjects *> objectList;
    std::vector<collidable *> collObjects;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
    model = glm::translate(model, glm::vec3(0,5,0));

    //standard: 4 5
    //for the cube using a 40 by 50 grid is better but it burns the cpu
    //addCloth(&objectList, 12, 15, 1, glm::vec3(1.0f,0.0f,0.0f), model, lightPosition,0);
    model = glm::mat4(1);
    model = glm::scale(model, glm::vec3(0.2,0.2,0.2));
    model = glm::translate(model, glm::vec3(0,5,0));
    //addCloth(&objectList, 12, 15, 1, glm::vec3(1.0f,0.0f,0.0f), model, lightPosition,0);
    //addCloth(&objectList, 5,8, 0, glm::vec3(2.0f,0.0f,0.0f), model, lightPosition);
    //addCloth(&objectList, 5,3, 1, glm::vec3(2.0f,0.0f,0.0f), glm::vec3(6.0f,0.0f,0.0f), lightPosition);

#if 1
    // horizontal cloth and sphere
    model = mat4(1);
    model = glm::scale(model, glm::vec3(0.3,0.3,0.3));
    model = glm::translate(model, glm::vec3(0,5,0));
    addCloth(&objectList, 2, 2, 1, glm::vec3(1.0f,0.0f,0.0f), model, lightPosition,0);
    addColl(&collObjects, 0, lightPosition, glm::vec3(1,0,0.5));
    addColl(&collObjects, 2, lightPosition, glm::vec3(0,0,0));
#elif 0
    //cloth falling on cube
    model = mat4(1);
    model = glm::scale(model, glm::vec3(0.2,0.2,0.2));
    model = glm::translate(model, glm::vec3(0,5,0));
    addCloth(&objectList, 15, 0, 1, glm::vec3(1.0f,0.0f,0.0f), model, lightPosition,1);
    addColl(&collObjects, 1, lightPosition, glm::vec3(1.5,0,1.5));
    addColl(&collObjects, 2, lightPosition, glm::vec3(0,0,0));
#elif 0
    //cube falling on sphere and sliding on plane
    //deformable cube
    glm::mat4 cMod(1);
    cMod = glm::translate(cMod, glm::vec3(0,2,0));
    addDefCube(&objectList, glm::vec3(1,1,1), cMod, lightPosition);
    addColl(&collObjects, 0, lightPosition, glm::vec3(1,0,1));
    addColl(&collObjects, 2, lightPosition, glm::vec3(0,0,0));
#elif 0
    //cube falling on cube
    //deformable cube
    glm::mat4 cMod(1);
    cMod = glm::translate(cMod, glm::vec3(0,2,0));
    addDefCube(&objectList, glm::vec3(1,1,1), cMod, lightPosition);
    addColl(&collObjects, 1, lightPosition, glm::vec3(1,0,1));
    addColl(&collObjects, 2, lightPosition, glm::vec3(0,0,0));
#elif 0
    //sphere falling on sphere
    glm::mat4 cMod(1);
    cMod = glm::translate(cMod, glm::vec3(0,2,0));
    addDefSphere(&objectList, glm::vec3(1,1,1), cMod, lightPosition);
    addColl(&collObjects, 0, lightPosition, glm::vec3(0.3,0,0));
    addColl(&collObjects, 2, lightPosition, glm::vec3(0,0,0));
#elif 0
    //sphere falling on cube
    glm::mat4 cMod(1);
    cMod = glm::translate(cMod, glm::vec3(0,2,0));
    addDefSphere(&objectList, glm::vec3(1,1,1), cMod, lightPosition);
    addColl(&collObjects, 1, lightPosition, glm::vec3(0.5,0,0));
    addColl(&collObjects, 2, lightPosition, glm::vec3(0,0,0));
#endif

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //infinite wisdom
    GLuint programId = LoadShaders("./shaders/vertexShader.vertexshader", "./shaders/fragmentShader.fragmentshader");
    GLuint texture = loadDDS("shaders/idk.DDS");
    GLuint textureId = glGetUniformLocation(programId, "myTextureSampler");
    GLuint texture2 = loadDDS("shaders/dak2.DDS");
    GLuint textureId2 = glGetUniformLocation(programId, "myTextureSampler");
    GLuint texture3 = loadDDS("shaders/dak3.DDS");
    GLuint textureId3 = glGetUniformLocation(programId, "myTextureSampler");

    std::vector<helperStruct *> cultureList;
    glm::mat4 cultureMod(1);
    cultureMod = glm::translate(cultureMod, glm::vec3(1, 0,0));
    //culture(&objectList, texture, textureId, cultureMod, lightPosition, programId);


    //phong lighting system
    GLuint lightSysId = LoadShaders("./shaders/shadowVert.vertexshader", "./shaders/shadowFrag.fragmentshader");

    //glfwSetKeyCallback(window, key_callback);
    std::cout << "about to enter while loop" << std::endl;

    std::cout<<"init successful"<<std::endl;

    //gui stuff
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
    static int azimuthal = 90;
    static int polar = 90;
    static float distance = 7.0f;
    static float wind[3]= {0.0f,0.0f,-1.0f};
    static float mass =1.0f;
    static float gravity = -1.0f;
    static float ks = 40.0f;

    //skybox
    GLuint skyboxProgramID = LoadShaders("shaders/skybox.vertexshader","shaders/skybox.fragmentshader");
    skybox * sky = new skybox(skyboxProgramID);


    //loading a custom model
    const char * path;
    std::vector<float>  out_vertices;
    std::vector<float> out_uvs;
    std::vector<float> out_normals;
    std::vector<float> out_tan;
    std::vector<float> out_bitan;
    std::vector<unsigned int> out_tris;
    //loadObj("./shaders/sphere.obj", out_vertices, out_uvs, out_normals, out_tan, out_bitan, out_tris);

    glm::mat4 mod(1);
    importedModels * chonky = new importedModels("./shaders/sphere.obj", mod);
    importedModels * teapot = new importedModels("./shaders/t2.obj", mod);

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
            if(ImGui::Button("Drop")){
                for(auto i: objectList){
                    i->emptySpecialParticles();
                }
            }
            if(ImGui::Button("Set wind")){
                for(auto i: objectList) {
                    setWind(i, wind);
                }
            }
            if(ImGui::Button("Set mass")){
                for(auto i: objectList) {
                    i->setMass(mass);
                }
            }
            if(ImGui::Button("Set stiffness")){
                for(auto i: objectList){
                    i->setStiffness(ks);
                }
            }
            if(ImGui::Button("Set gravity")) {
                for (auto i: objectList) {
                    i->setGravity(gravity);
                }
            }
                //reset all object and stop the simulation
            if(ImGui::Button("Reset")){
                threadShouldLive=false;
                shouldSimulate=true;
                for(auto i: objectList){
                    i->reset();
                }
            }
        }

        if(ImGui::IsMouseClicked(0) && !dragThreadShouldLive && !ImGui::IsMouseHoveringWindow()){
            dragThreadShouldLive=true;
            glm::vec3 pixelRay = getMouseRay(ImGui::GetMousePos());
            glm::vec3 cameraPos = getCameraPosition();
            mouseIntersectStruct tmp = isMouseOverColl(cameraPos, pixelRay, &collObjects);
            helperStruct tmpColl = isMouseOverDeformable(cameraPos, pixelRay, &objectList);

            //let's find the closest object to the mouse
            if(tmp.isMouseOver && !tmpColl.isMouseOver){
                dragMouse(tmp);
            }else if(!tmp.isMouseOver && tmpColl.isMouseOver){
                //in my infinite knowledge I had forgotten to chek whether the thread was already running
                if(!threadShouldLive){
                    //start the other thread
                    threadShouldLive=true;
                    shouldSimulate=false;
                    timer_start(1, objectList, collObjects);
                }
                dragMouse(tmpColl);
            }else if(tmp.isMouseOver && tmpColl.isMouseOver){
                //find the closest
                if(glm::length(tmp.point-cameraPos)<glm::length(tmpColl.point->getPosition()-cameraPos)){
                    dragMouse(tmp);
                }else{
                    if(!threadShouldLive){
                        //start the other thread
                        threadShouldLive=true;
                        shouldSimulate=false;
                        timer_start(1, objectList, collObjects);
                    }
                    dragMouse(tmpColl);
                }
            }
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
        //to have a double faced surface
        glDisable(GL_CULL_FACE);

        //I am not sure why rendering it first fixes everything but it does
        sky->render(ProjectionMatrix, ViewMatrix);

        //drawing all of the objects
        for(int i=0; i<objectList.size();++i){
            objectList[i]->render(ProjectionMatrix, ViewMatrix, lightSysId);
        }

        for(auto i : collObjects){
           i->render(ProjectionMatrix, ViewMatrix, lightSysId, false);
        }

        // please work
        chonky->render(ProjectionMatrix, ViewMatrix, programId, texture3, textureId);
        //teapot->render(ProjectionMatrix, ViewMatrix, programId, texture, textureId);

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
    glDeleteProgram(programId);
    glDeleteProgram(lightSysId);
    glDeleteVertexArrays(1, &VertexArrayID);

    //killing the threads, in case they were alive
    threadShouldLive=false;
    dragThreadShouldLive=false;
    //to make sure the threads actually die
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    //clean up the objectList
    for(auto & i : objectList){
        delete(i);
    }

    //clean up
    for(auto &i : collObjects){
        delete(i);
    }

    std::cout << "i don't even know" << std::endl;
    // Close OpenGL window and terminate GLFW
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}