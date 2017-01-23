/* 	This is part of my custom engine client/server to load CoD2 XModels
	Feel free to use this as reference or anything at all.
	- Richard
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "include_physfs.h"
#include "opsystem_imgui_console.h"
#include <vector>

#include "include_glm.h"

#include "format_cod2_xmodel.h"

#if 1


using namespace Format::CallOfDuty2;

#define  logDebug log

	void XModel::calculate_bounds() {
	
	}

#define BSP_SCALE 1.0f

int fs_readfile(const char *path, char **buffer, size_t *out_filesize) {
	PHYSFS_File *f = PHYSFS_openRead(path);
	if (f == NULL)
	{
		log("load_file: Can't open \"%s\"\n", path);
		return 1;
	}
    int length = PHYSFS_fileLength(f);
	*out_filesize = length;

    *buffer = (char *)calloc(length + 1, sizeof(char));
	PHYSFS_read(f, *buffer, 1, length + 1);
	PHYSFS_close(f);
	
    return 0;
}

bool load_xmodel_surfaces(XModel *model, const char *lod) {
	char path[256] = { 0 };
	snprintf(path, sizeof(path) - 1, "xmodelsurfs/%s", lod);

//#define hide_printf log

	logDebug("opening file %s\n", path);

	unsigned char *buf = NULL;
	size_t file_size=0;
	if (fs_readfile(path, (char**)&buf, &file_size)) {
		return false;
	}
	logDebug("xmodelsurf filesize=%d\n", file_size);
#if 0
	Con::MsgBox("buf=%02X\n", buf);
	__asm int 3
#endif
	size_t idx = 0;

	if (*(uint16_t*)buf != 20) {
		log("not valid xmodel surf\n");
		free(buf);
		return false;
	}

	idx += 2;
#if 0
	for (int i = idx; i < file_size; i++) {
		hide_printf("%02X ", (char)buf[i]);
	}
#endif
	uint16_t nummeshes = *(uint16_t*)(buf + idx);
	idx += 2;

	log("num meshes=%d\n", nummeshes);

	for (unsigned i = 0; i < nummeshes; i++) {
#define MODELTYPE_RIGID 0
#define MODELTYPE_PLAYERBODY 2

		uint8_t modeltype = *(uint8_t*)(buf + idx); idx++;
		//assumingly filetype? 2 = playerbody? 0 = rigid?

		if (modeltype != 0) {
			model->meshes.clear();
			free(buf);
			return false;
		}
		
		uint16_t numverts = *(uint16_t*)(buf + idx); idx += 2;
		uint16_t numtriangles = *(uint16_t*)(buf + idx); idx += 2;
		uint16_t numverts2 = *(uint16_t*)(buf + idx); idx += 2; //idk

		logDebug("numverts=%d(loc=%02X)\n", numverts,idx);
		logDebug("numtriangles=%d(loc=%02X)\n", numtriangles, idx);
		logDebug("numverts2=%d(loc=%02X)\n", numverts2,idx);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		bool y_up = false; //true = z up //false = y up

		for (int j = 0; j < numverts; j++) {
			idx += 12;

			uint32_t color = *(uint32_t*)(buf + idx);
			idx += 4; //0xffffffff?

			Vertex nv;
			float u, v;
			u = *(float*)(buf + idx); idx += 4;
			//v = 1.0f - *(float*)(buf + idx); idx += 4;
			v = *(float*)(buf + idx); idx += 4;
			uvs.push_back(glm::vec2(u, v));

			idx += 24; //idk

			//uint8_t numweights = *(uint8_t*)(buf + idx);

			//positions
			float x, y, z;
			x = *(float*)(buf + idx); idx += 4;
			if (!y_up) {
				z = *(float*)(buf + idx) * -1; idx += 4;
				y = *(float*)(buf + idx); idx += 4;
			} else {
				y = *(float*)(buf + idx); idx += 4;
				z = *(float*)(buf + idx); idx += 4;
			}

#define CM_TO_INCH 1
//#define CM_TO_INCH 0.3937007874015748031496062992126 // 1cm = 50 / 127in
			positions.push_back(glm::vec3(x/ CM_TO_INCH, y/ CM_TO_INCH, z/ CM_TO_INCH) * BSP_SCALE);
			normals.push_back(glm::vec3(1/ CM_TO_INCH, 1/ CM_TO_INCH, 1/ CM_TO_INCH));

#if 0
			if (modeltype == MODELTYPE_PLAYERBODY)
				idx += 3;//3 extra bytes??
#endif

			//hide_printf("BLOCK SIZE = %02X\n", idx - cur_idx);
		}

		logDebug("cur_idx=%d\n", idx);

		unsigned vIdx = 0;
		XMesh m;
		m.name = path;


		//if (i >= model->textures.size())
		//	m.texture = nullptr;
		//else
		//	m.texture = model->textures[i];


		logDebug("starting triangles indices at %02X\n", idx);
		for (int j = 0; j < numtriangles; j++) {
			uint16_t face[3];
			face[0] = *(uint16_t*)(buf + idx); idx += 2;
			face[1] = *(uint16_t*)(buf + idx); idx += 2;
			face[2] = *(uint16_t*)(buf + idx); idx += 2;

			//logDebug("face(%d,%d,%d)\n", face[0], face[1], face[2]);

			for (int i = 0; i < 3; i++) {
				Vertex vv;

				uint16_t indice = face[i];
				if (indice < 0 || indice > positions.size()) {
					logDebug("ERROR! indice = %d\n", indice);
					free(buf);
					return false;

					//break;
				}
				


				vv.origin = positions.at(indice);
				vv.normal = normals.at(indice);
				vv.uv = uvs.at(indice);
				


				m.vertices.push_back(vv);
				m.indices.push_back(vIdx++);
			}
		}
		model->meshes.push_back(m);
		logDebug("idx=%d,filesize=%d\n", idx, file_size);
		//logDebug("idx=%d,(%02X)\n", idx,idx);
	}

	free(buf);
	return true;
}

std::string cod2_get_color_map_from_material(const char *matpath) {

	char *buf = NULL;
	size_t file_size = 0;
	if (fs_readfile(matpath, (char**)&buf, &file_size)) {
		log("COULD NOT OPEN MATERIAL file %s\n", matpath);
		return "";
	}

	size_t idx = 0;

	uint32_t matname_i = *(uint32_t*)(buf + idx); idx += 4;
	uint32_t colormap_i = *(uint32_t*)(buf + idx);

	std::string ret = (char*)(buf + colormap_i);

	free(buf);
	return ret;
}

#include <map>
std::map<std::string, XModel *> loaded_xmodels;
XModel *XModel::getByFilename(char *filename) {
	auto it = loaded_xmodels.find(filename);
	if (it != loaded_xmodels.end()) {
		return it->second;
	}

	auto ret = new XModel();
	ret->load_xmodel(filename);
	loaded_xmodels[filename] = ret; // cache it
	return ret;
}

void XModel::load_xmodel(const char *path) {
	std::vector<std::string> lodstrings;
	int numlods = 0;
	bool read_mats = true;

	logDebug("load_xmodel(%s)\n", path);

#if 1
	//this->m_fullpath = path;
	char *buf=NULL;
	size_t file_size;
	if (fs_readfile(path, (char**)&buf, &file_size)) {
		log("cant read xmodel %s\n", path);
		load_success = 0;
		return;
	}
	logDebug("filesize=%d\n", file_size);
#if 0
	for (int i = 0; i < file_size; i++) {
		logDebug("%02X ", (char)buf[i]);
	}
#endif

	bool success = false;
	size_t idx = 0;
	uint16_t xmodelver = *(uint16_t*)buf;

	if (xmodelver != 20) {
		log("xmodel version is not 20!\n");
		goto _ret;
	}
	idx += 2;

	idx += 25; //cod4/cod2 have 25 bytes here idk

	for (int i = 0; i < 4; i++) {
		uint32_t some_int = *(uint32_t*)(buf + idx);
		idx += 4;

		char *lodfilename = (char*)(buf + idx);
		if (*lodfilename) {
			logDebug("lod=%s\n", lodfilename);
			lodstrings.push_back(lodfilename);
			numlods++;
		}

		idx += strlen(lodfilename) + 1;
	}
#ifdef BUILD_SERVER
	read_mats = false;
#else
	read_mats = true;
#endif
	if (*(uint32_t*)(buf + idx) != -1)
		read_mats = false;
	idx += 8;

#if 0
	uint32_t count = *(uint32_t*)(buf + idx);
	printf("count=%d\n", count);
	for (int cc = 0; cc != count; cc++) {
		uint32_t subcount = *(uint32_t*)(buf + idx);
		idx += (subcount * 48) + 36;
	}
	printf("STIEKEM GEDANST IDX = %02X\n", idx);
	idx += 8;
#endif
	{
		if (read_mats) {
			std::vector<std::string> matstrings;
			for (int i = 0; i < numlods; i++) {
				uint16_t num_materials = *(uint16_t*)(buf + idx);
				idx += 2;

				logDebug("num_materials=%d\n", num_materials);

				for (int k = 0; k < num_materials; k++) {
					char *matname = (char*)(buf + idx);
#if 1
					char matpath[128] = { 0 };
					snprintf(matpath, sizeof(matpath) - 1, "materials/%s", matname);
					std::string colmap = "images/" + cod2_get_color_map_from_material(matpath) + ".iwi";
					logDebug("colmap=%s\n", colmap.c_str());
					FormatIWI *texture = FormatIWI::getByFilename((char *)colmap.c_str());
					if (texture) {
#if 0
						if (strstr(lodstrings[0].c_str(), "bush") || strstr(lodstrings[0].c_str(), "grass"))
							texture->set_effect(texture->get_effect() | TEXTURE_EFFECT_SIN_WAVE);
#endif
						
						this->textures.push_back(texture);
					}
#endif
					matstrings.push_back(matname);

					idx += strlen(matname) + 1;
				}
			}

			for (auto & m : matstrings)
				logDebug("material: %s\n", m.c_str());
		}

		load_xmodel_surfaces(this, lodstrings[0].c_str());
	}
	load_success = true;

	makeAABB();


	_ret:
	free(buf);
	calculate_bounds();
#endif
}

// just dumped code from op_xmodel.cpp
void XModel::makeAABB() {
	//XMesh *firstmesh = &op->xmodel->meshes.front();
	//
	//float min = 999999;
	//float max = -999999;
	//std::vector<float> origins;
	//origins.reserve(firstmesh->vertices.size() * 3);
	//for (auto vertex : firstmesh->vertices) {
	//
	//	for (int i=0; i<3; i++) {
	//		if (vertex.origin[i] < min)
	//			min = vertex.origin[i];
	//		if (vertex.origin[i] > max)
	//			max = vertex.origin[i];
	//	}
	//
	//	origins.push_back(vertex.origin[0]);
	//	origins.push_back(vertex.origin[1]);
	//	origins.push_back(vertex.origin[2]);
	//}
	//
	//log("xmodel: min: %.2f max: %.2f\n", min, max);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, op->default_link_outputs[0].opengl_id);
	//glBufferData(GL_ARRAY_BUFFER, origins.size() * 4, origins.data(), GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void XModel::renderLegacyOpenGL(float *modelview, float *projection) {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(modelview);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);

	//glBegin(GL_TRIANGLES);
	//glVertex3f( 0.0f, 1.0f + 2.0f, 0.0f); // Top
	//glVertex3f(-1.0f,-1.0f + 2.0f, 0.0f); // Bottom Left
	//glVertex3f( 1.0f,-1.0f + 2.0f, 0.0f); // Bottom Right
	//glEnd();

	if ( ! this)
		return;
	if ( ! this->load_success)
		return;
	if (meshes.size() == 0)
		return;

	auto &indices = meshes[0].indices;
	auto &vertices = meshes[0].vertices;

	//log("num of indices: %d\n", indices.size());

	int n = indices.size();
	
	//glBindTexture(7)

	glEnable(GL_TEXTURE_2D);
	if (textures.size() == 0)
		glBindTexture(GL_TEXTURE_2D, 5);
	else
		glBindTexture(GL_TEXTURE_2D, textures[0]->textureid);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i=0; i<n; i+=3) {
		auto &vertex_a = vertices[i+0];
		auto &vertex_b = vertices[i+1];
		auto &vertex_c = vertices[i+2];

		
		
		glTexCoord2f( vertex_a.uv[0], vertex_a.uv[1]);
		glVertex3fv( glm::value_ptr( vertex_a.origin ));
		
		glTexCoord2f( vertex_c.uv[0], vertex_c.uv[1]);
		glVertex3fv( glm::value_ptr( vertex_c.origin ));
		
		glTexCoord2f( vertex_b.uv[0], vertex_b.uv[1]);
		glVertex3fv( glm::value_ptr( vertex_b.origin ));
	}
	glEnd();	

	glDisable(GL_TEXTURE_2D);
}

#endif