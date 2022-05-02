#pragma once
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


class VisualObject;
class Mesh;

class Camera
{
public:
	Camera(float width, float height, float retinascale);
	~Camera();

	void updateProjection();
	void updateView();
	void followObject(Mesh* obj);
	void Zoom(double zoomAmount);
	void rotate(float yaw, float pitch);

	//setters
	void setLocation(const glm::vec3 newpos) { mLocation = newpos; updateView(); }
	void updateWinSize(float width, float height, float retinascale);
	void setTarget(glm::vec3 target) { mTarget = target; updateView(); }
	void setPlayeryaw(float yaw) { pyaw = yaw; };
	

	//getters
	glm::vec3 getFront() { updateView(); return mFront; }
	glm::vec3 getUp() { updateView(); return mUp; }
	glm::vec3 getLocation() { updateView(); return mLocation; }
	glm::vec3 getRight() { updateView(); return mRight; }
	glm::mat4 getProjection() { updateProjection();  return mProjection; }
	glm::mat4 getView() { updateView(); return mView; }
	float getYaw() { return myaw; }
	float getPitch() { return mpitch; }
	float getRoll() { return mroll; }

	float getfar() { return mZFar; }
	float getnear() { return mZNear; }
	float getfov() {return fov; }


	glm::vec3 PlayerCamLocation{ 1.f };
	glm::vec3 PlayerCamForward{ 1.f };
	glm::vec3 PlayerCamRight{ 1.f };


	void resetrotation()
	{
		mpitch = ppitch;
		myaw = pyaw;
		mroll = proll;
		updateView();
	}; // set default rotation
private:

	
	glm::vec3 mUp{ 0.0f,0.f,1.0f };
	glm::vec3 mFront{ 0.0f,1.0f,0.0f };
	glm::vec3 mLocation{ 0.f,0.f,20.f }; // random start location
	const glm::vec3 worldup{ 0.f,0.f,1.f };
	//vec
	glm::vec3 mTarget{mLocation+mFront};
	glm::vec3 mDirection{ glm::normalize(mLocation - mTarget) };
	glm::vec3 mRight{ glm::normalize(glm::cross(mUp,mDirection)) };
	
	//view matrix
	glm::mat4 mView{ glm::lookAt(mLocation, mTarget, mUp)};

	glm::vec3 mDistanceFromPlayer{  0.f,-20.f,20.f};
	float mCamdistance{ 40.f };

	//Projection matrix
	float mZNear{ 0.1f };
	float mZFar{ 1000.f };

	float myaw{ -90.f }, mpitch{-10.f}, mroll{0.f};
	float pyaw{ -90.f }, ppitch{ -10.f }, proll{ 0.f }; // player default values

	float fov{ 45.0f };

	glm::mat4 mProjection{ 1.f };

	float mWidth, mHeight, mRetinascale;
};
