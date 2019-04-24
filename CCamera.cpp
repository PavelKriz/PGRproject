#include "CCamera.h"


void CCamera::recalculateYawPitch()
{
	pitch = (float) glm::asin(vFront.y);
	yaw = (float) glm::acos((-1.0 * (vFront.z / glm::cos(pitch))));
	if (vFront.x < 0) {
		yaw = yaw * (-1.0f);
	}
	pitch = glm::degrees(pitch);
	yaw = glm::degrees(yaw);
}

CCamera::CCamera(const glm::vec3 & toPosition, float fov, float aspect, float zNear, float zFar,const float toSpeed)
{
	float yaw = 0;
	float pitch = 0;
	cameraSpeed = toSpeed;
	state = FREE;
	nextState = FREE;
	mPerspective = glm::perspective(fov, aspect, zNear, zFar);
	vActualPos = vPosition = toPosition;
	vActualFront = vFront = glm::vec3(0.0f, 0.0f, -1.0f);
	vUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void CCamera::CameraMoveForward()
{
	if (state == FREE) { vPosition += cameraSpeed * vFront; }
	vActualPos = vPosition;
}

void CCamera::CameraMoveBackward()
{
	if (state == FREE) { vPosition -= cameraSpeed * vFront; }
	vActualPos = vPosition;
}

void CCamera::CameraMoveUp()
{
	if (state != FREE) { return; }
	glm::vec3 right = glm::normalize(glm::cross(vFront, vUp));
	glm::vec3 up = glm::normalize(glm::cross(vFront, right));
	vPosition -= up * cameraSpeed;
	vActualPos = vPosition;
}

void CCamera::CameraMoveDown()
{
	if (state != FREE) { return; }
	glm::vec3 right = glm::normalize(glm::cross(vFront, vUp));
	glm::vec3 up = glm::normalize(glm::cross(vFront, right));
	vPosition += up * cameraSpeed;
	vActualPos = vPosition;
}

void CCamera::CameraMoveLeft()
{
	if (state == FREE) { vPosition -= glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed; }
	vActualPos = vPosition;
}

void CCamera::CameraMoveRight()
{
	if (state == FREE) { vPosition += glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed; }
	vActualPos = vPosition;
}

void CCamera::setFront(const glm::vec3 & toFront)
{
	if (state == FREE) {
		vActualFront = vFront = toFront;
	}
}

void CCamera::changeViewType(viewState which, double time)
{
	switch (which) {
	case FREE:
		if (state == TRANSITION) break;
		if (state == FREE) break;
		transitionBaseTime = time;
		state = FREE;
		nextState = FREE;
		recalculateYawPitch();
		break;
	case TRANSITION:
		break;
	case LOCK_ONE:
		if (state == TRANSITION) break;
		if (state == LOCK_ONE) break;
		transitionBaseTime = time;
		state = TRANSITION;
		nextState = LOCK_ONE;

		vPositionNext = glm::vec3(1.901f * 5.0f, 1.2524f * 5.5f, -0.3343f * 5.0f);
		vFrontNext = glm::vec3(-0.7499f, -0.6494f, 0.1255f);
		vUpNext = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case LOCK_TWO:
		if (state == TRANSITION) break;
		if (state == LOCK_TWO) break;
		transitionBaseTime = time;
		state = TRANSITION;
		nextState = LOCK_TWO;

		vPositionNext = glm::vec3(-1.41f * 5.0f, -0.72f* 3.0f, 1.63f* 5.0f);
		vFrontNext = glm::vec3(0.65f, 0.22f, -0.71f);
		vUpNext = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	}
}

glm::mat4 CCamera::GetViewProjection(double time)
 {
	 double alpha = 0.0f;
	 glm::vec3 tempPos;
	 glm::vec3 tempFront;
	 glm::vec3 tempUp;
	 switch (state) {
	 case FREE:
		 return mPerspective * glm::lookAt(vPosition, vPosition + vFront, vUp);
	 case TRANSITION:
		 alpha = (time - transitionBaseTime) / 2;

		 if (alpha > 1.0f) {
			 alpha = 1.0f;
			 vActualPos = vPosition = vPositionNext;
			 vActualFront = vFront = vFrontNext;
			 vUp = vUpNext;

			 state = nextState;
		 }
		 vActualPos = tempPos = glm::mix(vPosition, vPositionNext, alpha);
		 vActualFront = tempFront = glm::mix(vFront, vFrontNext, alpha);
		 tempUp = glm::mix(vUp, vUpNext, alpha);
		 return mPerspective * glm::lookAt(tempPos, tempPos + tempFront, vUp);
	 case LOCK_ONE:
		 return mPerspective * glm::lookAt(vPosition, vPosition + vFront, vUp);
	 case LOCK_TWO:
		 return mPerspective * glm::lookAt(vPosition, vPosition + vFront, vUp);

	 }

	 return mPerspective * glm::lookAt(vPosition, vPosition + vFront, vUp);
 }


glm::vec3 CCamera::getPosition() {
	return vActualPos;
}

glm::vec3 CCamera::getDirection() {
	return vActualFront;
}
