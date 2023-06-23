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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    int img_width,
        img_height,
        colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load("3D/ayaya.png",
            &img_width,
            &img_height,
            &colorChannels,
            0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        img_width,
        img_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

    glViewport(0,
        0,
        width,
        height);

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

    std::string path = "3D/myCube.obj";
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
    // uv
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
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

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f,
    };

    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    /*glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO_UV);*/

    //were working with this VAO
    glBindVertexArray(VAO);
    //were working with this VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        //GL_STATIC_DRAW
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //XYZ
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    GLintptr uvPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        2,
        2, //XYZ
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)uvPtr
    );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])),
        &UV[0],
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );*/

    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float Tx=0, Ty=0, Tz=0;
    float Sx=1, Sy=1, Sz=1;
    float Rx=5, Ry=5, Rz=1;
    float theta=0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        //glm::mat4 projection = glm::ortho(-0.5f,
        //    0.5f,
        //    -0.5f,
        //    0.5f,
        //    -1.f,
        //    1.0f);
       glm::mat4 projection = glm::perspective(
            glm::radians(60.f), 
            height / width,
            1.f,
            100.f);

       glm::mat4 view = glm::lookAt(
           glm::vec3(0.0f, 0.0f, 10.0f), //camera movement
           glm::vec3(0.0f, 0.0f, 0.0f), //taas or baba view
           glm::vec3(0.0f, 1.0f, 0.0f)
       );

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix));

        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

        glUniformMatrix4fv(viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(view));

        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 5);
        /*glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );*/

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