
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cComplex.h"
#include "cEngine.h"
#include"shader_s.h"
#include<iostream>
#include<vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(2000, 2000, "Mandelbrot Set", NULL, NULL);
    if (!window) {

        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Engine* engine = new Engine();
    engine->init_shader();

    float old_time = 0, delta_time = 0, time = 0;

    while (!glfwWindowShouldClose(window)) {

        time = glfwGetTime();
        delta_time = time - old_time;
        if (delta_time < 1 / 30) {
            continue;
        }
        old_time = time;

        engine->processInput(window);

        engine->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}




