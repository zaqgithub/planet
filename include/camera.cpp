#include "precompile.h"
#include"camera.h"
#include"ShaderClass.h"


void Camera::ProcessMouseMovement(double xposIn,double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		cursorX = xpos;
		cursorY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - cursorX;
	float yoffset = cursorY - ypos; // reversed since y-coordinates go from bottom to top
	cursorX = xpos;
	cursorY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void Camera::ProcessMouseScroll(float yoffset) {
	fov -= yoffset;
	if (fov < 1.0f) {
		fov = 1.0f;
	}
	if (fov > 45.0f) {
		fov = 45.0f;
	}
}
void Camera::ProcessKeyBoard(DIRECTION direction,float speed) {
	switch (direction) {
	case(FORWARD): {
		cameraPos += speed * cameraFront;
		break;
	}
	case(BACKWARD): {
		cameraPos -= speed * cameraFront;
		break;
	}
	case(LEFT): {
		cameraPos -= speed * glm::normalize(glm::cross(cameraFront,up));
		break;
	}
	case(RIGHT): {
		cameraPos +=   glm::normalize(glm::cross(cameraFront, up))*speed;
		break;
	}
	//case(Q): {
	//	up += glm::normalize(glm::cross(cameraFront, up))*speed;
	//	break;
	//}
	//case(E): {
	//	up -= glm::normalize(glm::cross(cameraFront, up)) * speed;

	//	break;
	//}
	}	
}
void Camera::Focus() {

}
void Camera::dive(float speed) {
	cameraPos -= glm::vec3(0.0f, 1.0f, 0.0f)*speed;
}
void Camera::rise(float speed) {
	cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}
glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}