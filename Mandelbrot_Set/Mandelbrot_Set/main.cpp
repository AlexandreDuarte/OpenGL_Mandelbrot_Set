
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cComplex.h"
#include"shader_s.h"
#include<iostream>
#include<vector>

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

Complex function(Complex z, Complex c);

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


    glViewport(0, 0, 2000, 2000);




    unsigned int VBO, VAO, EBO;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    /*glGenBuffers(1, &EBO);*/


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(float), vertices.v_array, GL_STATIC_DRAW);

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
    view = glm::lookAt(glm::vec3(-2.0f, -3.0f, -2.0f), glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    projection = glm::ortho(-1.2f, 1.2f, -1.2f, 1.2f, 0.1f, 100.0f);

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    float old_time = 0, delta_time = 0, time = 0;

    while (!glfwWindowShouldClose(window)) {

        time = glfwGetTime();
        delta_time = time - old_time;
        if (delta_time < 1 / 30) {
            continue;
        }
        old_time = time;
        

        processInput(window);

        glBindVertexArray(0);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, vertices.size);

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

    int max_count = 1000;

    //int size = mask_mandelbrot_set(lenght, max_count);

    int points = lenght + 1;

    std::vector<float> vertices;

    //float* p_a = new float[size*3];

    Complex z, c;

    int count_points = 0;

    float* last_z = new float[10];
    float delta, delta_begin = 0, delta_end = 0;

    float* vertice;
    for (float i = 0; i <= lenght; i++) {
        for (float j = 0; j <= lenght; j++) {
            c.real = 4 * i / lenght - 2;
            c.imag = 4 * j / lenght - 2;
            z = c;
            for (int count = 0; z.mod2() <= 4.0f && count < max_count; count++) {
                z = function(z, c);

                *(last_z + count % 10) = z.real;

                if (count == max_count-1) {
                    delta = last_z[9] - last_z[0];
                    for (int lz_i = 8; lz_i > 0; lz_i--) {
                        if (last_z[lz_i] - last_z[0] < delta) {
                            delta = last_z[lz_i] - last_z[0];
                            delta_end = lz_i;
                        }
                    }


                    for (int lz_i = 0; lz_i <= delta_end; lz_i++) {

                        vertices.push_back(2 * i / lenght - 1);
                        vertices.push_back(2 * j / lenght - 1);
                        vertices.push_back(last_z[lz_i] / 5);

                        /*vertice = p_a + count_points * 3;

                        *vertice = (2 * i / lenght - 1);
                        *(vertice + 1) = (2 * j / lenght - 1);
                        *(vertice + 2) = last_z[lz_i] / 5;*/
                        count_points++;
                    }
                }
            }
        }
    }

    float* p_a = new float[3 * count_points];

    std::copy(vertices.begin(), vertices.end(), p_a);

    return vertices_array{p_a, 3* count_points };
}


Complex function(Complex z, Complex c) {
    return z * z + c;
}



