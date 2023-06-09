// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

float x_mod = 0;
float y_mod = 0;

void Key_CallBack(
    GLFWwindow* window,
    int key,
    int scancode,
    int action, //press/release
    int mod
) {
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += 0.1f;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        x_mod -= 0.1f;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        y_mod += 0.1f;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        y_mod -= 0.1f;
    }
}

int main(void)
{
    //start of initilization
    //creates window
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "Kassandra So", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    //glfwMakeContextCurrent(window);
    //end of initialization

    /* make the windows context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    // add
    vertBuff << vertSrc.rdbuf();
    //convert
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    //add
    fragBuff << fragSrc.rdbuf();
    //convert
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    glfwSetKeyCallback(window, Key_CallBack);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

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
        // x  y  z
        0.f, 0.5, 0.f, //0
        -0.5f, -0.5f, 0.f, //1
        0.5f, -0.5, 0.f//2
    };

    GLuint indices[]{
        0, 1, 2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //were working with this VAO
    glBindVertexArray(VAO);
    //were working with this VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
        //GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //XYZ
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float Tx=0, Ty=0, Tz=0;
    float Sx=1, Sy=1, Sz=1;
    float Rx=1, Ry=1, Rz=1;
    float theta=60;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //x_mod += 0.001f;

        glm::mat4 identity_matrix4 = glm::mat4(1.0f);

        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix4,
            glm::vec3(Tx, Ty, Tz)
        );
        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(Sx, Sy, Sz)
        );
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(Rx, Ry, Rz))
        );

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix));

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );

        /* swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    /* Loop until the user closes the window */
    /*
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        /*
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES); //top
        glVertex2f(0.f, 0.5f);
        glVertex2f(-0.5f, 0.f);
        glVertex2f(0.5f, 0.f);
        glEnd();

        glBegin(GL_TRIANGLES); //bot left
        glVertex2f(-0.3f, -0.5f);
        glVertex2f(0.3f, -0.5f);
        glVertex2f(-0.5f, -0.f);
        glEnd();

        glBegin(GL_TRIANGLES); //bot left
        glVertex2f(0.3f, -0.5f);
        glVertex2f(-0.5f, 0.f);
        glVertex2f(0.5f, 0.f);
        glEnd();
        */

        /* Swap front and back buffers */
        //glfwSwapBuffers(window);

        /* Poll for and process events */
        //glfwPollEvents();
    //}

    glfwTerminate();
    return 0;
}