#include "Heightmap.h"

#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include "Core/Utils.h"
#include "Core/renderwindow.h"

Heightmap::Heightmap(const float size, const char* map)
	:Mesh(RenderWindow::Get()->getShader("lightshadow"),glm::mat4{1.f}),mSize(size),mMapstr(map)
{

}

Heightmap::Heightmap(const float size, const char* map, const char* text)
	: Mesh(RenderWindow::Get()->getShader("lightshadow"), glm::mat4{ 1.f }), mSize(size), mMapstr(map)
{
	texture = new Texture(text);
}

Heightmap::~Heightmap()
{
}

void Heightmap::init()
{
	readmap();

	Mesh::init();

}

void Heightmap::draw()
{

	Mesh::draw();
	
}

float Heightmap::getHeight(glm::vec3 pos)
{

	//calculate barycentric cordinate
	glm::vec2 point;
	point.x = std::floor(pos.x);
	point.y = std::floor(pos.y);

	int a = ((int)point.x * mWidth) + (int)point.y;
	int b = (((int)point.x + 1) * mWidth) + (int)point.y;
	int c = (((int)point.x + 1) * mWidth) + (int)point.y + 1;
	int d = ((int)point.x * mWidth) + (int)point.y + 1;
	


	if (c > mVertices.size())
		return 1;

	glm::vec2 veca = glm::vec2(mVertices[a].x, mVertices[a].y);
	glm::vec2 vecb = glm::vec2(mVertices[b].x, mVertices[b].y);
	glm::vec2 vecc = glm::vec2(mVertices[c].x, mVertices[c].y);
	glm::vec2 vecd = glm::vec2(mVertices[d].x, mVertices[d].y);


	glm::vec3 baryLocation = Utils::Barycentric(point, veca, vecb, vecd);

	//check what triangle to use
	if (Utils::isInsideTri(baryLocation)) {

		auto ah = mVertices[a].z * baryLocation.x;
		auto bh = mVertices[b].z * baryLocation.y;
		auto ch = mVertices[c].z * baryLocation.z;
		auto height = ah + bh + ch;


		return height;
	}
	else
	{
		baryLocation = Utils::Barycentric(point, veca, vecc, vecd);

		auto ah = mVertices[a].z * baryLocation.x;
		auto bh = mVertices[c].z * baryLocation.y;
		auto ch = mVertices[d].z * baryLocation.z;
		auto height = ah + bh + ch;

		return height;
	}
}

void Heightmap::readmap()
{

	mLogger = Logger::getInstance();

	//initializeOpenGLFunctions();

	//texture loading
	int width, height, nrChannels;

	unsigned char* data = stbi_load(mMapstr, &width, &height, &nrChannels, 1);
	if (data) {
		//todo read a height map
		//stbi_set_flip_vertically_on_load(true);
		//const glm::vec3 color(1.f, 1.f, 1.f);

		//save variables for barycentric
		mWidth = width;
		mHeight = height;
		
		float lastZ{ 0.f };

			//calculate height
			for (int x = 0; x < height; x += 1)
			{
				for (int y = 0; y < width; y += 1)
				{
					float z = data[x * width + y]*mSize;

					mVertices.emplace_back(glm::vec3(x,y,z),glm::vec3( 0.f, 0.f, 1.f),glm::vec2(0.f,0.f));

				}
			}
			////make indices
			for (int x = 0; x < height-1; x += 1)
			{
				for (int y = 0; y < width-1; y += 1)
				{


					auto a = x * width + y;
					auto b = (x + 1) * width + y;
					auto c = (x + 1) * width + (y + 1);
					auto d = x * width + (y + 1);
					

					//indices
					mIndices.emplace_back(a); //a
					mIndices.emplace_back(b); //b
					mIndices.emplace_back(c); //c

					//indices
					mIndices.emplace_back(a); //a
					mIndices.emplace_back(c); //c
					mIndices.emplace_back(d); //d


				}
			}

			//calculate normals
			for (int x = 1; x < height-1; x += 1)
			{
				for (int y = 1; y < width-1; y += 1)
				{

					//edges have normal (0,0,1)
					
					auto a = mVertices[(x * width) + y].getCordinates();
					auto b = mVertices[((x + 1) * width) + y].getCordinates();
					auto c = mVertices[((x + 1) * width) + y + 1].getCordinates();
					auto d = mVertices[(x * width) + y + 1].getCordinates();
					auto e = mVertices[((x - 1) * width) + y].getCordinates();
					auto f = mVertices[(x * width) + (y - 1)].getCordinates();
					auto g = mVertices[((x - 1) * width) + (y - 1)].getCordinates();

					auto a0 = glm::cross(f - a, b - a);
					auto a1 = glm::cross(b - a, d - a);
					auto a2 = glm::cross(d - a, c - a);
					auto a3 = glm::cross(c - a, e - a);
					auto a4 = glm::cross(e - a, g - a);
					auto a5 = glm::cross(g - a, f - a);



					glm::vec3 normal = glm::normalize(a0+a1+a2+a3+a4+a5);
					mVertices[(x * height) + y].copyNormals(normal);

					//uv
					auto UVx = (1.f / height) * y;
					auto UVy = (1.f / width) * x;
					mVertices[(x * width) + y].copyUvs(glm::vec2(UVx, UVy));
				}
			}
		
			std::string logmsg = "Height map got " + std::to_string(nrChannels) + " channels";
			if (mLogger) mLogger->logText(logmsg, LogType::HIGHLIGHT);
		
		stbi_image_free(data);      //free image memory after bitmap generation
	}
	else {
		if (mLogger) mLogger->logText("Failed loading image texture", LogType::REALERROR);
	}
}
