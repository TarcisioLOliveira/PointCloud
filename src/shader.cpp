#include "shader.hpp"
#include <utility>
#include <vector>
#include <cstdio>

Shader::Shader(const char source[], GLenum shader_type):
    handler(new GLuint(glCreateShader(shader_type)), [](GLuint* shader){glDeleteShader(*shader);}){

    glShaderSource(*this->handler, 1, &source, nullptr);
    glCompileShader(*this->handler);

    // Error checking

	GLint result = GL_TRUE;
	glGetShaderiv(shader_type, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		int infoLogLength = 0;
		glGetShaderiv(shader_type, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMessage(infoLogLength);
		glGetShaderInfoLog(*this->handler, infoLogLength, NULL, &errorMessage[0]);
		printf("%s\n", &errorMessage[0]);
	}
}
