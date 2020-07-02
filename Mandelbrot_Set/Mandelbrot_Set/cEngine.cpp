
#include<future>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"





Engine::Engine() {

    parallel_finished = false;
    rotation = 0;
    v_rotation = 90;
    b_rotation = false;
    active_draw = -1;

    create_points(1000, 0, 1);


    glViewport(0, 0, 2000, 2000);

    glEnable(GL_DEBUG_OUTPUT);
}

void Engine::init_shader() {
    Shader s("shaders/shader.vs", "shaders/shader.fs");

    camera = new Camera(s.ID, glm::dvec3(2.0, -3.0, 2.0), glm::dvec3(-0.5, 0.0, 0.0), 1.0);
    
}

void Engine::update() {

    int reso = 1000, offs = 0, st = 1;

}

void Engine::render(double* delta_time, double zoom) {
    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if (b_rotation)
    camera->updateRotation(rotation, v_rotation);

    camera->updateZoom(zoom);

    if (parallel_finished) {
        std::cout << parallel_finished << std::endl;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, (*p_arrays.at(0)).size * sizeof(double), (*p_arrays.at(0)).p_array, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribLPointer(0, 3, GL_DOUBLE, 3 * sizeof(double), (void*)(0));

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        active_draw = 0;
        parallel_finished = false;
    }

    if (active_draw != -1) {
        glBindVertexArray(VAO);

        glDrawArrays(GL_POINTS, 0, p_arrays.at(active_draw)->size);

        glBindVertexArray(0);
    }
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

void Engine::create_points(int resolution, int offset, int step) {
    Engine::points_array* po_paralel = new points_array();

    

    auto generate_points = [](bool* finished, Engine::points_array * output, int resolution, int offset, int step) {

        unsigned int max_count = 1000;

        std::vector<double> points;


        size_t count_points = 0;

        double* last_z = new double[10];
        double delta;
        int delta_end = 0;

        double* vertice;
        for (size_t i = offset; i <= resolution; i += step) {
            for (size_t j = offset; j <= resolution; j += step) {

                std::complex<double> c(4 * (double)i / (double)resolution - 2, 4 * (double)j / (double)resolution - 2);
                std::complex<double> z = c;
                for (size_t count = 0; abs(z._Val[0]) + abs(z._Val[1]) <= 4.0 && count < max_count; count++) {
                    z = z * z + c;

                    *(last_z + count % 10) = z._Val[0];

                    if (count == max_count - 1) {
                        delta = last_z[9] - last_z[0];
                        for (size_t lz_i = 8; lz_i > 0; lz_i--) {
                            if (last_z[lz_i] - last_z[0] < delta) {
                                delta = last_z[lz_i] - last_z[0];
                                delta_end = lz_i;
                            }
                        }


                        for (size_t lz_i = 0; lz_i <= delta_end; lz_i++) {

                            points.push_back(2 * (double)i / (double)resolution - 1);
                            points.push_back(2 * (double)j / (double)resolution - 1);
                            points.push_back(last_z[lz_i] / 5);
                            count_points++;
                        }
                    }
                }
            }
        }

        double* p_a = new double[3 * count_points];

        std::copy(points.begin(), points.end(), p_a);

        *output = points_array{ p_a, 3 * count_points };
        *finished = true;
        
    };

    p_arrays.push_back(po_paralel);

    std::thread a(generate_points, &parallel_finished, std::ref(p_arrays.back()), resolution, offset, step);
    a.detach();

    
    
}

/*void Engine::generate_points(Engine::points_array* output, int resolution, int offset, int step) {

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
}*/

