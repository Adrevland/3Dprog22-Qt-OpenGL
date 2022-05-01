#include "VisualObject.h"

#include <QWindow>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include <glm/glm.hpp>

#include "TriangelSurface.h" // for the xy func


VisualObject::VisualObject(Shader* Shader)
	:mShaderProgram(Shader)
{
}

VisualObject::~VisualObject()
{
	/*glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);*/
}

void VisualObject::init(Camera& camera)
{
	initializeOpenGLFunctions();


	//mShaderProgram = new Shader("../3Dprog22/Shaders/TextureShader.vert", "../3Dprog22/Shaders/TextureShader.frag");
	//mWireFrameShader = new Shader("./Shaders/WireFrame.vert", "./Shaders/WireFrame.frag");
	mCamera = &camera; // get camera
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(Vertex), mIndices.data(), GL_STATIC_DRAW);

	glUseProgram(mShaderProgram->getProgram()); // use shader

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//normal shader
	mModelLoc = glGetUniformLocation(mShaderProgram->getProgram(), "model");
	mViewLoc = glGetUniformLocation(mShaderProgram->getProgram(), "view");
	mProjectionLoc = glGetUniformLocation(mShaderProgram->getProgram(), "projection");


}

void VisualObject::draw()
{
	mShaderProgram->use();

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	//send matrix to shader
	glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(mModelMat));
	glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getView()));
	glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getProjection()));

}

void VisualObject::moveObject(glm::vec3 newLocation)
{
	newLocation.z = 0;
	mModelMat = glm::translate(mModelMat, newLocation); //model matrix

	//calculate z
	mModelMat[3][2] = TriangelSurface::mFunc(mModelMat[3][0], mModelMat[3][1]);
}

void VisualObject::ButtonPress(int Button)
{
}

void VisualObject::sendTime(unsigned long long elapsedTime)
{
}

void VisualObject::setLocation(glm::vec3 newLocation)
{
	mModelMat[3][0] = newLocation.x;
	mModelMat[3][1] = newLocation.y;
	mModelMat[3][2] = newLocation.z;
}
