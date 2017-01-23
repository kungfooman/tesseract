#ifndef OP_FILE_H
#define OP_FILE_H

#define SHORTNAME "File"
#define CLASSNAME OpFile
#define METHOD(name) op_file_##name
#define STRUCTNAME struct op_file_s

#include "op.h"

#include "include_devil.h"
#include "include_physfs.h"

#include "format_iwi.h"

STRUCTNAME {
	#include "op_base.h"
	
	GLubyte *checkImage = NULL;

	char filename[512];
	PHYSFS_File *file_handle;
	char *file_buffer;
	int file_length;
	int file_length_read;


	GLuint textureid = 0;
	FormatIWI *format_iwi = NULL;

};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);
      void METHOD(makeCheckImage    )(CLASSNAME *op);
      void METHOD(generate_texture  )(CLASSNAME *op);



#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif