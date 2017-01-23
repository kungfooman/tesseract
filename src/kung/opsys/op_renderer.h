#ifndef OP_RENDERER_H
#define OP_RENDERER_H

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Renderer"
#define CLASSNAME OpRenderer
#define METHOD(name) op_renderer_##name
#define STRUCTNAME struct op_renderer_s

#include "op.h"

STRUCTNAME {
	#include "op_base.h"
	int drawsort; // based on position, maybe handcode it
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new               )();
      void METHOD(init              )(CLASSNAME *op);
      void METHOD(render            )(CLASSNAME *op);
      void METHOD(update            )(CLASSNAME *op);
      void METHOD(render_editor     )(CLASSNAME *op);
	  void METHOD(write_extra_data  )(CLASSNAME *op, FILE *f);
	  void METHOD(read_extra_data   )(CLASSNAME *op, char *line);
      void METHOD(draw_in_scene     )(CLASSNAME *op, float *matrix);
      void METHOD(draw_in_scene     )(CLASSNAME *op);



#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif