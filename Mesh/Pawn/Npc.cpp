#include "Npc.h"
#include "Core/renderwindow.h"
#include "Core/Utils.h"
#include "Mesh/Static/Heightmap.h"

Npc::Npc(Shader* Shader, glm::mat4 modelMat, const char* ObjFile, const char* texture)
	:Mesh(Shader, modelMat, ObjFile, texture)
{
}

Npc::~Npc()
{
}

void Npc::init()
{

	rightSensor = new BSphere(getLocation(),3.f);
	ForwardSensor = new BSphere(getLocation(), 3.f);
	LeftSensor = new BSphere(getLocation(), 3.f);
	TopSensor = new BSphere(getLocation(), 1.f); // if i get stuck under object
	//mCollisionPrimitive = new BSphere(getLocation(), Utils::radiusMesh(this));
	mCollisionPrimitive = new BSphere(getLocation(), 2.f);
	lasttime = std::chrono::high_resolution_clock::now();
	Collisiontime = std::chrono::high_resolution_clock::now();

	collisionSensors.emplace_back(rightSensor);
	collisionSensors.emplace_back(ForwardSensor);
	collisionSensors.emplace_back(LeftSensor);
	collisionSensors.emplace_back(TopSensor);

	Mesh::init();
	
}

void Npc::draw()
{
	Mesh::draw();
}

bool Npc::onBeginOverlap(CollisionPrimitive* primitive)
{
	return Mesh::onBeginOverlap(primitive);
}

void Npc::tick(float deltatime)
{
	auto currenttime = std::chrono::high_resolution_clock::now();
	if(!stunned)
	{
		//fix if it gets stuck

		
		auto SecondsSinceLastCollision = std::chrono::duration<float>(currenttime - Collisiontime).count();

		if (SecondsSinceLastCollision >= 0.5f)
		{
			
		}
		moveForward(1);
		updateSensors();
		if(score <= RENDERWINDOW->getLevel()->NpcScore)
		{
			score++;
			for (int i = 0; i < TrophyPoints.size(); ++i)
			{
				if (TrophyPoints[i] == target)
				{
					TrophyPoints.erase(TrophyPoints.begin() + i);
				}
			}

			if(TrophyPoints.size() >= 1)
			{
				target = glm::vec3(9999.f);
				findTarget();
			}
			/*TakenTargets.emplace_back(target);
			if(TakenTargets.size() < TrophyPoints.size())
			{
				findTarget();
			}*/
		}
		setonmap();
	}
	auto secondsStunned = std::chrono::duration<float>(currenttime - lasttime).count();

	if (secondsStunned >= 2 && stunned)
	{
		stunned = false;
		LOG_HIGHLIGHT("Npc No longer Stunned");
		lasttime = std::chrono::high_resolution_clock::now();
	}
}

void Npc::findTarget()
{
	//todo sort

	for(auto& pos: TrophyPoints)
	{
		if (glm::length(getLocation() - pos) < glm::length(getLocation() - target))
		{

			target = pos;

		}
	}


}

void Npc::rotate(bool right)
{

}

void Npc::moveForward(float amount)
{
	frontVector = findDirection();
	mModelMat = glm::translate(mModelMat,frontVector*(speed* amount));
	//LOG_HIGHLIGHT(Utils::vecToString(getLocation()));
}

void Npc::moveright(float rightamount)
{
	glm::vec3 rightvector = glm::cross(frontVector, glm::vec3(0.f, 0.f, 1.f));
	rightvector = glm::normalize(rightvector);
	mModelMat = glm::translate(mModelMat, rightvector * (rightamount * speed*2.f));
}

glm::vec3 Npc::findDirection()
{
	glm::vec3 direction = getLocation() - target;
	//draw sphere on target trophy
	direction.z = 0.f;
	return glm::normalize(-direction);
}

void Npc::handleCollision(CollisionPrimitive* sensor)
{
	//handle sensor collision
	
	if (sensor == TopSensor)
	{
		//if stuck under object move backwards and left/right
		Collisiontime = std::chrono::high_resolution_clock::now();
		moveForward(-2);
		if (lastcollision == rightSensor)
		{
			//move left
			moveright(-1.f);
			findTarget();
		}
		else
		{
			//moveright
			moveright(1);
			findTarget();
		}
	}
	else if(sensor == ForwardSensor)
	{
		pushback(getLocation());
		Collisiontime = std::chrono::high_resolution_clock::now();
		if(lastcollision == rightSensor)
		{
			//move left
			moveright(-1.f);
			findTarget();
		}
		else
		{
			//moveright
			moveright(1);
			findTarget();
		}
		//lastcollision = sensor;
		LOG_WARNING("Forward sensor collison");
	}
	else if(sensor == rightSensor)
	{
		//moveForward(1);
		lastcollision = sensor;
		LOG_WARNING("¨Right sensor collison");
	}
	else if(sensor == LeftSensor)
	{
		//moveForward(1);
		lastcollision = sensor;
		LOG_WARNING("Left sensor collison");
	}
}

void Npc::updateSensors()
{
	
	ForwardSensor->Center = getLocation() + frontVector* sensorDistance;
	glm::vec3 rightvector = glm::cross(frontVector, glm::vec3(0.f, 0.f, 1.f));
	rightvector = glm::normalize(rightvector);
	rightSensor->Center = getLocation() + rightvector* sensorDistance;
	LeftSensor->Center = getLocation() - rightvector * sensorDistance;
	TopSensor->Center = getLocation() + glm::vec3(0.f, 0.f, 1.f) * sensorDistance/2.f;
}

void Npc::drawDebugLines(bool bdraw)
{
	Mesh::drawDebugLines(bdraw);
	rightSensor->draw();
	ForwardSensor->draw();
	LeftSensor->draw();
	TopSensor->draw();
	//draw target
	glm::mat4 targetm = glm::translate(glm::mat4{ 1.f }, target);
	targetm = glm::scale(targetm, glm::vec3(10.f));
	RENDERWINDOW->drawDebugShape("sphere", targetm);
}

void Npc::pushback(glm::vec3 pos)
{
	auto newloc = pos - frontVector;
	setLocation(newloc);

	setonmap();// set z
}

void Npc::setonmap()
{
	if (mHeightmap)
	{
		auto p = getLocation();
		p.z = mHeightmap->getHeight(getLocation()) + 1;
		setLocation(p);
	}
	mCollisionPrimitive->Center = getLocation();
}
