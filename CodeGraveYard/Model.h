#pragma once

#include <string>
#include <vector>

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

#include "Mesh/Mesh.h"

struct MeshData
{
	MeshData(std::vector<Vertex> vert, std::vector<unsigned int> ind)
		:vertices(vert), indices(ind){};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

class Model
{
public:
	Model(const char* path);

	
	std::vector<MeshData*> mMeshdatas;
private:

	std::string mDirectory;

	//void loadModel(std::string path);
	//void processNode(aiNode* node, const aiScene* scene);
	//MeshData processMesh(aiMesh* mesh, const aiScene* scene);

	//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

