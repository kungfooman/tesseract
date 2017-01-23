#include "mesh.h"
#include <list>

std::list<Mesh *> meshes;

Mesh::Mesh() {
	//meshes.push_back(this);
	

	

}
void Mesh::init() {
	//make_cube_wireframe(data, x, y, z, n);
	//buffer = gen_buffer(sizeof(data), data);
}
Mesh::~Mesh() {
	//glDeleteBuffers(1, &buffer);
}
void Mesh::draw(Attrib *attrib) {
	//// was: draw_lines(attrib, wireframe_buffer, 3, 24);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer_position);
	//glEnableVertexAttribArray(attrib->position);
	//glVertexAttribPointer(attrib->position, /*components*/3, GL_FLOAT, GL_FALSE, 0, 0);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, buffer_color);
	//glEnableVertexAttribArray(attrib->color);
	//glVertexAttribPointer(attrib->color, /*components*/3, GL_FLOAT, GL_FALSE, 0, 0);
	//
	//glDrawArrays(GL_LINES, 0, /*count*/count);
	//glDisableVertexAttribArray(attrib->position);
	//glDisableVertexAttribArray(attrib->color);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}
