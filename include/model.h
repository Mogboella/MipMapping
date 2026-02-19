#ifndef MODEL_H
#define MODEL_H

#include "shaders.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <limits>

using namespace std;

struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec3 Tangents;
  glm::vec3 BitTangents;
  glm::vec2 UVs;
};

class Mesh
{
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  unsigned int VAO, VBO, EBO;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
  {
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
  }

  void Draw(Shader &shader)
  {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

private:
  void setupMesh()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));

    // Tangent attribute
    // Tangent attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Tangents));

    // Bitangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, BitTangents));

    // UV attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, UVs));

    glBindVertexArray(0);
  }
};

class Model
{
public:
  Model(const char *path) { loadModel(path); }

  void Draw(Shader &shader)
  {
    for (unsigned int i = 0; i < meshes.size(); i++)
      meshes[i].Draw(shader);
  }

  static unsigned int loadTexture2D(const char *path, bool flipVertically = true)
  {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    if (flipVertically)
      stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
      std::cerr << "Failed to load texture: " << path << std::endl;
      return 0;
    }

    GLenum format = GL_RGB;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
  }

private:
  vector<Mesh> meshes;
  string directory;

  void loadModel(string path)
  {
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals |
                                    aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
      cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene)
  {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene)
  {
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex;
      vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);

      if (mesh->HasNormals())
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z);
      else
        vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);

      if (mesh->mTextureCoords[0])
      {
        vertex.UVs = glm::vec2(mesh->mTextureCoords[0][i].x,
                               mesh->mTextureCoords[0][i].y);
      }
      else
      {
        vertex.UVs = glm::vec2(0.0f);
      }

      if (mesh->HasTangentsAndBitangents())
      {
        vertex.Tangents = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y,
                                    mesh->mTangents[i].z);
        vertex.BitTangents = glm::vec3(mesh->mBitangents[i].x,
                                       mesh->mBitangents[i].y,
                                       mesh->mBitangents[i].z);
      }
      else
      {
        vertex.Tangents = glm::vec3(1.0f, 0.0f, 0.0f);
        vertex.BitTangents = glm::vec3(0.0f, 1.0f, 0.0f);
      }

      vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
  }
};

#endif
