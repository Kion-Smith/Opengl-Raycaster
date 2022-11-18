#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "[ERROR] : unable to parse shader files" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	m_id = glCreateProgram();

	unsigned int fsID, vsID;
	vsID = glCreateShader(GL_VERTEX_SHADER);
	fsID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsID, 1, &vShaderCode, NULL);
	glShaderSource(fsID, 1, &fShaderCode, NULL);

	glCompileShader(vsID);
	glCompileShader(fsID);

	glAttachShader(m_id, vsID);
	glAttachShader(m_id, fsID);

	glLinkProgram(m_id);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

}

void Shader::bind() const
{
	glUseProgram(m_id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

int Shader::getKnownUniformLocation(const std::string& name)
{
	if (m_uniformLocationMap.find(name) != m_uniformLocationMap.end())
	{
		return m_uniformLocationMap[name];
	}

	int location = glGetUniformLocation(m_id, name.c_str());
	
	if (location == -1)
	{
		std::cout << "[ERROR] Could not find the specified uniform ("<< name <<") location in shader program " << m_id << std::endl;
	
	}
	else
	{
		m_uniformLocationMap[name] = location;
	}

	return location;
}

void Shader::setBool(const std::string& name, bool value) 
{
	glUniform1i(getKnownUniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value) 
{
	glUniform1i(getKnownUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) 
{
	glUniform1f(getKnownUniformLocation(name), value);
}

void Shader::setVec3f(const std::string& name, int vectorCount, GLfloat* value_ptr)
{
	glUniform3fv(getKnownUniformLocation(name), vectorCount, value_ptr);
}

void Shader::setMatrix4f(const std::string& name,int matrixCount,int shouldTranspose, GLfloat* value_ptr) 
{
	glUniformMatrix4fv(getKnownUniformLocation(name), matrixCount, shouldTranspose, value_ptr);
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}


