//
//  Model.hpp
//  Basic1
//
//  Created by Никита Исаенко on 09.01.2021.
//

#ifndef Model_hpp
#define Model_hpp

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type, path;
};

class Mesh
{
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram& shader);
    
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

class Model
{
public:
    Model(const char *path);
    void Draw(ShaderProgram &shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded; 
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
    GLuint TextureFromFile(const char* name, const std::string& directory);
};

#endif /* Model_hpp */
