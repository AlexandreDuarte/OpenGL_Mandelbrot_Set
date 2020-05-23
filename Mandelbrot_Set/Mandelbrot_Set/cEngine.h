#pragma once

#include"shader_s.h"
#include<vector>
#include<complex>

class Engine
{
public:
	struct points_array {
		float* p_array;
		int size;
	};
	


public:
	Engine();
	~Engine();

public:
	unsigned int shaderID;
	std::vector<unsigned int> size;
	unsigned int MatricesUniformBufferID;
	volatile float rotation;
	volatile float v_rotation;
	volatile bool b_rotation;
	unsigned int VBO, VAO;
	glm::mat4* v;
	glm::mat4* p;

public:
	void init_shader();
	void update();
	void render(double*, float);

public:
	void create_points(int&, int&, int&);
	void generate_points(Engine::points_array* ,int, int, int);
	void processInput(GLFWwindow* window, double*);

	Shader* getShader();



};

