#version 460 core

layout (std140) uniform MatrixBlock
{
  dmat4 projection;
  dmat4 view;
  double zoom;
};


layout (location = 0) in dvec3 vertex_position;

void main() {
	gl_Position = vec4(projection * view * dvec4(vertex_position.x, vertex_position.y, vertex_position.z * zoom, 1));
}