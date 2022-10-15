#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>



	class Shader
	{
		// Program Id
		GLuint ID;
	public:
		const std::string FRAGMENT = "fragment";;
		const std::string VERTEX = "vertex";
		const std::string PROGRAM = "program";

		Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
		{
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;

			// ensure ifstream can throw out exception
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				// open file
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				std::stringstream vShaderStream, fShaderStream;

				// read file content to buff;
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();

				// close file
				vShaderFile.close();
				fShaderFile.close();

				// convert buf stream to string
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();


			// compile the shader
			GLuint vertex, fragment;
			int success;
			char infoLog[512];

			// vertex shader 
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);
			checkCompileErrors(vertex, VERTEX);

			// fragment shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);
			checkCompileErrors(fragment, FRAGMENT);

			// link to program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);

			checkCompileErrors(ID, PROGRAM);

			glDeleteShader(vertex);
			glDeleteShader(fragment);
		};

		// use or active shader
		void use()
		{
			glUseProgram(ID);
		}

		// uniform tool method
		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}

		void setMat4f(const std::string& name, float* value) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
		}

		void setMat4(const std::string& name, glm::mat4& mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
		}

		void setVec3(const std::string& name, float r, float g, float b) const
		{
			glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
		}

		~Shader()
		{
			std::cout << "delete program" << std::endl;
			glDeleteProgram(ID);
		}

	private:
		// utilty function for checing shader compilation/link errors.
		void checkCompileErrors(GLuint shader, const std::string type)
		{
			int success;
			char infoLog[1024];
			if (type != PROGRAM)
			{
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(shader, 1024, NULL, infoLog);
					std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl
						<< infoLog << std::endl
						<< "-- ------------------------------ --" << std::endl;
				}
				else
				{
					glGetProgramiv(shader, GL_LINK_STATUS, &success);
					if (!success)
					{
						glGetProgramInfoLog(shader, 1024, NULL, infoLog);
						std::cout << "ERROR:PROGRAM_LINKING_ERROR of type: " << type << std::endl
							<< infoLog << std::endl
							<< "-- ----------------------------- --" << std::endl;
					}
				}
			}
		}
	};



#endif