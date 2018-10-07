#include <GL/glew.h>
#include "renderer.hpp"
#include "program.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <algorithm>

Renderer::Renderer(Camera* cam, GLFWwindow* window, Shader vertex_shader, Shader fragment_shader):
camera(cam), object_list(),
render_shader(std::move(vertex_shader)),
color_shader(std::move(fragment_shader)),
projection(0.0f),
render_program(){
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float) height;
	
	//this->projection = glm::ortho(width/2.0f, -width/2.0f, 0.0f, height/1.0f, -10000.0f, 10000.0f);
	this->projection = glm::perspective(45.0f, ratio, 0.1f, 10000.0f);
	glViewport(0, 0, width, height);
    this->render_program.attach(this->render_shader);
    this->render_program.attach(this->color_shader);
    this->render_program.link();

    this->MVP_ID = glGetUniformLocation(this->render_program.get_handler(), "MVP");
}

void Renderer::render_objects(){
	
	glm::mat4 view_matrix(this->camera->get_view_matrix());
	glm::mat4 model_matrix;
	glm::mat4 MVP;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->render_program.run();
    for( auto i = this->object_list.begin(); i < this->object_list.end(); ++i){
		glBindVertexArray((*i)->vao);

		model_matrix = glm::translate(glm::mat4(1.0f), (*i)->position);
        MVP = this->projection * view_matrix * model_matrix;
        glUniformMatrix4fv(this->MVP_ID, 1, GL_FALSE, &MVP[0][0]);

		glDrawArrays(GL_POINTS, 0, (*i)->count);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	this->render_program.stop();
}

void Renderer::add(Renderable* object){
    if(this->object_list.size() == 0)
        this->object_list.push_back(object);
    else{
        auto listEnd = this->object_list.end();
        for(auto i = this->object_list.begin(); i < listEnd; ++i){
            if(object->priority < (*i)->priority){
                this->object_list.insert(i, object);
			}
        }
		this->object_list.push_back(object);
    }
}
void Renderer::remove(Renderable* object){
	auto found = std::find(this->object_list.begin(), this->object_list.end(), object);
	if(found != this->object_list.end()){
		this->object_list.erase(found);
	}
}
