#ifndef MESH_H
#define MESH_H

#include <include_gl.h>

typedef struct {
    GLuint program;
    GLuint position;
	GLuint color;
    GLuint normal;
    GLuint uv;
    GLuint matrix;
    GLuint sampler;
    GLuint camera;
    GLuint timer;
    GLuint extra1;
    GLuint extra2;
    GLuint extra3;
    GLuint extra4;
} Attrib;

class Mesh {
public:
	GLuint buffer;
	GLuint prog = 0;
	float data[72];
	float x = 0, y = 0, z = 0, n = 0.53;
	int count;
	Mesh();
	void init();
	~Mesh();
	void draw(Attrib *attrib);
};


#endif