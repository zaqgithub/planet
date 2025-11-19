#pragma once
//define the enum type to determine the camera movement responsing to pressing of keyboard
enum DIRECTION { FORWARD, BACKWARD, LEFT, RIGHT,Q,E };	
class Camera {
public:
	const float sensitivity = 0.1f;							//the sensitivity of mousemovement  

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);		//record the position of the camera
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);	//record where the camera is looking at
	glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);				//indicate the up-side vector

	//global variables defined to control mouse event

	float cursorX = 400.0f;									//record the X position of mouse in coordinate
	float cursorY = 300.0f;									//record the Y position of mouse in coordinate

	float offsetX = 0;										//the offset in X axis between when the mouse move
	float offsetY = 0;										//the offset in X axis between when the mouse move

	float fov = 35.0f;										//the degree of perspective
	float pitch = 0.0f;										//the degree of pitch(row around X axis)
	float yaw = -90.0f;										//the degree of yaw(row around Y axis)
	bool firstMouse = true;
	void ProcessMouseMovement(double,double);
	void ProcessMouseScroll(float);
	void ProcessKeyBoard(DIRECTION, float);
	void Focus();
	void dive(float);
	void rise(float);
	glm::mat4 GetViewMatrix();
};
