#pragma once

#include"shader_s.h"
#include<vector>
#include<complex>
#include"camera.h"

class Engine
{

public:
	Engine();
	~Engine();

public:
	Camera* camera;
	bool parallel_finished;
	double rotation;
	double v_rotation;
	double x_offset;
	double y_offset;
	bool b_rotation;
	unsigned int VBO, VAO;
	int active_draw;

	std::vector<double> p_arrays;

public:
	void init_shader();
	void render(double*, double);

public:
	void create_points(double, double, double);
	//void generate_points(Engine::points_array* ,int, int, int);
	void processInput(GLFWwindow* window, double*, double);
};

