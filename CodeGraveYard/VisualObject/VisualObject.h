#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include <glm/glm.hpp>
#include "core/shader.h"
#include "math/Vertex.h"
#include "core/Camera.h"
#include <iostream>
#include <fstream>
#include <ostream>

class RenderWindow;

class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
	VisualObject(Shader* Shader);
	~VisualObject();

	virtual void init(Camera& camera);
	virtual void draw();

	void moveObject(glm::vec3 newLocation);
	glm::vec3 getLocation() { return glm::vec3(mModelMat[3][0], mModelMat[3][1], mModelMat[3][2]); };
	glm::mat4 getModelMat() { return mModelMat; };

	virtual void ButtonPress(int Button);
	virtual void sendTime(unsigned long long elapsedTime);

	void setLocation(glm::vec3 newLocation);
private:

protected:
	std::vector<Vertex> mVertices;
	std::vector<Vertex> mIndices;
	GLuint mVAO{ 0 };
	GLuint mVBO{ 0 };
	GLuint mEBO{ 0 };

	Camera* mCamera;

	Shader* mShaderProgram{ nullptr };    //holds pointer the GLSL shader program
	
	//OpenGL reference to the Uniform in the shader program
	GLint mModelLoc{0};
	GLint mViewLoc{0};
	GLint mProjectionLoc{0};

	GLint mWireModelLoc{ 0 };
	GLint mWireViewLoc{ 0 };
	GLint mWireProjectionLoc{ 0 };

	//matrixes
	glm::mat4 mModelMat{ 1.f };

	//texture
	unsigned int texture1;

};
