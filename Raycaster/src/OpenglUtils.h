#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "vendor/std_image/stb_image.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) glClearError(); \
    x;\
    ASSERT(glLogCall(#x,__FILE__, __LINE__))


void glClearError();

bool glLogCall(const char* function, const char* file, int line);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

ShaderProgramSource parseShader(const std::string& filePath);

unsigned int compileShader(unsigned int shaderType, const std::string& source);


unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

GLFWwindow* setupGLFW(const std::string& chapterName, const std::string& addToName, int width, int height);

bool set2DTexture(const std::string& filepath, unsigned int* textureID, int colorType);