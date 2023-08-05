#pragma once
#include <iostream>

#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class Light {
private:
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    float ambientStr;
    glm::vec3 ambientColor;
    float specStr;
    float specPhong;
    float intensityLevel;

    GLuint shaderProgram;
public:
    Light();
    void lightPosi();
    glm::vec3& getlightposi();

    void lightCol();
    glm::vec3& getlightcol();

    void ambCol();
    glm::vec3& getambcol();

    void setIntensityLevel();

    void setUnilightPos();
    void setUnilightCol();
    void setUniambCol();

    void setShader(GLuint shaderProgram);
};
#endif