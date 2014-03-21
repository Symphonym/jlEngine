#include "Shader.hpp"
#include <fstream>
#include <SDL2/SDL.h>
#include <vector>

namespace jl
{

	Shader::Shader() :
		m_shaderHandle(0)
	{

	}
	Shader::Shader(GLenum shaderType, const std::string &filePath) :
		m_shaderHandle(0)
	{
		loadFromFile(shaderType, filePath);
	}
	Shader::~Shader()
	{
		if(glIsShader(m_shaderHandle) == GL_TRUE)
			glDeleteShader(m_shaderHandle);
	}

	void Shader::loadFromFile(const std::string &filePath)
	{
		if(filePath.length() >= 5)
		{
			std::string extension = filePath.substr(filePath.length()-5, filePath.length());

			GLenum shaderType = 0;

			if(extension == ".vert")
				shaderType = GL_VERTEX_SHADER;
			else if(extension == ".frag")
				shaderType = GL_FRAGMENT_SHADER;
			else if(extension == ".geog")
				shaderType = GL_GEOMETRY_SHADER;

			if(shaderType != 0)
				loadFromFile(shaderType, filePath);

		}
	}
	void Shader::loadFromFile(GLenum shaderType, const std::string &filePath)
	{
		// Remove any existing shader, if any
		if(glIsShader(m_shaderHandle) == GL_TRUE)
			glDeleteShader(m_shaderHandle);

		m_shaderHandle = glCreateShader(shaderType);

		std::ifstream reader(filePath, std::ifstream::in);
		if(reader.is_open())
		{

			std::string shaderTypeStr = "";
			switch(shaderType)
			{
				case GL_VERTEX_SHADER: shaderTypeStr = "VERTEX"; break;
				case GL_FRAGMENT_SHADER: shaderTypeStr = "FRAGMENT"; break;
				case GL_GEOMETRY_SHADER: shaderTypeStr = "GEOMETRY"; break;
			}

			std::string line, source;
			while(std::getline(reader, line))
			{
				source += line + "\n";
			}

			const GLchar *sourcePtr = &source[0];

			// Compile shader
			glShaderSource(m_shaderHandle, 1, &sourcePtr, NULL);
			glCompileShader(m_shaderHandle);

			// Grab compilation status
			GLint compilationSuccess = 0;
			glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &compilationSuccess);
			
			// Check for compliation errors
			if(compilationSuccess == GL_FALSE)
			{
				GLint logLength = 0;
				glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &logLength);

				std::vector<char> logMessage(logLength);
				glGetShaderInfoLog(m_shaderHandle, logLength, &logLength, &logMessage[0]);

				SDL_Log("Shader compilation error:\n%s shader: \"%s\"\n%s", shaderTypeStr.c_str(), filePath.c_str(), &logMessage[0]);

				// Shader wasen't compiled correctly, delete it to avoid leaks
				glDeleteShader(m_shaderHandle);
			}
			else
				SDL_Log("Compiled shader (%s): \"%s\"", shaderTypeStr.c_str(), filePath.c_str());
		}
		else
			SDL_Log("Could not open shader file: %s", filePath.c_str());

		reader.close();


	}

	void Shader::attachTo(GLuint program)
	{
		glAttachShader(program, m_shaderHandle);
	}

}