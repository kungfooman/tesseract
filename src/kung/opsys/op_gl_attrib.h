#ifndef OP_GL_ATTRIB_H
#define OP_GL_ATTRIB_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "GLAttrib"
#define CLASSNAME OpGLAttrib
#define METHOD(name) op_gl_attrib_##name
#define STRUCTNAME struct op_gl_attrib_s

#include "op.h"

STRUCTNAME {
	#include "op_base.h"
	char attrib[128] = {0};
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);



#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif