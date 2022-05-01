#include "XYZ.h"

#include <glm/gtc/type_ptr.hpp>

XYZ::XYZ(Shader* shader)
	:VisualObject(shader)
{
	//XYZ Lines
	mVertices.emplace_back(Vertex{ 0,0,0,1,0,0 });
	mVertices.emplace_back(Vertex{ 100,0,0,1,0,0 });
	mVertices.emplace_back(Vertex{ 0,0,0,0,1,0 });
	mVertices.emplace_back(Vertex{ 0,100,0,0,1,0 });
	mVertices.emplace_back(Vertex{ 0,0,0,0,0,1 });
	mVertices.emplace_back(Vertex{ 0,0,100,0,0,1 });

}

XYZ::~XYZ()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}


void XYZ::init(Camera& camera)
{
	VisualObject::init(camera);

	mModelMat = glm::rotate(mModelMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //model matrix
	//mViewMat = glm::translate(mViewMat, glm::vec3{ 0.f,0.f,-5.f }); // view matrix

	initializeOpenGLFunctions();

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void XYZ::draw()
{
	VisualObject::draw();

	//glUniformMatrix4fv(mMatrixUniform, 1, GL_FALSE, glm::value_ptr(mMatrix));
	glDrawArrays(GL_LINES, 0, mVertices.size());

}
