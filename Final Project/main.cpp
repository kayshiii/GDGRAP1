#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include <string>

#include "model.h"
#include "shader.h"
#include "skybox.h"
#include "camera.h"
#include "light.h"
#include "player.h"

// Declare global variables
float posX = 0.0f;
float posY = 0.0f;
float posZ = 0.0f;
float tankRotation = 0.0f;

float offsetX = 0.0f;
float offsetY = 0.0f;
float offsetZ = 0.0f;

glm::vec3 camcen(0.0f, 0.0f, 10.0f);

float defaultFOV = 60.0f; 
float currentFOV = defaultFOV;

double lastMouseX = 0.0;
double lastMouseY = 0.0;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
float cameraHeightAboveTank = 10.0f;
int cameraMode = 0;

// Function to handle keyboard inputs
void Key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mod) {
    float speed = 0.5f; 
    if (cameraMode == 0) {
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            posX += speed * sin(glm::radians(tankRotation));
            posZ += speed * cos(glm::radians(tankRotation));
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            posX -= speed * sin(glm::radians(tankRotation));
            posZ -= speed * cos(glm::radians(tankRotation));
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            tankRotation += 1.5f;
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            tankRotation -= 1.5f;
        }
    }
    if (cameraMode == 1) {
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            camcen += speed * cameraFront;
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            camcen -= speed * cameraFront;
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            // Strafe left based on the camera's orientation (yaw)
            camcen += glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * speed;
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            // Strafe right based on the camera's orientation (yaw)
            camcen -= glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * speed;
        }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) { // Zoom in
            currentFOV -= 5.0f; 
            if (currentFOV < 10.0f) { // Set a minimum FOV to prevent excessive zooming
                currentFOV = 10.0f;
            }
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS) { // Zoom out
            currentFOV += 5.0f; 
            if (currentFOV > 100.0f) { // Set a maximum FOV to prevent excessive zooming out
                currentFOV = 100.0f;
            }
        }
    }
    if (cameraMode == 2) {
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            offsetZ += 1;
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) {
            offsetZ += -1;
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {
            offsetX += 1;
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) {
            offsetX += -1;
        }
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        if (cameraMode == 0) {
            cameraMode = 1;
        }
        else {
            cameraMode = 0;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        cameraMode = 2;
    }
}

