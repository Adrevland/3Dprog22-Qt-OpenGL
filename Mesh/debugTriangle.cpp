#include "debugTriangle.h"
#include "core/renderwindow.h"
#include "Core/Camera.h"
#include "Math/Vertex.h"

DebugTriangle::DebugTriangle()
	:Mesh(RenderWindow::Get()->getShader("color"), glm::mat4(1.f))
{

}

DebugTriangle::~DebugTriangle()
{
}

void DebugTriangle::init()
{
	Mesh::init();
	mDrawMode = GL_LINE_LOOP;
}

void DebugTriangle::draw()
{
	Mesh::draw();
}

void DebugTriangle::updateverts(std::vector<Vertex> verts)
{
	mVertices = verts;
	init();
}
