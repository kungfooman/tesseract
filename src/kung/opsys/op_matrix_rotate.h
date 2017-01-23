#ifndef OP_MATRIX_ROTATE_H
#define OP_MATRIX_ROTATE_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "MatrixRotate"
#define CLASSNAME OpMatrixRotate
#define METHOD(name) op_matrix_rotate_##name
#define STRUCTNAME struct op_matrix_rotate_s

STRUCTNAME {
	#include "op_base.h"
	float *matrix_a = default_link_inputs[0].matrix;
	float *matrix_b = default_link_inputs[1].matrix;
	float &x = default_link_inputs[0].val_f;
	float &y = default_link_inputs[1].val_f;
	float &z = default_link_inputs[2].val_f;
	float &angle = default_link_inputs[3].val_f;
	float tmp_matrix[16];
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