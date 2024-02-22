#pragma once
#ifndef SHADER_H		//beugt Linking Probleme vor, nur dann benutzen, wenn nicht schon andere included wurden
#define SHADER_H

#include<glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	//Path ist die Pfad zur Textdatei

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		//verwaltet ÷ffnen und Schlieﬂen
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//÷ffne Dateien
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			//Verwaltet Datenfluss
			std::stringstream vShaderStream, fShaderStream;

			//Dateibuffer in Stream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//Schlieﬂe Dateien
			vShaderFile.close();
			fShaderFile.close();

			//Konvertiere Stream in String
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		//Konvertiere in C-String
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//Kompiliere Shader wie vorher auch
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		//vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog); std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		//fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog); std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		ID = glCreateProgram(); 

		glAttachShader(ID, vertex); 
		glAttachShader(ID, fragment); 
		glLinkProgram(ID); 

		// Linking Errors?
		glGetProgramiv(ID, GL_LINK_STATUS, &success); 
		if(!success) 
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog); 
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex); 
		glDeleteShader(fragment);

	}


	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
};
#endif