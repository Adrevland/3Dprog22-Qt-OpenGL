#include "Oblig3.h"
#include "core/renderwindow.h"
#include "Mesh/static/Trophy.h"
#include "Mesh/Static/SkyBox.h"
#include "Physics/Shadows.h"
#include "Mesh/Billboard/BillBoard.h"

Oblig3::Oblig3(RenderWindow* rWindow)
	:Level(rWindow)
{
}

Oblig3::~Oblig3()
{
}

void Oblig3::init()
{
	Level::init();

	//make shaders

	//biggest oct size
	mOctTree = new OctTree(BoundingBox(glm::vec3(1.f), glm::vec3(1200.f), mShaderPrograms["color"]));

	mShadowMap = new Shadows();
	mShadowMap->setup();

	//billboards
	//mBillboards.emplace_back(new BillBoard("../3Dprog22/Assets/Textures/Quantamagazine-scaled.jpg"));
	mBillboards.emplace_back(new BillBoard("../3Dprog22/Assets/Textures/OleSolpepega.jpg"));
	mBillboards[0]->scale(glm::vec3(50.f));
	//light
	glm::vec3 lightpos{ 800.f,350.f,100.f };
	//sunvisualiser
	glm::mat4 sunloc = glm::translate(glm::mat4{ 1.f }, lightpos);
	mSun = (new Mesh(mShaderPrograms["Texture"], sunloc, "../3Dprog22/ObjFiles/Sphere.obj", "../3Dprog22/Textures/UvCheckers.jpg"));
	if (mSun)
	{
		mSun->init();
		mSun->setLightLoc(lightpos);
	}
	//heightmap
	mHeightmap = new Heightmap(0.2f, "../3Dprog22/Textures/Heightmap/brsundlargeheight.png", "../3Dprog22/Textures/Heightmap/brsundlargetexture.png");
	mHeightmap->init();
	mHeightmap->setLightLoc(lightpos);

	//player
	mPlayer = new Player(mShaderPrograms["lightshadow"], glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
	mMeshes.emplace_back(mPlayer);
	mPlayer->setLocation(glm::vec3(425.f, 740.f, 3.f));
	mPlayer->setHeightmap(mHeightmap);

	//make random trophys
	std::vector<glm::vec3> TrophyPoints;
	for (int i = 0; i < 10; ++i)
	{
		auto y = std::rand() % +mHeightmap->mHeight;
		auto x = std::rand() % +mHeightmap->mWidth;
		TrophyPoints.emplace_back(glm::vec3(x, y, mHeightmap->getHeight(glm::vec3(x, y, 0))+1));
	}

	std::vector<std::pair<const char*, const char*>> trophymeshes;
	trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/cherry.obj", "../3Dprog22/Textures/cherry.jpg"));
	trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/grape.obj", "../3Dprog22/Textures/grape.jpg"));

	for (auto vec : TrophyPoints)
	{
		glm::mat4 tmpTLoc = glm::translate(glm::mat4(1.f), vec);
		auto r = rand() % 2;
		mMeshes.emplace_back(new Trophy(mShaderPrograms["light"], tmpTLoc, trophymeshes.at(r).first, trophymeshes.at(r).second));
	}

	
	//skybox
	std::vector<std::string> skyboxfaces{
		"../3Dprog22/assets/Skyboxes/Medium_Mixed_Nebulae_Local_Stars/Textures/Medium_Mixed_Nebulae_Local_Stars_left.png",
		"../3Dprog22/assets/Skyboxes/Medium_Mixed_Nebulae_Local_Stars/Textures/Medium_Mixed_Nebulae_Local_Stars_right.png",
		"../3Dprog22/assets/Skyboxes/Medium_Mixed_Nebulae_Local_Stars/Textures/Medium_Mixed_Nebulae_Local_Stars_down.png",
		"../3Dprog22/assets/Skyboxes/Medium_Mixed_Nebulae_Local_Stars/Textures/Medium_Mixed_Nebulae_Local_Stars_up.png",
		"../3Dprog22/assets/Skyboxes/Medium_Mixed_Nebulae_Local_Stars/Textures/Medium_Mixed_Nebulae_Local_Stars_front.png",
		"../3Dprog22/assets/Skyboxes/Medium_Mixed_Nebulae_Local_Stars/Textures/Medium_Mixed_Nebulae_Local_Stars_back.png"
	};

	mSkyBox = new SkyBox(skyboxfaces);
	mSkyBox->init();

	//all meshes
	mAllMeshes.emplace_back(mHeightmap);
	mAllMeshes.emplace_back(mPlayer);
	//init and insert to oct tree
	for (auto& obj : mMeshes)
	{
		mAllMeshes.emplace_back(obj);
		obj->init();
		mOctTree->insert(obj);
		obj->setLightLoc(lightpos);
	}
	for(auto& billboard : mBillboards)
	{
		billboard->init();
	}

}

void Oblig3::render()
{
	Level::render();

	if (mShadowMap)mShadowMap->draw(mAllMeshes);

	if (mSkyBox)mSkyBox->draw();

	//glm::vec3 ploc = mPlayer->getLocation(); // get player location
	for (auto& mesh : mMeshes)
	{
		mesh->draw();
		mesh->sendTime(1);
	}
	
	if (mHeightmap)mHeightmap->draw();
	//if (mSun) mSun->draw();
	
	if(bDebugLines)
	{
		drawDebugshapes();
		mOctTree->drawTree();
	}

	//draw light direction
	auto lightdir = RENDERWINDOW->mLightDir;
	glm::mat4 sphere1{ 1.f };
	glm::mat4 sphere2{ 1.f };
	sphere2 = glm::translate(sphere2, glm::normalize(lightdir) * 10.f);
	RENDERWINDOW->drawDebugShape("sphere", sphere1);
	RENDERWINDOW->drawDebugShape("sphere", sphere2);

	/*glm::mat4 ma{ 1.f };
	glm::scale(ma, glm::vec3(10));
	RenderWindow::Get()->drawDebugShape("cube", mPlayer->getModelMat());*/

	//set sun location
	glm::vec3 newsunloc = mPlayer->getLocation()+ (lightdir *(mCamera->getfar()/2.f));
	newsunloc.z = 100.f+mPlayer->getLocation().z;
	mBillboards[0]->setlocation(newsunloc);
	//draw transparancy last
	for (auto& billboard : mBillboards)
	{
		billboard->draw();
	}

	mPlayer->tick(1.f);

	mOctTree->checkCollision(mPlayer);

	
}

void Oblig3::wheelEvent(QWheelEvent* event)
{
	Level::wheelEvent(event);
}

void Oblig3::keyPressEvent(QKeyEvent* event)
{
	Level::keyPressEvent(event);
}

void Oblig3::keyReleaseEvent(QKeyEvent* event)
{
	Level::keyReleaseEvent(event);
}

void Oblig3::exposeEvent()
{
	Level::exposeEvent();
}

void Oblig3::deleteMesh(Mesh* mesh)
{
	Level::deleteMesh(mesh);
}
