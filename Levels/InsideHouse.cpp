#include "InsideHouse.h"
#include "Core/renderwindow.h"
#include "Mesh/Static/House.h"

InsideHouse::InsideHouse(RenderWindow* rWindow)
	:Level(rWindow)
{
}

InsideHouse::~InsideHouse()
{
}

void InsideHouse::init()
{
	Level::init();

	//make shaders
	/*mShaderPrograms["Texture"] = new Shader("../3Dprog22/Shaders/TextureShader.vert", "../3Dprog22/Shaders/TextureShader.frag");
    mShaderPrograms["color"] = new Shader("../3Dprog22/Shaders/color.vert", "../3Dprog22/Shaders/color.frag");
	mShaderPrograms["light"] = new Shader("../3Dprog22/Shaders/light.vert", "../3Dprog22/Shaders/light.frag");*/

	mShaderPrograms["Texture"] = RenderWindow::Get()->getShader("Texture");
	mShaderPrograms["light"] = RenderWindow::Get()->getShader("light");
	mShaderPrograms["color"] = RenderWindow::Get()->getShader("color");

	//mCamera = new Camera(mRenderWindow->width(), mRenderWindow->height(), mRenderWindow->devicePixelRatio());

	//biggest oct size
	mOctTree = new OctTree(BoundingBox(glm::vec3(1.f), glm::vec3(100.f, 100.f, 50.f),mShaderPrograms["color"]));


	//house

	glm::mat4 tmphLoc = glm::rotate(glm::mat4{ 1.f }, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
	tmphLoc = glm::scale(tmphLoc, glm::vec3(5.f, 5.f, 5.f));
    mMeshes.emplace_back(new House(mShaderPrograms["light"], tmphLoc, "../3Dprog22/ObjFiles/tree.obj", "../3Dprog22/Textures/tree.jpg",false));

	//plane
    mMeshes.emplace_back(new Mesh(mShaderPrograms["light"],  glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Plane.obj", "../3Dprog22/Textures/grass.jpg"));

	//random stars
	for(int i{0}; i < 10; i++)
	{
		//object inside house
		tmphLoc = glm::translate(glm::mat4(1.f), glm::vec3((rand()%50)-25, (rand() % 50) - 25, rand() % 20));
		tmphLoc = glm::scale(tmphLoc, glm::vec3(rand()%5+1));
		tmphLoc = glm::rotate(tmphLoc, glm::radians(static_cast<float>(rand() % 90)), glm::vec3(0.f, 0.f, 1.f));
        mMeshes.emplace_back(new Mesh(mShaderPrograms["light"],  tmphLoc, "../3Dprog22/ObjFiles/star.obj", "../3Dprog22/Textures/star.jpg"));

	}

	//player
    mPlayer = new Player(mShaderPrograms["light"], glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
	mMeshes.emplace_back(mPlayer);

	glm::vec3 lightpos{ 0.f,-5.f,10.f };
	//init and insert to oct tree
	for (auto& obj : mMeshes)
	{
		obj->init();
		obj->setLightLoc(lightpos);
		mOctTree->insert(obj);
		
	}
}

void InsideHouse::render()
{
	Level::render();

	//mCamera->followObject(mPlayer);
	mCamera->setTarget(glm::vec3(1.f));
	//find hardcoded location
	mCamera->setLocation(glm::vec3(0.f,-30.f,40.f));

	for (auto& mesh : mMeshes)
	{
		mesh->draw();
		mesh->sendTime(1);
	}
	mOctTree->drawTree();

	mPlayer->tick(1.f);

	mOctTree->checkCollision(mPlayer);
}

void InsideHouse::wheelEvent(QWheelEvent* event)
{
	Level::wheelEvent(event);
}

void InsideHouse::keyPressEvent(QKeyEvent* event)
{
	Level::keyPressEvent(event);
}

void InsideHouse::keyReleaseEvent(QKeyEvent* event)
{
	Level::keyReleaseEvent(event);
}

void InsideHouse::exposeEvent()
{
	Level::exposeEvent();
}

void InsideHouse::resetPlayer()
{
	Level::resetPlayer();
	mPlayer->setLocation(glm::vec3(-10.f, -10.f, 1.f));
}
