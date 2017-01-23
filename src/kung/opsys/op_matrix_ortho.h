#ifndef OP_ORTHO_H
#define OP_ORTHO_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Ortho"
#define CLASSNAME OpOrtho
#define METHOD(name) op_ortho_##name
#define STRUCTNAME struct op_ortho_s

STRUCTNAME {
	#include "op_base.h"
	float &left   = default_link_inputs[0].val_f;
	float &right  = default_link_inputs[1].val_f;
	float &bottom = default_link_inputs[2].val_f;
	float &top    = default_link_inputs[3].val_f;
	float &znear  = default_link_inputs[4].val_f;
	float &zfar   = default_link_inputs[5].val_f;
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