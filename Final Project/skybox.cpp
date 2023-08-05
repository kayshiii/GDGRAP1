#include "skybox.h"
#include "model.h"

Skybox::Skybox(const std::string& vertPath, const std::string& fragPath, const std::vector<std::string>& faces) : vertPath(vertPath), 
    fragPath(fragPath), facesSkybox(faces) {
    std::fstream sky_vertSrc(vertPath.c_str());
    std::stringstream sky_vertBuff; 
    sky_vertBuff << sky_vertSrc.rdbuf(); //add
    std::string sky_vertS = sky_vertBuff.str(); //convert
    const char* sky_v = sky_vertS.c_str();

    std::fstream sky_fragSrc(fragPath.c_str());
    std::stringstream sky_fragBuff;
    sky_fragBuff << sky_fragSrc.rdbuf(); //add
    std::string sky_fragS = sky_fragBuff.str(); //convert
    const char* sky_f = sky_fragS.c_str();

    GLuint sky_vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sky_vertShader, 1, &sky_v, NULL);
    glCompileShader(sky_vertShader);

    GLuint sky_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sky_fragShader, 1, &sky_f, NULL);
    glCompileShader(sky_fragShader);

    GLuint skyboxProgram = glCreateProgram();
    glAttachShader(skyboxProgram, sky_vertShader);
    glAttachShader(skyboxProgram, sky_fragShader);

    glLinkProgram(skyboxProgram);

    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
    //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    //unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

   /* std::string facesSkybox[]{
        "Skybox/right.png",
        "Skybox/left.png",
        "Skybox/up.png",
        "Skybox/down.png",
        "Skybox/front.png",
        "Skybox/back.png"
    };*/

    //unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
        }
    }
    stbi_set_flip_vertically_on_load(true);
}

void Skybox::draw(const glm::mat4 & view, const glm::mat4 & projection) {
     glDepthMask(GL_FALSE);
     glDepthFunc(GL_LEQUAL);
     glUseProgram(skyboxProgram);

     // Render skybox
     glm::mat4 skyView = glm::mat4(1.f);
     skyView = glm::mat4(
         glm::mat3(view)
     );

     unsigned int sky_ProjectionLoc = glGetUniformLocation(skyboxProgram, "projection");
     glUniformMatrix4fv(sky_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

     unsigned int sky_ViewLoc = glGetUniformLocation(skyboxProgram, "view");
     glUniformMatrix4fv(sky_ViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

     glBindVertexArray(skyboxVAO);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

     glDepthMask(GL_TRUE);
     glDepthFunc(GL_LESS);
}

GLuint Skybox::getshaderProgram() {
    return skyboxProgram;
}