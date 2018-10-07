#ifndef POLYGON_HPP_INCLUDED
#define POINT_CLOUD_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <vector>
#include <initializer_list>
#include "renderer.hpp"
#include <memory>
#include <utility>
#include <cstdint>
#include "renderer.hpp"

struct Point{
    public:
    Point()=default;
    Point(float x, float y, float z):x(x), y(y), z(z){}
    float x, y, z;
};

struct Color{
	public:
	Color()=default;
	Color(uint32_t ARGB);
	glm::vec4 color;
};

struct Vertex{
    public:
    glm::vec3 vertex = glm::vec3(0.0);
    glm::vec4 color = glm::vec4(0.0);
    Vertex() = default;
    Vertex(glm::vec3 vec, glm::vec4 color):vertex(std::move(vec)), color(std::move(color)){}
};

class PointCloud{
    public:
    PointCloud();
    PointCloud(const std::vector<Vertex>& vertex_list);
	PointCloud(const PointCloud&) = default;
	PointCloud(PointCloud&&) = default;
	PointCloud& operator=(const PointCloud&) = default;
	PointCloud& operator=(PointCloud&&) = default;
	~PointCloud();

    void render(float x, float y, float z, Renderer* renderer);
    void hide();

    bool operator==(const PointCloud& p);
    bool operator!=(const PointCloud& p);

    private:
    bool rendered;
    Renderable poly;
	std::shared_ptr<GLuint> vert_handler;
	std::shared_ptr<GLuint> vbo;
	//std::shared_ptr<GLuint> ibo;
	Renderer* renderer = nullptr;
};

#endif // POLYGON_HPP_INCLUDED
