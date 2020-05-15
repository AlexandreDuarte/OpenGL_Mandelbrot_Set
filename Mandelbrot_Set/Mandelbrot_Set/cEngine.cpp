
#include<future>
#include "cComplex.h"
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"



Engine::Engine() {


    points_array vertices;
    generate_points(vertices, 1000, 0, 1);

    glViewport(0, 0, 2000, 2000);

    unsigned int VBO, VAO;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(float), vertices.p_array, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    size.push_back(vertices);

}

void Engine::init_shader() {
    Shader s("shaders/shader.vs", "shaders/shader.fs");
    s.use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(-2.0f, -3.0f, -2.0f), glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    projection = glm::ortho(-1.2f, 1.2f, -1.2f, 1.2f, 0.1f, 100.0f);

    s.setMat4("projection", projection);
    s.setMat4("view", view);
    s.setMat4("model", model);
    
    shader = &s;
}

void Engine::update() {

    int reso = 1000, offs = 0, st = 1;

}

void Engine::render() {
    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    glBindVertexArray(1);
    glDrawArrays(GL_POINTS, 0, size.at(0).size);

    glBindVertexArray(0);
}


void Engine::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Engine::create_points(int& resolution, int& offset, int& step) {
    Engine::points_array& p ();
   // std::future<points_array> a = std::async(Engine::generate_points, std::ref(p), resolution, offset, step);
}

void Engine::generate_points(Engine::points_array& output, int resolution, int offset, int step) {

    unsigned int max_count = 1000;

    std::vector<float> points;

    Complex z, c;

    unsigned int count_points = 0;

    float* last_z = new float[10];
    float delta, delta_begin = 0, delta_end = 0;

    float* vertice;
    for (unsigned int i = offset; i <= resolution; i+=step) {
        for (unsigned int j = offset; j <= resolution; j+=step) {
            
            c.real = 4 * (float)i / (float)resolution - 2;
            c.imag = 4 * (float)j / (float)resolution - 2;
            z = c;
            for (unsigned int count = 0; z.mod2() <= 4.0f && count < max_count; count++) {
                z = function(z, c);

                *(last_z + count % 10) = z.real;

                if (count == max_count - 1) {
                    delta = last_z[9] - last_z[0];
                    for (int lz_i = 8; lz_i > 0; lz_i--) {
                        if (last_z[lz_i] - last_z[0] < delta) {
                            delta = last_z[lz_i] - last_z[0];
                            delta_end = lz_i;
                        }
                    }


                    for (unsigned int lz_i = 0; lz_i <= delta_end; lz_i++) {

                        points.push_back(2 * (float)i / (float)resolution - 1);
                        points.push_back(2 * (float)j / (float)resolution - 1);
                        points.push_back(last_z[lz_i] / 5);
                        count_points++;
                    }
                }
            }
        }
    }

    float* p_a = new float[3 * count_points];

    std::copy(points.begin(), points.end(), p_a);

    output = points_array{ p_a, 3 * count_points };
}


const Complex Engine::function(const Complex& z, const Complex& c) {
    return z * z + c;
}


Shader* Engine::getShader() {
    return shader;
}
