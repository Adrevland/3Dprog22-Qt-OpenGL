#include "BezierCurve.h"

BezierCurve::BezierCurve(Shader* Shader, glm::mat4 modelMat)
	:Mesh::Mesh(Shader,modelMat)
{
	/*mControlPoints.emplace_back(glm::vec3(20.f, 0.f, 3.f) + getLocation());
	mControlPoints.emplace_back(glm::vec3(10.f, 5.f, 5.f) + getLocation());
	mControlPoints.emplace_back(glm::vec3(15.f, 10.f, 10.f) + getLocation());
	mControlPoints.emplace_back(glm::vec3(30.f, 10.f, 15.f) + getLocation());*/

	mControlPoints.emplace_back(glm::vec3(4.f, 0.f, 0.f) + getLocation());
	mControlPoints.emplace_back(glm::vec3(0.f, 0.f, 0.f) + getLocation());
	mControlPoints.emplace_back(glm::vec3(0.f, 4.f, 0.f) + getLocation());
	mControlPoints.emplace_back(glm::vec3(4.f, 4.f, 0.f) + getLocation());

	mDrawMode = GL_LINE_STRIP; // override drawmode
}

BezierCurve::~BezierCurve()
{
	mControlPoints.clear();
	mVisualPoints.clear();
}

void BezierCurve::draw()
{
	Mesh::draw();

	for (auto obj : mVisualPoints)
	{
		obj->draw();
	}
	if(mVisualLines)mVisualLines->draw();
}

void BezierCurve::init()
{
	//visual lines
	mModelMat = glm::mat4{ 1.f };

	glm::vec3 color{125/255.f, 255/255.f , 50/255.f };
	const float stepsize{ 3.f/4.f };
	for (float t{0.f}; t < 1.f + stepsize/2.f; t += stepsize)
	{
		glm::vec3 tmp = calcBezier(t);
		mVertices.emplace_back(Vertex(tmp.x, tmp.y, tmp.z, color.r, color.g, color.b));
	}

	Mesh ::init();

	std::vector<Vertex> tmpVertices;
	for (auto points : mControlPoints)
	{
		
		mVisualPoints.emplace_back(new Mesh(mShaderProgram, 
											glm::scale(glm::translate(glm::mat4(1.f), points),glm::vec3(0.5f,0.5f,0.5f)), 
											"ObjFiles/Sphere.obj", "Textures/UvCheckers.jpg"
											));

		tmpVertices.emplace_back(Vertex(points.x, points.y, points.z));
	}
	mVisualLines = new Mesh(mShaderProgram, glm::mat4(1.f), tmpVertices, GL_LINE_STRIP);
	for(auto obj: mVisualPoints)
	{
		obj->init();
	}
	if (mVisualLines)mVisualLines->init();
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


	
