#pragma once
#include <iostream>

#ifndef MODEL_H
#define MODEL_H

#include "stb_image.h"
#include "tiny_obj_loader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class Model3D {
private:
    std::string texturePath;
    GLuint texture;

    std::string normalPath;
    GLuint norm_tex;

    std::string modelPath;
    std::vector<GLfloat> fullVertexData;

    GLuint VAO, VBO;
    GLuint shaderProgram;

    float Tx = 0, Ty = -3.0, Tz = 0;
    float Sx = 1.0, Sy = 1.0, Sz = 1.0;
    float Rx = 0, Ry = 1, Rz = 0;
    float theta = 0;
    glm::mat4 transformation_matrix = glm::mat4(1.0f);

public:
    Model3D(const std::string& texturePath, const std::string& normalPath, const std::string& modelPath);

    void loadTexture();
    GLuint getTexture();

    void loadNormals();
    GLuint getnormTex();

    void loadModel();

    std::vector<GLfloat> getfullVertexData();

    void buffers();
    GLuint getVAO();
    GLuint getVBO();

    glm::vec3 getTrans();
    void transformations(float Tx, float Tz, float theta);
    void transformationsObj(float Tx, float Tz, float Sx, float Sy, float Sz);

    void draw();

    void setUniTex0();
    void setUniTex1();

    void setShader(GLuint shaderProgram);
};

#endif