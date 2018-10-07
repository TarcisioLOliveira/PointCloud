#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include "shader.hpp"
#include <memory>

class Program{
    public:
    Program();

    void attach(const Shader& s);
    void detach(const Shader& s);
    void link();
    void run();
    void stop();
    inline GLuint get_handler(){ return *this->handler; }

    private:
	std::shared_ptr<GLuint> handler;
};

#endif // PROGRAM_HPP_INCLUDED
