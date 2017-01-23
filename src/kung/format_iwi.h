#ifndef FORMAT_IWI_H
#define FORMAT_IWI_H

#include "include_gl.h"
#include "include_physfs.h"
#include "include_devil.h"
#include <kung/include_imgui.h>

class FormatIWI {
public:
	GLuint textureid = 0;
	ILuint devilID = 0;


	char filename[512];
	PHYSFS_File *file_handle;
	char *file_buffer;
	int file_length;
	int file_length_read;
	
	static FormatIWI *getByFilename(char *filename);
	//static FormatIWI *getByData(char *buffer, int size); // unsure how i shall cache this via std::map
	void imgui();
};

#endif