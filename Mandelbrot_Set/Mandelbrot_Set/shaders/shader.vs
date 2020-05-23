#version 330 core

layout (std140) uniform MatrixBlock
{
  mat4 projection;
  mat4 view;
};


layout (location = 0) in vec3 vertex_position;

uniform mat4 model; 

void main() {
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}