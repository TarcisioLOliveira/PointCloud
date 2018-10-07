#include "program.hpp"
#include <cstdio>
#include <vector>
#include <string>

Program::Program():
    handler(new GLuint(glCreateProgram()), [](GLuint* program){glDeleteProgram(*program);}){
}

void Program::attach(const Shader& s){
    glAttachShader(*this->handler, s.get_handler());
}

void Program::detach(const Shader& s){
    glDetachShader(*this->handler, s.get_handler());
}

void Program::link(){
    glLinkProgram(*this->handler);

	GLint result = GL_TRUE;
	glGetProgramiv(*this->handler, GL_LINK_STATUS, &result);
	if(result == GL_FALSE){
		int infoLogLength = 0;
		glGetProgramiv(*this->handler, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> errorMessage(std::max(infoLogLength, int(1)));
		glGetProgramInfoLog(*this->handler, infoLogLength, NULL, &errorMessage[0]);
		printf("Program link error: %s\n", &errorMessage[0]);
	}
}

void Program::run(){
    glUseProgram(*this->handler);
}

void Program::stop(){
    glUseProgram(0);
}
