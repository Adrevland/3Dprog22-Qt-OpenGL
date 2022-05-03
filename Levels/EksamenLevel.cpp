#include "EksamenLevel.h"
#include "Core/renderwindow.h"
#include "Core/Utils.h"
#include "Mesh/Static/Trophy.h"
#include "Mesh/Static/SkyBox.h"
#include "Physics/Shadows.h"
#include "Mesh/Billboard/BillBoard.h"
#include "glm/glm.hpp"
#include "Mesh/Static/BezierCurve.h"
#include "Mesh/Pawn/BomberNpc.h"
#include "Mesh/Pawn/Npc.h"
#include "Mesh/Static/Fence.h"

EksamenLevel::EksamenLevel()
	:Level(RENDERWINDOW)
{

}

EksamenLevel::~EksamenLevel()
{
}

void EksamenLevel::init()
{
	Level::init();

	std::srand(std::time(nullptr)); // seed rand using current time

	//biggest oct size
	mOctTree = new OctTree(BoundingBox(glm::vec3(1.f), glm::vec3(1200.f), mShaderPrograms["color"]));

	//creating shadows
	mShadowMap = new Shadows();
	mShadowMap->setup();

	//heightmap
	mHeightmap = new Heightmap(0.5f, "../3Dprog22/Assets/Heightmaps/Eksamenheightmap.png", "../3Dprog22/Assets/Heightmaps/Eksamentexture.png");
	mHeightmap->init();

	//ui elements
	mUiElements["stun"] = new BillBoard("../3Dprog22/Assets/BillBoards/Stunned.png");
	mUiElements["loose"] = new BillBoard("../3Dprog22/Assets/BillBoards/Loose.png");
	mUiElements["win"] = new BillBoard("../3Dprog22/Assets/BillBoards/vinner.png");
	for(auto& board: mUiElements)
	{
		
		board.second->init();
		board.second->scale(glm::vec3(30.f));
	}

	//bezier
	std::vector<glm::vec3> bezierpoints;
	for (int i = 0; i < 16; ++i)
	{
		auto y = std::rand() % +(mHeightmap->mHeight-50) + 50;
		auto x = std::rand() % +(mHeightmap->mWidth-50) +50;
		bezierpoints.emplace_back(glm::vec3(x, y, mHeightmap->getHeight(glm::vec3(x, y, 0)) + 50));
	}
	mBezier = new BezierCurve(bezierpoints);
	mBezier->init();
	//mMeshes.emplace_back(mBezier);
	//billboards sun //todo change to mesh sun
	//mBillboards.emplace_back(new BillBoard("../3Dprog22/Assets/Textures/Quantamagazine-scaled.jpg"));
	////mBillboards.emplace_back(new BillBoard("../3Dprog22/Assets/Textures/OleSolpepega.jpg"));
	//mBillboards[0]->scale(glm::vec3(50.f));

	//light
	//directional light
	glm::vec3 sunpos{ 600.f,600.f,100.f };
	glm::vec3 lightpos{ RENDERWINDOW->mLightDir }; //directional light used for shadows. //todo make shadows use point light or yeet shadows
	//sun
	//todo sun mesh
	glm::mat4 sunloc = glm::translate(glm::mat4{ 1.f }, sunpos);
	sunloc = glm::scale( sunloc, glm::vec3{10.f} );
	mSun = (new Mesh(mShaderPrograms["Texture"], sunloc, "../3Dprog22/ObjFiles/Sphere.obj", "../3Dprog22/Assets/Textures/suncolor.png"));
	if (mSun)
	{
		mSun->init();
		mSun->setLightLoc(sunpos);
	}

	mHeightmap->setLightLoc(lightpos);

	//player
	mPlayer = new Player(mShaderPrograms["lightshadow"], glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
	mMeshes.emplace_back(mPlayer);
	mPlayer->setLocation(glm::vec3(600.f, 600.f, 49.f));
	mPlayer->setHeightmap(mHeightmap);

	//bomber npc
	npcBomber = new BomberNpc("../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg", mBezier);
	npcBomber->init();

	//npc
	mNpc = new Npc(mShaderPrograms["lightshadow"], glm::mat4{ 1.f }, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
	mNpc->init();
	mNpc->setLocation(glm::vec3(650.f, 650.f, 49.f));
	mNpc->setHeightmap(mHeightmap);
	//editor camera mesh
	mCameraMesh = new Mesh(mShaderPrograms["lightshadow"], glm::scale(glm::mat4{ 1.f }, glm::vec3(0.5f)), "../3Dprog22/Assets/Meshes/Camera/camera.obj", "../3Dprog22/Assets/Meshes/Camera/camera.png");
	mCameraMesh->init();

	//make random trophys locations
	std::vector<glm::vec3> TrophyPoints;
	for (int i = 0; i < 20; ++i)
	{
		auto y = std::rand() % +(mHeightmap->mHeight - 50) + 50;
		auto x = std::rand() % +(mHeightmap->mWidth - 50) + 50;
		TrophyPoints.emplace_back(glm::vec3(x, y, mHeightmap->getHeight(glm::vec3(x, y, 0)) + 5));
	}

	std::vector<std::pair<const char*, const char*>> trophymeshes;
	trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/cherry.obj", "../3Dprog22/Textures/cherry.jpg"));
	trophymeshes.emplace_back(std::pair<const char*, const char*>("../3Dprog22/ObjFiles/grape.obj", "../3Dprog22/Textures/grape.jpg"));

	for (int i = 0; i < TrophyPoints.size(); ++i)
	{
		glm::mat4 tmpTLoc = glm::translate(glm::mat4(1.f), TrophyPoints[i]);
		tmpTLoc = glm::scale(tmpTLoc, glm::vec3(5.f));
		if(i >= TrophyPoints.size()/2.f)
		{
			auto tmp = new Trophy(mShaderPrograms["lightshadow"], tmpTLoc, trophymeshes.at(0).first, trophymeshes.at(0).second);
			tmp->trophytype = TROPHYTYPE::Red;
			mMeshes.emplace_back(tmp);
		}
		else
		{
			auto tmp = new Trophy(mShaderPrograms["lightshadow"], tmpTLoc, trophymeshes.at(1).first, trophymeshes.at(1).second);
			tmp->trophytype = TROPHYTYPE::Blue;
			mMeshes.emplace_back(tmp);
			mNpc->sendTrophyspoints(TrophyPoints[i]);
		}

	}
	//create fences
	for (int i = 0; i < 20; ++i)
	{
		float y = std::rand() % +(mHeightmap->mHeight - 50) + 50;
		float x = std::rand() % +(mHeightmap->mWidth - 50) + 50;
		float scaleX = std::rand() % +30;
		float scaleY = std::rand() % +30;
		float scalez = std::rand() % +30;

		glm::mat4 mat = glm::translate(glm::mat4{ 1.f }, glm::vec3(x, y, mHeightmap->getHeight(glm::vec3(x, y, 0)) + scalez));
		mat = glm::scale(mat, glm::vec3(scaleX, scaleY, scalez));
		//todo change from kirby mesh
		auto fence = new Fence(mShaderPrograms["lightshadow"], mat, "../3Dprog22/ObjFiles/Kirby.obj", "../3Dprog22/Textures/kirby.jpg");
		fence->setViewMode(2);
		fences.emplace_back(fence);
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
	mAllMeshes.emplace_back(npcBomber);
	mAllMeshes.emplace_back(mNpc);

	//init and insert to oct tree
	for(auto& obj: fences)
	{
		obj->init();
		mAllMeshes.emplace_back(obj);
	}

	for (auto& obj : mMeshes)
	{
		mAllMeshes.emplace_back(obj);
		obj->init();
		mOctTree->insert(obj);
		obj->setLightLoc(lightpos);
	}
	for (auto& billboard : mBillboards)
	{
		billboard->init();
	}

}

void EksamenLevel::render()
{
	Level::render();
	//check win condition
	if (PlayerScore >= WinScore)
		PlayerWon = true;
	else if (NpcScore >= WinScore)
		NpcWon = true;


	if (mShadowMap)mShadowMap->draw(mAllMeshes);

	if (mSkyBox)mSkyBox->draw();

	if (mBezier)mBezier->draw();
	if (npcBomber)npcBomber->draw();
	if (mNpc)mNpc->draw();
	for (auto& mesh : mMeshes)
	{
		mesh->draw();
		mesh->sendTime(1);
	}
	for(auto& fenc: fences)
	{
		fenc->draw();
	}
	if (mHeightmap)mHeightmap->draw();
	if (mSun) mSun->draw();
	//draw camera
	if (CameraType == CAMERAMODE::Flying && mCameraMesh){
		glm::mat4 rotationmat = {
			{mCamera->PlayerCamRight,0.f},
			{mCamera->PlayerCamForward,0.f},
			{0.f,0.f,1.f,0.f},
			{0.f,0.f,0.f,1.f},
		};

		glm::vec3 campos = mCamera->PlayerCamLocation;
		glm::mat4 translation = glm::translate(glm::mat4(1.f), campos);
		mCameraMesh->setModelMat(translation * rotationmat);

		mCameraMesh->draw();
		glm::mat4 xyzscale = glm::scale(glm::mat4{ 1.f }, glm::vec3(1000.f));
		RENDERWINDOW->drawDebugShape("xyz", xyzscale);
	}
	if (bDebugLines)
	{
		drawDebugshapes();
		mOctTree->drawTree();
		for (auto mesh : fences)
		{
			mesh->drawDebugLines(bDebugLines);
		}
	}

	////draw light direction
	
	
	//LOG_HIGHLIGHT(Utils::vecToString(RENDERWINDOW->mLightDir));
	/*glm::mat4 sphere1{1.f};
	glm::mat4 sphere2{ 1.f };
	sphere2 = glm::translate(sphere2, glm::normalize(lightdir) * 10.f);
	RENDERWINDOW->drawDebugShape("sphere", sphere1);
	RENDERWINDOW->drawDebugShape("sphere", sphere2);*/

	/*glm::mat4 ma{ 1.f };
	glm::scale(ma, glm::vec3(10));
	RenderWindow::Get()->drawDebugShape("cube", mPlayer->getModelMat());*/
	//todo pause game
	if(CameraType == CAMERAMODE::Follow && !PlayerWon && !NpcWon)
	{
		auto lightdir = RENDERWINDOW->mLightDir;
		//rotate sun
		glm::mat4 rotmat = glm::rotate(glm::mat4(1.f), glm::radians(daylightcyclespeed), glm::vec3(0.f, 0.f, 1.f));
		lightdir = glm::vec3(rotmat * glm::vec4(lightdir, 1.f));

		RENDERWINDOW->mLightDir = glm::normalize(lightdir);
		//set sun location
		glm::vec3 newsunloc = mPlayer->getLocation() + (lightdir * (mCamera->getfar() / 1.2f));
		newsunloc.z = 100.f + mPlayer->getLocation().z; // set sun closer to ground
		mSun->setLocation(newsunloc);
		for (auto& mesh : mAllMeshes)
		{
			mesh->setLightLoc(lightdir);
		}
		mPlayer->tick(1.f);
		npcBomber->tick();
		if (mNpc)mNpc->tick(1);
		for (auto& obj : fences)
		{
			obj->checkOverlap(mPlayer);
			obj->checkOverlap(mNpc);
		}
		mOctTree->checkCollision(mPlayer);
		if (npcBomber)npcBomber->checkoverlap(mPlayer);
		if (npcBomber)npcBomber->checkoverlap(mNpc);
	}

	//draw transparancy last
	for (auto& billboard : mBillboards)
	{
		billboard->draw();
	}

	//draw stun message
	if(mPlayer->isStunned())
	{
		glm::vec3 billboardloc = mPlayer->getLocation();
		billboardloc.z = 10.f + mPlayer->getLocation().z;
		mUiElements["stun"]->setlocation(billboardloc);
		mUiElements["stun"]->draw();
	}
	if (PlayerWon)
	{
		glm::vec3 billboardloc = mPlayer->getLocation();
		billboardloc.z = 15.f + mPlayer->getLocation().z;
		mUiElements["win"]->setlocation(billboardloc);
		mUiElements["win"]->draw();
	}
	else if (NpcWon)
	{
		glm::vec3 billboardloc = mPlayer->getLocation();
		billboardloc.z = 15.f + mPlayer->getLocation().z;
		mUiElements["loose"]->setlocation(billboardloc);
		mUiElements["loose"]->draw();
	}

}

void EksamenLevel::wheelEvent(QWheelEvent* event)
{
	Level::wheelEvent(event);
}

void EksamenLevel::keyPressEvent(QKeyEvent* event)
{
	Level::keyPressEvent(event);
}

void EksamenLevel::keyReleaseEvent(QKeyEvent* event)
{
	Level::keyReleaseEvent(event);
}

void EksamenLevel::exposeEvent()
{
	Level::exposeEvent();
}

void EksamenLevel::deleteMesh(Mesh* mesh)
{
	Level::deleteMesh(mesh);
}
