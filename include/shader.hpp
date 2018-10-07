#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

class Shader{
    public:
    Shader() = default;
    Shader(const char source[], GLenum shaderType);

    inline GLuint get_handler() const{return *this->handler;}

    private:
	std::shared_ptr<GLuint> handler;
};

#endif // SHADER_HPP_INCLUDED
