#ifndef COD2_XMODEL_H
#define COD2_XMODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "include_physfs.h"
#include "opsystem_imgui_console.h"
#include <vector>
#include "include_glm.h"
#include "format_iwi.h"

namespace Format {
namespace CallOfDuty2 {

class Vertex {
public:
	glm::vec3 origin;
	glm::vec3 normal;
	glm::vec2 uv;
};

class XMesh {
public:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

class XModel {
public:
	std::vector<XMesh> meshes;
	std::vector<FormatIWI *> textures;
	int load_success;
	void load_xmodel(const char *path);
	// todo
	void calculate_bounds();
	void renderLegacyOpenGL(float *modelview, float *projection);
	void makeAABB();
	static XModel *getByFilename(char *filename);
};

}}

#endif