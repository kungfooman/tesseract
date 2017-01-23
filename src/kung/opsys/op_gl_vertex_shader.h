#ifndef OP_VERTEX_SHADER_H
#define OP_VERTEX_SHADER_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "VertexShader"
#define CLASSNAME OpVertexShader
#define METHOD(name) op_vertex_shader_##name
#define STRUCTNAME struct op_vertex_shader_s

STRUCTNAME {
	#include "op_base.h"
	char filename[128] = {"shaders/line2_vertex.glsl"};
	GLuint &vertexshader = default_link_outputs[0].opengl_id;
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


#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif