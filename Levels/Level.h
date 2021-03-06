#pragma once
#include <qevent.h>

#include "core/OctTree.h"
#include <unordered_map>
#include "Mesh/Pawn/Player.h"
#include "Mesh/Pawn/Npc.h"

class BezierCurve;
class BillBoard;
class SkyBox;
class Heightmap;
class RenderWindow;
class Camera;
class Logger;
class Mesh;
class Player;
class Shader;
class Shadows;

enum CAMERAMODE
{
	Follow = 1,
	Flying
};

class Level
{
public:
	Level(RenderWindow* rWindow);
	~Level();

	virtual void init();
	virtual void render();

	void drawDebugshapes();

	void logPlayerLocation();

	void moveObject(Mesh* mesh);
	
	void setViewMode();
	void setCameramode();
	void setPlayerSpeed(float speed) { mPlayer->setSpeed(speed); }
	void setDebugLines();
	//setters 
	void keyBoardinput(std::unordered_map<int, bool> Keyboard) { mKeyboard = Keyboard; }
	void mouseinput(std::unordered_map<int, bool> Mouse) { mMouse = Mouse; }
	
	//getters
	double deltatime() { return mDeltatime; }

	virtual void deleteMesh(Mesh* mesh);
	virtual void wheelEvent(QWheelEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void exposeEvent();

	virtual void resetPlayer();

	void editorCamera();
	Shadows* getShadowmap() { return mShadowMap; }
	Player* getPlayer() { return mPlayer; }
	Npc* getNpc() { return mNpc; }
	Heightmap* getHeightmap() { return mHeightmap; }



	//gidder ikke getters og setters
	bool PlayerWon{ false };
	bool NpcWon{ false };
	int PlayerScore{ 0 };
	int NpcScore{ 0 };

protected:
	RenderWindow* mRenderWindow{nullptr};
	
	CAMERAMODE CameraType{ CAMERAMODE::Follow };
	bool bDebugLines{ true };

	//meshes
	Npc* mNpc{ nullptr };
	Player* mPlayer; //want player accesable outside of vector
	Mesh* mSun{nullptr}; // because it is outside oct
	Mesh* mCameraMesh{ nullptr };

	std::vector <Mesh*> mMeshes;
	std::vector <Mesh*> mAllMeshes;
	std::vector<BillBoard*> mBillboards;
	std::unordered_map<std::string, BillBoard*> mUiElements;
	OctTree* mOctTree;
	Heightmap* mHeightmap{nullptr};
	BezierCurve* mBezier{ nullptr };
	//Shaders
	std::unordered_map<std::string, Shader*> mShaderPrograms;

	std::unordered_map<int, bool> mKeyboard;
	std::unordered_map<int, bool> mMouse;

	Camera* mCamera{nullptr};
	Logger* mLogger{nullptr}; //maybe i need it 
	double mDeltatime{0};

	qreal oldmousex;
	qreal oldmousey;

	//logger functions
	void logCamera();

	SkyBox* mSkyBox{ nullptr };
	Shadows* mShadowMap{ nullptr };


	
private:
};

 