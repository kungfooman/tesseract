#ifndef OP_MATRIX_IDENTITY_H
#define OP_MATRIX_IDENTITY_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "MatrixIdentity"
#define CLASSNAME OpMatrixIdentity
#define METHOD(name) op_matrix_identity_##name
#define STRUCTNAME struct op_matrix_identity_s

STRUCTNAME {
	#include "op_base.h"
	float tmp_matrix[16];
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