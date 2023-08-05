#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class Shaders {
private:
	std::string vertPath;
	std::string fragPath;
	GLuint shaderProgram;
public:
	Shaders(const std::string& vertPath, const std::string& fragPath);
	void vertFrag();
	GLuint getshaderProgram();
};

#endif