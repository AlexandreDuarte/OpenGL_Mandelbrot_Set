#pragma once

#include"shader_s.h"
#include "cComplex.h"
#include<vector>

class Engine
{
public:
	struct points_array {
		float* p_array;
		unsigned int size;
	};
	

public:
	Engine();
	~Engine();

public:
	Shader* shader;
	std::vector<points_array> size;

public:
	void init_shader();
	void update();
	void render();

public:
	void create_points(int&, int&, int&);
	void generate_points(Engine::points_array& ,int, int, int);
	const Complex function(const Complex&, const Complex&);
	void processInput(GLFWwindow* window);

	Shader* getShader();



};

