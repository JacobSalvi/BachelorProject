#include "controls.hpp"
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

using namespace glm;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::vec3 cameraPosition;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}

glm::vec3 getCameraPosition(){
    return cameraPosition;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 50);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 90.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(){

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024/2 - xpos );
    //verticalAngle   += mouseSpeed * float( 768/2 - ypos );
    if(verticalAngle+mouseSpeed * float( 768/2 - ypos )<1.5f&&verticalAngle+mouseSpeed * float( 768/2 - ypos )>-1.5f){
        verticalAngle   += mouseSpeed * float( 768/2 - ypos );
    }
    //printf("%f\n", verticalAngle);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross( right, direction );

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
        position -= right * deltaTime * speed;
    }

    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
            position,           // Camera is here
            position+direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

//compute viewMatrix and projectionMatrix based on the input
//this code might or might not be inspired from last year code
void computeMatricesFromInputs(int * azimuthal, int* polar, float distance){
    //compute angles
    float az = ((float) *azimuthal) /360.0f*2.0f*M_PI;
    float po = ((float)*polar)/360.0f*2.0f*M_PI;

    //compute camera position
    float x = distance*sin(po)*cos(az);
    float y = distance*cos(po);
    float z = distance*sin(po)*sin(az);

    cameraPosition = glm::vec3(x,y,z);

    ViewMatrix= glm::lookAt(glm::vec3(x,y,z), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));

    ProjectionMatrix = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
}

//check whether the mouse touches an object
glm::vec3 getMouseRay(ImVec2 mousePosition){
    //step 1
    float x = (2.0f*mousePosition[0])/1024.0f-1.0f;
    float y = 1.0f -(2.0f*mousePosition[1])/768.0f;
    float z = 1.0f;
    glm::vec3 ray_nds(x,y,z);

    //step2
    glm::vec4 ray_clip(x, y, -1.0f, 1.0f);

    //step3
    glm::vec4 ray_eye = glm::inverse(ProjectionMatrix)*ray_clip;
    ray_eye.z=-1.0f;
    ray_eye.w=0.0f;

    //step4
    glm::vec4 tmp = (glm::inverse(ViewMatrix)*ray_eye);
    glm::vec3 ray_wor(tmp.x,tmp.y,tmp.z);
    ray_wor = glm::normalize(ray_wor);

    return ray_wor;
}