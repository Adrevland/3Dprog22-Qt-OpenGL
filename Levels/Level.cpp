#include "Level.h"

#include <glm/gtc/quaternion.hpp>

#include "Mesh/Mesh.h"
#include "Core/Utils.h"
#include "Mesh/Pawn/Player.h"
#include "core/logger.h"
#include "core/renderwindow.h"

Level::Level(RenderWindow* rWindow)
	:mRenderWindow(rWindow)
{
}

Level::~Level()
{
    //delete all pointers

}

void Level::init()
{
    mCamera = RenderWindow::Get()->getCamera();
    setDebugLines();
    ////general OpenGL stuff:
    //glEnable(GL_BLEND);
    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_MULTISAMPLE); // AA
    //glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    //glClearColor(21.f / 255, 17.f / 255.f, 51.f / 255.f, 1.0f);    //Background color used in glClear GL_COLOR_BUFFER_BIT

}

void Level::render()
{

    if(CameraType == CAMERAMODE::Follow)
    {
        mCamera->setLocation(mPlayer->getLocation());
	    mCamera->followObject(mPlayer);

    }
    if (CameraType == CAMERAMODE::Flying)
        editorCamera();

    /*glm::mat4 light{ 1.f };
    RENDERWINDOW->mLightDir = glm::rotate(light, glm::radians(1.f),glm::vec3(1.f, 0.f, 0.f)) *glm::vec4(1.f);*/
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

}

void Level::drawDebugshapes()
{
        for (auto mesh : mMeshes)
        {
            mesh->drawDebugLines(bDebugLines);
        }
}

void Level::logPlayerLocation()
{
    std::string Playerpos{"Player position : x "};
    Playerpos += (std::to_string(mPlayer->getLocation().x));
    Playerpos += ", y ";
    Playerpos += (std::to_string(mPlayer->getLocation().y));
    Playerpos += ", z ";
    Playerpos += (std::to_string(mPlayer->getLocation().z));
    //RenderWindow::Get()->log(Playerpos, LogType::HIGHLIGHT);
    LOG_HIGHLIGHT(Playerpos);

    //log abcd from map
    //debug only remove false to get debug location back
    if (mHeightmap && false)
    {

        auto vertcopy = mHeightmap->getVertices();
        auto getstring = [](int a,std::vector<Vertex> &v)
        {
            std::string A;
            A += ", X ";
        	A += std::to_string(v[a].x);
            A += ", y ";
            A += std::to_string(v[a].y);
            A += ", z ";
            A += std::to_string(v[a].z);
            return A;
        };

        int a = mPlayer->getLocation().x * mHeightmap->mWidth + mPlayer->getLocation().y;
        RenderWindow::Get()->log("A " + getstring(a,vertcopy), LogType::HIGHLIGHT);

        int b = (mPlayer->getLocation().x + 1) * mHeightmap->mWidth + mPlayer->getLocation().y;
        RenderWindow::Get()->log("B " + getstring(b, vertcopy), LogType::HIGHLIGHT);

        int c = (mPlayer->getLocation().x + 1) * mHeightmap->mWidth + mPlayer->getLocation().y + 1;
        RenderWindow::Get()->log("C " + getstring(c, vertcopy), LogType::HIGHLIGHT);

        int d = (mPlayer->getLocation().x) * mHeightmap->mWidth + mPlayer->getLocation().y + 1;
        RenderWindow::Get()->log("D " + getstring(d, vertcopy), LogType::HIGHLIGHT);

       
    }
    
}

void Level::moveObject(Mesh* mesh)
{
    auto PlayerOct = mOctTree->find(mesh);
    auto playerlocOct = mOctTree->find(mesh->getLocation());
    if (PlayerOct != playerlocOct)
    {
        mOctTree->move(mesh);
    }
}

void Level::setViewMode()
{
    auto vmode = mAllMeshes[0]->getViewMode() + 1;
    for(auto& mesh: mAllMeshes)
    {
        if (vmode > 2) vmode = 0;
        mesh->setViewMode(vmode);
    }
    
}

void Level::setCameramode()
{
    if (CameraType == CAMERAMODE::Follow) 
        CameraType = CAMERAMODE::Flying;
    else
       CameraType = CAMERAMODE::Follow;
}

void Level::setDebugLines()
{
    bDebugLines = !bDebugLines;
    /*for(auto mesh: mMeshes)
    {
        mesh->drawDebugLines(bDebugLines);
    }*/
}

void Level::resetPlayer()
{
    mPlayer->setLocation(glm::vec3(1.f));
}

void Level::editorCamera()
{
	auto forwardspeed = 0.f;
	auto rightspeed = 0.f;
    float speed{ 1.f };

    if (mKeyboard[Qt::Key_W])
        forwardspeed += speed;
    if (mKeyboard[Qt::Key_S])
        forwardspeed -= speed;
    if (mKeyboard[Qt::Key_A])
        rightspeed -= speed;
    if (mKeyboard[Qt::Key_D])
        rightspeed += speed;

    //calc direction
    glm::vec3 rightvector = mCamera->getRight() * rightspeed;
    glm::vec3 frontvector = mCamera->getFront() * forwardspeed;
    glm::vec3 movevector = frontvector + rightvector;

    

    if(movevector != glm::vec3(0.f))
    {
	    mCamera->setLocation(mCamera->getLocation() + movevector);
        mCamera->setTarget(mCamera->getLocation() + mCamera->getFront());
    }
}

