#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model3D {
private:
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    std::vector<GLuint> mesh_indices;
    glm::mat4 transformation_matrix;
    std::string objFilePath;

public:
    glm::vec3 position;
    float theta;
    float theta1;

    Model3D(const std::string& filePath)
        : objFilePath(filePath), position(0.0f, 0.0f, 0.0f)
    {
        init();
    }

    void init()
    {
        // Load vertex shader source code from file
        std::fstream vertSrc("Shaders/sample.vert");
        std::stringstream vertBuff;
        vertBuff << vertSrc.rdbuf();
        std::string vertS = vertBuff.str();
        const char* v = vertS.c_str();

        // Load fragment shader source code from file
        std::fstream fragSrc("Shaders/sample.frag");
        std::stringstream fragBuff;
        fragBuff << fragSrc.rdbuf();
        std::string fragS = fragBuff.str();
        const char* f = fragS.c_str();

        // Compile and link shaders
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &v, NULL);
        glCompileShader(vertShader);

        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &f, NULL);
        glCompileShader(fragShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);

        // Load the 3D model using tinyobjloader
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;

        tinyobj::attrib_t attributes;

        bool success = tinyobj::LoadObj(
            &attributes,
            &shapes,
            &materials,
            &warning,
            &error,
            objFilePath.c_str()
        );

        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
        }

        // Generate and bind vertex array object (VAO)
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        this->shaderProgram = shaderProgram;
    }

    void draw(float width, float height)
    {
        glm::mat4 identity_matrix4 = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(60.f), height / width, 1.f, 100.f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        view = glm::rotate(view, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(theta1), glm::vec3(1.0f, 0.0f, 0.0f));

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update()
    {
        transformation_matrix = glm::translate(glm::mat4(1.0f), position);
    }
};

float x_mod = 0;
float y_mod = 0;
float z_mod = 0;
float cameraSpeed = 1.0f;
float sensitivity = 0.1f;

bool canSpawn = true; // Declare canSpawn outside the Key_CallBack function
std::vector<Model3D> spawnedModels; // List of spawned models

void Key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    static double lastSpawnTime = glfwGetTime();
    static bool canSpawn = true;
    static bool isSpaceKeyPressed = false;
    //static Model3D spawnedModel("3D/banana.obj"); // Initialize the spawned model object

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += cameraSpeed;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        x_mod -= cameraSpeed;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        z_mod += cameraSpeed;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        z_mod -= cameraSpeed;
    }
    /*if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        model.theta -= 5.0f;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        model.theta += 5.0f;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        model.theta1 += 5.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        model.theta1 -= 5.0f;
    }*/

    if (key == GLFW_KEY_SPACE)
    {
        if (action == GLFW_PRESS && !isSpaceKeyPressed)
        {
            isSpaceKeyPressed = true;
            double currentTime = glfwGetTime();
            double cooldown = 3.0; // 3-second cooldown

            if (currentTime - lastSpawnTime >= cooldown)
            {
                lastSpawnTime = currentTime;

                Model3D newModel("3D/banana.obj"); // Create a new instance of Model3D
                newModel.position = glm::vec3(x_mod, y_mod, z_mod - 5.0f); // Adjust the position of the new model
                newModel.update();
                spawnedModels.push_back(newModel); // Add the new instance to the list of spawned models
            }
        }
        else if (action == GLFW_RELEASE)
        {
            isSpaceKeyPressed = false;
        }
    }
}

// Mouse callback function
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = xpos;
    static float lastY = ypos;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Update camera yaw and pitch
    x_mod += xoffset;
    y_mod += yoffset;

    // Clamp pitch to avoid flipping upside down
    if (y_mod > 89.0f)
        y_mod = 89.0f;
    if (y_mod < -89.0f)
        y_mod = -89.0f;
}

int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    float height = 600.0f;
    float width = 600.0f;

    window = glfwCreateWindow(width, height, "Kassandra So", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, width, height);

    glfwSetKeyCallback(window, Key_CallBack);
    glfwSetCursorPosCallback(window, Mouse_Callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Model3D model("3D/banana.obj");
    //Model3D spawnedModel("3D/banana.obj");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        model.position.x = x_mod;
        model.position.y = y_mod;
        model.position.z = z_mod;
        model.update();
        model.draw(width, height);

        // Update and draw the spawned models
        for (size_t i = 0; i < spawnedModels.size(); ++i) {
            // Update the position of each spawned model based on the camera position
            spawnedModels[i].position.x = x_mod;
            spawnedModels[i].position.y = y_mod;
            spawnedModels[i].position.z = z_mod - 5.0f; // Adjust the position as needed
            spawnedModels[i].update();
            // Draw each spawned model
            spawnedModels[i].draw(width, height);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
//3D Model Banana is from sketchfab.com