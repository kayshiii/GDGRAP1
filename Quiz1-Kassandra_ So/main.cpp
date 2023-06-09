
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Define the viewport parameters for each instance
struct Viewport {
    int x, y, width, height;
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Kassandra So", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //initializers
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL());
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL());
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    std::vector<Viewport> viewports = {
    {0, -100, 600, 600},   // First instance viewport
    {-100, 0, 600, 600},  // Second instance viewport
    {100, 0, 600, 600}  // third instance viewport
    };

    glm::mat4 identity_matrix4 = glm::mat4(1.0f);
    
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }

    GLfloat vertices[]{
     //     x,  y,  z
        -0.5f, -0.5f, 0, //0
        0, 0.5f, 0, //1
        0.5, -0.5f, 0 //2
    };

    GLuint indices[]{
        0, 1, 2
    };

    //create vao/vbo
    GLuint VAO, VBO, EBO;
    //initialize
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //we work wt this vao any call auto points to vao
    glBindVertexArray(VAO); //collection of vao
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind vbo to vao
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW //optimization 
    );

    glVertexAttribPointer(
        0,//index position 1 uv, 2 texture
        3, //xyz
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GLfloat),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //wala ka na nawwork

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        //glDrawArrays(GL_TRIANGLES, 0, 3); // primitve, index, points
       /* glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );*/

        for (int i = 0; i < viewports.size(); i++) {
            // Set the viewport for the current instance
            glViewport(viewports[i].x, viewports[i].y, viewports[i].width, viewports[i].height);

            //create a rotation matrix for each instance
            glm::mat4 rotation_matrix = glm::rotate(
                identity_matrix4,
                glm::radians(90.0f),
                glm::vec3(0, 
                    0, 
                    1)
            );

            //pass the rotation matrix to the shader
            unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLoc, 
                1, 
                GL_FALSE, 
                glm::value_ptr(rotation_matrix));

            //render the object
            glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}