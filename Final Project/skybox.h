#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

//#include "model.h"

class Skybox {
private:

	float *skyboxVertices;
	unsigned int *skyboxIndices;

	std::string vertPath;
	std::string fragPath;

	std::vector<std::string> facesSkybox;
	GLuint skyboxTex;

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO; 

	GLuint skyboxProgram;
	GLuint shaderProgram;

public:
	Skybox(const std::string& vertPath, const std::string& fragPath, const std::vector<std::string>& faces);

	void draw(const glm::mat4& view, const glm::mat4& projection);

	GLuint getshaderProgram();
};

#endif