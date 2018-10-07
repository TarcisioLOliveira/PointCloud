#ifndef RENDERER_HPP_INCLUDED
#define RENDERER_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <vector>
#include "camera.hpp"
#include <functional>
#include "shader.hpp"
#include "program.hpp"


class Renderable{
    public:
    enum Priority{MAP, UNIT, GUI} priority;

    Renderable() = default;
	Renderable(const Renderable&) = default;
	Renderable(Renderable&&) = default;
	Renderable& operator=(const Renderable&) = default;
	Renderable& operator=(Renderable&&) = default;
    ~Renderable() = default;

	GLuint vao;
    glm::vec3 position = glm::vec3(0.0);
	GLuint count;
};

class Renderer{
    public:
    Renderer(Camera* cam, GLFWwindow* window, Shader vertex_shader, Shader fragment_shader);

    void add(Renderable* object);
	void remove(Renderable* object);
    void render_objects();

    private:
    Camera* camera;
    std::vector<Renderable*> object_list;
    GLuint MVP_ID = 0;
    Shader render_shader;
    Shader color_shader;
	glm::mat4 projection;
    Program render_program;
};

#endif // RENDERER_HPP_INCLUDED
