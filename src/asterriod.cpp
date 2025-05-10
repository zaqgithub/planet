#include"precompile.h"
#include"camera.h"
#include"model.h"
#include"feedbackFunctions.h"
#include"Timer.h"
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

const float width = 800;
const float height = 600;
float deltatime, lastframe;
Camera camera;
int blinnphong = 0;
int mian() {
	//initial the glfw 
	glfwInit();
	//specify the version
	glfwwindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
	//choose the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//generate window
	GLFWwindow* window = glfwCreateWindow(width,height,"planet",NULL,NULL);
	//handle the error message if the window doesn't created properly
	if(!window){
		std::cout<<"Failed to create the windwo\n";
		glfwTerminate();
		return -1;
	}
	//choose the current window as current context
	glfwMakeContextCurrent(window);
	//resize the window dynamicly
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	//check if glad library loaded properly
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout<<"Failed to load glad"<<std::endl;
		return -1;
	}
	//enable depth test
	glEnable(GL_DEPTH_TEST);
	//choose the input mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader rockShader(std::string("glsl/asterriodvertexshader.glsl", "glsl/asterriodfragmentshader.glsl");
	Model rockModel("models/rock/rock.obj");
	
	Shader plannetShader("glsl/plannetvertexshader.glsl", "glsl/plannetfragmentshader.glsl");
	Model plannetModel("models/planet/planet.obj");
	//store 10000 rotation for instance
	unsigned int amount = 10000; 
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 25.0;
	float offset = 5.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
	{
		unsigned int VAO = rockModel.meshes[i].VAO;
		glBindVertexArray(VAO);
		// vertex attributes
		std::size_t vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
	//generate the uniform buffer
	unsigned int ub;
	glGenBuffers(1, &ub);
	glBindBuffer(GL_UNIFORM_BUFFER, ub);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ub);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//set view and project matrix 
	glm::mat4  view = glm::mat4(1.0f);
	glm::mat4  project = glm::mat4(1.0f);
	glm::mat4  rockmodel = glm::mat4(12.0f);
	glm::mat4  plannetmodel = glm::mat4(1.0f);
	while (!glfwWindowShouldClose(window)) {
		float now = glfwGetTime();
		deltatime = now - lastframe;
		lastframe = now;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		processInput(window);

		view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, ub);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));

		project = glm::perspective(glm::radians(camera.fov), width / height, 0.1f, 1000.0f);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(project));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		Timer* time = new Timer;
		rockShader.use();
		for (unsigned int i = 0; i < rockModel.meshes.size(); i++)
		{
			glBindVertexArray(rockModel.meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
			);
		}

		plannetShader.use();
		plannetShader.setMat4("model", plannetmodel);
		plannetModel.draw(plannetShader);
		delete time;
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

}
//resize the window to adjust the changing of the size of window
void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
		glViewport(0, 0, width, height);
}

//capture the position of mouse
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	camera.ProcessMouseMovement(xposIn, yposIn);
}

//capture the keyboard input
void processInput(GLFWwindow* window) {
	const float cameraSpeed = 10.0f * deltatime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyBoard(FORWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyBoard(BACKWARD, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyBoard(LEFT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyBoard(RIGHT, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		camera.Focus();
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.dive(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.rise(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyBoard(Q, cameraSpeed);
		std::cout << "Q";
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyBoard(E,cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		blinnphong = true;
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
		blinnphong = false;
	}
}

//capture the scroll movement
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//bind texture
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format=3;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
unsigned int load_cubemaps(std::vector<std::string>&paths) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int size = paths.size();
	unsigned char* data;
	GLenum format = 3;
	int width, height, nrComponents=0;


	for (int i = 0;i < size;i++) {
		data = stbi_load(paths[i].c_str(), &width, &height, &nrComponents, 0);

		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;


		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return textureID;
}