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
        stbi_load("3D/brickwall.jpg",
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
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    int img_width2, img_height2, colorChannel2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* normal_bytes =
        stbi_load("3D/brickwall_normal.jpg",
            &img_width2,
            &img_height2,
            &colorChannel2,
            0
        );

    glEnable(GL_DEPTH_TEST);

    glViewport(0,
        0,
        width,
        height);

    GLuint norm_tex;

    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width2,
        img_height2,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        normal_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(normal_bytes);

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

    glfwSetKeyCallback(window, Key_CallBack);

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
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png"
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

    std::string path = "3D/plane.obj";
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

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shape[0].mesh.indices.size(); i += 3) {

        tinyobj::index_t vData1 = shape[0].mesh.indices[i];
        tinyobj::index_t vData2 = shape[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shape[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
            );
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
            );
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
            );
        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV2.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    };

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }
    // uv
    std::vector<GLfloat> fullVertexData;
    std::vector<GLfloat> normals;

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

        // assignment for sword
        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );
        //
        fullVertexData.push_back(
            tangents[i].x
        );
        fullVertexData.push_back(
            tangents[i].y
        );
        fullVertexData.push_back(
            tangents[i].z
        );
        fullVertexData.push_back(
            bitangents[i].x
        );
        fullVertexData.push_back(
            bitangents[i].y
        );
        fullVertexData.push_back(
            bitangents[i].z
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
        14 * sizeof(float),
        (void*)0
    );

    GLintptr normalPtr = 3 * sizeof(float); // + 3 for normals
    glVertexAttribPointer(
        1,
        3, //XYZ
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),  // 8 na laman
        (void*)normalPtr
    );

    GLintptr uvPtr = 6 * sizeof(float); // + 3 for normals
    glVertexAttribPointer(
        2,
        2, //XYZ
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),  // 8 na laman
        (void*)uvPtr
    );

    GLintptr tangentPtr = 8 * sizeof(float);
    GLintptr bitangentPtr = 11 * sizeof(float);

    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)tangentPtr
    );

    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)bitangentPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

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
    float Sx=2.0, Sy=2.0, Sz=2.0;
    float Rx=5, Ry=5, Rz=1;
    float theta=0;

    //for blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, //foreground layer
        GL_ONE_MINUS_SRC_ALPHA //background layer
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //x_mod += 0.001f;
        theta += 0.01f; // Update the rotation angle

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

       glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f); //camera movement

       glm::mat4 view = glm::lookAt(
           glm::vec3(0.0f, 0.0f, 10.0f), //camera movement
           glm::vec3(0.0f, 0.0f, 0.0f), //taas or baba view
           glm::vec3(0.0f, 1.0f, 0.0f)
       );

       glDepthMask(GL_FALSE);
       glDepthFunc(GL_LEQUAL);
       glUseProgram(skyboxProgram);

       glm::mat4 skyView = glm::mat4(1.f);
       skyView = glm::mat4(
           glm::mat3(view)
       );

       unsigned int sky_ProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
       glUniformMatrix4fv(sky_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

       unsigned int sky_ViewLoc = glGetUniformLocation(skyboxProgram, "view");
       glUniformMatrix4fv(sky_ViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

       glBindVertexArray(skyboxVAO);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

       glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

       glDepthMask(GL_TRUE);
       glDepthFunc(GL_LESS);

       glUseProgram(shaderProgram);

       glm::vec3 lightPos = glm::vec3(-10, 3, 0);
       glm::vec3 lightColor = glm::vec3(1, 1, 1);

       float ambientStr = 0.1f;
       glm::vec3 ambientColor = lightColor;

       float specStr = 5.0f;
       float specPhong = 16;

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

        GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProgram, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProgram, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProgram, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0loc = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0loc, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1loc = glGetUniformLocation(shaderProgram, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1loc, 0);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
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