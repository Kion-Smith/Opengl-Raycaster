#include "OpenglUtils.h"
void glClearError()
{
	while (glGetError() != GL_NO_ERROR);

}

bool glLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL ERROR]: (0x" << std::hex << error << ")" << " when running function: " << function << " " << file << " at line: " << line << std::endl;
		return false;
	}

	return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//std::cout << "<LOG> : The viewport was resized" << std::endl;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//Will close the window when escape is pressed
	//Any key not pressed will be GLFW_RELEASED
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

ShaderProgramSource parseShader(const std::string& filePath)
{
	enum class ShaderType 
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];
	ShaderType curType = ShaderType::NONE;

	while (getline(stream, line))
	{
		//Have we found the shader on this current line
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				//set vertex
				curType = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//set fragment
				curType = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)curType] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int compileShader(unsigned int shaderType, const std::string& source)
{
	const char* src = source.c_str(); // could have done &source[0] ie address to the first char in string
	GLCall(int id = glCreateShader(shaderType));

	//shader we are calling, how many source codes, pointer of src string,length of string, max
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		//alloca. allocate space on the stack
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "[ERROR] : FAILED TO COMPILE " << (shaderType == GL_VERTEX_SHADER ? "Vertex " : "Fragment ") << "Shader\n\t" << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//normally we write shaders in a file and load it in, here we are just doing that in a string
	unsigned int program = glCreateProgram();
	GLCall(unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader));

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));

	int linkResult;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linkResult));

	if (linkResult == GL_FALSE)
	{
		char* message = (char*)alloca(512 * sizeof(char));
		glGetProgramInfoLog(program, 512, NULL, message);
		std::cout << "[ERROR] : Unable to link program :: " << message << std::endl;
	}

	//Validate program
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

GLFWwindow* setupGLFW(const std::string& chapterName, const std::string& addToName, int width,int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::string windowName = chapterName + " - " + addToName;
	GLFWwindow* window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

	glfwMakeContextCurrent(window);

	//This is optional
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Link the Opengl functions with glad
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "GLAD error" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, width, height);

	return window;
}

bool set2DTexture(const std::string& filepath, unsigned int* textureID, int colorType)
{
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	int width, height, nrChannels;

	std::ifstream testFilePath(filepath.c_str());
	if (!testFilePath)
	{
		std::cout << "[ERROR] OpenglUtils: Unable to find the file path:: "<< filepath << std::endl;
		testFilePath.close();
		return false;
	}
	testFilePath.close();
	
	unsigned char* textureData = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

	//How will opengl handle the 'x' direction of the image if too large
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (textureData) 
	{
		glTexImage2D(GL_TEXTURE_2D,0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "[ERROR] : Unable to load desired texture" << std::endl;
		return false;
	}

	stbi_image_free(textureData);

	return true;
}

