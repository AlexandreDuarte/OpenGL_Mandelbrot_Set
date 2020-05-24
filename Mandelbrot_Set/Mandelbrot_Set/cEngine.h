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
		int size;
	};
	


public:
	Engine();
	~Engine();

public:
	Camera* camera;
	bool parallel_finished;
	volatile double rotation;
	volatile double v_rotation;
	volatile bool b_rotation;
	unsigned int VBO, VAO;
	int active_draw;

	std::vector<Engine::points_array*> p_arrays;

public:
	void init_shader();
	void update();
	void render(double*, double);

public:
	void create_points(int, int, int);
	//void generate_points(Engine::points_array* ,int, int, int);
	void processInput(GLFWwindow* window, double*);
};

