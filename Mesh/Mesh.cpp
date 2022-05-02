#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>

#include "core/renderwindow.h"
#include "Physics/BoundingBox.h"
#include "Physics/Shadows.h"

Mesh::Mesh(Shader* Shader, glm::mat4 modelMat)
	:mShaderProgram(Shader), mModelMat(modelMat)
{

}

Mesh::Mesh(Shader* Shader, glm::mat4 modelMat, int Drawmode)
	:Mesh(Shader,modelMat)
{
	mDrawMode = Drawmode;
}

Mesh::Mesh(Shader* Shader, glm::mat4 modelMat, std::vector<Vertex> vertices, int Drawmode)
	:Mesh(Shader,modelMat,Drawmode)
{
	mVertices = vertices;
}

Mesh::Mesh(Shader* Shader, glm::mat4 modelMat, std::vector<Vertex> vertices, std::vector<unsigned> indices,
	int Drawmode)
	: Mesh(Shader, modelMat, Drawmode)
{
	mVertices = vertices;
	mIndices = indices;
}

Mesh::Mesh(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:mShaderProgram(Shader), mModelMat(modelMat), objFile(new ObjLoader(ObjFile)), texture(new Texture(texture))
{
}

Mesh::~Mesh()
{
	mVertices.clear();
	mIndices.clear();

	//delete mBoxComponent;
	//delete objFile;
	//delete texture;

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);

}

void Mesh::init()
{

	if(LEVEL->getShadowmap())shadowCascadeLevels = LEVEL->getShadowmap()->getCascadeLevels();
	mCamera = RenderWindow::Get()->getCamera();
	mLogger = Logger::getInstance();

	initializeOpenGLFunctions();

	if(objFile)
	{
		objFile->init();
		mVertices = objFile->getVertices();
		mIndices = objFile->getIndices();
	}

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

	glUseProgram(mShaderProgram->getProgram()); // use shader

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//colors
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//normals
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	//shader
	mModelLoc = glGetUniformLocation(mShaderProgram->getProgram(), "model");
	mViewLoc = glGetUniformLocation(mShaderProgram->getProgram(), "view");
	mProjectionLoc = glGetUniformLocation(mShaderProgram->getProgram(), "projection");
	mLightLoc = glGetUniformLocation(mShaderProgram->getProgram(), "lightPos");
	mCamLoc = glGetUniformLocation(mShaderProgram->getProgram(), "viewPos");
	mRenderMode = glGetUniformLocation(mShaderProgram->getProgram(), "ViewMode");

	//shadow
	mShadowMapsloc = glGetUniformLocation(mShaderProgram->getProgram(), "shadowmaps");
	mLightSpaceMatricesLoc = glGetUniformLocation(mShaderProgram->getProgram(), "LightSpaceMatrices");
	mCascadeDistanceloc = glGetUniformLocation(mShaderProgram->getProgram(), "cascadePlaneDistances");
	mCascadeCountLoc = glGetUniformLocation(mShaderProgram->getProgram(), "cascadeCount");
	mLightdirLoc = glGetUniformLocation(mShaderProgram->getProgram(), "lightDir");
	mFarPlaneLoc = glGetUniformLocation(mShaderProgram->getProgram(), "farPlane");

	/*std::string cascade1 = { "cascadePlaneDistances[0]" };
	std::string cascade2 = { "cascadePlaneDistances[1]" };
	std::string cascade3 = { "cascadePlaneDistances[2]" };
	std::string cascade4 = { "cascadePlaneDistances[3]" };*/

	/*mCascadeloc0 = glGetUniformLocation(mShaderProgram->getProgram(), cascade1.c_str());
	mCascadeloc1 = glGetUniformLocation(mShaderProgram->getProgram(), cascade2.c_str());
	mCascadeloc2 = glGetUniformLocation(mShaderProgram->getProgram(), cascade3.c_str());
	mCascadeloc3 = glGetUniformLocation(mShaderProgram->getProgram(), cascade4.c_str());*/

	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		mCascadeLocations.emplace_back(glGetUniformLocation(mShaderProgram->getProgram(), ("cascadePlaneDistances[" + std::to_string(i) + "]").c_str()));
	}

	RenderWindow::Get()->getShader("shadow")->use();
	mShadowModel = glGetUniformLocation(RenderWindow::Get()->getShader("shadow")->getProgram(), "model");
}

