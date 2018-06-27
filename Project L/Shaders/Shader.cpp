#include "Shader.h"

#include "ShaderConfig.h"
#include "../Utils/Error.h"

#include <fstream>

#include "../GameConfig.h"

Shader::Shader(const std::string & pathName)
{
	init(pathName, VERTEX_SHADER | FRAGMENT_SHADER);
}

Shader::Shader(const std::string & pathName, unsigned int flags)
{
	init(pathName, flags);
}

Shader::Shader(const std::string & fragmentPath, const std::string & vertexPath)
{
	this->id = glCreateProgram();
	std::string pathToReasources(PATH_TO_RESOURCES);
	addShaderpart(pathToReasources+vertexPath, GL_VERTEX_SHADER);
	addShaderpart(pathToReasources+fragmentPath, GL_FRAGMENT_SHADER);
	link();
}

Shader::~Shader()
{
	glDeleteProgram(this->id);
}

void Shader::bind() const
{
	glUseProgram(this->id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setTexture2D(const std::string & name, unsigned int unit)
{
	glUniform1i(addUniform(name), unit);
}

void Shader::setTexture2D(const std::string & name, unsigned int unit, GLuint textureID)
{
	setTexture2D(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Shader::setUniform1i(const std::string & name, int value)
{
	glUniform1i(addUniform(name), value);
}

void Shader::setUniform1b(const std::string & name, bool value)
{
	setUniform1i(name, (int)value);
}

void Shader::setUniform1f(const std::string & name, float v1)
{
	glUniform1f(addUniform(name), v1);
}

void Shader::setUniform2f(const std::string & name, float v1, float v2)
{
	glUniform2f(addUniform(name), v1, v2);
}

void Shader::setUniform2fv(const std::string & name, unsigned int count, const float * values)
{
	glUniform2fv(addUniform(name), count, values);
}

void Shader::setUniform3f(const std::string & name, float v1, float v2, float v3)
{
	glUniform3f(addUniform(name), v1, v2, v3);
}

void Shader::setUniform3fv(const std::string & name, unsigned int count, const float * values)
{
	glUniform3fv(addUniform(name), count, values);
}

void Shader::setUniform4f(const std::string & name, float v1, float v2, float v3, float v4)
{
	glUniform4f(addUniform(name), v1, v2, v3, v4);
}

void Shader::setUniform4fv(const std::string & name, unsigned int count, const float * values)
{
	glUniform4fv(addUniform(name), count, values);
}

void Shader::setUniformMatrix2fv(const std::string & name, unsigned int count, bool transpose, const float * values)
{
	glUniformMatrix2fv(addUniform(name), count, transpose, values);
}

void Shader::setUniformMatrix3fv(const std::string & name, unsigned int count, bool transpose, const float * values)
{
	glUniformMatrix3fv(addUniform(name), count, transpose, values);
}

void Shader::setUniformMatrix4fv(const std::string & name, unsigned int count, bool transpose, const float * values)
{
	glUniformMatrix4fv(addUniform(name), count, transpose, values);
}

const GLuint Shader::getID() const
{
	return this->id;
}

void Shader::init(const std::string & pathName, unsigned int flags)
{
	this->id = glCreateProgram();

	std::string name = pathName.substr(pathName.find_last_of("/\\")+1, pathName.size()-1);
	if (flags & VERTEX_SHADER)
		addShaderpart(pathName + "/" + name + ".vs", GL_VERTEX_SHADER);

	if (flags & FRAGMENT_SHADER)
		addShaderpart(pathName + "/" + name + ".fs", GL_FRAGMENT_SHADER);

	link();
}

int Shader::addUniform(const std::string & name)
{
	if (this->uniforms.find(name) == this->uniforms.end())
	{
		int uniformLocation = glGetUniformLocation(this->id, name.c_str());
		if (uniformLocation == -1)
			Error::printError("SHADER::addUniform()", "Can't find uniform '" + name + "'!");
		this->uniforms.insert({ name, uniformLocation });
	}
	return this->uniforms[name];
}

void Shader::addShaderpart(const std::string & pathName, GLuint type)
{
	GLuint shaderPartID = loadShaderPart(pathName, type);
	compileShaderPart(shaderPartID);
	glAttachShader(this->id, shaderPartID);
	glDeleteShader(shaderPartID);
}

GLuint Shader::loadShaderPart(const std::string & pathName, GLuint type)
{
	GLuint id = glCreateShader(type);
	std::ifstream file(pathName);
	std::string shaderText;
	if (file.is_open())
	{
		shaderText = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	else
	{
		Error::printError("SHADER::addShaderPart()", "Could not open shader: " + std::string(pathName));
	}
	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(id, 1, &shaderTextPtr, nullptr);
	this->ids.push_back(id);
	return id;
}

bool Shader::compileShaderPart(GLuint id)
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = GL_FALSE;
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		glGetShaderInfoLog(id, 1024, nullptr, buff);
		Error::printError("SHADER::compileShaderPart()", std::string(buff));
		return false;
	}
	return true;
}

void Shader::link()
{
	glLinkProgram(this->id);

	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = GL_FALSE;
	glGetProgramiv(this->id, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		memset(buff, 0, 1024);
		glGetProgramInfoLog(this->id, 1024, nullptr, buff);
		buff[1023] = '\n';
		Error::printError("SHADER::link()", std::string(buff));
	}

	for (GLuint partID : this->ids)
		glDetachShader(this->id, partID);
}
