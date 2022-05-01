#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "vector"
#include "Core/renderwindow.h"


class Mesh;

class Shadows : public QOpenGLFunctions_4_1_Core
{
public:
	Shadows();
	~Shadows();

	static std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4 &view);

	void setup() { calcLightView(); genShadowMaps(); }

	void genShadowMaps();
	void bindShadowMaps();
	void draw(std::vector<Mesh*> meshes);
	std::vector<float> getCascadeLevels() { return shadowCascadeLevels; }
private:

	glm::mat4 calcLightView();
	glm::mat4 calcLightViewProj(const float nearPlane, const float farPlane);

	glm::vec3 mLightDir = RENDERWINDOW->mLightDir;

	glm::mat4 mlightview{ 1.f };
	glm::mat4 mLightProj{ 1.f };

	GLuint matricesUBO;
	GLuint lightFBO;
	GLuint lightDepthMaps;

	int depthMapResolution{4096*2};

	struct Cascade
	{
		glm::mat4 LightViewProj{ 1.f };
	};

	std::vector<float> shadowCascadeLevels{
		RenderWindow::Get()->getCamera()->getfar() / 20.0f,
		RenderWindow::Get()->getCamera()->getfar() / 15.0f,
		RenderWindow::Get()->getCamera()->getfar() / 10.0f,
		RenderWindow::Get()->getCamera()->getfar() / 5.0f};

	std::vector < glm::mat4 > shadowCascades;


};

