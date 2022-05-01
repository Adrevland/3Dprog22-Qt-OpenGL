#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <glm/glm.hpp>

class Shader;
class Camera;
class Texture;
class Vertex;

class BillBoard
	:public QOpenGLFunctions_4_1_Core
{
public:

	BillBoard(const char* texture);
	~BillBoard();

	void init();
	void draw();
	void scale(glm::vec3 scale);
	void setlocation(glm::vec3 location);

protected:

	void translate();

	glm::mat4 mModelmat{ 1.f };

	const char* mtexturestr;

	glm::vec3 mScale = glm::vec3(1.f);
	glm::vec3 mLocation = glm::vec3(1.f);

	Camera* mCamera{ nullptr };

	Texture* mTexture{nullptr};

	Shader* mShaderProgram{ nullptr };

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;


	GLuint mVAO{ 0 };
	GLuint mVBO{ 0 };
	GLuint mEBO{ 0 };

	GLint mModelLoc{ 0 };
	GLint mViewLoc{ 0 };
	GLint mProjectionLoc{ 0 };
};

