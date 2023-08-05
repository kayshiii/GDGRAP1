#pragma once
#include <iostream>

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "model.h"

class MyCamera {
private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPos;
    glm::vec3 center;
    glm::vec3 up;

    float width;
    float height;

    float yaw;
    float pitch;

    GLuint shaderProgram;

public:
    MyCamera(float width, float height);

    void persp();
    void persp1(float fov);
    void ortho();

    void move(glm::vec3 offset);
    void move1(glm::vec3 offset);

    void viewMat(glm::vec3 x);
    void viewMatOrtho();

    void firstPerson(glm::vec3 position, float yaw, float pitch);
    void viewMatPersp(glm::vec3 x, glm::vec3 y);

    void setCamPos(float x, float y, float z);
    void setCen(glm::vec3 x);
    glm::vec3& getCamPos();

    glm::mat4& getProj();
    glm::mat4& getView();

    void setUniProj();
    void setUniView();
    void setUniCamPos();

    void setShader(GLuint shaderProgram);

};


#endif