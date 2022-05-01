#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Mesh;


class Utils
{
public:
	Utils() = delete;

	static glm::vec3 Barycentric(const glm::vec2& point, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
	static bool isInsideTri(glm::vec3 v);
	static std::string vecToString(glm::vec3 v);

	static float radiusMesh(Mesh* mesh);
};