void Mesh::draw()
{
	if(bDraw)
	{

		mShaderProgram->use();
		if(texture)texture->bind(); // bind texture
		if (LEVEL->getShadowmap())LEVEL->getShadowmap()->bindShadowMaps();

		glBindVertexArray(mVAO);
		/*glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);*/

		//send matrix to shader
		glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(mModelMat));
		glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getView()));
		glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mCamera->getProjection()));
		glUniform3f(mLightLoc, mLightpos.x, mLightpos.y, mLightpos.z );
		glUniform3f(mCamLoc, mCamera->getLocation().x, mCamera->getLocation().y, mCamera->getLocation().z);
		glUniform1i(mRenderMode,mViewMode);

		glUniform3f(mLightdirLoc, RenderWindow::Get()->mLightDir.x, RenderWindow::Get()->mLightDir.y, RenderWindow::Get()->mLightDir.z);

		for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
		{
			glUniform1f(mCascadeLocations[i], shadowCascadeLevels[i]);
		}

		/*glUniform1f(mCascadeloc0, RenderWindow::Get()->getCamera()->getfar() / 50.0f);
		glUniform1f(mCascadeloc1, RenderWindow::Get()->getCamera()->getfar() / 25.0f);
		glUniform1f(mCascadeloc2, RenderWindow::Get()->getCamera()->getfar() / 10.0f);
		glUniform1f(mCascadeloc3, RenderWindow::Get()->getCamera()->getfar() / 2.0f);*/
		glUniform1i(mShadowMapsloc, 1);
		glUniform1f(mFarPlaneLoc, RenderWindow::Get()->getCamera()->getfar());
		glUniform1i(mCascadeCountLoc, 4);

		//(draw arrays
		if(mIndices.empty())
			glDrawArrays(mDrawMode, 0, mVertices.size());
		else
			glDrawElements(mDrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
}

void Mesh::checkOverlap(Mesh* mesh)
{
	if(mCollisionPrimitive)
		onBeginOverlap(mesh->mCollisionPrimitive);
}

void Mesh::drawShadowShader()
{
	RenderWindow::Get()->getShader("shadow")->use();

	glBindVertexArray(mVAO);
	/*glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);*/

	//send matrix to shader
	glUniformMatrix4fv(mShadowModel, 1, GL_FALSE, glm::value_ptr(mModelMat));

	//(draw arrays
	if (mIndices.empty())
		glDrawArrays(mDrawMode, 0, mVertices.size());
	else
		glDrawElements(mDrawMode, mIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

bool Mesh::onBeginOverlap(CollisionPrimitive* primitive)
{
	if (!primitive || mCollisionPrimitive == primitive) return false;
	if (mCollisionPrimitive->isOverlapping(primitive))
	{
		return true;
	}
	return false;
}

//bool Mesh::onBeginOverlap(CollisionPrimitive* primitive)
//{
//	if (!mCollisionPrimitive) return false;
//	return Collision::isOverlapping(mCollisionPrimitive, primitive);
//}

void Mesh::setLocation(glm::vec3 newlocation)
{
	mModelMat[3][0] = newlocation.x;
	mModelMat[3][1] = newlocation.y;
	mModelMat[3][2] = newlocation.z;
}

void Mesh::scale(glm::vec3 scale)
{
	mModelMat = glm::scale(mModelMat, scale);
}

void Mesh::drawDebugLines(bool b)
{
	mCollisionPrimitive->draw();
}

void Mesh::sendTime(unsigned long long elapsedTime)
{

}

void Mesh::ButtonPress(int Button)
{

}

void Mesh::destroy()
{
	RenderWindow::Get()->destroy(this);
}
