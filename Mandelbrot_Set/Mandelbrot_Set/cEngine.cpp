
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"
#include <intrin.h>
#include <limits>
#include <math.h>

Engine::Engine() {

    parallel_finished = false;
    rotation = 0;
    v_rotation = 90;
    b_rotation = false;
    active_draw = -1;
    x_offset = 0;
    y_offset = 0;

    create_points(1000, 0, 1);


    glViewport(0, 0, 2000, 2000);

    glEnable(GL_DEBUG_OUTPUT);
}

Engine::~Engine() {

}

void Engine::init_shader() {
    Shader s("shaders/shader.vs", "shaders/shader.fs");

    camera = new Camera(s.ID, glm::dvec3(2.0, -2.0, 2.0), glm::dvec3(-0.5, 0.0, 0.0), 1.0);

}

void Engine::render(double* delta_time, double zoom) {
    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if (b_rotation)
    camera->updateRotation(rotation, v_rotation, x_offset, y_offset);

    camera->updateZoom(zoom);

    if (parallel_finished) {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, p_arrays.size() * sizeof(double), p_arrays.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribLPointer(0, 3, GL_DOUBLE, 3 * sizeof(double), (void*)(0));

        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        active_draw = 0;
        parallel_finished = false;
    }

    if (active_draw != -1) {
        glBindVertexArray(VAO);

        glDrawArrays(GL_POINTS, 0, p_arrays.size()/3);

        glBindVertexArray(0);
    }
}

void Engine::create_points(double resolution, double offset, double step) {



        int64_t max_count = 1500;
        __m256i _zero = _mm256_set1_epi64x(0);
        __m256d _four = _mm256_set1_pd(4.0);
        __m256d _two = _mm256_set1_pd(2.0);
        __m256i _one = _mm256_set1_epi64x(1);
        __m256i _ten = _mm256_set1_epi64x(10);
        __m256i _nine = _mm256_set1_epi64x(9);
        __m256d _resolution = _mm256_set1_pd(resolution);
        __m256i _count;
        __m256i _lz_i;
        __m256i _max_count = _mm256_set1_epi64x(max_count);
        __m256i _max_count_minus_ten = _mm256_set1_epi64x(max_count - 19);
        __m256i _c;

        double delta;
        int delta_end = 0;

        int mask;

        __m256d _zr;
        __m256d _zi;

        __m256d _cr;
        __m256d _ci;

        __m256d _zr2, _zi2, _zrzi;

        __m256d _lz;

        //masks 
        __m256i _mask2 = _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFF);
        __m256d _mask1 = _mm256_castsi256_pd(_mask2);
        __m256i _maskfinal = _mm256_set1_epi64x(0);

        __m256i _c_mask = _mask2;

        __m256d _i;
        __m256d _j;

        //last iterations array
        __m256d _last_z[20]{0};

        int _llz;
        int m = 1;

        double d[4 * 20]{0};

        for (double i = offset; i <= resolution; i += 4 * step) {
            _i = _mm256_set_pd(i + 3.0, i + 2.0, i + 1.0, i + 0.0);

            _cr = _mm256_sub_pd(_mm256_mul_pd(_four, _mm256_div_pd(_i, _resolution)), _two);

            for (double j = offset; j <= resolution; j += step) {

                _llz = 0;

                _j = _mm256_set1_pd(j);

                //c init
                _ci = _mm256_sub_pd(_mm256_mul_pd(_four, _mm256_div_pd(_j, _resolution)), _two);

                //c = z;
                _zr = _cr;
                _zi = _ci;

                _mask2 = _mm256_set1_epi64x(0xFFFFFFFFFFFFFFFF);

                _maskfinal = _mm256_set1_epi64x(0);

                _count = _zero;

                _c = _zero;

                while (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0) {

                    //z = z*z + c
                    //z = zr + zi * i (complex number), same as c
                    _zr2 = _mm256_mul_pd(_zr, _zr);

                    _zi2 = _mm256_mul_pd(_zi, _zi);

                    _zi = _mm256_mul_pd(_zr, _zi);

                    _zr = _mm256_sub_pd(_zr2, _zi2);

                    _zr = _mm256_add_pd(_zr, _cr);

                    _zi = _mm256_fmadd_pd(_zi, _two, _ci);

                    //|z|^2 <= 4
                    _mask1 = _mm256_cmp_pd(_mm256_add_pd(_zr2, _zi2), _four, _CMP_LT_OQ);

                    //count < max_count
                    _mask2 = _mm256_cmpgt_epi64(_max_count, _count);

                    _mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));

                    _c = _mm256_and_si256(_one, _mask2);

                    _count = _mm256_add_epi64(_count, _c);

                    _c_mask = _mm256_cmpgt_epi64(_count, _max_count_minus_ten);

                    //saving last 10 iterations

                    if (_mm256_movemask_pd(_mm256_castsi256_pd(_c_mask)) > 0) {

                        _maskfinal = _mm256_or_si256(_maskfinal, _mm256_castpd_si256(_mask1));

                        _last_z[_llz] = _zr;

                        _llz++;
                    }
                }

                //from 10 last iterations validate the different converging values
                for (size_t lz_i = 0; lz_i < 20; lz_i++) {
                    _mm256_store_pd(&d[lz_i * 4], _last_z[lz_i]);
                }

                //_mask1: z^2 <= 4, to 4 bit int
                mask = _mm256_movemask_pd(_mm256_castsi256_pd(_maskfinal));


                //use m as a mask between the 4 bits in mask that represent _mask1 condition for the 4 doubles
                m = 0b1000;
                if (mask == 0x0000) {
                    continue;
                }
                for (size_t ii = 0; ii < 4; ii++) {
                    //cycle throw the 4 first bits in mask
                    if (!((mask & m)>0)) {
                        //shift to check next condition from double bundle
                        m >>= 1;
                        continue;
                    }
                    m >>= 1;

                    //search for periodic converging numbers and add them to points pool
                    delta = 1.0/200000;
                    delta_end = 0;
                    
                    //search for periodic
                    for (size_t lz_i = 1; lz_i < 20; lz_i++) {
                        if (d[ii + lz_i * 4] - d[ii] < delta) {
                            delta_end = lz_i;
                        }
                    }
                    //add to pool
                    for (size_t lz_i = 0; lz_i < delta_end; lz_i++) {
                        p_arrays.push_back(2 * (i + (double)ii) / resolution - 1);
                        p_arrays.push_back(2 * (double)(j) / resolution - 1);
                        p_arrays.push_back(d[ii + lz_i * 4] / 5.0);
                    }

                }
            

            }
        }

        parallel_finished = true;
}

void Engine::processInput(GLFWwindow* window, double* delta_time, double zoom)
{

    double z = exp(-zoom / 100);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (rotation + 100 * *delta_time >= 360)
            rotation -= 360;
        rotation += 100 * *delta_time;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (rotation - 100 * *delta_time <= 0)
            rotation += 360;
        rotation -= 100 * *delta_time;
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

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        x_offset += 0.01*z;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

        x_offset -= 0.01 * z;
        b_rotation = true;
    }

    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
        b_rotation = false;
    }


    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        y_offset += 0.01 * z;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        y_offset -= 0.01 * z;
        b_rotation = true;
    }

    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
        b_rotation = false;
    }
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

