#pragma once
#include"precompile.h"
//this header file contains some feedback function for opengl
unsigned int loadTexture(const char* sda);

unsigned int load_cubemaps(std::vector<std::string>&);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
