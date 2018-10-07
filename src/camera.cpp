#include <GLFW/glfw3.h>
#include "camera.hpp"
#include <utility>
#include <glm/gtx/rotate_vector.hpp>
#include <cstdio>
#include <iostream>

Camera::Camera(glm::vec3 initial_position, glm::vec3 initial_direction, GLFWwindow* w):
position(std::move(initial_position)), direction(initial_direction), orig_direction(std::move(initial_direction)),
rotation(0.0f), window(w){
	glfwGetCursorPos(this->window, &this->mouse_x, &this->mouse_y);
}

bool Camera::check_keys(int key, int action){
	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		int movement = 0;
		if(glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
			movement |= Camera::TOP;
		}
		if(glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS){
			movement |= Camera::BOTTOM;
		}
		if(glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS){
			movement |= Camera::LEFT;
		}
		if(glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS){
			movement |= Camera::RIGHT;
		}
		if(movement != 0){
			this->move(movement);
			return true;
		}
	}
	return false;
}

void Camera::check_mouse(double x, double y){
	double front = (this->mouse_y - y)*0.001f;
    double side = (this->mouse_x - x)*0.001f;
	this->mouse_x = x;
	this->mouse_y = y;
	this->rotate(front, side);
}

void Camera::move(uint8_t direction){
	float multiplier = 0.04f; //20.0f;//0.04f;
    if((direction & this->TOP) != 0){
		this->position += this->direction*multiplier;
        //this->position.x += multiplier;
		//this->position.y += multiplier;
    }
    if((direction & this->BOTTOM) != 0){
        this->position -= this->direction*multiplier;
        //this->position.x -= multiplier;
		//this->position.y -= multiplier;
    }
    if((direction & this->LEFT) != 0){
		//this->position.x += multiplier;
		//this->position.y -= multiplier;
        this->position -= glm::vec3{this->direction.y, -this->direction.x, 0.0f}*multiplier;
    }
    if((direction & this->RIGHT) != 0){
		//this->position.x -= multiplier;
		//this->position.y += multiplier;
        this->position += glm::vec3{this->direction.y, -this->direction.x, 0.0f}*multiplier;
    }
}

void Camera::rotate(double front, double side){
	if(front > 0){
		if(rotation.x + front < 1.50){
			this->rotation.x += front;
		} else {
			this->rotation.x = 1.50;
		}
    } else if(front < 0){
		if(rotation.x + front > -1.50){
			this->rotation.x += front;
		} else {
			this->rotation.x = -1.50;
		}
    }
    this->rotation.z += side;

	this->direction = glm::rotate( this->orig_direction, this->rotation.x, {this->orig_direction.y, -this->orig_direction.x, 0.0f});
	this->direction = glm::rotate( this->direction, this->rotation.z, {0.0f, 0.0f, 1.0f});
}
