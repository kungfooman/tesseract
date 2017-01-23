#ifndef OP_FRAGMENT_SHADER_H
#define OP_FRAGMENT_SHADER_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "FragmentShader"
#define CLASSNAME OpFragmentShader
#define METHOD(name) op_fragment_shader_##name
#define STRUCTNAME struct op_fragment_shader_s

STRUCTNAME {
	#include "op_base.h"
	char filename[128] = {"shaders/line2_fragment.glsl"};
	GLuint &fragmentshader = default_link_outputs[0].opengl_id;
	int haserror = 0;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);


#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif