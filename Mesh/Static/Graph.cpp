#include "Graph.h"

#include <iostream>
#include <fstream>

#include "core/renderwindow.h"
#include "Mesh/Static/IdentityCube.h"


Graph::Graph(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:Mesh(Shader,modelMat,ObjFile,texture)
{
	writeFile(Graph::quadtratic, "./quadtraticfunc.txt");
	writeFile(Graph::interpol, "./interpolfunc.txt");
}

Graph::~Graph()
{


}

void Graph::init()
{
	QuadtraticGraph = new Mesh(RenderWindow::Get()->getShader("color"), glm::mat4{1.f}, readFile("./quadtraticfunc.txt", Quadtratic), GL_LINE_STRIP);
	InterpolGraph = new Mesh(RenderWindow::Get()->getShader("color"), glm::mat4{ 1.f }, readFile("./interpolfunc.txt", Quadtratic), GL_LINE_STRIP);

	QuadtraticGraph->init();
	InterpolGraph->init();

	Mesh::init();

}

void Graph::draw()
{
	Mesh::draw();

	switch (mActiveGraph)
	{
	case Quadtratic:
		QuadtraticGraph->draw();
		break;
	case Interpol:
		InterpolGraph->draw();
		break;
	}

}

void Graph::ButtonPress(int Button)
{
	Mesh::ButtonPress(Button);

	if(Button == Qt::Key_Up)
	{
		swapGraph(Quadtratic);
	}
	else if (Button == Qt::Key_Down)
	{
		swapGraph(Interpol);
	}
}

void Graph::sendTime(unsigned long long elapsedTime)
{
	Mesh::sendTime(elapsedTime);
	updateNpc(elapsedTime);
}


void Graph::writeFile(float func(float x), std::string filename)
{
	std::ofstream file(filename);

	
	const float jump{ 0.5f };
	const glm::vec3 color{ 255.f / 255.f, 255.f / 255.f, 0.f / 255.f }; //red

	file << abs(startValue - endValue)/jump+1 << "\n";

	for (float x = startValue; x <= endValue; x += jump)
	{
		file << Vertex{ x,func(x), 3, color.r, color.g, color.b };
	}
}

std::vector<Vertex> Graph::readFile(std::string filename, int vecloc)
{
	std::vector<Vertex> Vertices;
	std::ifstream file(filename);
	std::string line;

	if (file >> line)
		mVertCount = std::stoi(line);
	float numb;

	for (int j = 0; j < mVertCount; ++j)
	{
		float vertdata[8];
		for (int i = 0; i < 8; ++i)
		{
			file >> line;
			vertdata[i] = std::stof(line);
		}
		Vertices.emplace_back(Vertex(vertdata[0], vertdata[1], vertdata[2], vertdata[3], vertdata[4], vertdata[5], vertdata[6], vertdata[7]));
	}

	file.close();
	return Vertices;
}

void Graph::swapGraph(Function mathFunc)
{
	mActiveGraph = mathFunc;
}

void Graph::updateNpc(int xloc)
{
	if (npcLoc.x > endValue || npcLoc.x < startValue) mNpcSpeed *= -1;
	npcLoc.x += xloc * mNpcSpeed;
	npcLoc.z = 3;
	switch(mActiveGraph)
	{
	case Quadtratic:
		npcLoc.y = Graph::quadtratic(npcLoc.x);
		break;
	case Interpol:
		npcLoc.y = Graph::interpol(npcLoc.x);
		break;
	default: 
		npcLoc.y = 1;
		break;
	}
	setLocation(npcLoc);
}