void Level::logCamera(){

	
        //right
		RenderWindow::Get()->log("Camera Right = " + Utils::vecToString(mCamera->getRight()), LogType::HIGHLIGHT);
        //up
        RenderWindow::Get()->log("Camera up = " + Utils::vecToString(mCamera->getUp()), LogType::HIGHLIGHT);
        //forward
        RenderWindow::Get()->log("Camera front = " + Utils::vecToString(mCamera->getFront()), LogType::HIGHLIGHT);
        //pitch yaw roll
        RenderWindow::Get()->log("Camera Pitch = " + std::to_string(mCamera->getPitch()), LogType::HIGHLIGHT);
        RenderWindow::Get()->log("Camera Yaw = " + std::to_string(mCamera->getYaw()), LogType::HIGHLIGHT);
        RenderWindow::Get()->log("Camera Roll = " + std::to_string(mCamera->getRoll()), LogType::HIGHLIGHT);

   }


void Level::deleteMesh(Mesh* mesh)
{
    //remove from oct
    mOctTree->remove(mesh);

    //remove from vector
    auto it = std::find(mMeshes.begin(), mMeshes.end(), mesh);
    if (it != mMeshes.end())
    {
        mMeshes.erase(it);
    }

    //todo delete pointer
    //delete mesh; // crashes the program
}

void Level::wheelEvent(QWheelEvent* event)
{
	if(CameraType == CAMERAMODE::Follow)
	{
		auto numDegrees = event->angleDelta() / 8;
		mCamera->Zoom(std::clamp(numDegrees.ry(), -1, 1));
	}
}

void Level::keyPressEvent(QKeyEvent* event)
{
    mKeyboard[event->key()] = true;

    if(event->key() == Qt::Key_E)
    {
        logPlayerLocation();
    }
    if (CameraType == CAMERAMODE::Follow)
		mPlayer->sendKeyboard(mKeyboard);


    if (event->key() == Qt::Key_C)
    {
        logCamera();
    }
}

void Level::keyReleaseEvent(QKeyEvent* event)
{
    mKeyboard[event->key()] = false;

    if (CameraType == CAMERAMODE::Follow)
		mPlayer->sendKeyboard(mKeyboard);
}

void Level::mousePressEvent(QMouseEvent* event)
{
    mMouse[event->button()] = true;
}

void Level::mouseReleaseEvent(QMouseEvent* event)
{
    mMouse[event->button()] = false;
}

void Level::mouseMoveEvent(QMouseEvent* event)
{
    auto [mouseX, mouseY] = event->position();
    
    auto mouseDX = oldmousex - mouseX;
    auto mouseDY = oldmousey - mouseY;
    
    oldmousex = mouseX;
    oldmousey = mouseY;

	if(CameraType == CAMERAMODE::Follow) {
		//yeet rotation from Sten
    	static float currentRotationX{};
    	static float currentRotationY{ -0.2f };
    	if (mMouse[Qt::RightButton])
    	{
    		////QCursor::setPos(oldmousex, oldmousey); // locks mouse in place
    		//currentRotationX += mouseDX / 200.f;
    		//currentRotationY += mouseDY / 200.f;
    		//currentRotationY = std::clamp<float>(currentRotationY, -glm::half_pi<float>() + 0.2f, glm::half_pi<float>() - 1.6f);

    		//// Use matrix to rotate from mouse input.
    		//glm::mat4 camPos(1.f);
    		//camPos = glm::rotate(camPos, currentRotationX, { 0.f, 0.f, 1.f });
    		//camPos = glm::rotate(camPos, currentRotationY, { 1.f, 0.f, 0.f });
    		//glm::vec3 camPosV = camPos * glm::vec4{ 0.f, 0.f, 0.f, 1.f };


            const float sensitivity = 0.1f;
            mouseDX *= sensitivity;
            mouseDY *= sensitivity;


            mCamera->rotate(mouseDX, mouseDY);

    		// Move the rotated camera to the player.
    		/*glm::vec3 newarm = camPosV + mCamera->getSpringarm();
        
    		mCamera->setSpringArm(newarm);*/
    		//mCamera->setTarget(mCamera->getLocation());
    		/*std::string camlocation = std::to_string(mCamera->getLocation().x) +" " + std::to_string(mCamera->getLocation().y) +" " + std::to_string(mCamera->getLocation().z);
    		if (mLogger)mLogger->logText(camlocation, LogType::HIGHLIGHT);
    		else mLogger = Logger::getInstance();*/
    	}
	}
    else if(CameraType == CAMERAMODE::Flying)
    {
        if (mMouse[Qt::RightButton])
        {

            const float sensitivity = 0.1f;
            mouseDX *= sensitivity;
            mouseDY *= sensitivity;


            mCamera->rotate(mouseDX, mouseDY);

        }

    }
	    

    
}

void Level::exposeEvent()
{
    mCamera->updateWinSize(mRenderWindow->width(), mRenderWindow->height(), mRenderWindow->devicePixelRatio());
}
