#include "Model.h"

Model::Model(const char* path)
{
	//loadModel(path);
}


//void Model::loadModel(std::string path)
//{
//    Assimp::Importer import;
//    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//        return;
//
//    mDirectory = path.substr(0, path.find_last_of('/'));
//
//    processNode(scene->mRootNode, scene);
//}

//void Model::processNode(aiNode* node, const aiScene* scene)
//{
//    // process all the node's meshes (if any)
//    for (unsigned int i = 0; i < node->mNumMeshes; i++)
//    {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        mMeshdatas.emplace_back(processMesh(mesh, scene));
//    }
//    // then do the same for each of its children
//    for (unsigned int i = 0; i < node->mNumChildren; i++)
//    {
//        processNode(node->mChildren[i], scene);
//    }
//}

//MeshData Model::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    //std::vector<Texture> textures;
//
//    //walk trought every vertices
//    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//    {
//        Vertex vertex;
//        glm::vec3 vector;
//        // process vertex positions, normals and texture coordinates
//        vector.x = mesh->mVertices[i].x;
//        vector.y = mesh->mVertices[i].y;
//        vector.z = mesh->mVertices[i].z;
//
//        vertex.copyPosition(vector);
//
//        if (mesh->HasNormals())
//        {
//            vector.x = mesh->mNormals[i].x;
//            vector.y = mesh->mNormals[i].y;
//            vector.z = mesh->mNormals[i].z;
//            vertex.copyNormals(vector);
//        }
//        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//        {
//            glm::vec2 vec;
//            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
//            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//            vec.x = mesh->mTextureCoords[0][i].x;
//            vec.y = mesh->mTextureCoords[0][i].y;
//            vertex.copyUvs(vec);
//
//        }
//        else
//            vertex.copyUvs(glm::vec2(0.0f, 0.0f)); // no texture
//
//        //push vertices after gotten verts normals and uvs
//        vertices.push_back(vertex);
//    }
//    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//    {
//        aiFace face = mesh->mFaces[i];
//        // retrieve all indices of the face and store them in the indices vector
//        for (unsigned int j = 0; j < face.mNumIndices; j++)
//            indices.push_back(face.mIndices[j]);
//    }
//
//    return MeshData(vertices, indices);
//}
