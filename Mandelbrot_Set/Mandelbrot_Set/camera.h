#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::dvec3 location;
	glm::dvec3 focus;
	double zoom;
	unsigned int shaderID;
	unsigned int MatricesUniformBufferID;

public:
	Camera(unsigned int, glm::dvec3, glm::dvec3, double);

public:
	void updateRotation(double, double);
	void updateZoom(double);

};