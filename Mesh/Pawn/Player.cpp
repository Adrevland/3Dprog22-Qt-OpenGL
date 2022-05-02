#include "Player.h"

#include <algorithm>
#include "Core/Utils.h"
#include "Core/renderwindow.h"
#include "Mesh/debugTriangle.h"
#include "glm/glm.hpp"


Player::Player(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:Mesh(Shader, modelMat, ObjFile, texture)
{
}

Player::~Player()
{
}

void Player::init()
{
	Mesh::init();
	
	mCollisionPrimitive = new BSphere(getLocation(), Utils::radiusMesh(this));
}

void Player::draw()
{
	Mesh::draw();
}


bool Player::onBeginOverlap(CollisionPrimitive* primitive)
{
	return mCollisionPrimitive->isOverlapping(primitive);
}

void Player::tick(float deltatime)
{
	//movement
	if(!stunned){
		forwardspeed = 0.f;
		rightspeed = 0.f;
		float upspeed{ 0.f };


		if (mkeybInput[Qt::Key_W])
			forwardspeed += speed;
		if (mkeybInput[Qt::Key_S])
			forwardspeed -= speed;
		if (mkeybInput[Qt::Key_A])
			rightspeed -= speed;
		if (mkeybInput[Qt::Key_D])
			rightspeed += speed;
		if (mkeybInput[Qt::Key_Space])
		{
			upspeed += speed;
		}
		if (mkeybInput[Qt::Key_Shift])
		{
			upspeed -= speed;
		}
		if(mkeybInput[Qt::Key_1])
		{
			//log location
			auto loc = getLocation();
			std::string playerloc = std::to_string(loc.x) + " " + std::to_string(loc.y) + " " + std::to_string(loc.z);
			if (mLogger)mLogger->logText(playerloc, LogType::HIGHLIGHT);
		}
		
		rightspeed = std::clamp(rightspeed, -speed, speed);
		forwardspeed = std::clamp(forwardspeed, -speed, speed);

		//calc direction
		//glm::vec3 rightvector = mCamera->getRight() * rightspeed;
		float rotateright = rightspeed;
		glm::vec3 frontvector = mCamera->getFront() * forwardspeed;
		frontvector.z = 0.f;
		/*frontvector *= -1.f;*/
		//glm::vec3 movevector = glm::normalize(frontvector + rightvector);
		glm::vec3 movevector = glm::normalize(frontvector);
		glm::vec3 velocity = -movevector;
		velocity = -glm::normalize(velocity);
		mCamera->rotate(-rotateright*2, 0);
		if(!glm::any(glm::isnan(velocity)))
		{
			glm::vec3 right = glm::cross(velocity , glm::vec3(0.f, 0.f, 1.f) );
			//glm::vec3 right = mCamera->getRight();
			right.z = 0.f;
			right = glm::normalize(right);
			glm::vec3 front = mCamera->getFront();
			front.z = 0.f;
			front = glm::normalize(front);
			glm::mat4 rotationmat = {
				{right,0.f},
				{velocity,0.f},
				{0.f,0.f,1.f,0.f},
				{0.f,0.f,0.f,1.f},
			};
			//glm::mat4 rotationmat = glm::lookAt(mCamera->getLocation(), mCamera->getFront(), glm::vec3(0.f, 0.f, 1.f));
			//	glm::vec3 lastpos = getLocation();
			//	mModelMat = glm::mat4{ 1.f };
			//	//setLocation(lastpos + movevector*speed);
			//	setLocation(lastpos + (mCamera->getFront()*forwardspeed) * speed);
			//	
			
			glm::mat4 translation = glm::translate(mModelMat, velocity * speed);
			mModelMat = translation;// *rotationmat;
			move();

			/*mModelMat = rotationmat;*/
			
		}
	}
	auto currenttime = std::chrono::high_resolution_clock::now();
	auto secondsStunned = std::chrono::duration<float>(currenttime - lasttime).count();

	if(secondsStunned >= 2)
	{
		LOG_WARNING("PLAYER NO LONGER STUNNED");
		stunned = false;
		lasttime = std::chrono::high_resolution_clock::now();
	}
	//move player
	//if(movevector != glm::vec3(0.f))
	//{
	//	mModelMat = glm::translate(mModelMat, movevector);
	//	move();
	//	/*rotate();*/
	//}


}

void Player::sendKeyboard(std::unordered_map<int, bool> &keyInput)
{
	mkeybInput = keyInput;
}

bool Player::isOnMap()
{
	//todo make this right
	if (getLocation().x > 1 && getLocation().y > 1) return true;
	else return false;

}

void Player::move()
{
	if (mHeightmap && isOnMap())
	{
		auto p = getLocation();
		p.z = mHeightmap->getHeight(getLocation()) + 1;
		setLocation(p);
	}
	mCollisionPrimitive->Center = getLocation();
	
}

void Player::rotate()
{
	//rotate towards cameraforward;
	float rotateamount;

	glm::vec3 target = mCamera->getFront();
	target.z = 0;
	target = glm::normalize(target);

	

	glm::mat4 rotationmat = glm::lookAt(getLocation(), target,glm::vec3(0.f,0.f,1.f));

	mModelMat *= rotationmat;
}
