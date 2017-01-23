#ifndef OP_PERSPECTIVE_H
#define OP_PERSPECTIVE_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Perspective"
#define CLASSNAME OpPerspective
#define METHOD(name) op_perspective_##name
#define STRUCTNAME struct op_perspective_s

STRUCTNAME {
	#include "op_base.h"
	float &fov = default_link_inputs[0].val_f;
	float &aspect = default_link_inputs[1].val_f;
	float &znear = default_link_inputs[2].val_f;
	float &zfar = default_link_inputs[3].val_f;
	float tmp_matrix[16] = {-1};
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