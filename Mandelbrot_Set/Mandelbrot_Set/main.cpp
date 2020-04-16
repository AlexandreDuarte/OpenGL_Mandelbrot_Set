
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"shader_s.h"
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

struct vertices_array {
    float* v_array;
    int size;
};

vertices_array generate_vertices(int lenght);


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




    vertices_array vertices = generate_vertices(1000);





    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // set up view
    glViewport(0, 0, 2000, 2000);




    unsigned int VBO, VAO, EBO;

    glEnable(GL_DEBUG_OUTPUT);


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    /*glGenBuffers(1, &EBO);*/


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.v_array, GL_STATIC_DRAW);

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe mode


    Shader shader ("shaders/shader.vs", "shaders/shader.fs");
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
   /* model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::ortho(0, 2000, 0, 2000);*/

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    while (!glfwWindowShouldClose(window)) {


        float time = glfwGetTime();

        processInput(window);

        glBindVertexArray(0);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1000);

        glBindVertexArray(0);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwTerminate();
    return 0;
}

vertices_array generate_vertices(int lenght) {

    int points = lenght + 1;

    int p = points * 3 + points * 33;

    float* p_a = new float[p];



    float* vertice;
    for (float i = 0; i <= lenght; i++) {
        for (float j = 0; j <= lenght; j++) {

            vertice = p_a + (int)i * 3 + (int)j * 33;

            *vertice = (2*i / lenght - 1);
            *(vertice + 1) = (2*j / lenght - 1);
            *(vertice + 2) = 0.0f;
        }
    }

    return vertices_array{p_a, p};
}



/*void generate_indices(int g[600]) {

    int* p_g = &g[0];

    int* id;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            id = (p_g + i * 6 + j * 10 * 6);
            *id = i * 2 + j * 2 * 22;
            *(id + 1) = (i) * 2 + 1 + (j * 2) * 22;
            *(id + 2) = (i) * 2 + 1 + (j * 2 + 1) * 22;
            *(id + 3) = (i) * 2 + 1 + (j * 2 + 1) * 22;
            *(id + 4) = i * 2 + 0 + (j * 2 + 1) * 22;
            *(id + 5) = i * 2 + 0 + (j * 2 * 22);
        }
    }



    //0, 1, 11, 11, 10, 0, 10, 11, 21, 21, 20, 10 
}*/
