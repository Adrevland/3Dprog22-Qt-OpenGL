#include "BillBoard.h"

#include <glm/gtc/type_ptr.hpp>

#include "Math/Vertex.h"
#include "Core/renderwindow.h"

BillBoard::BillBoard(const char* texture)
	:mtexturestr(texture)
{
	mVertices.emplace_back(Vertex(-0.5f,-0.5f,0.f,0.f,0.f));
	mVertices.emplace_back(Vertex(-0.5f, 0.5f, 0.f, 0.f, 1.f));
	mVertices.emplace_back(Vertex(0.5f, 0.5f, 0.f, 1.f, 1.f));
	mVertices.emplace_back(Vertex(0.5f, -0.5f, 0.f, 1.f, 0.f));

	GLint indices[]
	{
		2,1,0,
		3,2,0
	};
	for (auto& i : indices)
	{
		mIndices.emplace_back(i);
	}
	
}

BillBoard::~BillBoard()
{
	mVertices.clear();
	mIndices.clear();

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

void BillBoard::init()
{
	mCamera = RENDERWINDOW->getCamera();

	initializeOpenGLFunctions();
	
	mTexture = new Texture(mtexturestr);
	mShaderProgram = RENDERWINDOW->getShader("billboard");

	//VAO
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	//VBO
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

	//EBO
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	////colors
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	////normals
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(9 * sizeof(float)));
	//glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	mShaderProgram->use();

	mModelLoc = glGetUniformLocation(mShaderProgram->getProgram(), "model");
	mViewLoc = glGetUniformLocation(mShaderProgram->getProgram(), "view");
	mProjectionLoc = glGetUniformLocation(mShaderProgram->getProgram(), "projection");

	//mModelmat = glm::scale(glm::mat4{ 1.f },glm::normalize(glm::vec3(mTexture->mHeight, mTexture->mWidth,1.f))*50.f); // big
	mModelmat = glm::mat4(1.f);// big
}

void BillBoard::draw()
{
	/*glDisable(GL_CULL_FACE);*/
	glDepthMask(GL_FALSE);
	mShaderProgram->use();
	mTexture->bind();

	glBindVertexArray(mVAO);

	glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(mModelmat));
	glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getView()));
	glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getProjection()));

	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	/*glEnable(GL_CULL_FACE);*/
}

void BillBoard::scale(glm::vec3 scale)
{
	mScale = scale;
	translate();
}

void BillBoard::setlocation(glm::vec3 location)
{
	mLocation = location;
	translate();
}

void BillBoard::translate()
{
	mModelmat = glm::translate(glm::mat4(1.f), mLocation);
	mModelmat = glm::scale(mModelmat, mScale);
}
