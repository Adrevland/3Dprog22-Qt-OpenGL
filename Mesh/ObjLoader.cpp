#include "ObjLoader.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Core/renderwindow.h"


ObjLoader::ObjLoader(const char* ObjFile)
	:filepath(ObjFile)
{

}

ObjLoader::~ObjLoader()
{
}

void ObjLoader::init()
{
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	//log error
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string error = "ERROR::ASSIMP::"; error += importer.GetErrorString();
		RenderWindow::Get()->log(error,LogType::REALERROR );
		return;
	}
	// scene->mRootNode->mNumMeshes
	for (unsigned int i{0}; i < scene->mNumMeshes; i++)
	{

		//find first mesh only
		aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			Vertex vertex;
			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = mesh->mVertices[j].x;
			vector.y = mesh->mVertices[j].y;
			vector.z = mesh->mVertices[j].z;
			vertex.copyPosition(vector);

			vector.x = mesh->mNormals[j].x;
			vector.y = mesh->mNormals[j].y;
			vector.z = mesh->mNormals[j].z;
			vertex.copyNormals(vector);

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][j].x;
				vec.y = mesh->mTextureCoords[0][j].y;
				vertex.copyUvs(vec);
			}
			else
				vertex.copyUvs(glm::vec2(0.0f, 0.0f));

			mVertices.push_back(vertex);
		}
		// process indices
		for (unsigned int k = 0; k < mesh->mNumFaces; k++)
		{
			aiFace face = mesh->mFaces[k];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				mIndices.push_back(face.mIndices[j]);
		}
	}

}
