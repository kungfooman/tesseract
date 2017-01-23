#ifndef OP_COS_H
#define OP_COS_H

#include "op.h"

// Just fix and copy&paste to op_*.cpp
#define SHORTNAME "Cos"
#define CLASSNAME OpCos
#define METHOD(name) op_cos_##name
#define STRUCTNAME struct op_cos_s

STRUCTNAME {
	#include "op_base.h"
	float &val = default_link_inputs[0].val_f;
	float tmp_val;
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