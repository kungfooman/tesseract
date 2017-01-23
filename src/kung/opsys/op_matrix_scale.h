#ifndef OP_MATRIX_SCALE_H
#define OP_MATRIX_SCALE_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "MatrixScale"
#define CLASSNAME OpMatrixScale
#define METHOD(name) op_matrix_scale_##name
#define STRUCTNAME struct op_matrix_scale_s

STRUCTNAME {
	#include "op_base.h"
	float tmp_matrix[16];
	float &dx = default_link_inputs[0].val_f;
	float &dy = default_link_inputs[1].val_f;
	float &dz = default_link_inputs[2].val_f;
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