#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <glm/glm.hpp>

#include "ObjLoader.h"
#include "Core/Camera.h"
#include "core/shader.h"
#include "Core/Texture.h"
#include "Math/Vertex.h"
#include "Physics/Collision.h"

class BoundingBox;

enum ViewMode
{
	DEFAULT = 0,
	NORMALS,
	LIGHT

};

class Mesh
	:public QOpenGLFunctions_4_1_Core
{
public:
	Mesh(Shader* Shader, glm::mat4 modelMat);
	Mesh(Shader* Shader, glm::mat4 modelMat,int Drawmode);
	Mesh(Shader* Shader, glm::mat4 modelMat, std::vector<Vertex> vertices, int Drawmode);
	Mesh(Shader* Shader, glm::mat4 modelMat, std::vector<Vertex> vertices,std::vector<unsigned int> indices, int Drawmode);
	Mesh(Shader* Shader, glm::mat4 modelMat,const char* ObjFile, const char* texture);
	//Mesh(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture, int Drawmode);
	~Mesh();

	virtual void init();
	virtual void draw();

	virtual void sendTime(unsigned long long elapsedTime);
	virtual void ButtonPress(int Button);
	virtual void destroy();

	virtual void checkOverlap(Mesh* mesh);
	void drawMode(int drawmode) { mDrawMode = drawmode;};
	glm::vec3 getLocation() const { return glm::vec3(mModelMat[3][0], mModelMat[3][1], mModelMat[3][2]); }
	glm::vec3 getScale() const { return glm::vec3(mModelMat[0][0], mModelMat[1][1], mModelMat[2][2]); }
	glm::mat4 getModelMat() const { return mModelMat; }
	std::vector<Vertex> getVertices()const { return mVertices; }

	void setName(const std::string name) { mName = name; }
	std::string getName() { return mName; }
	void setLocation(glm::vec3 newlocation);
	void scale(glm::vec3 scale);
	void setLightLoc(glm::vec3 &newpos) { mLightpos = newpos; }
	void setbDraw(bool b) { bDraw = b; }
	void setModelMat(glm::mat4 mmat) { mModelMat = mmat; }

	void drawDebugLines(bool b);

	void setViewMode(int mode) { mViewMode = mode; }
	int getViewMode() { return mViewMode; }
	
	void drawShadowShader();

	void stun() { if(!stunned)stunned = true; }
	CollisionPrimitive* getcollision() { return mCollisionPrimitive; }
	bool isStunned() { return stunned; }
private:
	const char* whitetexture{ "./Textures/white.jpg" };
	//Model* mModel{nullptr};
protected:
	virtual bool onBeginOverlap(CollisionPrimitive* primitive);
	//virtual bool onBeginOverlap(CollisionPrimitive* primitive);

	ObjLoader* objFile{ nullptr };
	Texture* texture{ nullptr };

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;


	GLuint mVAO{ 0 };
	GLuint mVBO{ 0 };
	GLuint mEBO{ 0 };

	Camera* mCamera{nullptr};

	Shader* mShaderProgram{ nullptr };    //holds pointer the GLSL shader program

	//OpenGL reference to the Uniform in the shader program
	GLint mModelLoc{ 0 };
	GLint mShadowModel{ 0 };
	GLint mViewLoc{ 0 };
	GLint mProjectionLoc{ 0 };
	GLint mLightLoc{ 0 };
	GLint mCamLoc{ 0 };
	GLint mRenderMode{ 0 };

	//shadow
	GLint mShadowMapsloc{ 0 };
	GLint mLightSpaceMatricesLoc{ 0 };
	GLint mCascadeDistanceloc{ 0 };
	GLint mCascadeCountLoc{ 0 };
	GLint mLightdirLoc{ 0 };
	GLint mFarPlaneLoc{ 0 };
	std::vector<GLint> mCascadeLocations;
	GLint mCascadeloc0;
	GLint mCascadeloc1;
	GLint mCascadeloc2;
	GLint mCascadeloc3;

	std::vector<float> shadowCascadeLevels;

	//matrixes
	glm::mat4 mModelMat{ 1.f };

	glm::vec3 mLightpos{1.f};
	//texture
	unsigned int texture1;

	int mDrawMode{ GL_TRIANGLES };
	int mViewMode{ 0 };
	std::string mName;

	CollisionPrimitive* mCollisionPrimitive{ nullptr };

	//BoundingBox* mBoxComponent{ nullptr };
	bool bDraw{ true };
	bool bDebugLines{ false };
	Logger* mLogger{ nullptr };

	bool stunned{ false };
};

