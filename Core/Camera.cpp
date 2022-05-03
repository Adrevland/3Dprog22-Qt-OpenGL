#include "Camera.h"

#include "renderwindow.h"
#include "Mesh/Mesh.h"

Camera::Camera(float width, float height, float retinascale)
{
	mProjection = glm::perspective(glm::radians(45.f), (width * retinascale / height * retinascale), mZNear, mZFar); // perspective matrix
}

Camera::~Camera()
{
}

void Camera::updateProjection()
{
	mProjection = glm::perspective(glm::radians(fov), (mWidth * mRetinascale / mHeight * mRetinascale), mZNear, mZFar); // perspective matrix
} 
void Camera::updateWinSize(float width, float height, float retinascale)
{
	mWidth = width;
	mHeight = height;
	mRetinascale = retinascale;

	updateProjection();
}
void Camera::updateView()
{
	//mDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//mDirection.y = sin(glm::radians(pitch));
	//mDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	////mDirection = glm::normalize(mTargetPos - mCameraPos);
	//mFront = mDirection;
	mRight = glm::normalize(glm::cross(mFront, worldup));
	mUp = glm::normalize(glm::cross(mFront, -mRight));
	
	mView = glm::lookAt(mLocation, mTarget, mUp);
}

void Camera::followObject(Mesh* obj)
{
	//mLocation = obj->getLocation();

	glm::vec3 objloc =  obj->getLocation();
	
	mLocation = objloc - mFront*mCamdistance;
	if(mLocation.x > 10 && mLocation.y > 10)
	{
		if (mLocation.z <= RENDERWINDOW->getLevel()->getHeightmap()->getHeight(mLocation)+4.f)
		{

			mpitch -= 0.5f;
			mTarget = objloc;

			mDirection.x = cos(glm::radians(myaw)) * cos(glm::radians(mpitch));
			mDirection.y = sin(glm::radians(myaw)) * cos(glm::radians(mpitch));
			mDirection.z = sin(glm::radians(mpitch));

			mFront = glm::normalize(mDirection);
			mTarget = mLocation + mFront;
			updateView();
			updateProjection();

		}
		else if (mpitch < ppitch && mLocation.z >= RENDERWINDOW->getLevel()->getHeightmap()->getHeight(mLocation) + 10.f)
		{

			mpitch += 0.1f;
			mTarget = objloc;

			mDirection.x = cos(glm::radians(myaw)) * cos(glm::radians(mpitch));
			mDirection.y = sin(glm::radians(myaw)) * cos(glm::radians(mpitch));
			mDirection.z = sin(glm::radians(mpitch));

			mFront = glm::normalize(mDirection);
			mTarget = mLocation + mFront;
			updateView();
			updateProjection();

		}
		else
		{
			mTarget = objloc;

			updateView();
		}

	}
	else
	{
		mTarget = objloc;

		updateView();
	}
	

	
}

void Camera::Zoom(double zoomAmount)
{
	/*mLocation += -mFront * (float)zoomAmount;*/
	if (mCamdistance < 10) mCamdistance = 10;
	else if (mCamdistance > 100) mCamdistance = 100;

		mCamdistance -= zoomAmount;
		updateProjection();
		updateView();

}

void Camera::rotate(float yaw, float pitch)
{
	mTarget = mLocation + mFront;

	mpitch += pitch;
	myaw += yaw;

	if (mpitch > 89.0f)
		mpitch = 89.0f;
	else if (mpitch < -89.0f)
		mpitch = -89.0f;

	/*if (mpitch > 359.0f)
		mpitch = 0.0f;
	else if (mpitch < 0.0f)
		mpitch = 359.0f;*/


	mDirection.x = cos(glm::radians(myaw)) * cos(glm::radians(mpitch));
	mDirection.y = sin(glm::radians(myaw)) * cos(glm::radians(mpitch));
	mDirection.z = sin(glm::radians(mpitch));

	
	//mDirection = glm::normalize(mTargetPos - mCameraPos);
	mFront = glm::normalize(mDirection);
	mTarget = mLocation + mFront;
	updateView();
	updateProjection();
}


