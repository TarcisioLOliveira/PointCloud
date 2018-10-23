#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "main.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "point_cloud.hpp"

Camera default_camera;

void error_callback(int error, const char* description)
{
    std::cout << "GLFWError "<<error<<" : " << description << std::endl;
}

int main(int argc, const char* argv[]){
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(1366, 768, "Point Cloud", 
			nullptr,//glfwGetPrimaryMonitor(), 
			nullptr);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!mainWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(mainWindow);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit()){
        printf("GLEW init failure\n");
    }

	default_camera = Camera({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, mainWindow);

    auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)mutable{
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if(default_camera.check_keys(key, action))
			return;
	};
	auto cursor_pos_callback = [](GLFWwindow* window, double xpos, double ypos){
		default_camera.check_mouse(xpos, ypos);
	};

    // Callbacks
    glfwSetKeyCallback(mainWindow, key_callback);
    glfwSetCursorPosCallback(mainWindow, cursor_pos_callback);
    glfwSetErrorCallback(error_callback);
	
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPointSize(1.0);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    Shader render_shader(
        "#version 330 core\n"
        "#define MAX_TEXTURES 4\n"
        "layout(location = 0) in vec3 points;"
        "layout(location = 1) in vec4 in_color;"
        "out vec4 color;"
        "uniform mat4 MVP;"
        "uniform mat4 model_matrix;"
        "void main(){"
            "gl_Position = MVP * vec4(points, 1.0);"
            "gl_PointSize = 4.0;"
            "color = in_color;"
         "}", GL_VERTEX_SHADER);
    Shader color_shader(
        "#version 330 core\n"
        "#define MAX_TEXTURES 4\n"
        "layout(location = 0) out vec4 out_color;"
        "in vec4 color;"
        "void main(){"
            "out_color = color;"
        "}", GL_FRAGMENT_SHADER);

    Renderer mainRenderer(&default_camera, mainWindow, render_shader, color_shader);
    
    if(argc <= 1){
        return 0;
    }

    std::stringstream parser;
    std::string line;
    glm::vec3 vertex(0.0);
    glm::vec3 color(0.0);
    std::vector<Vertex> points;
    for(int i = 1; i < argc; ++i){
        std::ifstream file(argv[i]);
        while(!file.eof()){
            //std::getline(file, line, '\n');
            file >> vertex.x >> vertex.y >> vertex.z >> color.b >> color.g >> color.r;
            points.push_back({
                vertex,
                glm::vec4(color/255.0f, 1.0)
            });
        }
    }

    PointCloud cloud(points);
    cloud.render(0.0, 0.0, 0.0, &mainRenderer);

    while (!glfwWindowShouldClose(mainWindow)){
        glfwPollEvents();
        mainRenderer.render_objects();
        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
