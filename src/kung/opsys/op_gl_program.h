#ifndef OP_PROGRAM_H
#define OP_PROGRAM_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Program"
#define CLASSNAME OpProgram
#define METHOD(name) op_program_##name
#define STRUCTNAME struct op_program_s

STRUCTNAME {
	#include "op_base.h"
	char filename[128] = {"shaders/line2_fragment.glsl"};
	// inputs
	GLuint &fragmentshader = default_link_inputs[0].opengl_id;
	GLuint &vertexshader = default_link_inputs[1].opengl_id;
	// outputs
	GLuint &program = default_link_outputs[0].opengl_id;
	int haserror = 0;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new                 )();
      void METHOD(init                )(CLASSNAME *op);
      void METHOD(render              )(CLASSNAME *op);
      void METHOD(update              )(CLASSNAME *op);
      void METHOD(render_editor       )(CLASSNAME *op);
	  void METHOD(write_extra_data    )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data     )(CLASSNAME *op, char *line);
	  void METHOD(on_link_disconnect  )(CLASSNAME *op, int id);
	  void METHOD(on_link_update      )(CLASSNAME *op, int id);


#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif