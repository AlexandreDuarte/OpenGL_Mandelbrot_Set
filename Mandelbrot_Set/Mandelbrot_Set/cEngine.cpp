
#include<future>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"





Engine::Engine() {


    rotation = 0;
    v_rotation = 90;
    b_rotation = false;

    points_array* vertices = new points_array();
    generate_points(vertices, 2000, 0, 1);

    glViewport(0, 0, 2000, 2000);

    

    glEnable(GL_DEBUG_OUTPUT);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (*vertices).size * sizeof(float), (*vertices).p_array, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    size.push_back((*vertices).size);

}

void Engine::init_shader() {
    Shader s("shaders/shader.vs", "shaders/shader.fs");
    s.use();

    std::cout << s.ID << std::endl;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(2.0f, -3.0f, 2.0f), glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    projection = glm::ortho(-1.2f, 1.2f, -1.2f, 1.2f, 0.1f, 100.0f);

    s.setMat4("model", model);

    unsigned int MatricesUniformBlockID = glGetUniformBlockIndex(s.ID, "MatrixBlock");
    glUniformBlockBinding(s.ID, MatricesUniformBlockID, 0);

    glGenBuffers(1, &MatricesUniformBufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, MatricesUniformBufferID, 0, 2 * sizeof(glm::mat4));

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    shaderID = s.ID;
}

void Engine::update() {

    int reso = 1000, offs = 0, st = 1;

}

void Engine::render(double* delta_time, float zoom) {
    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    
    glm::mat4 view = glm::lookAt(glm::vec3((3.0f) * cos(glm::radians(rotation)), (3.0f) * sin(glm::radians(rotation)), 3.0f * sin(glm::radians(v_rotation))), glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    
    glm::mat4 projection = glm::ortho(-1.2f*exp(-zoom / 100), 1.2f * exp(-zoom / 100), -1.2f * exp(-zoom / 100), 1.2f * exp(-zoom / 100), 0.1f, 100.0f);

   

    glUseProgram(shaderID);

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    glBindVertexArray(VAO);

    glDrawArrays(GL_POINTS, 0, size.at(0));

    glBindVertexArray(0);
}




void Engine::processInput(GLFWwindow* window, double* delta_time)
{


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (rotation + 100 * *delta_time >= 360)
            rotation -= 360;
        rotation += 100**delta_time;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (rotation - 100 * *delta_time <= 0)
            rotation += 360;
        rotation -= 100* *delta_time;
        b_rotation = true;
    }
    
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        b_rotation = false;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (v_rotation + 100 * *delta_time >= 90)
            v_rotation = 90;
        else v_rotation += 100 * *delta_time;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (v_rotation - 100 * *delta_time <= -90)
            v_rotation = -90;
        else v_rotation -= 100 * *delta_time;
        b_rotation = true;
    }
    
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        b_rotation = false;
    }
}

void Engine::create_points(int& resolution, int& offset, int& step) {
    Engine::points_array& p ();
   // std::future<points_array> a = std::async(Engine::generate_points, std::ref(p), resolution, offset, step);
}

void Engine::generate_points(Engine::points_array* output, int resolution, int offset, int step) {

    unsigned int max_count = 1000;

    std::vector<float> points;


    int count_points = 0;

    float* last_z = new float[10];
    float delta;
    int delta_end = 0;

    float* vertice;
    for (unsigned int i = offset; i <= resolution; i+=step) {
        for (unsigned int j = offset; j <= resolution; j+=step) {
            
            std::complex<float> c(4 * (float)i / (float)resolution - 2, 4 * (float)j / (float)resolution - 2);
            std::complex<float> z = c;
            for (unsigned int count = 0; abs(z._Val[0]) + abs(z._Val[1]) <= 4.0f && count < max_count; count++) {
                z = z * z + c;

                *(last_z + count % 10) = z._Val[0];

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

    *output = points_array{ p_a, 3 * count_points };
}

