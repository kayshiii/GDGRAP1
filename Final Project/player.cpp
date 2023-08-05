#include "player.h"
#include "model.h"
#include "shader.h"

Player::Player() {}

//void Key_CallBack(GLFWwindow* window, int key, int scancode, int action, int mod) {
//    float speed = 2.0f; // Adjust the speed as needed
//
//    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
//        //tankPosX += speed * sin(glm::radians(tankRotation));
//        tankPosZ += speed * cos(glm::radians(tankRotation));
//    }
//    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
//        //tankPosX -= speed * cos(glm::radians(tankRotation));
//        tankPosZ -= speed * sin(glm::radians(tankRotation));
//    }
//    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
//        tankRotation -= 2.0f;
//    }
//    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
//        tankRotation += 2.0f;
//    }
//}