#include "Outside.h"

#include "core/renderwindow.h"
#include "Mesh/static/BezierCurve.h"
#include "Mesh/static/Graph.h"
#include "Mesh/static/House.h"
#include "Mesh/static/Trophy.h"
#include "Mesh/static/Heightmap.h"

Outside::Outside(RenderWindow* rWindow)
	:Level(rWindow)
{
}

Outside::~Outside()
{
}

void Outside::init()
{
	Level::init();

	//make shaders
	/*mShaderPrograms["Texture"] = new Shader("../3Dprog22/Shaders/TextureShader.vert", "../3Dprog22/Shaders/TextureShader.frag");
	mShaderPrograms["light"] = new Shader("../3Dprog22/Shaders/light.vert", "../3Dprog22/Shaders/light.frag");
    mShaderPrograms["color"] = new Shader("../3Dprog22/Shaders/color.vert", "../3Dprog22/Shaders/color.frag");*/
	mShaderPrograms["Texture"] = RenderWindow::Get()->getShader("Texture");
	mShaderPrograms["light"] = RenderWindow::Get()->getShader("light");
	mShaderPrograms["color"] = RenderWindow::Get()->getShader("color");

	//mCamera = new Camera(mRenderWindow->width() , mRenderWindow->height(), mRenderWindow->devicePixelRatio());

	//biggest oct size
	mOctTree = new OctTree(BoundingBox(glm::vec3(1.f), glm::vec3(100.f, 100.f, 50.f),mShaderPrograms["color"]));


	//make every object

	//trophies
	std::vector<glm::vec3> TrophyPoints;
	TrophyPoints.emplace_back(glm::vec3(-45.f, -45.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(-23.f, -25.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(-15.f, -20.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(-5.f, 15.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(13.f, 10.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(25.f, 15.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(17.f, -15.f, 1.f));
	TrophyPoints.emplace_back(glm::vec3(25.f, -22.f, 1.f));

	std::vector<std::pair<const char*, const char*>> trophymeshes;
    trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/cherry.obj","../3Dprog22/Textures/cherry.jpg"));
    trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/grape.obj", "../3Dprog22/Textures/grape.jpg"));

	for(auto vec: TrophyPoints)
	{
		glm::mat4 tmpTLoc = glm::translate(glm::mat4(1.f), vec);
		//tmpTLoc = glm::scale(tmpTLoc, glm::vec3(0.5f, 0.5f, 0.5f));
		auto r = rand()%2;
		mMeshes.emplace_back(new Trophy(mShaderPrograms["light"], tmpTLoc, trophymeshes.at(r).first, trophymeshes.at(r).second));
	}

	//house location
	//door -25,27
	glm::mat4 tmphLoc = glm::translate(glm::mat4(1.f), glm::vec3(-30.f, 30.f, 0.f));
	//glm::mat4 tmphLoc = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
	//tmphLoc = glm::rotate(tmphLoc, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
	//tmphLoc = glm::scale(tmphLoc, glm::vec3(0.5f, 0.5f, 0.5f));
    mMeshes.emplace_back(new House(mShaderPrograms["light"], tmphLoc, "../3Dprog22/ObjFiles/tree.obj", "../3Dprog22/Textures/tree.jpg",true));

	//graph
    mGraph = new Graph(mShaderPrograms["light"], glm::mat4(1.f), "../3Dprog22/ObjFiles/spike.obj", "../3Dprog22/Textures/spike.jpg");
	mMeshes.emplace_back(mGraph);

	//plane
    mMeshes.emplace_back(new Mesh(mShaderPrograms["light"],  glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Plane.obj", "../3Dprog22/Textures/grass.jpg"));

	//bezier
	glm::mat4 bezloc = glm::translate(glm::mat4{ 1.f }, glm::vec3(10.f, 20.f, 5.f));
	mMeshes.emplace_back(new BezierCurve(mShaderPrograms["color"], bezloc));

	//player
    mPlayer = new Player(mShaderPrograms["light"],  glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
	mMeshes.emplace_back(mPlayer);

	glm::vec3 lightpos{ 0.f,-30.f,50.f };
	//sunvisualiser
	glm::mat4 sunloc = glm::translate(glm::mat4{ 1.f }, lightpos);
    mSun = (new Mesh(mShaderPrograms["Texture"], sunloc, "../3Dprog22/ObjFiles/Sphere.obj", "../3Dprog22/Textures/UvCheckers.jpg"));
	if(mSun)
	{
		mSun->init();
		mSun->setLightLoc(lightpos);
	}
	
	
	//mHeightmap = new Heightmap(0.2f, "../3Dprog22/Textures/Heightmap/brsundheight.png", "../3Dprog22/Textures/Heightmap/brsundtexture.png");
	mHeightmap = new Heightmap(0.1f, "../3Dprog22/Textures/Heightmap/brsundlargeheight.png", "../3Dprog22/Textures/Heightmap/brsundlargetexture.png");
	//mHeightmap = new Heightmap(0.3f, "../3Dprog22/Textures/Heightmap/moiranaheight.png", "../3Dprog22/Textures/Heightmap/moiranatexture.png");
	//mHeightmap = new Heightmap(0.6f, "../3Dprog22/Textures/Heightmap/gizaheight.png", "../3Dprog22/Textures/Heightmap/gizatexture.png");
	mPlayer->setHeightmap(mHeightmap);
	mHeightmap->init();
	mHeightmap->setLightLoc(lightpos);

	//all meshes
	mAllMeshes.emplace_back(mHeightmap);

	//init and insert to oct tree
	for (auto& obj : mMeshes)
	{
		mAllMeshes.emplace_back(obj);
		obj->init();
		mOctTree->insert(obj);
		obj->setLightLoc(lightpos);
	}

}

void Outside::render()
{
	Level::render();
	//draw everything

	auto lightpos = mPlayer->getLocation();
	lightpos.z += 50;
	mCamera->followObject(mPlayer);
	//glm::vec3 ploc = mPlayer->getLocation(); // get player location
	for(auto& mesh: mMeshes)
	{
		mesh->draw();
		mesh->sendTime(1);
		mesh->setLightLoc(lightpos); //set light to player location
	}
	if(mHeightmap)mHeightmap->draw();
	if(mHeightmap)mHeightmap->setLightLoc(lightpos);
	if(mSun) mSun->draw();

	mOctTree->drawTree();

	mPlayer->tick(1.f);

	mOctTree->checkCollision(mPlayer);

	/*if(mOctTree->find(mPlayer) != mOctTree->find(mPlayer->getLocation()))
	{
		mOctTree->move(mPlayer);
	}*/
}

void Outside::wheelEvent(QWheelEvent* event)
{
	Level::wheelEvent(event);

	/*auto numDegrees = event->angleDelta() / 8;
	mCamera->Zoom(std::clamp(numDegrees.ry(), -1, 1));*/
}

void Outside::keyPressEvent(QKeyEvent* event)
{

	Level::keyPressEvent(event);

	if(mKeyboard[Qt::Key_Up])
	{
	    mGraph->ButtonPress(Qt::Key_Up);
	}
	if (mKeyboard[Qt::Key_Down])
	{
		mGraph->ButtonPress(Qt::Key_Down);
	}
}

void Outside::keyReleaseEvent(QKeyEvent* event)
{

	Level::keyReleaseEvent(event);

	
}

void Outside::exposeEvent()
{
	Level::exposeEvent();
}

void Outside::deleteMesh(Mesh* mesh)
{

	Level::deleteMesh(mesh);
}
