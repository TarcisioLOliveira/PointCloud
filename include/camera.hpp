#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
    public:
    enum Movement{TOP = 1, BOTTOM = 2, LEFT = 4, RIGHT = 8};
    enum Rotation{CLOCKWISE = 1, COUNTERCLOCKWISE = 2, DOWN = 4, UP = 8};
    enum Zoom{IN = 1, OUT = 2};
	Camera() = default;
    Camera(glm::vec3 initial_position, glm::vec3 initial_direction, GLFWwindow* window);

    void move(uint8_t direction);
    void rotate(double front, double side);

	bool check_keys(int key, int action);
	void check_mouse(double x, double y);

    inline glm::vec3 get_position(){return this->position;}
    inline glm::vec3 get_rotation(){return this->rotation;}
	inline glm::vec3 get_direction(){return this->direction;}
	inline glm::mat4 get_view_matrix(){return glm::lookAt(
															this->position,
															this->direction + this->position,
															glm::vec3(0.0f, 0.0f, 1.0f)
														);}

    private:
    glm::vec3   position;
	glm::vec3   direction;
	glm::vec3   orig_direction;
    glm::vec3   rotation;
	GLFWwindow* window;
	double		mouse_x, mouse_y;
};

#endif // CAMERA_HPP_INCLUDED
