#include "DebugShapes.h"
#include <glm/gtc/type_ptr.hpp>

#include "Core/renderwindow.h"

DebugShapes::DebugShapes(std::vector<Vertex> v, std::vector<unsigned> i)
	:mVertices(v) , mIndices(i)
{
}

DebugShapes::~DebugShapes()
{
}

void DebugShapes::draw(glm::mat4 modelMat)
{

	RenderWindow::Get()->getShader("debug")->use();

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

	//send matrix to shader
	glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(RenderWindow::Get()->getCamera()->getView()));
	glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(RenderWindow::Get()->getCamera()->getProjection()));

	glDrawElements(GL_LINE_STRIP, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void DebugShapes::init()
{
	initializeOpenGLFunctions();
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

	glUseProgram(RenderWindow::Get()->getShader("debug")->getProgram()); // use shader

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//colors
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//shader
	mModelLoc = glGetUniformLocation(RenderWindow::Get()->getShader("debug")->getProgram(), "model");
	mViewLoc = glGetUniformLocation(RenderWindow::Get()->getShader("debug")->getProgram(), "view");
	mProjectionLoc = glGetUniformLocation(RenderWindow::Get()->getShader("debug")->getProgram(), "projection");
	
}
