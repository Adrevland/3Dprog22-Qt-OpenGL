#include "HeightmapLevel.h"
#include "core/renderwindow.h"
#include "Mesh/static/Heightmap.h"

#include "core/Utils.h"
#include "Core/OctTree.h"
#include "Mesh/static/Trophy.h"


HeightmapLevel::HeightmapLevel(RenderWindow* rWindow)
	:Level(rWindow)
{
}

HeightmapLevel::~HeightmapLevel()
{
}

void HeightmapLevel::init()
{
	Level::init();

	mShaderPrograms["Texture"] = RenderWindow::Get()->getShader("Texture");
	mShaderPrograms["light"] = RenderWindow::Get()->getShader("light");
	mShaderPrograms["color"] = RenderWindow::Get()->getShader("color");

	//mCamera = RenderWindow::Get()->getCamera();

	//biggest oct size
	mOctTree =new OctTree(BoundingBox(glm::vec3(1.f), glm::vec3(1000.f), mShaderPrograms["color"]));

	mPlayer =new Player(mShaderPrograms["light"], glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
	mMeshes.emplace_back(mPlayer);

	glm::vec3 lightpos{ 0.f,0.f,100.f };
	//sunvisualiser
	glm::mat4 sunloc = glm::translate(glm::mat4{ 1.f }, lightpos);
	mSun = new Mesh(mShaderPrograms["Texture"], sunloc, "../3Dprog22/ObjFiles/Sphere.obj", "../3Dprog22/Textures/UvCheckers.jpg");
	if (mSun)
	{
		mSun->init();
		mSun->setLightLoc(lightpos);
	}

	mHeightmap = new Heightmap(0.3f, "../3Dprog22/Textures/Heightmap/brsundtexture.png", "../3Dprog22/Textures/Heightmap/brsundtexture.png");
	mPlayer->setHeightmap(mHeightmap);
	mHeightmap->init();
	mHeightmap->setLightLoc(lightpos);
	//mMeshes.emplace_back(mHeightmap);

	//mMeshes.emplace_back(new Heightmap(1.f, "../3Dprog22/Textures/worldHeightmap.jpg", mCamera));

	//trophys

	//make random trophys
	std::vector<glm::vec3> TrophyPoints;
	for (int i = 0; i < 10; ++i)
	{
		auto y = std::rand() % +mHeightmap->mHeight;
		auto x = std::rand() % +mHeightmap->mWidth;
		TrophyPoints.emplace_back(glm::vec3(x, y, mHeightmap->getHeight(glm::vec3(x, y, 0))));
	}

	std::vector<std::pair<const char*, const char*>> trophymeshes;
	trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/cherry.obj", "../3Dprog22/Textures/cherry.jpg"));
	trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/grape.obj", "../3Dprog22/Textures/grape.jpg"));

	for (auto vec : TrophyPoints)
	{
		glm::mat4 tmpTLoc = glm::translate(glm::mat4(1.f), vec);
		//tmpTLoc = glm::scale(tmpTLoc, glm::vec3(0.5f, 0.5f, 0.5f));
		auto r = rand() % 2;
		mMeshes.emplace_back(new Trophy(mShaderPrograms["light"], tmpTLoc, trophymeshes.at(r).first, trophymeshes.at(r).second));
	}


	//init and insert to oct tree
	for (auto& obj : mMeshes)
	{
		obj->init();
		mOctTree->insert(obj);
		obj->setLightLoc(lightpos);
	}

}

void HeightmapLevel::render()
{
	Level::render();
	
	mCamera->followObject(mPlayer);
	//glm::vec3 ploc = mPlayer->getLocation();

	mHeightmap->draw();

	for (auto& mesh : mMeshes)
	{
		mesh->draw();
		mesh->sendTime(1);
		//mesh->setLightLoc(ploc);
	}

	if (mSun) mSun->draw();

	mOctTree->drawTree();

	mPlayer->tick(1.f);

	mOctTree->checkCollision(mPlayer);
}

void HeightmapLevel::deleteMesh(Mesh* mesh)
{
	Level::deleteMesh(mesh);
}

void HeightmapLevel::wheelEvent(QWheelEvent* event)
{
	Level::wheelEvent(event);
}

void HeightmapLevel::keyPressEvent(QKeyEvent* event)
{
	Level::keyPressEvent(event);
}

void HeightmapLevel::keyReleaseEvent(QKeyEvent* event)
{
	Level::keyReleaseEvent(event);
}

void HeightmapLevel::exposeEvent()
{
	Level::exposeEvent();
}

void HeightmapLevel::resetPlayer()
{
	Level::resetPlayer();
}
