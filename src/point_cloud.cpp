#include <GL/glew.h>
#include "point_cloud.hpp"
#include <string>
#include <memory>
#include <iostream>

PointCloud::PointCloud():
rendered(false), poly(), vert_handler(new GLuint(0), [](GLuint* vertex){glDeleteVertexArrays(1, vertex);}), 
vbo(new GLuint(0), [](GLuint* vbo){glDeleteBuffers(1, vbo);}){}

PointCloud::PointCloud(const std::vector<Vertex>& vertex_list):
rendered(false), poly(), vert_handler(new GLuint(0), [](GLuint* vertex){glDeleteVertexArrays(1, vertex);}), 
vbo(new GLuint(0), [](GLuint* vbo){glDeleteBuffers(1, vbo);}){

	this->poly.count = vertex_list.size();

    glGenVertexArrays(1, this->vert_handler.get());
	glBindVertexArray(*this->vert_handler);

	glGenBuffers(1, this->vbo.get());
	glBindBuffer(GL_ARRAY_BUFFER, *this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_list.size(), &vertex_list[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)(offsetof(Vertex, color)));

	glBindVertexArray(0);
}

PointCloud::~PointCloud(){
	this->hide();
}

bool PointCloud::operator==(const PointCloud& p){
    if(this->vert_handler == p.vert_handler &&
		this->vbo == p.vbo){
        return true;
    }
    return false;
}

bool PointCloud::operator!=(const PointCloud& p){
    return !(*this == p);
}

void PointCloud::render(float x, float y, float z, Renderer* renderer){
    if(this->rendered) return;
	if(this->poly.count == 0) return;

    this->rendered = true;

	this->poly.vao = *this->vert_handler;
    this->poly.position = glm::vec3(x, y, z);

    renderer->add(&this->poly);
	this->renderer = renderer;
}

void PointCloud::hide(){
	if(this->rendered){
		this->renderer->remove(&this->poly);
	}
}

Color::Color(uint32_t ARGB):
	color((ARGB & 0xFF0000)/(0x10000*255.0),
		  (ARGB & 0xFF00)/(0x100*255.0),
		  (ARGB & 0xFF)/(255.0),
		  (ARGB & 0xFF000000)/(0x1000000*255.0)){}
