#pragma once
#include <iostream>

#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class Player {
private: 
	float tankPosX = 0.0f;
	float tankPosY = 0.0f;
	float tankPosZ = 0.0f;
	float tankRotation = 0.0f;

public: 
	Player();
	void Key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mod);
};

#endif