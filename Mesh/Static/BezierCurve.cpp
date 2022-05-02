#include "BezierCurve.h"

#include "Core/renderwindow.h"

BezierCurve::BezierCurve(std::vector<glm::vec3> points)
	:Mesh::Mesh(RENDERWINDOW->getShader("debug"), glm::mat4{1.f}),mControlPoints(points)
{

	mDrawMode = GL_LINE_STRIP; // override drawmode
}

BezierCurve::~BezierCurve()
{
	mControlPoints.clear();
}

void BezierCurve::draw()
{
	Mesh::draw();

	for(auto& points: mControlPoints)
	{
		glm::mat4 mat = glm::translate(glm::mat4(1.f), points);
		RENDERWINDOW->drawDebugShape("sphere", mat);
	}
	for (int i = 0; i < mControlPoints.size()-1; ++i)
	{
		RENDERWINDOW->drawDebugLine(mControlPoints[i], mControlPoints[i + 1], glm::vec3(1.f));
	}
}

void BezierCurve::init()
{
	//visual lines

	glm::vec3 color{125/255.f, 255/255.f , 50/255.f };
	const float stepsize{ 0.0001f };
	for (float t{0.f}; t < 1.f + stepsize/2.f; t += stepsize)
	{
		glm::vec3 tmp = calcBezier(t);
		mVertices.emplace_back(Vertex(tmp.x, tmp.y, tmp.z, color.r, color.g, color.b));
	}

	Mesh::init();

}

glm::vec3 BezierCurve::calcBezier(float t)
{

	//todo some magic

	std::vector<glm::vec3> tmp = mControlPoints;

	for(int k = tmp.size()-1; k > 0; k--) 
	{
		for(int i = 0; i < k; i++) 
			tmp[i] = tmp[i] * (1-t) + tmp[i + 1] * t;
	}
	return tmp[0];

}


	