// Function to handle mouse inputs
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos) {
    if (cameraMode == 0) {
        if (firstMouse)
        {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastMouseX;
        float yoffset = lastMouseY - ypos;
        lastMouseX = xpos;
        lastMouseY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float height = 600.0f;
    float width = 600.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Kassandra So", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* make the windows context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    // Load 3D models and textures
    Model3D tankModel("3D/military.jpg", "3D/used.jpg", "3D/tank.obj");
    tankModel.loadTexture();
    tankModel.loadNormals();
    tankModel.loadModel();
    tankModel.buffers();

    Model3D groundModel("3D/military.jpg", "3D/crack.jpg", "3D/ground.obj");
    groundModel.loadTexture();
    groundModel.loadNormals();
    groundModel.loadModel();
    groundModel.buffers();

    Model3D phoneModel("3D/call.jpg", "3D/crack.jpg", "3D/phone.obj");
    phoneModel.loadTexture();
    phoneModel.loadNormals();
    phoneModel.loadModel();
    phoneModel.buffers();

    Model3D skullModel("3D/old.jpg", "3D/crack.jpg", "3D/skull.obj");
    skullModel.loadTexture();
    skullModel.loadNormals();
    skullModel.loadModel();
    skullModel.buffers();

    Model3D letterModel("3D/help.jpg", "3D/used.jpg", "3D/letter.obj");
    letterModel.loadTexture();
    letterModel.loadNormals();
    letterModel.loadModel();
    letterModel.buffers();

    Model3D graveModel("3D/old.jpg", "3D/rough.jpg", "3D/grave.obj");
    graveModel.loadTexture();
    graveModel.loadNormals();
    graveModel.loadModel();
    graveModel.buffers();

    Model3D cutterModel("3D/old.jpg", "3D/used.jpg", "3D/cutter.obj");
    cutterModel.loadTexture();
    cutterModel.loadNormals();
    cutterModel.loadModel();
    cutterModel.buffers();

    Model3D ropeModel("3D/old.jpg", "3D/rough.jpg", "3D/rope.obj");
    ropeModel.loadTexture();
    ropeModel.loadNormals();
    ropeModel.loadModel();
    ropeModel.buffers();

    MyCamera camera(width, height);
    Light light;

    glfwSetKeyCallback(window, Key_CallBack);
    glfwSetCursorPosCallback(window, Mouse_Callback);

    // Load shaders and set them for each model
    Shaders shader("Shaders/sample.vert", "Shaders/sample.frag");
    shader.vertFrag();
    tankModel.setShader(shader.getshaderProgram());
    groundModel.setShader(shader.getshaderProgram());
    phoneModel.setShader(shader.getshaderProgram());
    skullModel.setShader(shader.getshaderProgram());
    letterModel.setShader(shader.getshaderProgram());
    graveModel.setShader(shader.getshaderProgram());
    cutterModel.setShader(shader.getshaderProgram());
    ropeModel.setShader(shader.getshaderProgram());

    std::fstream sky_vertSrc("Shaders/skybox.vert");
    std::stringstream sky_vertBuff;
    sky_vertBuff << sky_vertSrc.rdbuf(); //add
    std::string sky_vertS = sky_vertBuff.str(); //convert
    const char* sky_v = sky_vertS.c_str();

    std::fstream sky_fragSrc("Shaders/skybox.frag");
    std::stringstream sky_fragBuff;
    sky_fragBuff << sky_fragSrc.rdbuf(); //add
    std::string sky_fragS = sky_fragBuff.str(); //convert
    const char* sky_f = sky_fragS.c_str();

    GLuint sky_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sky_vertShader, 1, &sky_v, NULL);
    glCompileShader(sky_vertShader);

    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
    glCompileShader(sky_fragShader);

    GLuint skyboxProgram = glCreateProgram();
    glAttachShader(skyboxProgram, sky_vertShader);
    glAttachShader(skyboxProgram, sky_fragShader);

    glLinkProgram(skyboxProgram);

    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
    //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    std::string facesSkybox[]{
        "Skybox/right.png",
        "Skybox/left.png",
        "Skybox/up.png",
        "Skybox/down.png",
        "Skybox/front.png",
        "Skybox/back.png"
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
        }
    }
    stbi_set_flip_vertically_on_load(true);

    // For blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, //foreground layer
        GL_ONE_MINUS_SRC_ALPHA //background layer
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle camera view and transformations based on camera mode
        if (cameraMode == 0) {  // 3rd pov
            camera.persp();
            float cameraOffsetX = -5.0f * sin(glm::radians(tankRotation));
            float cameraOffsetZ = -5.0f * cos(glm::radians(tankRotation));
            camera.setCamPos(posX + cameraOffsetX, posY, posZ + cameraOffsetZ);
            camera.viewMatPersp(cameraFront, tankModel.getTrans());
        }
        if (cameraMode == 1) { // 1st pov
            camera.persp1(currentFOV);
            float cameraOffsetX = 2.0f * sin(glm::radians(tankRotation));
            float cameraOffsetY = 2.0f * cos(glm::radians(tankRotation));
            cameraFront = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
            camera.move1(cameraFront);
            camera.viewMat(camcen + glm::vec3(cameraOffsetX, cameraOffsetY, 0.0f));
        }
        else if (cameraMode == 2) { // Birds eye
            camera.ortho();
            camera.move(glm::vec3(offsetX, 0.0f, offsetZ));
            camera.viewMatOrtho();
        }

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxProgram);

        // Render skybox
        glm::mat4 skyView = glm::mat4(1.f);
        skyView = glm::mat4(
            glm::mat3(camera.getView())
        );

        unsigned int sky_ProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
        glUniformMatrix4fv(sky_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProj()));

        unsigned int sky_ViewLoc = glGetUniformLocation(skyboxProgram, "view");
        glUniformMatrix4fv(sky_ViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // Set shader and lighting properties for the scene
        glUseProgram(shader.getshaderProgram());

        light.lightPosi();
        light.lightCol();
        light.ambCol();
        float ambientStr = 0.1f;
        float specStr = 5.0f;
        float specPhong = 16;

        camera.setShader(shader.getshaderProgram());
        camera.setUniProj();
        camera.setUniView();

        light.setShader(shader.getshaderProgram());
        light.setUnilightPos();
        light.setUnilightCol();

        GLuint ambientStrAddress = glGetUniformLocation(shader.getshaderProgram(), "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        light.setUniambCol();
        camera.setUniCamPos();

        GLuint specStrAddress = glGetUniformLocation(shader.getshaderProgram(), "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shader.getshaderProgram(), "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        // Render 3D models in the scene
        tankModel.setUniTex0();
        tankModel.setUniTex1();
        tankModel.transformations(posX, posZ, tankRotation);
        tankModel.draw();

        groundModel.setUniTex0();
        groundModel.setUniTex1();
        groundModel.transformations(0, 0, 0);
        groundModel.draw();

        phoneModel.setUniTex0();
        phoneModel.setUniTex1();
        phoneModel.transformations(3, 0, 0);
        phoneModel.draw();

        skullModel.setUniTex0();
        skullModel.setUniTex1();
        skullModel.transformations(7, 0, 0);
        skullModel.draw();

        letterModel.setUniTex0();
        letterModel.setUniTex1();
        letterModel.transformations(12, 0, 0);
        letterModel.draw();

        graveModel.setUniTex0();
        graveModel.setUniTex1();
        graveModel.transformations(10, 0, 0);
        graveModel.draw();

        cutterModel.setUniTex0();
        cutterModel.setUniTex1();
        cutterModel.transformations(-5, 0, 0);
        cutterModel.draw();

        ropeModel.setUniTex0();
        ropeModel.setUniTex1();
        ropeModel.transformations(-7, 0, 0);
        ropeModel.draw();

        /* swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    // Cleanup and exit
   /* glDeleteVertexArrays(1, &tankModel.getVAO());
    glDeleteBuffers(1, &VBO);*/

    glfwTerminate();
    return 0;
}