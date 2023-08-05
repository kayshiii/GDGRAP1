#include "shader.h"
#include "skybox.h"

Shaders::Shaders(const std::string& vertPath, const std::string& fragPath) : vertPath(vertPath), fragPath(fragPath){
    shaderProgram;
}

void Shaders::vertFrag() {
    std::fstream vertSrc(vertPath.c_str());
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc(fragPath.c_str());
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);
}
GLuint Shaders::getshaderProgram() {
    return shaderProgram;
}