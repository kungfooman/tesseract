#ifndef OP_MATRIX_MULTIPLY_H
#define OP_MATRIX_MULTIPLY_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "MatrixMultiply"
#define CLASSNAME OpMatrixMultiply
#define METHOD(name) op_matrix_multiply_##name
#define STRUCTNAME struct op_matrix_multiply_s
//#define FUCKYOU "fuckyou"
STRUCTNAME {
	#include "op_base.h"
	float *matrix_a = default_link_inputs[0].matrix;
	float *matrix_b = default_link_inputs[1].matrix;
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

#undef SHORTNAME
#undef CLASSNAME
#undef METHOD
#undef STRUCTNAME

#endif