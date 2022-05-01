#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <glm/fwd.hpp>

#include "Math/Vertex.h"
#include "vector"

class DebugShapes: public QOpenGLFunctions_4_1_Core
{
public:
	DebugShapes(std::vector<Vertex> , std::vector<unsigned int>);
	~DebugShapes();

	void draw(glm::mat4 modelMat);
	void init();

private:

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;

	GLuint mVAO{ 0 };
	GLuint mVBO{ 0 };
	GLuint mEBO{ 0 };

	GLint mModelLoc{ 0 };
	GLint mViewLoc{ 0 };
	GLint mProjectionLoc{ 0 };

	
};

