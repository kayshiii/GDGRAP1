#include "camera.h"
#include "shader.h"

// Constructor
MyCamera::MyCamera(float width, float height)
    : width(width), height(height), projection(glm::perspective(glm::radians(60.f), width / height, 1.f, 100.f)),
      cameraPos(glm::vec3(0.0f, 0.0f, 10.0f)), center(glm::vec3(0.0f, 0.0f, 0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), shaderProgram(0) {}
// Set perspective projection (for 3RD POV)
void MyCamera::persp() {
    projection = glm::perspective(glm::radians(60.f), height / width, 1.f, 100.f);
}// Set perspective projection with custom FOV (for FPS)
void MyCamera::persp1(float fov) {
    projection = glm::perspective(glm::radians(fov), height / width, 1.f, 100.f);
}
// Set orthographic projection
void MyCamera::ortho() {
    projection = glm::ortho(-10.5f, 10.5f, -10.5f, 10.5f, -100.f, 100.0f);
    this->cameraPos = glm::vec3(0.0f, 20.0f, 0.0f);
    this->center = glm::vec3(0.0f, 0.0f, 0.0f);
    this->up = glm::vec3(0.0f, 0.0f, 1.0f);
}
// Move the camera's center (useful for moving the focus point while keeping the camera position unchanged)
void MyCamera::move(glm::vec3 offset) {
    this->center += offset;
    view = glm::lookAt(cameraPos, center, up); 
}
// Move the camera's position (useful for moving the camera's position and focus point together)
void MyCamera::move1(glm::vec3 offset) {
    this->cameraPos += offset;
    this->center += offset;
    view = glm::lookAt(cameraPos, center, up);
}
// Set view matrix for perspective projection (FPS)
void MyCamera::viewMat(glm::vec3 x) {
    this->cameraPos = glm::vec3(0, 1.0, 0);
    this->center = x + glm::vec3(0.0, 0.0, 1.0);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(cameraPos, center, up); 
}
// Set view matrix for orthographic projection
void MyCamera::viewMatOrtho() {
    view = glm::lookAt(cameraPos, center, up);
}
// Set view matrix for perspective projection (3RD POV)
void MyCamera::viewMatPersp(glm::vec3 x, glm::vec3 y) {
    this->cameraPos = x * 10.f + y;
    this->center = y;
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos, center, up);
}
// Set camera position
void MyCamera::setCamPos(float x, float y, float z) {
    cameraPos = glm::vec3(x, y, z);
}
// Set camera center (focus point)
void MyCamera::setCen(glm::vec3 x) {
    center = x;
}
// Get camera position
glm::vec3& MyCamera::getCamPos() {
    return cameraPos;
}
// Get projection matrix
glm::mat4& MyCamera::getProj() {
    return projection;
}
// Get view matrix
glm::mat4& MyCamera::getView() {
    return view;
}
// Set uniform projection matrix in the shader
void MyCamera::setUniProj() {
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection));
}
// Set uniform view matrix in the shader
void MyCamera::setUniView() {
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc,
        1,
        GL_FALSE,
        glm::value_ptr(view));
}
// Set uniform camera position in the shader
void MyCamera::setUniCamPos() {
    GLuint cameraPosAddress = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));
}
// Set the shader program used by the camera
void MyCamera::setShader(GLuint shaderProgram) {
    this->shaderProgram = shaderProgram;
}