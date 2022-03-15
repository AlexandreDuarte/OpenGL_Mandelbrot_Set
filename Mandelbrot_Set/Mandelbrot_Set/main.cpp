
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"
#include "shader_s.h"
#include <iostream>
#include <vector>

float zoom;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    zoom += yoffset;
}



int main()
{
    zoom = 0;
    static GLFWwindow* window;
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(2000, 2000, "Mandelbrot Set", NULL, NULL);
    if (!window) {

        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetScrollCallback(window, scroll_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Engine* engine = new Engine();
    engine->init_shader();

    static double old_time = 0.0, delta_time = 0.0, time = 0.0;

    while (!glfwWindowShouldClose(window)) {

        time = glfwGetTime();
        delta_time = time - old_time;

        if (delta_time <= (double)1 / 120) {
            continue;
        }

        

        old_time = time;

        engine->processInput(window, &delta_time, zoom);

        engine->render(&delta_time, zoom);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}




