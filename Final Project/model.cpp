#include "model.h"
#include "shader.h"

// Constructor
Model3D::Model3D(const std::string& texturePath, const std::string& normalPath, const std::string& modelPath)
    : texturePath(texturePath), normalPath(normalPath), modelPath(modelPath) {
}
// Load texture from the specified file path
void Model3D::loadTexture() {
    int img_width,
        img_height,
        colorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes =
        stbi_load(texturePath.c_str(),
            &img_width,
            &img_height,
            &colorChannels,
            0);
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
}
GLuint Model3D::getTexture() {
    return texture;
}
// Load normal map from the specified file path
void Model3D::loadNormals() {
    int img_width2, img_height2, colorChannel2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* normal_bytes =
        stbi_load(normalPath.c_str(),
            &img_width2,
            &img_height2,
            &colorChannel2,
            0
        );

    glEnable(GL_DEPTH_TEST);

    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width2,
        img_height2,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        normal_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(normal_bytes);
}
GLuint Model3D::getnormTex() {
    return norm_tex;
}
// Load the 3D model from the specified file path using tinyobjloader
void Model3D::loadModel() {
    std::string path = modelPath.c_str();
    std::vector<tinyobj::shape_t> shape;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shape,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shape[0].mesh.indices.size(); i += 3) {

        tinyobj::index_t vData1 = shape[0].mesh.indices[i];
        tinyobj::index_t vData2 = shape[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shape[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );
        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );
        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );
        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV2.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    };

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shape[0].mesh.indices[i].vertex_index
        );
    }
    // uv
    std::vector<GLfloat> normals;

    for (int i = 0; i < shape[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shape[0].mesh.indices[i];

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        // assignment for sword
        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );
        //
        fullVertexData.push_back(
            tangents[i].x
        );
        fullVertexData.push_back(
            tangents[i].y
        );
        fullVertexData.push_back(
            tangents[i].z
        );
        fullVertexData.push_back(
            bitangents[i].x
        );
        fullVertexData.push_back(
            bitangents[i].y
        );
        fullVertexData.push_back(
            bitangents[i].z
        );
    }
}
std::vector<GLfloat>Model3D::getfullVertexData() {
    return fullVertexData;
}
// Generate buffers and store the 3D model data in OpenGL VAO and VBO
void Model3D::buffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        //GL_STATIC_DRAW
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3, //XYZ
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)0
    );

    GLintptr normalPtr = 3 * sizeof(float); // + 3 for normals
    glVertexAttribPointer(
        1,
        3, //XYZ
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),  // 8 na laman
        (void*)normalPtr
    );

    GLintptr uvPtr = 6 * sizeof(float); // + 3 for normals
    glVertexAttribPointer(
        2,
        2, //XYZ
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),  // 8 na laman
        (void*)uvPtr
    );

    GLintptr tangentPtr = 8 * sizeof(float);
    GLintptr bitangentPtr = 11 * sizeof(float);

    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)tangentPtr
    );

    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GLfloat),
        (void*)bitangentPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
GLuint Model3D::getVAO() {
    return VAO;
}
GLuint Model3D::getVBO() {
    return VBO;
}
// Get the translation vector of the 3D model
glm::vec3 Model3D::getTrans() {
    return glm::vec3(Tx, Ty, Tz);
}
// Apply transformations to the 3D model using translation, scaling, and rotation
void Model3D::transformations(float Tx, float Tz, float theta) {
    this->Tx = Tx; this->Ty = Ty; this->Tz = Tz;
    this->theta = theta;
    
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);

    glm::mat4 transformation_matrix = glm::translate(
        identity_matrix4,
        glm::vec3(Tx, Ty, Tz)
    );
    transformation_matrix = glm::scale(
        transformation_matrix,
        glm::vec3(Sx, Sy, Sz)
    );
    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta),
        glm::normalize(glm::vec3(Rx, Ry, Rz))
    );
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(transformation_matrix));
}

void Model3D::transformationsObj(float Tx, float Tz, float Sx, float Sy, float Sz) {
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);
    glm::mat4 transformation_matrix = glm::translate(
        identity_matrix4,
        glm::vec3(Tx, Ty, Tz)
    );
    transformation_matrix = glm::scale(
        transformation_matrix,
        glm::vec3(Sx, Sy, Sz)
    );
    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta),
        glm::normalize(glm::vec3(Rx, Ry, Rz))
    );

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(transformation_matrix));
}

// Draw the 3D model using the shader program
void Model3D::draw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8); 
}
// Set uniform texture sampler for texture0 in the shader
void Model3D::setUniTex0() {
    glActiveTexture(GL_TEXTURE0);
    GLuint tex0loc = glGetUniformLocation(shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(tex0loc, 0);
}

void Model3D::setUniTex1() {
    glActiveTexture(GL_TEXTURE1);
    GLuint tex1loc = glGetUniformLocation(shaderProgram, "norm_tex");
    glBindTexture(GL_TEXTURE_2D, norm_tex);
    glUniform1i(tex1loc, 1); 
}

void Model3D::setShader(GLuint shaderProgram) {
    this->shaderProgram = shaderProgram;
}