#pragma once

#include"shader_s.h"
#include<vector>
#include<complex>
#include"camera.h"

class Engine
{
public:
	struct points_array {
		double* p_array;
		size_t size;
	};
	


public:
	Engine();
	~Engine();

public:
	Camera* camera;
	bool parallel_finished;
	double rotation;
	double v_rotation;
	bool b_rotation;
	unsigned int VBO, VAO;
	int active_draw;

	std::vector<Engine::points_array*> p_arrays;

public:
	void init_shader();
	void update();
	void render(double*, double);

public:
	void create_points(double, double, double);
	//void generate_points(Engine::points_array* ,int, int, int);
	void processInput(GLFWwindow* window, double*);
};

