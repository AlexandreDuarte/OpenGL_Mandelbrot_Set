

#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Camera::Camera(unsigned int shaderID, glm::dvec3 location, glm::dvec3 focus, double zoom) {
	Camera::shaderID = shaderID;
	Camera::location = location;
	Camera::focus = focus;
	Camera::zoom = zoom;

    glm::mat4 view = glm::dmat4(1.0);
    glm::mat4 projection = glm::dmat4(1.0);

    view = glm::lookAt(location, focus, glm::dvec3(0.0, 0.0, -1.0));
    projection = glm::ortho(-1.2, 1.2, -1.2, 1.2, 0.1, 100.0);

    unsigned int MatricesUniformBlockID = glGetUniformBlockIndex(shaderID, "MatrixBlock");
    
    glUseProgram(shaderID);
    
    glUniformBlockBinding(shaderID, MatricesUniformBlockID, 0);

    glGenBuffers(1, &MatricesUniformBufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::dmat4) + sizeof(double), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, MatricesUniformBufferID, 0, 2 * sizeof(glm::dmat4) + sizeof(double));

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::dmat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::dmat4), sizeof(glm::dmat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    double z_i = 1.0;

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::dmat4), sizeof(double), &z_i);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void Camera::updateRotation(double rotation, double v_rotation) {

    glm::dvec3 r_transform = glm::dvec3(cos(glm::radians(rotation)), sin(glm::radians(rotation)), sin(glm::radians(v_rotation)));

    glm::dvec3 n_location = location * r_transform;

    //glm::mat4 view = glm::lookAt(glm::vec3((3.0f) * cos(glm::radians(rotation)), (3.0f) * sin(glm::radians(rotation)), 3.0f * sin(glm::radians(v_rotation))), glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    glm::dmat4 view = glm::lookAt(n_location, focus, glm::dvec3(0.0, 0.0, -1.0));


    glUseProgram(shaderID);
    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::dmat4), sizeof(glm::dmat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::updateZoom(double zoom) {

    Camera::zoom = zoom;

    double z = exp(-zoom / 100);

    glm::dmat4 projection = glm::ortho(-1.2 * z, 1.2 * z, -1.2 * z, 1.2 * z, 0.1, 100.0);

    if (z > 1) {
        z = 1.0f;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::dmat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    glBindBuffer(GL_UNIFORM_BUFFER, MatricesUniformBufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::dmat4), sizeof(double), &z);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}



