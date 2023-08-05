	#include "light.h"

Light::Light() : lightPos(glm::vec3(-10, 3, 0)), lightColor(glm::vec3(1, 1, 1)), 
ambientStr(0.1f), ambientColor(lightColor), specStr(5.0f), specPhong(16), intensityLevel(1.0f), shaderProgram(0) {}

void Light::lightPosi() {
	lightPos = glm::vec3(-10, 3, 0);
}
glm::vec3& Light::getlightposi() {
	return lightPos;
}

void Light::lightCol() {
	lightColor = glm::vec3(1, 1, 1);
}
glm::vec3& Light::getlightcol() {
	return lightColor;
}

void Light::ambCol() {
	ambientColor = lightColor;
}
glm::vec3& Light::getambcol() {
	return ambientColor;
}

void Light::setIntensityLevel() { 
	intensityLevel = intensityLevel; 
}

void Light::setUnilightPos() {
	GLuint lightAddress = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
}
void Light::setUnilightCol() {
	glm::vec3 adjustedLightColor = lightColor * intensityLevel;
	GLuint lightColorAddress = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3fv(lightColorAddress, 1, glm::value_ptr(adjustedLightColor));
}
void Light::setUniambCol() {
	GLuint ambientColorAddress = glGetUniformLocation(shaderProgram, "ambientColor");
	glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));
}

void Light::setShader(GLuint shaderProgram) {
	this->shaderProgram = shaderProgram;
}