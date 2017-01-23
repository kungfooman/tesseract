#ifndef OP_MATRIX_H
#define OP_MATRIX_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Matrix"
#define CLASSNAME OpMatrix
#define METHOD(name) op_matrix_##name
#define STRUCTNAME struct op_matrix_s

STRUCTNAME {
	#include "op_base.h"
	float *matrix = default_link_inputs[0].matrix;
};

typedef STRUCTNAME CLASSNAME;

CLASSNAME *METHOD(new             )();
      void METHOD(init            )(CLASSNAME *op);
      void METHOD(render          )(CLASSNAME *op);
      void METHOD(update          )(CLASSNAME *op);
      void METHOD(render_editor   )(CLASSNAME *op);

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif